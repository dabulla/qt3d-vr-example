!include( ../vr-sdks.pri ) {
    error( "Couldn't find the vr-sdks.pri file!" )
}

TARGET   = virtualreality

TEMPLATE = lib

DEFINES += QT3DVR_LIBRARY

#MODULE   = virtualreality
QT      += qml quick \
           core core-private 3dcore 3dcore-private 3drender 3drender-private 3dinput 3dlogic 3dquick \
           qml qml-private 3dquick 3drender 3drender-private 3dlogic

# Qt3D is free of Q_FOREACH - make sure it stays that way:
DEFINES += QT_NO_FOREACH

#load(qt_module)

CONFIG += c++11

# track dependecies
CONFIG += create_prl

SOURCES += \
    vrbackends/ovr/virtualrealityapiovr.cpp \
    vrbackends/ovr/framebufferovr.cpp \
    vrbackends/openvr/virtualrealityapiopenvr.cpp \
    qvirtualrealityapi.cpp \
    qheadmounteddisplay.cpp \
    frontend/qvirtualrealityaspect.cpp \
    frontend/qvirtualrealitycamera.cpp \
    frontend/qvirtualrealitymesh.cpp \
    frontend/querytrackedobjectsjob.cpp \
    qvirtualrealitygeometry.cpp \
    handler.cpp \
    frontend/qtrackedtransform.cpp \
    trackedtransform.cpp

HEADERS += \
    vrbackends/ovr/virtualrealityapiovr.h \
    vrbackends/ovr/framebufferovr.h \
    vrbackends/openvr/virtualrealityapiopenvr.h \
    qvirtualrealityapi.h \
    qvirtualrealityapi_p.h \
    qvirtualrealityapibackend.h \
    qheadmounteddisplay.h \
    qt3dvr_global.h \
    frontend/qvirtualrealityaspect.h \
    frontend/qvirtualrealityaspect_p.h \
    frontend/qvirtualrealitycamera.h \
    frontend/qvirtualrealitymesh.h \
    frontend/querytrackedobjectsjob_p.h \
    qvirtualrealitygeometry.h \
    qvirtualrealitygeometry_p.h \
    handler_p.h \
    frontend/qtrackedtransform.h \
    trackedtransform_p.h

###### OpenVR ######
if($$WITH_VR_SDK_OPENVR) {
  message("Building with OpenVR support")
  INCLUDEPATH += $$VR_SDK_OPENVR_INCLUDE
  LIBS += -L$$VR_SDK_OPENVR_LIB_PATH
  LIBS += "$$VR_SDK_OPENVR_LIB_PATH/libopenvr_api.so"
  #LIBS += -L$$VR_SDK_OPENVR_LIB_PATH -lopenvr_api
  DEFINES+="QT3DVR_COMPILE_WITH_OPENVR=1"
} else {
  DEFINES+="QT3DVR_COMPILE_WITH_OPENVR=0"
}

###### OculusVR ######
if($$WITH_VR_SDK_OCULUSVR) {
  message("Building with Oculus support")
  INCLUDEPATH += $$VR_SDK_OCULUSVR_INCLUDE
  LIBS += -L$$VR_SDK_OCULUSVR_LIB_PATH  -lLibOVR
  DEFINES+="QT3DVR_COMPILE_WITH_OCULUSVR=1"
} else {
  DEFINES+="QT3DVR_COMPILE_WITH_OCULUSVR=0"
}

###### OSVR ######
if($$WITH_VR_SDK_OSVR) {
  message("Building with OSVR support")
  INCLUDEPATH += $$VR_SDK_OSVR_INCLUDE
  LIBS += -L$$VR_SDK_OSVR_LIB_PATH -l
  DEFINES+="QT3DVR_COMPILE_WITH_OSVR=1"
} else {
  DEFINES+="QT3DVR_COMPILE_WITH_OSVR=0"
}

win32 {
  LIBS += -ldxgi
}
