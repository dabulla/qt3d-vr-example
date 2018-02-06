//****************************************************************************
//**
//** Author: Daniel Bulla
//** Contact: qt3d-vr@danielbulla.de
//**
//** GNU Lesser General Public License Usage
//** General Public License version 3 as published by the Free Software
//** Foundation and appearing in the file LICENSE.LGPL3 included in the
//** packaging of this file. Please review the following information to
//** ensure the GNU Lesser General Public License version 3 requirements
//** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
//**
//****************************************************************************/

#include "qheadmounteddisplay.h"
#include <Qt3DQuick/QQmlAspectEngine>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DRender/qrendersurfaceselector.h>
#include <Qt3DRender/private/qrendersurfaceselector_p.h>
#include <Qt3DInput/qinputaspect.h>
#include <Qt3DInput/qinputsettings.h>
#include <Qt3DLogic/qlogicaspect.h>

#include <QQmlContext>
#include <qqmlincubator.h>
#include <QGuiApplication>
#include <QScreen>

#include <QtGui/qopenglcontext.h>
#include <QSurface>
#include "qvirtualrealityapibackend.h"
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/private/qrenderaspect_p.h>
#include <Qt3DCore/private/qabstractaspectjobmanager_p.h>
#include "frontend/qvirtualrealitycamera.h"
#include "frontend/qvirtualrealitymesh.h"
#include <QOpenGLDebugLogger>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

namespace {

class Qt3DQuickVirtualRealityIncubationController : public QObject, public QQmlIncubationController
{
    Q_OBJECT
public:
    explicit Qt3DQuickVirtualRealityIncubationController(QHeadMountedDisplay *hmd)
        : QObject(hmd)
        , m_hmd(hmd)
        //, m_incubationTime(std::max(1, int(1000 / hmd->refreshRate()) / 3)) // Allow incubation for 1/3 of a frame.
    {
        startTimer(hmd->refreshRate());
    }

    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE
    {
        int incubationTime = qMax(0, m_hmd->timeUntilNextFrame()-6); // present ~6ms before scanout
        qDebug() << "Incubating for" << incubationTime;
        if(incubationTime > 0)
            incubateFor(incubationTime);
    }

private:
    //const int m_incubationTime;
    QHeadMountedDisplay *m_hmd;
};

} // anonymous

QHeadMountedDisplay::QHeadMountedDisplay(int hmdId, const QHeadMountedDisplayFormat &formathmd, QVirtualRealityApi *api, QVirtualRealityApiBackend *apibackend)
    : QObject()
    , m_engine(nullptr)
    , m_renderAspect(nullptr)
    , m_inputAspect(nullptr)
    , m_logicAspect(nullptr)
    , m_virtualRealityAspect(nullptr)
    , m_initialized(false)
    , m_incubationController(nullptr)
    , m_api(api)
    , m_apibackend(apibackend)
    , m_hmdId(hmdId)
    , m_fbo(nullptr)
    , m_context(nullptr)
    , m_surface(new QOffscreenSurface)
    , m_rootItem(nullptr)
{
    //Note: m_apibackend is not yet initialized here. Wait for openGLContext creation

    QSurfaceFormat format;
    // Qt Quick may need a depth and stencil buffer. Always make sure these are available.
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);

    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();


    m_surface = new QOffscreenSurface;
    // Pass m_context->format(), not format. Format does not specify and color buffer
    // sizes, while the context, that has just been created, reports a format that has
    // these values filled in. Pass this to the offscreen surface to make sure it will be
    // compatible with the context's configuration.
    m_surface->setFormat(m_context->format());
    m_surface->create();
    m_context->makeCurrent(m_surface);

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);

    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, [](const QOpenGLDebugMessage &debugMessage){
        qDebug() << debugMessage;
    }, Qt::DirectConnection);
    logger->enableMessages();
    logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    emit surfaceChanged(m_surface);

    m_engine.reset(new Qt3DCore::Quick::QQmlAspectEngine);
    m_renderAspect = new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Synchronous);
    m_inputAspect = new Qt3DInput::QInputAspect;
    m_logicAspect = new Qt3DLogic::QLogicAspect;
    m_virtualRealityAspect = new Qt3DVirtualReality::QVirtualRealityAspect;
    m_virtualRealityAspect->setHeadmountedDisplay(this);
    //m_virtualRealityAspect->setVirtualRealityApi(m_api);
    m_virtualRealityAspect->setVirtualRealityApiBackend(m_apibackend);

    m_engine->aspectEngine()->registerAspect(m_renderAspect);
    m_engine->aspectEngine()->registerAspect(m_inputAspect);
    m_engine->aspectEngine()->registerAspect(m_logicAspect);
    m_engine->aspectEngine()->registerAspect(m_virtualRealityAspect);
    connect(this, &QHeadMountedDisplay::requestRun, this, &QHeadMountedDisplay::run, Qt::QueuedConnection);
}

