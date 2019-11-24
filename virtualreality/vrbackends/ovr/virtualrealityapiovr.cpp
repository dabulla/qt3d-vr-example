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

#if(QT3DVR_COMPILE_WITH_OCULUSVR)
#include "virtualrealityapiovr.h"
#include "framebufferovr.h"

#include "Extras/OVR_Math.h"
#include "OVR_CAPI_GL.h"

using namespace OVR;

#if defined(_WIN32)
    #include <dxgi.h> // for GetDefaultAdapterLuid
    //#pragma comment(lib, "dxgi.lib")
#endif


static ovrGraphicsLuid GetDefaultAdapterLuid()
{
    ovrGraphicsLuid luid = ovrGraphicsLuid();

#if defined(_WIN32)
    IDXGIFactory* factory = nullptr;

    if (SUCCEEDED(CreateDXGIFactory(IID_PPV_ARGS(&factory))))
    {
        IDXGIAdapter* adapter = nullptr;

        if (SUCCEEDED(factory->EnumAdapters(0, &adapter)))
        {
            DXGI_ADAPTER_DESC desc;

            adapter->GetDesc(&desc);
            memcpy(&luid, &desc.AdapterLuid, sizeof(luid));
            adapter->Release();
        }

        factory->Release();
    }
#endif

    return luid;
}

bool VirtualRealityApiOvr::isRuntimeInstalled()
{
    ovrResult result = ovr_Initialize(nullptr);
    return OVR_SUCCESS(result);
}

VirtualRealityApiOvr::VirtualRealityApiOvr()
    : m_sessionStarted(false)
    , m_sensorSampleTime(0.0)
    , m_frameIndex(0)
    , m_swapChain(nullptr)
{
}

VirtualRealityApiOvr::~VirtualRealityApiOvr()
{
    // Don't call shutdown. It has been called already at this point.
    Q_ASSERT(m_swapChain);
}

bool VirtualRealityApiOvr::initializeIfHmdIsPresent()
{
    if(!m_sessionStarted) {
        ovrResult result = ovr_Initialize(nullptr);
        if (!OVR_SUCCESS(result))
        {
            qDebug() << "Failed to initialize libOVR.";
            return false;
        }
        result = ovr_Create(&m_session, &m_luid);
        if (!OVR_SUCCESS(result))
        {
            qDebug() << "Failed to create session libOVR.";
            return false;
        }
        m_sessionStarted = true;
    }
    if (memcmp(&m_luid, &GetDefaultAdapterLuid(), sizeof(m_luid))) // If luid that the Rift is on is not the default adapter LUID...
    {
        qDebug() << "OpenGL supports only the default graphics adapter.";
        return false;
    }
    return true;
}

bool VirtualRealityApiOvr::isHmdPresent()
{
    return initializeIfHmdIsPresent();
}

void VirtualRealityApiOvr::initialize()
{
    bool hmdPresent = initializeIfHmdIsPresent();
    if(!hmdPresent)
        return;
    m_hmdDesc = ovr_GetHmdDesc(m_session);
    ovr_SetTrackingOriginType(m_session, ovrTrackingOrigin_FloorLevel);
    m_swapChain = new OvrSwapChain(m_session, getRenderTargetSize());
}

void VirtualRealityApiOvr::shutdown()
{
    if(m_swapChain != nullptr)
        delete m_swapChain;
}

bool VirtualRealityApiOvr::bindFrambufferObject(int hmdId)
{
    m_swapChain->bindCurrentChainIndexFramebuffer();
    return true;
}

void VirtualRealityApiOvr::swapToHeadset()
{
    m_swapChain->commit();

    ovrLayerEyeFov ld;
    ld.Header.Type  = ovrLayerType_EyeFov;
    ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

    ld.ColorTexture[ovrEye_Left] = m_swapChain->ovrTextureChain();
    int halfWidth = m_swapChain->size().width()/2;
    int height = m_swapChain->size().height();
    ld.Viewport[ovrEye_Left]     = Recti(0, 0, halfWidth, height);
    ld.Fov[ovrEye_Left]          = m_hmdDesc.DefaultEyeFov[ovrEye_Left];
    ld.RenderPose[ovrEye_Left]   = m_eyeRenderPose[ovrEye_Left];

    ld.ColorTexture[ovrEye_Right] = m_swapChain->ovrTextureChain();
    ld.Viewport[ovrEye_Right]     = Recti(halfWidth, 0, halfWidth, height);
    ld.Fov[ovrEye_Right]          = m_hmdDesc.DefaultEyeFov[ovrEye_Right];
    ld.RenderPose[ovrEye_Right]   = m_eyeRenderPose[ovrEye_Right];

    ld.SensorSampleTime  = m_sensorSampleTime;

    ovrLayerHeader* layers = &ld.Header;
    ovrResult result = ovr_SubmitFrame(m_session, m_frameIndex, nullptr, &layers, 1);

    if (!OVR_SUCCESS(result))
    {
        ovrErrorInfo inf;
        ovr_GetLastErrorInfo(&inf);
        qDebug() << inf.ErrorString;
        return;
    }
    m_frameIndex++;
}

