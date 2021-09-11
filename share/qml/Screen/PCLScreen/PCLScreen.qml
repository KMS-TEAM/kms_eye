import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import "../../Component"
import "../../Component/Common"

import AppEnums 1.0

Item {
    id: pclScreen
    width: QmlConst.DEFAULT_WIDTH - QmlConst.MENU_BAR_WIDTH
    height: QmlConst.DEFAULT_HEIGHT


//    PCLViewer{
//        anchors.fill: parent
//        anchors.bottomMargin: 50
//        anchors.rightMargin: 0
//    }

    Timer {
        id: timer
    }

    QRec {
        id: mainScreen
        width: pclScreen.width
        height: pclScreen.height - 50
        anchors.top: pclScreen.top
        anchors.bottomMargin: 50
        anchors.leftMargin: 0

        color: QmlConst.COLOR_INVISIBLE

        Loader {
            id: pclLoader
            visible: true
            anchors.fill: mainScreen
    //        source: QmlConst.QML_PCL_LOADER
        }
    }

    FileDialog {
        id: openDialog2
        title: "Please choose setting file"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: true

        onAccepted: {
            QmlHandler.qmlMessage("Loading Setting File")
            console.log(openDialog2.fileUrl)
            QmlModel.setSettingPath(openDialog2.fileUrl)
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_RECONSTRUCTION_SETTING_PATH)
        }
     }

    Button {
        id: settingPath
        y: 440
        text: qsTr("Setting")
        anchors.left: pclScreen.left
        anchors.bottom: pclScreen.bottom
        anchors.leftMargin: 10
        anchors.bottomMargin: 5
    }

    Button {
        id: rect
        y: 440
        text: qsTr("Reconstruction")
        anchors.left: settingPath.right
        anchors.bottom: pclScreen.bottom
        anchors.leftMargin: 20
        anchors.bottomMargin: 5

        onClicked: {
            QmlHandler.qmlMessage("Reconstruction Started")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_RUN_RECONSTRUCTION)
        }
    }

    Button {
        id: view
        y: 440
        text: qsTr("View")
        anchors.left: rect.right
        anchors.bottom: pclScreen.bottom
        anchors.leftMargin: 20
        anchors.bottomMargin: 5

        onClicked: {
            QmlHandler.qmlMessage("3D View")
            QmlHandler.qmlSendEvent(Enums.EVT_CLICK_PCL_VIEW)
            pclLoader.source = QmlConst.QML_PCL_LOADER
            delay(1000);
            pclLoader.source = QmlConst.QML_PCL_LOADER
        }
    }

    Connections{
        target: settingPath
        onClicked: openDialog2.open()
    }

    function delay(delayTime) {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.start();
    }
}