QHeadMountedDisplay::~QHeadMountedDisplay()
{
    if(m_surface)
        delete m_surface;
}

void QHeadMountedDisplay::registerAspect(Qt3DCore::QAbstractAspect *aspect)
{
    m_engine->aspectEngine()->registerAspect(aspect);
}

void QHeadMountedDisplay::registerAspect(const QString &name)
{
    m_engine->aspectEngine()->registerAspect(name);
}

void QHeadMountedDisplay::setSource(const QUrl &source)
{
    m_source = source;
    if(!m_initialized)
    {
        // Connect to the QQmlAspectEngine's statusChanged signal so that when the QML is loaded
        // and th eobjects hav ebeen instantiated, but before we set them on the QAspectEngine we
        // can swoop in and set the window surface and camera on the framegraph and ensure the camera
        // respects the window's aspect ratio
        connect(m_engine.data(), &Qt3DCore::Quick::QQmlAspectEngine::sceneCreated,
                this, &QHeadMountedDisplay::onSceneCreated);


        qmlRegisterType<QVirtualrealityCamera>("vr", 2, 0, "VrCamera");
        qmlRegisterType<QVirtualRealityMesh>("vr", 2, 0, "TrackedObjectMesh");
        m_engine->setSource(m_source);

        // Set the QQmlIncubationController on the window
        // to benefit from asynchronous incubation
        if (!m_incubationController)
            m_incubationController = new Qt3DQuickVirtualRealityIncubationController(this);

        m_engine->qmlEngine()->setIncubationController(m_incubationController);

        m_initialized = true;
    }
    else
    {
        qDebug() << "source already set, not yet implemented."; // TO DO
    }
}

Qt3DCore::Quick::QQmlAspectEngine *QHeadMountedDisplay::engine() const
{
    return m_engine.data();
}

qreal QHeadMountedDisplay::refreshRate()
{
    Q_ASSERT(m_apibackend);
    return m_apibackend->refreshRate(m_hmdId);
}

QObject *QHeadMountedDisplay::surface() const
{
    return qobject_cast<QObject*>(m_surface);
}

QSize QHeadMountedDisplay::renderTargetSize() const
{
    return m_apibackend->getRenderTargetSize();
}

int QHeadMountedDisplay::timeUntilNextFrame()
{
    return m_apibackend->timeUntilNextFrame();
}

QOpenGLContext *QHeadMountedDisplay::context()
{
    return m_context;
}

void QHeadMountedDisplay::onSceneCreated(QObject *rootObject)
{
    Q_ASSERT(rootObject);

    setWindowSurface(rootObject);

    m_rootItem = rootObject;//qobject_cast<QQuickItem *>(rootObject);
//    if (m_cameraAspectRatioMode == AutomaticAspectRatio) {
//        // Set aspect ratio of first camera to match the window
//        QList<Qt3DRender::QCamera *> cameras
//                = rootObject->findChildren<Qt3DRender::QCamera *>();
//        if (cameras.isEmpty()) {
//            qWarning() << "No camera found";
//        } else {
//            m_camera = cameras.first();
//            setCameraAspectModeHelper();
//        }
//    }

    if(m_rootItem) {
        QVirtualrealityCamera *vrCamera = m_rootItem->findChild<QVirtualrealityCamera *>();
        if(vrCamera) {
            QMatrix4x4 projL;
            QMatrix4x4 projR;
            m_apibackend->getProjectionMatrices(projL, projR);
            vrCamera->setProjections(projL, projR);
            vrCamera->setLeftNormalizedViewportRect(QRectF(0.0f, 0.0f, 0.5f, 1.0f));
            vrCamera->setRightNormalizedViewportRect(QRectF(0.5f, 0.0f, 0.5f, 1.0f));
        } else {
            Q_ASSERT(vrCamera); //TO DO logging
        }
    } else {
        Q_ASSERT(m_rootItem); //TO DO logging
    }

    // Set ourselves up as a source of input events for the input aspect
    Qt3DInput::QInputSettings *inputSettings = rootObject->findChild<Qt3DInput::QInputSettings *>();
    if (inputSettings) {
        inputSettings->setEventSource(this);
    } else {
        qWarning() << "No Input Settings found, keyboard and mouse events won't be handled";
    }
    Q_EMIT sceneCreated(m_rootItem);
}

