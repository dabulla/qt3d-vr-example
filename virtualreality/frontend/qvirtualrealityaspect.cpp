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

#include "qvirtualrealityaspect.h"
#include "qvirtualrealityaspect_p.h"
#include "querytrackedobjectsjob_p.h"

using namespace Qt3DCore;

namespace Qt3DVirtualReality {

QVirtualRealityAspectPrivate::QVirtualRealityAspectPrivate()
    : QAbstractAspectPrivate()
    , m_time(0)
    , m_initialized(false)
    , m_queryTrackedObjectsJob(new Qt3DVirtualReality::QueryTrackedObjectsJob)
    , m_hmd(nullptr)
    , m_apibackend(nullptr)
{
}

void QVirtualRealityAspectPrivate::onEngineAboutToShutdown()
{

}

void QVirtualRealityAspectPrivate::registerBackendTypes()
{
    Q_Q(QVirtualRealityAspect);
    //q->registerBackendType<QQueryTrackedObjectsJob>();
    //q->registerBackendType<QVirtualRealityTrackedObjectInstantiator>();
}

/*!
  Constructs a new QVirtualRealityAspect instance with \a parent.
*/
QVirtualRealityAspect::QVirtualRealityAspect(QObject *parent)
    : QVirtualRealityAspect(*new QVirtualRealityAspectPrivate(), parent) {}

QVirtualRealityAspect::QVirtualRealityAspect(QVirtualRealityAspectPrivate &dd, QObject *parent)
    : QAbstractAspect(dd, parent)
{
    Q_D(QVirtualRealityAspect);
    setObjectName(QStringLiteral("Virtual Reality Aspect"));
    d->registerBackendTypes();
}

QVirtualRealityAspect::~QVirtualRealityAspect()
{
}

void QVirtualRealityAspect::setHeadmountedDisplay(QHeadMountedDisplay *hmd)
{
    Q_D(QVirtualRealityAspect);
    d->m_hmd = hmd;
}

void QVirtualRealityAspect::setVirtualRealityApiBackend(QVirtualRealityApiBackend *apiBackend)
{
    Q_D(QVirtualRealityAspect);
    d->m_apibackend = apiBackend;
    d->m_queryTrackedObjectsJob->setVirtualRealityApiBackend(d->m_apibackend);
}

QVector<Qt3DCore::QAspectJobPtr> QVirtualRealityAspect::jobsToExecute(qint64 time)
{
    Q_D(QVirtualRealityAspect);
    QVector<Qt3DCore::QAspectJobPtr> jobs;
    jobs.append(d->m_queryTrackedObjectsJob);
    return jobs;
}

void QVirtualRealityAspect::onRegistered()
{
}

void QVirtualRealityAspect::onUnregistered()
{
}

///*! \internal */
//void QVirtualRealityAspect::onEngineStartup()
//{
//    Q_D(QLogicAspect);
//    d->m_executor->setScene(d->m_arbiter->scene());
//}

QVariant QVirtualRealityAspect::executeCommand(const QStringList &args)
{
    return QVariant();
}

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

QT3D_REGISTER_NAMESPACED_ASPECT("virtualreality", QT_PREPEND_NAMESPACE(Qt3DVirtualReality), QVirtualRealityAspect)

