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

#ifndef QT3DVIRTUALREALITY_QUERYTRACKEDPOBJECTS_P_H
#define QT3DVIRTUALREALITY_QUERYTRACKEDPOBJECTS_P_H

#include <Qt3DCore/qaspectjob.h>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class QVirtualRealityApiBackend;

class QueryTrackedObjectsJob : public Qt3DCore::QAspectJob
{
public:
    QueryTrackedObjectsJob();
    void setVirtualRealityApiBackend(QVirtualRealityApiBackend *apibackend);

    void run() Q_DECL_OVERRIDE;

private:
    QVirtualRealityApiBackend *m_apibackend;
};

} // namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // QT3DLOGIC_LOGIC_CALLBACKJOB_P_H
