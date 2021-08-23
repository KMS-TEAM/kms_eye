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
            // qconfig.dataPath = openDialog.fileUrl
            console.log(openDialog.fileUrl)
            // textField.displayText = qconfig.dataPath
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
     }

     Button {
        id: backButton
        y: 410
        text: qsTr("Previous")
        anchors.left: nextButton.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.leftMargin: 30
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