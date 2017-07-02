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

#include "qvirtualrealityapi.h"
#include "qheadmounteddisplay.h"
#include "qvirtualrealityapi_p.h"

#include <QOpenGLContext>
namespace Qt3DVirtualReality {

QVirtualRealityApi::QVirtualRealityApi(Qt3DVirtualReality::QVirtualRealityApi::Type type)
    : QVirtualRealityApi()
{
    Q_D(QVirtualRealityApi);
    d->setType(type);
}

QVirtualRealityApi::QVirtualRealityApi()
    : Qt3DVirtualReality::QVirtualRealityApi(*new Qt3DVirtualReality::QVirtualRealityApiPrivate)
{}


QVirtualRealityApi::QVirtualRealityApi(Qt3DVirtualReality::QVirtualRealityApiPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{}

QVirtualRealityApi::~QVirtualRealityApi()
{
    Q_D(QVirtualRealityApi);
    d->m_apibackend->shutdown();
}

bool QVirtualRealityApi::isHmdPresent()
{
    Q_D(QVirtualRealityApi);
    return d->m_apibackend->isHmdPresent();
}

bool QVirtualRealityApi::isRuntimeInstalled(Qt3DVirtualReality::QVirtualRealityApi::Type type)
{
    return QVirtualRealityApiPrivate::isRuntimeInstalled(type);
}

QHeadMountedDisplay* QVirtualRealityApi::getHmd(int hmdId, const QHeadMountedDisplayFormat &format)
{
    Q_D(QVirtualRealityApi);
    Qt3DVirtualReality::QHeadMountedDisplay *hmd(new Qt3DVirtualReality::QHeadMountedDisplay(hmdId, format, this, d->m_apibackend));
    hmd->context()->makeCurrent(static_cast<QOffscreenSurface*>(hmd->surface()));
    d->initialize();
    return hmd;
}
}
