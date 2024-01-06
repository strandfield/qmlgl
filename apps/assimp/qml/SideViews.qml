import QtQuick 2.15

import Assimp 1.0

Item {
    id: sideViews

    property real separatorSize: 4
    property real viewportsWidth : sideViews.width - separatorSize

    property bool synchronizeZoom: true
    
    function notifyFrustumHeightChanged(fh) {
        if (!synchronizeZoom) return;

        topViewport.frustumHeight = fh;
        rightViewport.frustumHeight = fh;
        frontViewport.frustumHeight = fh;
    }


    Rectangle {
        id: verticalSeparator
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: contentColumn.left
        width: sideViews.separatorSize
        color: "#333333"
    }

    Column {
        id: contentColumn

        property real viewportsHeight: (parent.height - (sideViews.separatorSize * 2)) / 3
        width: 1.6 * viewportsHeight

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        SideViewport {
            id: topViewport
            title: "TOP"
            width: contentColumn.width
            height: contentColumn.viewportsHeight
            axis: BboxSideCameraController.ZAxis

            onFrustumHeightChanged: {
                notifyFrustumHeightChanged(frustumHeight);
            }
        }

        Rectangle {
            height: sideViews.separatorSize
            width: contentColumn.width
            color: "#333333"
        }

        
        SideViewport {
            id: rightViewport
            title: "RIGHT"
            width: contentColumn.width
            height: contentColumn.viewportsHeight
            axis: BboxSideCameraController.YAxis

            onFrustumHeightChanged: {
                notifyFrustumHeightChanged(frustumHeight);
            }
        }

        Rectangle {
            height: sideViews.separatorSize
            width: contentColumn.width
            color: "#333333"
        }

        SideViewport {
            id: frontViewport
            title: "FRONT"
            width: contentColumn.width
            height: contentColumn.viewportsHeight
            axis: BboxSideCameraController.XAxis

            onFrustumHeightChanged: {
                notifyFrustumHeightChanged(frustumHeight);
            }
        }
    }
}
