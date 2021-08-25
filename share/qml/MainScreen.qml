import QtQuick 2.0
import AppEnums 1.0
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2

Item {
    id: mainScreenID
    Rectangle {
        id: bg
        anchors.fill: parent
        color: "black"
        opacity: 0.5
    }

    ImageViewer{
        id: imageViewer
        // height: 340
        anchors.fill: parent
        anchors.bottomMargin: 140
        leftImage: QmlModel.currentImagePath[0]
        rightImage: QmlModel.currentImagePath[1]
        leftDepth: QmlModel.disparityMap

    }

    Button {
        id: setpath
        y: 343
        text: qsTr("Setting path")
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.bottomMargin: 80
    }

    FileDialog {
        id: openDialog
        title: "Please choose setting file"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: true

        onAccepted: {
            QmlHandler.qmlMessage("Loading Setting File")
            console.log(openDialog.fileUrl)
            QmlModel.setSettingPath(openDialog.fileUrl)
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_SETTING_PATH)
        }
     }

     Button {
        id: runButton
        x: 12
        text: qsTr("Run")
        anchors.top: setpath.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.topMargin: 10

        onClicked: {
            QmlHandler.qmlMessage("Run")
            // QmlHandler.qmlSendEvent(Enums.EVT)
        }
     }

//     Button {
//        id: pauseButton
//        y: 410
//        text: qsTr("Pause")
//        anchors.left: runButton.right
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 30
//        anchors.leftMargin: 30
//     }

     Button {
        id: nextButton
        y: 410
        text: qsTr("Next")
        anchors.left: runButton.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.leftMargin: 30

        onClicked: {
            QmlHandler.qmlMessage("Next")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_NEXT_IMAGE)
        }
     }

     Button {
        id: backButton
        y: 410
        text: qsTr("Previous")
        anchors.left: nextButton.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.leftMargin: 30

        onClicked: {
            QmlHandler.qmlMessage("Back")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_PREVIOUS_IMAGE)
        }
     }

     Connections{
         target: setpath
         onClicked: openDialog.open()
     }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:3}
}
##^##*/
