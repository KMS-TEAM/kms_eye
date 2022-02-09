import QtQuick 2.12
import "Common"
import QmlCustomItem 1.0
import QtQuick.Controls 2.12
import AppEnums 1.0

QRec {
    id: root
    color: QmlConst.COLOR_MENU_BAR
    width: 80
    height: 680

    Button {
        id: imageScreen
        y: 50
        width: 80
        height: 40
        text: qsTr("Image")
        anchors.left: parent.left
        anchors.leftMargin: 0

        onClicked:{
            QmlHandler.qmlMessage("Selected Image Screen")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_IMAGE_SCREEN)
        }
    }

    Button {
        id: plcScreen
        y: 110
        width: 80
        height: 40
        text: qsTr("PCL")
        anchors.left: parent.left
        anchors.leftMargin: 0

        onClicked:{
            QmlHandler.qmlMessage("Selected PCL Screen")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_PCL_SCREEN)
        }
    }

    Button {
        id: cameraScreen
        y: 170
        width: 80
        height: 40
        text: qsTr("Camera")
        anchors.left: parent.left
        anchors.leftMargin: 0

        onClicked:{
            QmlHandler.qmlMessage("Selected Camera Screen")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_CAMERA_SCREEN)
        }
    }

}
