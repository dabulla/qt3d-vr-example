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

#ifndef QVRCAMERA
#define QVRCAMERA
#include <QObject>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCameraLens>
#include <qvirtualrealityapibackend.h> //TO DO: THis include is only for transforms over camera

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class QVirtualrealityCamera : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QCameraLens * leftCameraLens READ leftCameraLens NOTIFY leftCameraLensChanged)
    Q_PROPERTY(Qt3DRender::QCameraLens * rightCameraLens READ rightCameraLens NOTIFY rightCameraLensChanged)
    Q_PROPERTY(Qt3DCore::QEntity * leftCamera READ leftCamera NOTIFY leftCameraChanged)
    Q_PROPERTY(Qt3DCore::QEntity * rightCamera READ rightCamera NOTIFY rightCameraChanged)
    Q_PROPERTY(float disparity READ disparity NOTIFY disparityChanged)
//    Q_PROPERTY(QVector3D headPosPredicted READ headPosPredicted NOTIFY headPosPredictedChanged)
//    Q_PROPERTY(QQuaternion headOrientationPredicted READ headOrientationPredicted NOTIFY headOrientationPredictedChanged)
    Q_PROPERTY(QVector3D offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QQuaternion offsetOrientation READ offsetOrientation WRITE setOffsetOrientation NOTIFY offsetOrientationChanged)
//    Q_PROPERTY(QMatrix4x4 viewMatrixPredicted READ viewMatrixPredicted NOTIFY viewMatrixPredictedChanged)
    Q_PROPERTY(float playerHeight READ playerHeight NOTIFY playerHeightChanged)
    Q_PROPERTY(QRectF leftNormalizedViewportRect READ leftNormalizedViewportRect WRITE setLeftNormalizedViewportRect NOTIFY leftNormalizedViewportRectChanged)
    Q_PROPERTY(QRectF rightNormalizedViewportRect READ rightNormalizedViewportRect WRITE setRightNormalizedViewportRect NOTIFY rightNormalizedViewportRectChanged)
public:
    QVirtualrealityCamera(QNode *parent = nullptr);

    Qt3DCore::QEntity * leftCamera();
    Qt3DCore::QEntity * rightCamera();

    void setProjections(const QMatrix4x4& leftProjection, const QMatrix4x4& rightProjection);
    void update(const QMatrix4x4 &viewLeft, const QMatrix4x4 &viewRight);
    float disparity() const;

//    QMatrix4x4 headPosPredicted() const;
//    QMatrix4x4 headOrientationPredicted() const;
    QVector3D offset() const;
//    QMatrix4x4 viewMatrixPredicted() const;
    float playerHeight() const;
    QRectF leftNormalizedViewportRect() const;
    QRectF rightNormalizedViewportRect() const;
    QQuaternion offsetOrientation() const;

    // Transforms should be handled by special "VrTransform" Class, which has two backend nodes (one in renderaspect and one in vr aspect).
    // Then it should always uptade before rendering. For the moment, this will create a minimium viable product.
    Q_INVOKABLE QMatrix4x4 trackedObjectMatrixTmp(int trackedObjectId);
    Q_INVOKABLE QList<int> trackedObjectsTmp();
    void setVrBackendTmp(QVirtualRealityApiBackend* backend); //only for trackedObjectMatrixTmp
    Q_INVOKABLE bool isTriggerTmp();
    Qt3DRender::QCameraLens * leftCameraLens() const
    {
        return m_leftCameraLens;
    }

    Qt3DRender::QCameraLens * rightCameraLens() const
    {
        return m_rightCameraLens;
    }

public Q_SLOTS:

    void setOffset(QVector3D offset);
    void setLeftNormalizedViewportRect(QRectF leftNormalizedViewportRect);
    void setRightNormalizedViewportRect(QRectF rightNormalizedViewportRect);
    void setOffsetOrientation(QQuaternion offsetOrientation);

Q_SIGNALS:

    void disparityChanged(float disparity);
//    void headPosPredictedChanged(QVector3D headPosPredicted);
//    void headOrientationPredictedChanged(QMatrix4x4 headOrientationPredicted);
    void offsetChanged(QVector3D offset);
//    void viewMatrixPredictedChanged(QMatrix4x4 viewMatrixPredicted);
    void playerHeightChanged(float playerHeight);
    void leftNormalizedViewportRectChanged(QRectF leftNormalizedViewportRect);
    void rightNormalizedViewportRectChanged(QRectF rightNormalizedViewportRect);
    void leftCameraChanged(Qt3DCore::QEntity * leftCamera);
    void rightCameraChanged(Qt3DCore::QEntity * rightCamera);
    void offsetOrientationChanged(QQuaternion offsetOrientation);
    void leftCameraLensChanged(Qt3DRender::QCameraLens * leftCameraLens);
    void rightCameraLensChanged(Qt3DRender::QCameraLens * rightCameraLens);

private:
    Qt3DRender::QCameraLens *m_leftCameraLens;
    Qt3DRender::QCameraLens *m_rightCameraLens;
    Qt3DCore::QTransform *m_leftTransform;
    Qt3DCore::QTransform *m_rightTransform;
    Qt3DCore::QEntity *m_leftCamera;
    Qt3DCore::QEntity *m_rightCamera;
    float m_disparity;
//    QVector3D m_headPosPredicted;
//    QQuaternion m_headOrientationPredicted;
    QVector3D m_offset;
//    QMatrix4x4 m_viewMatrixPredicted;
    float m_playerHeight;
    QRectF m_leftNormalizedViewportRect;
    QRectF m_rightNormalizedViewportRect;
    QQuaternion m_offsetOrientation;
    QVirtualRealityApiBackend *m_apibackend; //TO DO: tmp

};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif
