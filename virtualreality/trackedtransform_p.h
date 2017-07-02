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

#ifndef TRACKEDTRANSFORM_P_H
#define TRACKEDTRANSFORM_P_H

#include <Qt3DCore/qbackendnode.h>

QT_BEGIN_NAMESPACE

namespace Qt3DVirtualReality {

class TrackedTransform : public Qt3DCore::QBackendNode
{
public:
    TrackedTransform();
    
    // QBackendNode interface
protected:
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
    {
    }
    
private:
    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
    {
    }
};

} //namespace Qt3DVirtualReality

QT_END_NAMESPACE

#endif // TRACKEDTRANSFORM_P_H
