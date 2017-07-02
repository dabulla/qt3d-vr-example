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

#ifndef QT3DHEADMOUNTEDDISPLAY_H
#define QT3DHEADMOUNTEDDISPLAY_H

#include "qvirtualrealityapi.h"

#include <frontend/qvirtualrealityaspect.h>

#include <QOffscreenSurface>
#include <QtCore/qpointer.h>
#include <QScopedPointer>
#include <QUrl>
#include <QOpenGLFramebufferObject>

#include <QQuickItem>

QT_BEGIN_NAMESPACE

class QQmlIncubationController;

namespace Qt3DCore {
class QAbstractAspect;
namespace Quick {
class QQmlAspectEngine;
}
}

namespace Qt3DRender {
class QRenderAspect;
class QCamera;
}

namespace Qt3DInput {
class QInputAspect;
}

namespace Qt3DLogic {
class QLogicAspect;
}

namespace Qt3DVirtualReality {

class QVirtualRealityApiBackend;

class QT3DVR_EXPORT QHeadMountedDisplay : public QObject /*: public QQuickItem*/ {
    Q_OBJECT
    Q_PROPERTY(QObject* surface READ surface NOTIFY surfaceChanged)
    Q_PROPERTY(QSize renderTargetSize READ renderTargetSize NOTIFY renderTargetSizeChanged)

public:
    QHeadMountedDisplay(int hmdId, const QHeadMountedDisplayFormat &formathmd, QVirtualRealityApi *api, QVirtualRealityApiBackend *apibackend);
    ~QHeadMountedDisplay();

    void registerAspect(Qt3DCore::QAbstractAspect *aspect);
    void registerAspect(const QString &name);

    void setSource(const QUrl &source);
    Qt3DCore::Quick::QQmlAspectEngine *engine() const;

    // Hmd specific
    qreal refreshRate();
    qreal superSamplingFactor();

    QObject* surface() const;
    QSize renderTargetSize() const;

    int timeUntilNextFrame();
    QOpenGLContext *context();
signals:
    void requestRun();
    void surfaceChanged(QSurface* surface);
    void renderTargetSizeChanged(QSize renderTargetSize);

public slots:
    void run();

private:
    void onSceneCreated(QObject *rootObject);
    void setWindowSurface(QObject *rootObject);

    QScopedPointer<Qt3DCore::Quick::QQmlAspectEngine> m_engine;

    // Aspects
    Qt3DRender::QRenderAspect *m_renderAspect;
    Qt3DInput::QInputAspect *m_inputAspect;
    Qt3DLogic::QLogicAspect *m_logicAspect;
    Qt3DVirtualReality::QVirtualRealityAspect *m_virtualRealityAspect;

    QUrl m_source;
    bool m_initialized;
    QPointer<Qt3DRender::QCamera> m_camera;
    QQmlIncubationController *m_incubationController;
    QVirtualRealityApi *m_api;
    QVirtualRealityApiBackend *m_apibackend;
    int m_hmdId;
    QOpenGLFramebufferObject *m_fbo;
    QOpenGLContext *m_context;
    QOffscreenSurface *m_surface;
    QObject *m_rootItem;
};

} // Qt3DVirtualReality

QT_END_NAMESPACE

#endif
