import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.0 as QQ2

import vr 2.0

Entity {
    id: root

    components: RenderSettings {
        StereoFrameGraph {
            id: stereoFrameGraph
            leftCamera: vrCam.leftCamera
            rightCamera: vrCam.rightCamera
        }
    }
    Item {
        id: cameraProps
        readonly property real cameraRadius: obstaclesRepeater.radius - 50
        readonly property vector3d circlePosition: Qt.vector3d(cameraRadius * Math.cos(circleRotation), 0.0, cameraRadius * Math.sin(circleRotation))
        readonly property vector3d tan: circlePosition.crossProduct(Qt.vector3d(0, 1, 0).normalized())
        property real circleRotation: 0

        QQ2.NumberAnimation {
            target: cameraProps
            property: "circleRotation"
            from: 0; to: Math.PI * 2
            duration: 10000
            loops: QQ2.Animation.Infinite
            running: false
        }
    }
    // Camera
    VrCamera {
        id: vrCam
        offset: Qt.vector3d(80.0,-90.0,0.0)
        //offsetOrientation:
        //offset: cameraProps.circlePosition.plus(Qt.vector3d(0, 45 * Math.sin(cameraProps.circleRotation * 2), 0)).plus(cameraProps.tan.times(-2))
    }

    // Torus obsctacles
    NodeInstantiator {
        id: obstaclesRepeater
        model: 40
        readonly property real radius: 3.0
        readonly property real det: 1.0 / model
        delegate: Entity {
            components: [
                TorusMesh {
                    radius: 0.5
                    minorRadius: 0.05
                    rings: 100
                    slices: 20
                },
                Transform {
                    id: transform
                    readonly property real angle: Math.PI * 2.0 * index * obstaclesRepeater.det
                    translation: Qt.vector3d(obstaclesRepeater.radius * Math.cos(transform.angle),
                                             0.0,
                                             obstaclesRepeater.radius * Math.sin(transform.angle))
                    rotation: fromAxisAndAngle(Qt.vector3d(0.0, 1.0, 0.0), -transform.angle * 180 / Math.PI)
                },
                PhongMaterial {
                    diffuse: Qt.rgba(Math.abs(Math.cos(transform.angle)), 204 / 255, 75 / 255, 1)
                    specular: "white"
                    shininess: 20.0
                }
            ]
        }
    }
}
