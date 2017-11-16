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

#include <Qt3DQuickExtras/qt3dquickwindow.h>
#include <Qt3DQuick/QQmlAspectEngine>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include "qvirtualrealityapi.h"
#include "qheadmounteddisplay.h"

int main(int argc, char* argv[])
{
    qDebug() << "VR Window demo";
    QGuiApplication app(argc, argv);
    Qt3DVirtualReality::QVirtualRealityApi::Type requestedVrApi(Qt3DVirtualReality::QVirtualRealityApi::OpenVR);
    bool apiAvialable = Qt3DVirtualReality::QVirtualRealityApi::isRuntimeInstalled(requestedVrApi);
    if(!apiAvialable)
    {
        qDebug() << "Vr API not available. Try to choose another VR SDK or recompile with support for other VR SDK.";
        return 1;
    }
    qDebug() << "Starting...";
    Qt3DVirtualReality::QVirtualRealityApi vrapi(requestedVrApi);
    Qt3DVirtualReality::QHeadMountedDisplayFormat fmt;
    Qt3DVirtualReality::QHeadMountedDisplay *hmd(vrapi.getHmd(0, fmt));
    if( hmd == nullptr ) {
        qDebug() << "Head Mounted disply could not be initialized";
        return 1;
    }
    // Expose the head mounted display as a context property so we can set the aspect ratio
    hmd->engine()->qmlEngine()->rootContext()->setContextProperty("_hmd", hmd);
    hmd->setSource(QUrl("qrc:/main.qml"));

    hmd->run();
    return app.exec();
}
