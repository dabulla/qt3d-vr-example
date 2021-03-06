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

#if(QT3DVR_COMPILE_WITH_OPENVR)
#include "virtualrealityapiopenvr.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QThread>

//// Process SteamVR controller state
//for( vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++ )
//{
//    vr::VRControllerState_t state;
//    if( m_pHMD->GetControllerState( unDevice, &state, sizeof(state) ) )
//    {
//        m_rbShowTrackedDevice[ unDevice ] = state.ulButtonPressed == 0;
//    }
//}

//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string getTrackedDeviceString( vr::IVRSystem *hmd, vr::TrackedDeviceIndex_t device, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *error = NULL )
{
    uint32_t requiredBufferLen = hmd->GetStringTrackedDeviceProperty( device, prop, NULL, 0, error );
    if( requiredBufferLen == 0 )
        return "";

    char *buffer = new char[ requiredBufferLen ];
    requiredBufferLen = hmd->GetStringTrackedDeviceProperty( device, prop, buffer, requiredBufferLen, error );
    std::string result = buffer;
    delete [] buffer;
    return result;
}

//-----------------------------------------------------------------------------
// Purpose: Gets a Matrix Projection Eye with respect to nEye.
//-----------------------------------------------------------------------------
QMatrix4x4 VirtualRealityApiOpenVR::getHmdMatrixProjectionEye( vr::Hmd_Eye nEye )
{
    if ( !m_hmd )
        return QMatrix4x4();
    float nearClip = 0.2f;
    float farClip = 1000.0f;
    vr::HmdMatrix44_t mat = m_hmd->GetProjectionMatrix( nEye, nearClip, farClip );

    return convertSteamVrMatrixToQMatrix4x4(mat);
//    return QMatrix4x4(
//        mat.m[0][0], mat.m[0][1], -mat.m[0][2], mat.m[0][3],
//        mat.m[1][0], mat.m[1][1], -mat.m[1][2], mat.m[1][3],
//        mat.m[2][0], mat.m[2][1], -mat.m[2][2], mat.m[2][3],
//        mat.m[3][0], mat.m[3][1], -mat.m[3][2], mat.m[3][3]
//    );
}

//-----------------------------------------------------------------------------
// Purpose: Gets an HMDMatrixPoseEye with respect to nEye.
//-----------------------------------------------------------------------------
QMatrix4x4 VirtualRealityApiOpenVR::getHmdMatrixPoseEye( vr::Hmd_Eye nEye )
{
    if ( !m_hmd )
        return QMatrix4x4();

    vr::HmdMatrix34_t mat = m_hmd->GetEyeToHeadTransform( nEye );
    return convertSteamVrMatrixToQMatrix4x4(mat);//.inverted();
}

