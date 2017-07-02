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

#ifndef QT3DVIRTUALREALITYAPIBACKEND_H
#define QT3DVIRTUALREALITYAPIBACKEND_H

#include "qt3dvr_global.h"
#include <QOpenGLFramebufferObject>
#include <QMatrix4x4>
#include <qopengl.h>
#include <QSize>
#include <QSurfaceFormat>
#include <QOpenGLTexture>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

/*!
 * \brief The QVirtualRealityApiBackend class hides the concrete implementation for a vr headset.
 * It should make it easy to add more Virtual reality devices. However, becaus we can't look into the future
 * this interface might be extended with new features.
 */
class QVirtualRealityApiBackend
{
public:
    enum TrackedObjectType {
        Head,
        LeftHand,
        RightHand,
        LighthouseOrSensor,
        Other
    };
    virtual bool isHmdPresent() = 0;

    /*!
     * \brief initialize is to initialize the concrete vr framework.
     * This is called while correct opengl context is current and already initialized
     * It is also known, that the runtime is installed at this point
     */
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    /*!
     * \brief bindFrambufferObject for rendering. Has to bind the framebuffer of the current
     * headset for rendering.
     * \return success
     */
    virtual bool bindFrambufferObject(int hmdId) = 0;

    /*!
     * \brief refreshRate of the selected headset. E.g. 90 Hz
     * \param hmdId
     * \return
     */
    virtual qreal refreshRate(int hmdId) const = 0;

    /*!
     * \brief headPose estimated for the next frame
     * //TO DO: add estimation parameters.
     * It is likely that this function never returns the same headpose twice.
     * \param hmdId
     * \return
     */
    virtual QMatrix4x4 headPose(int hmdId) = 0;

    //TO DO: introduce getRecomendedSize()
    virtual QSize getRenderTargetSize() = 0;

    virtual int timeUntilNextFrame() = 0;
    virtual void swapToHeadset() = 0;

    /*!
     * \brief getEyeMatrices without projection applied. Relative to transform-origin.
     * //TO DO: Add transform origin concept. Introduce a way to get interpupilar distance and offset to headPose.?
     * \param leftEye
     * \param rightEye
     */
    virtual void getEyeMatrices(QMatrix4x4 &leftEye, QMatrix4x4 &rightEye) = 0;

    /*!
     * \brief getProjectionMatrices get correct projection matrix for each eye. These can be asymetrical.
     * \param leftProjection
     * \param rightProjection
     */
    virtual void getProjectionMatrices(QMatrix4x4 &leftProjection, QMatrix4x4 &rightProjection) = 0;

    virtual int numberOfTrackedObjects() = 0;
    virtual void getTrackedObject(int id, QMatrix4x4 &transform) = 0;
    virtual TrackedObjectType getTrackedObjectType(int id) = 0;
    virtual void getTrackedObjectVertices(int id, QVector<float> &vertices) = 0;
    virtual void getTrackedObjectTexture(int id, QOpenGLTexture *texture) = 0;

    /*!
     * \brief getMirrorTexture
     * Used to get a texture witch can be rendered in another window/thread.
     * Some SDKs will not automaticly fill the QOpenGLTextue itself but return an OpenGL texture name.
     * This method has to be called every time the mirror should update to allow copying in this case.
     * It might work for some SDKs, but not for others.
     * \param outMirrorTexture
     */
    virtual void getMirrorTexture(QOpenGLTexture *outMirrorTexture) = 0;

};

}

QT_END_NAMESPACE

#endif