void VirtualRealityApiOvr::getEyeMatrices(QMatrix4x4 &leftEye, QMatrix4x4 &rightEye)
{
    ovrEyeRenderDesc eyeRenderDesc[ovrEye_Count];
    eyeRenderDesc[ovrEye_Left] = ovr_GetRenderDesc(m_session, ovrEye_Left, m_hmdDesc.DefaultEyeFov[ovrEye_Left]);
    eyeRenderDesc[ovrEye_Right] = ovr_GetRenderDesc(m_session, ovrEye_Right, m_hmdDesc.DefaultEyeFov[ovrEye_Right]);
    // Get eye poses, feeding in correct IPD offset
    ovrPosef               HmdToEyeOffset[2] = { eyeRenderDesc[ovrEye_Left].HmdToEyePose,
                                                    eyeRenderDesc[ovrEye_Right].HmdToEyePose };


    ovr_GetEyePoses(m_session, m_frameIndex, ovrTrue, HmdToEyeOffset, m_eyeRenderPose, &m_sensorSampleTime);

    ovrVector3f &posLeft = m_eyeRenderPose[ovrEye_Left].Position;
    ovrVector3f &posRight = m_eyeRenderPose[ovrEye_Right].Position;
    ovrQuatf orientLeft = m_eyeRenderPose[ovrEye_Left].Orientation;
    ovrQuatf orientRight = m_eyeRenderPose[ovrEye_Right].Orientation;

    QVector3D zero(0.0f,0.0f,0.0f);
    Matrix4f finalRollPitchYaw = Matrix4f(orientLeft).Transposed();
    Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
    Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
    leftEye.translate(QVector3D(posLeft.x, posLeft.y, posLeft.z));
    leftEye.lookAt(zero, QVector3D(finalForward.x, finalForward.y, finalForward.z), QVector3D(finalUp.x, finalUp.y, finalUp.z));
    finalRollPitchYaw = Matrix4f(orientRight).Transposed();
    finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
    finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
    rightEye.translate(QVector3D(posRight.x, posRight.y, posRight.z));
    rightEye.lookAt(zero, QVector3D(finalForward.x, finalForward.y, finalForward.z), QVector3D(finalUp.x, finalUp.y, finalUp.z));
}

void VirtualRealityApiOvr::getProjectionMatrices(QMatrix4x4 &leftProjection, QMatrix4x4 &rightProjection)
{
    Matrix4f   projLeft   = ovrMatrix4f_Projection(m_hmdDesc.DefaultEyeFov[ovrEye_Left], 0.2f, 1000.0f, ovrProjection_None);
    Matrix4f   projRight  = ovrMatrix4f_Projection(m_hmdDesc.DefaultEyeFov[ovrEye_Right], 0.2f, 1000.0f, ovrProjection_None);
    QMatrix4x4 projL(projLeft.M[0][0], projLeft.M[0][1], projLeft.M[0][2], projLeft.M[0][3],
                     projLeft.M[1][0], projLeft.M[1][1], projLeft.M[1][2], projLeft.M[1][3],
                     projLeft.M[2][0], projLeft.M[2][1], projLeft.M[2][2], projLeft.M[2][3],
                     projLeft.M[3][0], projLeft.M[3][1], projLeft.M[3][2], projLeft.M[3][3]);
    QMatrix4x4 projR(projRight.M[0][0], projRight.M[0][1], projRight.M[0][2], projRight.M[0][3],
                     projRight.M[1][0], projRight.M[1][1], projRight.M[1][2], projRight.M[1][3],
                     projRight.M[2][0], projRight.M[2][1], projRight.M[2][2], projRight.M[2][3],
                     projRight.M[3][0], projRight.M[3][1], projRight.M[3][2], projRight.M[3][3]);
    leftProjection = projL;
    rightProjection = projR;
    //TODO: don't copy
}

QList<int> VirtualRealityApiOvr::currentlyTrackedObjects()
{
    //TODO: does this work?
    //return ovr_GetTrackerCount(m_session);
    return QList<int>();
}

void VirtualRealityApiOvr::getTrackedObject(int id, QMatrix4x4 &transform)
{

}

Qt3DVirtualReality::QVirtualRealityApiBackend::TrackedObjectType VirtualRealityApiOvr::getTrackedObjectType(int id)
{
    return Qt3DVirtualReality::QVirtualRealityApiBackend::Other;
}

void VirtualRealityApiOvr::getTrackedObjectModel(int id, QVector<float> &vertices, QVector<int> &indices, QOpenGLTexture *texture)
{

}

void VirtualRealityApiOvr::getMirrorTexture(QOpenGLTexture *outMirrorTexture)
{

}

bool VirtualRealityApiOvr::isTriggerTmp()
{
    //TO DO: check for real trigger
    return false;
}

qreal VirtualRealityApiOvr::refreshRate(int hmdId) const
{
    return m_hmdDesc.DisplayRefreshRate;
}

QMatrix4x4 VirtualRealityApiOvr::headPose(int hmdId)
{
    return QMatrix4x4();
}

QSize VirtualRealityApiOvr::getRenderTargetSize()
{
    if(m_swapChain == nullptr)
    {
        ovrSizei idealTextureSizeLeft = ovr_GetFovTextureSize(m_session, ovrEye_Left, m_hmdDesc.DefaultEyeFov[ovrEye_Left], 1.0f);
        ovrSizei idealTextureSizeRight = ovr_GetFovTextureSize(m_session, ovrEye_Right, m_hmdDesc.DefaultEyeFov[ovrEye_Right], 1.0f);
        return QSize(idealTextureSizeLeft.w+idealTextureSizeRight.w, idealTextureSizeLeft.h);
    }
    return m_swapChain->size();
}

int VirtualRealityApiOvr::timeUntilNextFrame()
{
    // TO DO what happens if m_frame has not yet been incremented?
    double absolutFrameTime = ovr_GetPredictedDisplayTime(m_session, m_frameIndex-1);
    double now = ovr_GetTimeInSeconds();
    return static_cast<int>(std::floor(1000.0*(absolutFrameTime-now)));
}
#endif
