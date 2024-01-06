import QtQuick 2.15

import QmlGL 1.0 

Item {
    id: mainWindow

    width: 1024
    height: 768

    property bool hasLoadedModel: q_3dmodel.filePath !== ""
    property real separatorSize: 4

    AppViewport {
        id: mainViewport
        x: 0
        y: 0
        height: parent.height
        width:  {
            if (!hasLoadedModel)
                return parent.width
            else 
                return parent.width - (1.6 * (parent.height - separatorSize * 2) / 3)
        }

        drawWorldFrame: true
        drawCameraOrientationAxes: true

        /*Connections {
            target: q_3dmodel

            function onModelCenterChanged() {
                console.log(q_3dmodel.modelCenter);
            }
        }*/

        camera: Camera {
            controller: OrbitalCameraController {
                //target: q_3dmodel.modelCenter
                target: q_3dmodel.boundingBox ? q_3dmodel.boundingBox.center : Qt.vector3d(0, 0, 0)
            }
        }
    }

    SideViews {
        anchors.left: mainViewport.right
        anchors.right: parent.right
        height: parent.height
        visible: hasLoadedModel
    }

    DropArea {
        id: dropArea
        anchors.fill: parent
        onEntered: {
            drag.accept(Qt.LinkAction);
        }
        onDropped: {
            q_3dmodel_controller.openModel(drop.urls[0]);
        }
        onExited: {
        }
    }
    
    Text {
        id: cameraPosText
        anchors.margins: 8
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        //text: "Camera position: " + `(${mainViewport.camera.position.x.toFixed(3)}, ${mainViewport.camera.position.y.toFixed(3)}, ${mainViewport.camera.position.z.toFixed(3)})`
        text: q_3dmodel.filePath === "" ? "No models loaded" : q_3dmodel.filePath
        color: "white"
        font.pixelSize: 14
    }
}
