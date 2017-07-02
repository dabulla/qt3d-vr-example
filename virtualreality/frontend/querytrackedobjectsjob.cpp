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

#include "querytrackedobjectsjob_p.h"

#include <Qt3DCore/private/qaspectjob_p.h>

#include <QDebug> //TODO: remove DBG

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

namespace JobTypes {

    enum JobType {
        QueryTrackedObjects = 16384
    };

} // JobTypes

QueryTrackedObjectsJob::QueryTrackedObjectsJob()
    : QAspectJob()
    , m_apibackend(nullptr)
{
    SET_JOB_RUN_STAT_TYPE(this, JobTypes::QueryTrackedObjects, 0);
}

void QueryTrackedObjectsJob::setVirtualRealityApiBackend(QVirtualRealityApiBackend *apibackend)
{
    m_apibackend = apibackend;
}

void QueryTrackedObjectsJob::run()
{
    qDebug() << "TODO:DBG:Running query object tracked vr!";
}

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE
