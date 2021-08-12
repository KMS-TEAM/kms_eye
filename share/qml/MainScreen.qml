import QtQuick 2.0
import AppEnums 1.0

Item {
    id: mainScreenID
    Rectangle {
        id: bg
        anchors.fill: parent
        color: "black"
        opacity: 0.5
    }
    Text {
        id: textID
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
            verticalCenterOffset: -200
        }
        text: QmlModel.currentPath
        font.pixelSize: 30
    }

    Rectangle {
        id: btnA
        anchors {
            bottom: parent.bottom
            right: btnB.left
        }
        border.color: "black"
        border.width: 2

        width: QmlConst.DEFAULT_BTN_W
        height: QmlConst.DEFAULT_BTN_H
        Text { anchors.centerIn: parent; text: "A"; font.pixelSize: 40}
        MouseArea {
            anchors.fill: parent
            onClicked: {
                QmlHandler.qmlMessage("Clicked button A")
                QmlHandler.qmlSendEvent(Enums.EVT_CLICK_BUTTON_A)
            }
        }
    }

    Rectangle {
        id: btnB
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        border.color: "black"
        border.width: 2

        width: QmlConst.DEFAULT_BTN_W
        height: QmlConst.DEFAULT_BTN_H
        Text { anchors.centerIn: parent; text: "B"; font.pixelSize: 40}
        MouseArea {
            anchors.fill: parent
            onClicked: {
                QmlHandler.qmlMessage("Clicked button B")
                QmlHandler.qmlSendEvent(Enums.EVT_CLICK_BUTTON_B)
            }
        }
    }


    Rectangle {
        id: btnC
        anchors {
            bottom: parent.bottom
            left: btnB.right
        }
        border.color: "black"
        border.width: 2

        width: QmlConst.DEFAULT_BTN_W
        height: QmlConst.DEFAULT_BTN_H
        Text { anchors.centerIn: parent; text: "C"; font.pixelSize: 40}
        MouseArea {
            anchors.fill: parent
            onClicked: {
                QmlHandler.qmlMessage("Clicked button C")
                QmlHandler.qmlSendEvent(Enums.EVT_CLICK_BUTTON_C)
            }
        }
    }
}
