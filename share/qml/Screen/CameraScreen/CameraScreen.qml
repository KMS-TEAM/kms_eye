import QtQuick 2.0
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Item {
    id: cameraScreen

    Rectangle {
        id: imageRect
        anchors.horizontalCenter: cameraScreen.horizontalCenter
        anchors.verticalCenter: cameraScreen.verticalCenter

        width: 600
        height: 400

        color: QmlConst.COLOR_INVISIBLE
        border.color: "black"
        border.width: 3

        Image{
            id: opencvImage
            anchors.fill: imageRect
            fillMode: Image.PreserveAspectFit
            property bool counter: false

            visible: false
            source: "image://live/image"

            asynchronous: false
            cache: false

            function reload()
            {
                counter = !counter
                source = "image://live/image?id=" + counter
            }
        }

    }

    Button{
        id: startButton
        x: imageRect.x/2 - startButton.width/2
        y:imageRect.height/6 + imageRect.y
        text: "Open"

        onClicked: {
            QmlHandler.qmlMessage("Camera run")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_CAMERA_RUN)
        }
    }

    Connections{
        target: liveImageProvider
        function onImageChanged()
        {
            opencvImage.reload()
        }
    }
}
