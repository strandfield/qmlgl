import QtQuick 2.15

import QmlGL 1.0 
import Assimp 1.0

AppViewport {
    property alias axis: camcon.axis
    property string title: ""
    property alias frustumHeight: camcon.frustumHeight

    drawWorldFrame: true
    drawCameraOrientationAxes: false

    camera: Camera {
        controller: BboxSideCameraController {
            id: camcon
            boundingBox: q_3dmodel.boundingBox
        }
    }
}