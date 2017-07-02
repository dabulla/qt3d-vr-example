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

#ifndef QT3DVIRTUALREALITYAPI_P_H
#define QT3DVIRTUALREALITYAPI_P_H

#include "qvirtualrealityapi.h"

#include <QtOpenGL/QGL>
#include <private/qobject_p.h>
#include "qvirtualrealityapibackend.h"

#if(QT3DVR_COMPILE_WITH_OCULUSVR)
#  include "vrbackends/ovr/virtualrealityapiovr.h"
#endif
#if(QT3DVR_COMPILE_WITH_OPENVR)
#  include "vrbackends/openvr/virtualrealityapiopenvr.h"
#endif
#if(QT3DVR_COMPILE_WITH_OSVR)
#  include "vrbackends/osvr/vrapiosvr.h"
#endif

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class QVirtualRealityApiPrivate : public QObjectPrivate
{
public:

    Q_DECLARE_PUBLIC(QVirtualRealityApi)

    //VrApiBackend
    QVirtualRealityApiBackend *m_apibackend;

    void setType(QVirtualRealityApi::Type vendor) {
#if(QT3DVR_COMPILE_WITH_OCULUSVR)
        if(QVirtualRealityApi::OculusVR == vendor) {
            m_apibackend = new VirtualRealityApiOvr();
            return;
        }
#endif
#if(QT3DVR_COMPILE_WITH_OPENVR)
        if(QVirtualRealityApi::OpenVR == vendor) {
            m_apibackend = new VirtualRealityApiOpenVR();
            return;
        }
#endif
    }
    static bool isRuntimeInstalled(QVirtualRealityApi::Type vendor) {
#if(QT3DVR_COMPILE_WITH_OCULUSVR)
        if(QVirtualRealityApi::OculusVR == vendor) {
            return VirtualRealityApiOvr::isRuntimeInstalled();
        }
#endif
#if(QT3DVR_COMPILE_WITH_OPENVR)
        if(QVirtualRealityApi::OpenVR == vendor) {
            return VirtualRealityApiOpenVR::isRuntimeInstalled();
        }
#endif
    }
    void initialize() {
        if(!m_initialized) {
            Q_ASSERT(m_apibackend != nullptr);
            m_apibackend->initialize();
            m_initialized = true;
        }
    }
    QVirtualRealityApiPrivate()
      : m_apibackend(nullptr)
      , m_initialized(false)
    {

    }

    ~QVirtualRealityApiPrivate() {
        if(m_apibackend)
            delete m_apibackend;
    }
private:
    bool m_initialized;
};

}

QT_END_NAMESPACE

#endif
