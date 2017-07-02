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

#ifndef QT3DVIRTUALREALITY_QVIRTUALREALITYASPECT_P_H
#define QT3DVIRTUALREALITY_QVIRTUALREALITYASPECT_P_H

#include "qvirtualrealityaspect.h"
#include <Qt3DCore/private/qabstractaspect_p.h>
#include <QtCore/qsharedpointer.h>
#include "querytrackedobjectsjob_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class QHeadMountedDisplay;
class QVirtualRealityApi;
class QVirtualRealityApiBackend;

class QVirtualRealityAspectPrivate : public Qt3DCore::QAbstractAspectPrivate
{
    QVirtualRealityAspectPrivate();

    Q_DECLARE_PUBLIC(QVirtualRealityAspect)

    void onEngineAboutToShutdown() Q_DECL_OVERRIDE;
    void registerBackendTypes();

    qint64 m_time;
    bool m_initialized;
    QSharedPointer<QueryTrackedObjectsJob> m_queryTrackedObjectsJob;


    QHeadMountedDisplay *m_hmd;
    //QVirtualRealityApi *m_api;
    QVirtualRealityApiBackend *m_apibackend;
};

} // namespace Qt3DLogic

QT_END_NAMESPACE

#endif // QT3DVIRTUALREALITY_QVIRTUALREALITYASPECT_P_H
