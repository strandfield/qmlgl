import QtQuick 2.15
import QtQuick.Controls 2.15

import QmlGL 1.0 
import HeightMap 1.0

Rectangle {
    border.width: 0
    color: "grey"

    property real margins: 8
    width: content.width + 2 * margins

    Column {
        id: content
        x: margins
        y: margins

        spacing: 6

        Text {
            text: "Mesh size"
        }

        SpinBox {
            id: myHeightFieldSizeSpinBox
            value: heightfield_model.size.width
            from: 1
            to: 100
            editable: true
            
            onValueModified: {
                heightfield_model.size = Qt.size(value, value);
            }
        }

        Text {
            text: "Mesh resolution (cm)"
        }

        SpinBox {
            id: myHeightFieldResolutionSpinBox
            value: heightfield_model.resolution * 100
            from: 1
            to: 20 * 1000
            editable: true
            
            onValueModified: {
                heightfield_model.resolution = value / 100;
            }
        }

        Text {
            text: "Mesh origin (cm)"
        }

        function updateMeshOrigin() {
            heightfield_model.origin = Qt.vector3d(myHeightFieldMeshOriginXSpinBox.value / 100, myHeightFieldMeshOriginYSpinBox.value / 100, 0);
        }

        Row {
            spacing: 6

            SpinBox {
                id: myHeightFieldMeshOriginXSpinBox
                value: heightfield_model.origin.x * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    content.updateMeshOrigin();
                }
            }

            SpinBox {
                id: myHeightFieldMeshOriginYSpinBox
                value: heightfield_model.origin.y * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    content.updateMeshOrigin();
                }
            }
        }

        Text {
            text: "Heightmap altitude (cm)"
        }

        Row {
            spacing: 6

            SpinBox {
                value: heightfield_model.heightmap.altMin * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    heightfield_model.heightmap.altMin = value / 100;
                }
            }

            SpinBox {
                value: heightfield_model.heightmap.altMax * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    heightfield_model.heightmap.altMax = value / 100;
                }
            }
        }

        function updateHeightMapGeometry() {
            let geom = {
                "bottom_left": {
                    "x": myHeightMapBottomLeftXSpinBox.value / 100,
                    "y": myHeightMapBottomLeftYSpinBox.value / 100
                },
                "top_right": {
                    "x": myHeightMapTopRightXSpinBox.value / 100,
                    "y": myHeightMapTopRightYSpinBox.value / 100
                }
            };

            heightfield_model.heightmap.setGeometry(geom);
        }

        Text {
            text: "Heightmap bottom left (cm)"
        }

        Row {
            spacing: 6

            SpinBox {
                id: myHeightMapBottomLeftXSpinBox
                value: heightfield_model.heightmap.bottomLeft.x * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    content.updateHeightMapGeometry();
                }
            }

            SpinBox {
                id: myHeightMapBottomLeftYSpinBox
                value: heightfield_model.heightmap.bottomLeft.y  * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    content.updateHeightMapGeometry();
                }
            }
        }

        Text {
            text: "Heightmap top right (cm)"
        }

        Row {
            spacing: 6

            SpinBox {
                id: myHeightMapTopRightXSpinBox
                value: heightfield_model.heightmap.topRight.x  * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    content.updateHeightMapGeometry();
                }
            }

            SpinBox {
                id: myHeightMapTopRightYSpinBox
                value: heightfield_model.heightmap.topRight.y * 100
                from: -100 * 1000
                to: 100 * 1000
                editable: true
                
                onValueModified: {
                    content.updateHeightMapGeometry();
                }
            }
        }

        Image {
            width: Math.min(parent.width - 8, sourceSize.width)
            anchors.margins: 8
            source: heightmapImage.imageUrl

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    heightmap_controller.openDialog();
                }
            }
        }

        /*
        Canvas {
            id: canvas
            width: 100
            height: 100

            onPaint: {
                const ctx = canvas.getContext("2d");
                
                // Create a conic gradient
                // The start angle is 0
                // The center position is 50, 50
                const gradient = ctx.createConicalGradient(50, 50, 0);
                
                // Add five color stops
                gradient.addColorStop(0, "red");
                gradient.addColorStop(0.25, "orange");
                gradient.addColorStop(0.5, "yellow");
                gradient.addColorStop(0.75, "green");
                gradient.addColorStop(1, "blue");
                
                // Set the fill style and draw a rectangle
                ctx.fillStyle = gradient;
                ctx.fillRect(20, 20, 70, 70);

                //ctx.beginPath();
                //ctx.ellipse(100, 100, 50, 75, Math.PI / 4, 0, 2 * Math.PI);
                //ctx.fill();

            }
        }
        */
    }
}
