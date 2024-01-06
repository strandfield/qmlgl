import QtQuick 2.15

import QmlGL 1.0 

Item {
    id: mainWindow

    width: 1024
    height: 768

    AppViewport {
        id: mainViewport
        anchors.fill:parent

        drawWorldFrame: false
        drawCameraOrientationAxes: true

        camera: Camera {
            controller: OrbitalCameraController {
                target: Qt.vector3d(0, 0, 0.3)
            }
        }

        Rectangle {
            property real scaleRatio: myMouseArea.containsMouse ? 0.25 : 0.16
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 8
            width: scaleRatio * parent.width
            height: width * (parent.height / parent.width)
            color: "transparent"
            border.width: 2
            border.color: "gray"
            
            AppViewport {
                anchors.fill: parent
                
                drawWorldFrame: false
                drawCameraOrientationAxes: false
                
                camera: Camera {
                    controller: PictureInPictureCameraController {
                        sourceCamera: mainViewport.camera
                        distToViewCenter: 2.9
                    }
                }
                
                z: 1
            }
            
            MouseArea {
                id: myMouseArea
                hoverEnabled: true
                anchors.fill: parent
                z: 2
            }
        }
    }
    
    Text {
        anchors.margins: 8
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text: "Camera position: " + `(${mainViewport.camera.position.x.toFixed(3)}, ${mainViewport.camera.position.y.toFixed(3)}, ${mainViewport.camera.position.z.toFixed(3)})`
        color: "white"
        font.pixelSize: 14
    }
}
