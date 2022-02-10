import QtQuick 2.0
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QmlCustomItem 1.0
import AppEnums 1.0

Item {
    id: cameraScreen

    Rectangle{
        id: bg
        anchors.fill: parent
        color: "black"
        opacity: 0.5
    }

    FrameViewer{
        id: frameViewer
        anchors.fill: parent
        anchors.bottomMargin: 140
    }
}
