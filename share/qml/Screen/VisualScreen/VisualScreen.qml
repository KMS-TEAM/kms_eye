import QtQuick 2.0
import AppEnums 1.0
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.0
import MyImage 1.0

Item {
    id: visualScreenID
    Rectangle {
        id: gb
        anchors.fill: parent
        color: "black"
        opacity: 0.5
    }
    VisualViewer{
        id: visualViewer
        anchors.fill: parent
        anchors.bottomMargin: 120
    }   
        Button {
        id: setpath
        y: 343
        text: qsTr("button 1")
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
         Connections{
         target: setpath
         onClicked: openDialog.open()
     }
    
}
