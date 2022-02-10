import QtQuick 2.0
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QmlCustomItem 1.0
import AppEnums 1.0


Item{
    id: frameViewer

    Rectangle {
        id: imageRect
        anchors.horizontalCenter: frameViewer.horizontalCenter
        anchors.verticalCenter: frameViewer.verticalCenter

        width: 1000
        height: 600

        color: QmlConst.COLOR_INVISIBLE
        border.color: "black"
        border.width: 3

        GridLayout{

            id: grid
            anchors.fill: parent
            columns: 2
            rows: 1

            Image{
                id: leftCamera

                width: (imageRect.width-10)/2
                height: (imageRect.height-10)/2

                fillMode: Image.PreserveAspectFit
                property bool counter: false

                visible: false
                source: "image://liveLeft/image"

                asynchronous: false
                cache: false

                function reload()
                {
                    counter = !counter
                    source = "image://liveLeft/image?id=" + counter
                }

                Layout.column: 0
                Layout.row: 0

                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Image{
                id: rightCamera

                width: (imageRect.width-10)/2
                height: (imageRect.height-10)/2

                fillMode: Image.PreserveAspectFit
                property bool counter: false

                visible: false
                source: "image://liveRight/image"

                asynchronous: false
                cache: false

                function reload()
                {
                    counter = !counter
                    source = "image://liveRight/image?id=" + counter
                }

                Layout.column: 1
                Layout.row: 0

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }



    }

    Button{
        id: startButton
        x: imageRect.x/2 - startButton.width/2
        y:imageRect.height/6 + imageRect.y
        text: "Open"

        onClicked: {

            leftCamera.visible = true
            rightCamera.visible = true

            QmlHandler.qmlMessage("Camera run")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_CAMERA_RUN)
        }
    }

    Connections{
        target: liveLeftImageProvider
        function onImageChanged()
        {
            leftCamera.reload()
        }
    }

    Connections{
        target: liveRightImageProvider
        function onImageChanged()
        {
            rightCamera.reload()
        }
    }
}
