!include( ../vr-sdks.pri ) {
    error( "Couldn't find the vr-sdks.pri file!" )
}

QT += 3dcore 3drender 3dinput 3dquick qml quick 3dquickextras

CONFIG += link_prl

HEADERS += \

SOURCES += \
    main.cpp

OTHER_FILES += \
    main.qml \
    *.qml

RESOURCES += \
    vr.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../virtualreality/release/ -lvirtualreality
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../virtualreality/debug/ -lvirtualreality
else:unix: LIBS += -L$$OUT_PWD/../virtualreality/ -lvirtualreality

INCLUDEPATH += $$PWD/../virtualreality
DEPENDPATH += $$PWD/../virtualreality