void QHeadMountedDisplay::run() {

    m_context->makeCurrent(m_surface);


//    QOpenGLFramebufferObjectFormat format;
//    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
//    format.setTextureTarget(GL_TEXTURE_2D);
//    format.setSamples(0);
//    format.setMipmap(false);

//    QSize renderTargetSize = m_apibackend->getRenderSurfaceSize();
//    if(m_fbo)
//        delete m_fbo;
//    m_fbo = new QOpenGLFramebufferObject(renderTargetSize, format, m_apibackend->currentTextureId() );
//    m_fbo->bind();

    //glViewport(0, 0, renderTargetSize.width(), renderTargetSize.height());

//    Q_ASSERT(false);//TO DO: wrong thread for m_fbo->bind(). need QSGNode. What about createTextureFromId?
//        if (m_aspectEngine->rootEntity() != m_item->entity())
//            scheduleRootEntityChange();
    //TODO: QVrSelector. This is the object with all parameters then
    QVirtualrealityCamera *vrCamera(nullptr);
    QList<QVirtualRealityMesh*> vrGeometries;
    if(m_rootItem) {
        vrCamera = m_rootItem->findChild<QVirtualrealityCamera *>();
        QList<QVirtualRealityMesh*> vrGeometries = m_rootItem->findChildren<QVirtualRealityMesh*>();
    }
    m_apibackend->bindFrambufferObject(m_hmdId);
    for(QList<QVirtualRealityMesh*>::iterator iter(vrGeometries.begin()); iter != vrGeometries.end(); ++iter) {
        (*iter)->setVrApiBackendTmp(m_apibackend);
    }
    //static_cast<Qt3DRender::QRenderAspectPrivate*>(Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->jobManager()->waitForAllJobs();
    static_cast<Qt3DRender::QRenderAspectPrivate*>(Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderSynchronous();
    QMatrix4x4 leftEye;
    QMatrix4x4 rightEye;
    m_apibackend->getEyeMatrices(leftEye, rightEye);
    if(vrCamera != nullptr) {
        vrCamera->update(leftEye, rightEye);
        vrCamera->setVrBackendTmp(m_apibackend); // only for transforms
    }
    m_fbo->bindDefault();
    m_apibackend->swapToHeadset();
    emit requestRun();
}

void QHeadMountedDisplay::setWindowSurface(QObject *rootObject)
{
    //    if(!(m_context = QOpenGLContext::currentContext()))
    //    {
    //        m_context = new QOpenGLContext();
    //        m_context->create();
    //        m_context->makeCurrent(m_surface);
    //        Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
//    }
    m_context->makeCurrent(m_surface);
    static_cast<Qt3DRender::QRenderAspectPrivate*>(Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderInitialize(m_context);
    Qt3DRender::QRenderSurfaceSelector *surfaceSelector = Qt3DRender::QRenderSurfaceSelectorPrivate::find(rootObject);
    if (surfaceSelector)
        surfaceSelector->setSurface(m_surface);
    if (m_fbo) {
        delete m_fbo;
        m_fbo = nullptr;
    }
    const QSize texSize(m_apibackend->getRenderTargetSize());

    //TODO: At the moment FBO is recreated each frame
    //if (!m_fbo) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setTextureTarget(GL_TEXTURE_2D);
        format.setSamples(0);
        format.setMipmap(false);

//        QSurfaceFormat notUsedTodo;
//        m_apibackend->createSurface(m_hmdId, texSize, notUsedTodo);
        emit renderTargetSizeChanged(texSize);
        //m_fbo = new QOpenGLFramebufferObject(texSize, format, texId );
        //m_quickWindow->setRenderTarget(m_fbo);
    //}
//    if(!sizeOkay)
//    {
//        // Size of m_window can differ from m_quickWindow and rendertarget size
//        m_quickWindow->resize(texSize);
//        m_quickWindow->contentItem()->setWidth(texSize.width());
//        m_quickWindow->contentItem()->setHeight(texSize.height());
//        m_quickWindow->setGeometry(0, 0, texSize.width(), texSize.height());
//    }
}

} // Qt3DVirtualReality

QT_END_NAMESPACE

#include "qheadmounteddisplay.moc"
