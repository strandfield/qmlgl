import QtQuick 2.15

import QmlGL 1.0 
import HeightMap 1.0

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
    }
    
    Text {
        anchors.margins: 8
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text: "Camera position: " + `(${mainViewport.camera.position.x.toFixed(3)}, ${mainViewport.camera.position.y.toFixed(3)}, ${mainViewport.camera.position.z.toFixed(3)})`
        color: "white"
        font.pixelSize: 14
    }

    function digAHole(zvalues, rows, mincol, maxcol, minrow, maxrow) {
        for (let x = mincol; x < maxcol; x++) {
            for (let y = minrow; y < maxrow; y++) {
                let i = x * rows + y;
                if (i < zvalues.length) {
                    zvalues[i] = -1;
                }
            }
        }
    }

    function generateHeightMap(rows, cols) {
        let zvalues = [];

        for (let x = 0; x < cols; x++) {
            for (let y = 0; y < rows; y++) {
                let z = (Math.cos(x / 30) + Math.sin(y / 50)) / 4 + 0.5;
                zvalues.push(z);
            }
        }

        digAHole(zvalues, rows, 20, 40, 20, 40);
        digAHole(zvalues, rows, 120, 160, 90, 130);

        return {"zvalues": zvalues, "cols": cols };
    }

    function genHeightMap() {
        let cols = 256;
        let rows = cols;
        return generateHeightMap(rows, cols);
    }

    /*
    HeightMapObject {
        id: myHeightmap

        Component.onCompleted: {
            myHeightmap.fill(genHeightMap());
        }
    }
    */

    property HeightMapObject myHeightmap: heightfield_model.heightmap

    Component.onCompleted: {
            myHeightmap.fill(genHeightMap());
            myHeightmap.setGeometry({
              bottom_left: { x: -10, y: -10 },
              top_right: {  x: 10, y: 10  }
            });
            myHeightmap.altMin = -1;
            myHeightmap.altMax = 2;
            heightfield_model.size = Qt.size(20, 20);
        }

    HeightMapImage {
        id: heightmapImage
        heightmap: myHeightmap
    }

    Image {
        id: incrustedImage
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 8
        source: heightmapImage.imageUrl
        visible: false
    }
    
    property bool showUserInterface: true

    LeftPane {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: showUserInterface
    }

    Shortcut {
        sequence: "A"
        onActivated: {
            mainViewport.drawCameraOrientationAxes = !mainViewport.drawCameraOrientationAxes;
        }
    }

    Shortcut {
        sequence: "I"
        onActivated: {
            mainWindow.showUserInterface = !mainWindow.showUserInterface;
        }
    }

    Shortcut {
        sequence: "M"
        onActivated: {
            incrustedImage.visible = !incrustedImage.visible;
        }
    }
}