//-----------------------------------------------------------------------------
// Purpose: Gets a Current View Projection Matrix with respect to nEye,
//          which may be an Eye_Left or an Eye_Right.
//-----------------------------------------------------------------------------
QMatrix4x4 VirtualRealityApiOpenVR::getCurrentViewMatrix( vr::Hmd_Eye nEye )
{
    QMatrix4x4 matMVP;
    if( nEye == vr::Eye_Left )
    {
        matMVP = m_eyePosLeft * m_hmdPose;
    }
    else if( nEye == vr::Eye_Right )
    {
        matMVP = m_eyePosRight *  m_hmdPose;
    }

    return matMVP;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void VirtualRealityApiOpenVR::updateHmdMatrixPose()
{
    // This queries the pose for user. User gets a good estimation of the HMD pose that will be used.
    // However, the engine is free to use a newer estimation for the head pose for rendering, than returned here.
    // Only one estimation will be made on the first request of poses per frame.
    // This way e.g. not every qml binding will query the vr sdk on its own.
    if(m_poseNewEnough)
        return;
    m_poseNewEnough = true;
    if ( !m_hmd )
        return;

    vr::VRCompositor()->WaitGetPoses(m_trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0 );

    m_validPoseCount = 0;
    m_poseClasses = "";
    for ( int deviceIndex = 0; deviceIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIndex )
    {
        if ( m_trackedDevicePose[deviceIndex].bPoseIsValid )
        {
            m_validPoseCount++;
            m_devicePose[deviceIndex] = convertSteamVrMatrixToQMatrix4x4( m_trackedDevicePose[deviceIndex].mDeviceToAbsoluteTracking );
            if (m_devClassChar[deviceIndex]==0)
            {
                switch (m_hmd->GetTrackedDeviceClass(deviceIndex))
                {
                case vr::TrackedDeviceClass_Controller:        m_devClassChar[deviceIndex] = 'C'; break;
                case vr::TrackedDeviceClass_HMD:               m_devClassChar[deviceIndex] = 'H'; break;
                case vr::TrackedDeviceClass_Invalid:           m_devClassChar[deviceIndex] = 'I'; break;
                case vr::TrackedDeviceClass_GenericTracker:    m_devClassChar[deviceIndex] = 'G'; break;
                case vr::TrackedDeviceClass_TrackingReference: m_devClassChar[deviceIndex] = 'T'; break;
                default:                                       m_devClassChar[deviceIndex] = '?'; break;
                }
            }
            m_poseClasses += m_devClassChar[deviceIndex];
        }
    }

    if ( m_trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid )
    {
        m_hmdPose = m_devicePose[vr::k_unTrackedDeviceIndex_Hmd];
        //m_hmdPose = m_hmdPose.inverted();
    }

    m_isTrigger = false;
    for( vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++ )
    {
        vr::VRControllerState_t state;
        if( m_hmd->GetControllerState( unDevice, &state, sizeof(state) ) )
        {
            m_isTrigger |= state.ulButtonPressed != 0;
        }
    }
}

void VirtualRealityApiOpenVR::setupCameras()
{
    m_eyePosLeft = getHmdMatrixPoseEye( vr::Eye_Left );
    m_eyePosRight = getHmdMatrixPoseEye( vr::Eye_Right );
    qDebug() << "Left eye " << m_eyePosLeft.column(3);
    qDebug() << "Right eye" << m_eyePosRight.column(3);
}

//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
QMatrix4x4 VirtualRealityApiOpenVR::convertSteamVrMatrixToQMatrix4x4( const vr::HmdMatrix34_t matPose )
{
     return QMatrix4x4(matPose.m[0][0], matPose.m[0][1], matPose.m[0][2], matPose.m[0][3],
                       matPose.m[1][0], matPose.m[1][1], matPose.m[1][2], matPose.m[1][3],
                       matPose.m[2][0], matPose.m[2][1], matPose.m[2][2], matPose.m[2][3],
                       0.0f, 0.0f, 0.0f, 1.0f);
}

QMatrix4x4 VirtualRealityApiOpenVR::convertSteamVrMatrixToQMatrix4x4( const vr::HmdMatrix44_t matPose )
{
     return QMatrix4x4(matPose.m[0][0], matPose.m[0][1], matPose.m[0][2], matPose.m[0][3],
                       matPose.m[1][0], matPose.m[1][1], matPose.m[1][2], matPose.m[1][3],
                       matPose.m[2][0], matPose.m[2][1], matPose.m[2][2], matPose.m[2][3],
                       matPose.m[3][0], matPose.m[3][1], matPose.m[3][2], matPose.m[3][3]);
}

//QMatrix4x4 VirtualRealityApiOpenVR::convertSteamVrMatrixToQMatrix4x4( const vr::HmdMatrix44_t matPose )
//{
//     return QMatrix4x4(matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], matPose.m[3][0],
//                       matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], matPose.m[3][1],
//                       matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], matPose.m[3][2],
//                       matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], matPose.m[3][3]);
//}

bool VirtualRealityApiOpenVR::isRuntimeInstalled()
{
    return vr::VR_IsRuntimeInstalled();
}

VirtualRealityApiOpenVR::VirtualRealityApiOpenVR()
    : m_fbo(nullptr)
    , m_poseNewEnough(false)
    , m_isTrigger(false)
{

}

bool VirtualRealityApiOpenVR::isHmdPresent()
{
    return vr::VR_IsHmdPresent();
}

void VirtualRealityApiOpenVR::initialize()
{
    vr::EVRInitError error = vr::VRInitError_None;
    m_hmd = vr::VR_Init( &error, vr::VRApplication_Scene );

    if ( error != vr::VRInitError_None )
    {
        m_hmd = NULL;
        qDebug() << "Unable to init VR runtime:" << vr::VR_GetVRInitErrorAsEnglishDescription( error );
        return;
    }

    m_driver = getTrackedDeviceString( m_hmd, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String );
    m_display = getTrackedDeviceString( m_hmd, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String );

    vr::EVRInitError peError = vr::VRInitError_None;

    if ( !vr::VRCompositor() )
    {
        qDebug() << "Compositor initialization failed. See log file for details";
        return;
    }
    setupCameras();

    m_fbo = new QOpenGLFramebufferObject(getRenderTargetSize(), QOpenGLFramebufferObject::Depth);
    m_fbo->addColorAttachment(m_fbo->size(), GL_RGBA8);
}

void VirtualRealityApiOpenVR::shutdown()
{
    if( m_hmd ) {
        vr::VR_Shutdown();
        m_hmd = NULL;
    }
    if(m_fbo) {
        delete m_fbo;
        m_fbo = nullptr;
    }
}


void VirtualRealityApiOpenVR::processVrEvent( const vr::VREvent_t & event )
{
    switch( event.eventType )
    {
    case vr::VREvent_TrackedDeviceActivated:
        {
//            SetupRenderModelForTrackedDevice( event.trackedDeviceIndex );
            qDebug() << "Device" << event.trackedDeviceIndex << "attached. Setting up render model.";
        }
        break;
    case vr::VREvent_TrackedDeviceDeactivated:
        {
            qDebug() << "Device" << event.trackedDeviceIndex << "detached.";
        }
        break;
    case vr::VREvent_TrackedDeviceUpdated:
        {
        qDebug() << "Device" << event.trackedDeviceIndex << "updated.";
        }
        break;
    }
}

bool VirtualRealityApiOpenVR::bindFrambufferObject(int hmdId)
{
    return m_fbo->bind();
}

qreal VirtualRealityApiOpenVR::refreshRate(int hmdId) const
{
    return 90.f;
}

QMatrix4x4 VirtualRealityApiOpenVR::headPose(int hmdId)
{
    updateHmdMatrixPose();
    return m_hmdPose;
}

QSize VirtualRealityApiOpenVR::getRenderTargetSize()
{
    if ( !m_hmd ) return QSize(0, 0);
    uint32_t width, height;
    m_hmd->GetRecommendedRenderTargetSize( &width, &height );
    return QSize(width*2, height);
    //return QSize(width, height);
}

int VirtualRealityApiOpenVR::timeUntilNextFrame()
{
    return 1000/90;
}

void VirtualRealityApiOpenVR::swapToHeadset()
{
    vr::Texture_t leftEyeTexture = {(void*)m_fbo->handle(), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    vr::VRTextureBounds_t leftViewport = {0.0f, 0.0f, 0.5f, 1.0f};
    vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture, &leftViewport );
    vr::Texture_t rightEyeTexture = {(void*)m_fbo->handle(), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    vr::VRTextureBounds_t rightViewport = {0.5f, 0.0f, 1.0f, 1.0f};
    vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture, &rightViewport );
    m_poseNewEnough = false;
//    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
//    f->glFlush();
//    f->glFinish();
}

void VirtualRealityApiOpenVR::getEyeMatrices(QMatrix4x4 &leftEye, QMatrix4x4 &rightEye)
{
    updateHmdMatrixPose();
    leftEye = getCurrentViewMatrix(vr::Eye_Left);
    rightEye = getCurrentViewMatrix(vr::Eye_Right);
}

void VirtualRealityApiOpenVR::getProjectionMatrices(QMatrix4x4 &leftProjection, QMatrix4x4 &rightProjection)
{
    leftProjection = getHmdMatrixProjectionEye( vr::Eye_Left );
    rightProjection = getHmdMatrixProjectionEye( vr::Eye_Right );
}

QList<int> VirtualRealityApiOpenVR::currentlyTrackedObjects()
{
    QList<int> tracked;
    for( uint32_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++ ) {
        if( m_hmd->IsTrackedDeviceConnected( unTrackedDevice ) )
            tracked.push_back(unTrackedDevice);
    }
    return tracked;
}

void VirtualRealityApiOpenVR::getTrackedObject(int id, QMatrix4x4 &transform)
{
    if( id >= vr::k_unMaxTrackedDeviceCount) {
        qWarning("Requested tracked object: Index out of bounds.");
        return;
    }
    if( !m_hmd->IsTrackedDeviceConnected( id ) ) {
        qWarning((QString("Requested VR Device was not connected. (id: ") + QString::number(id) + ")").toLatin1());
        return;
    }
    transform = m_devicePose[ id ];
}

Qt3DVirtualReality::QVirtualRealityApiBackend::TrackedObjectType VirtualRealityApiOpenVR::getTrackedObjectType(int id)
{
    return Qt3DVirtualReality::QVirtualRealityApiBackend::Other;
}
bool VirtualRealityApiOpenVR::isTriggerTmp()
{
    return m_isTrigger;
}

void VirtualRealityApiOpenVR::getTrackedObjectModel(int id, QVector<float> &vertices, QVector<int> &indices, QOpenGLTexture *texture)
{
    if( id >= vr::k_unMaxTrackedDeviceCount) {
        qWarning("Requested tracked object vertices: Index out of bounds.");
        return;
    }

    if (m_models.contains(id)){
        vertices = m_models[id]->vertices;
        indices = m_models[id]->indices;
        texture = m_models[id]->texture;
        return;
    }

    std::string sRenderModelName = getTrackedDeviceString( m_hmd, id, vr::Prop_RenderModelName_String );

    //TODO: load renderModels only once

    vr::RenderModel_t *pModel;
    vr::EVRRenderModelError error;
    while ( 1 ) {
        error = vr::VRRenderModels()->LoadRenderModel_Async( sRenderModelName.c_str(), &pModel );
        if ( error != vr::VRRenderModelError_Loading )
            break;
        QThread::currentThread()->sleep( 1 );
    }

    if ( error != vr::VRRenderModelError_None ) {
        qWarning( "Unable to load render model %s - %s\n", sRenderModelName.c_str(), vr::VRRenderModels()->GetRenderModelErrorNameFromEnum( error ) );
        return;
    }

    vr::RenderModel_TextureMap_t *pTexture;
    while ( 1 ) {
        error = vr::VRRenderModels()->LoadTexture_Async( pModel->diffuseTextureId, &pTexture );
        if ( error != vr::VRRenderModelError_Loading )
            break;
        QThread::currentThread()->sleep( 1 );
    }

    if ( error != vr::VRRenderModelError_None ) {
        qWarning( "Unable to load render texture id:%d for render model %s\n", pModel->diffuseTextureId, sRenderModelName.c_str() );
        vr::VRRenderModels()->FreeRenderModel( pModel );
        return;
    }

    //Position, Normal, Texture
    vertices.resize(pModel->unVertexCount * (3 + 3 + 2));
    memcpy(vertices.data(), pModel->rVertexData, sizeof(float) * vertices.size());
    indices.resize(pModel->unTriangleCount * 3);
    for(int i=0; i<pModel->unTriangleCount; i++) {
        indices[i*3  ] = pModel->rIndexData[i*3  ];
        indices[i*3+1] = pModel->rIndexData[i*3+1];
        indices[i*3+2] = pModel->rIndexData[i*3+2];
    }

    //texture = new QOpenGLTexture(QOpenGLTexture::BindingTarget2D);
    texture->setSize(pTexture->unWidth, pTexture->unHeight);
    texture->setData( QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, pTexture->rubTextureMapData );

    m_models.insert(id,new TrackedObjectModel(vertices, indices, texture));

    vr::VRRenderModels()->FreeRenderModel( pModel );
    vr::VRRenderModels()->FreeTexture( pTexture );
}

void VirtualRealityApiOpenVR::getMirrorTexture(QOpenGLTexture *outMirrorTexture)
{

}
#endif
