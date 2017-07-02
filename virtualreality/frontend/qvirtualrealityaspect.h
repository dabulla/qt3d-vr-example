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

#ifndef QT3DVIRTUALREALITY_QVIRTUALREALITYASPECT_H
#define QT3DVIRTUALREALITY_QVIRTUALREALITYASPECT_H

#include <Qt3DCore/qabstractaspect.h>
#include <qt3dvr_global.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
class QRenderAspect;
}

namespace Qt3DVirtualReality {

class QVirtualRealityAspectPrivate;
class QHeadMountedDisplay;
//class QVirtualRealityApi;
class QVirtualRealityApiBackend;

//TO DO: might needs access to RenderAspect. UpdateWorldTransformJob must depend on QueryTrackedObjectsJob

class QT3DVR_EXPORT QVirtualRealityAspect : public Qt3DCore::QAbstractAspect
{
    Q_OBJECT
public:
    explicit QVirtualRealityAspect(QObject *parent = nullptr);
    ~QVirtualRealityAspect();

    void setHeadmountedDisplay(QHeadMountedDisplay *hmd);
    //void setVirtualRealityApi(QVirtualRealityApi *api);
    void setVirtualRealityApiBackend(QVirtualRealityApiBackend *apiBackend);
private:
    QVariant executeCommand(const QStringList &args) Q_DECL_OVERRIDE;
    QVector<Qt3DCore::QAspectJobPtr> jobsToExecute(qint64 time) Q_DECL_OVERRIDE;
    void onRegistered() Q_DECL_OVERRIDE;
    void onUnregistered() Q_DECL_OVERRIDE;

protected:
    explicit QVirtualRealityAspect(QVirtualRealityAspectPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(QVirtualRealityAspect)
};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QT3DVIRTUALREALITY_QVIRTUALREALITYASPECT_H
