# qt3d-vr-example
Qt3d example with support for OpenVR and OculusVR natively

## Setup
Change _vr-sdks.pri_ to fit your directory structure and installation path for vr sdks.
The current implementation uses private headers of Qt3d 5.9.
If the application crashes at startup, it could be that the vr library is incompatible to your build.

### OpenVR
You might have to put _openvr_api.dll_ into the executable directory.
Incompatible configurations of the dll with your build can be visualized using the tool "dependency walker".

### OculusVR (LibOVR.lib)
Note that the Oculus SDK (LibOVR.lib) is built with incompatible ABI to QtCreator/Visual Studio standard settings.
* LibOVR.lib compiles with static runtime by default (/MT, /MTd).
* Recompiling the lib using is the recommended way to solve this (simple task).
* Visual Studio projects for LibOVR.lib are part of the OculusSDK
* In the Visual Studio Property Pages go to "C/C++ -> Codegeneration -> Runtime Library" and select Multithreaded-Debug (/MTd) for the Debug configuration. (...and /MT for the release config)
* For 32bit you might add "\_ITERATOR_DEBUG_LEVEL=0" under C/C++ -> Preprocessor -> Preprocessordefinitions

# API

The project vr-window is an example usage.
The project virtualreality is a library.

In order to render to the headset, QHeadMountedDisplay must be used in favor of QQuickWindow.
VR Will render only to the Headset. It is not possible to mirror something to the desktop yet (e.g. as a Qml element). This is because VR takes control of the rendering thread. In the future mirroring might be possible, but will very likely use a different Qml scene.

```cpp
int main(int argc, char* argv[])
    QGuiApplication app(argc, argv);
    auto requestedVrApi = QVirtualRealityApi::OculusVR;
    bool apiAvialable = QVirtualRealityApi::isRuntimeInstalled(requestedVrApi);
    if(!apiAvialable) {
        ...
    }
    QVirtualRealityApi vrapi(requestedVrApi);
    QHeadMountedDisplayFormat fmt;
    QHeadMountedDisplay *hmd(vrapi.getHmd(0, fmt));
    hmd->setSource(QUrl("qrc:/main.qml"));
    hmd->run();
    return app.exec();
```

In the qt3d scene, VrCamera must be used to enable head tracking. Moreover the rendertarget must have two viewports for stereoscopic rendering.

```qml
VrCamera {
    id: vrCam
    offset: Qt.vector3d(80.0,-90.0,0.0)
}
```

And in the framegraph the setup looks like this:
```qml
StereoFrameGraph {
    id: stereoFrameGraph
    leftCamera: vrCam.leftCamera
    rightCamera: vrCam.rightCamera
}
RenderSurfaceSelector {
    id: surfaceSelector
    externalRenderTargetSize: _hmd.renderTargetSize

    // Draw with left eye
    CameraSelector {
        camera: vrCam.leftCamera
        Viewport {
            ...
            normalizedRect: Qt.rect(0,0,0.5,1)
        }
    }

    // Draw with right eye
    CameraSelector {
        camera: vrCam.rightCamera
        Viewport {
            ...
            normalizedRect: Qt.rect(0.5,0,0.5,1)
        }
    }
}
```
