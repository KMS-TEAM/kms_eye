import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

import kms.team.dirhelper 1.0
import kms.team.qconfig 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("KMS EyE")

    function imageLoad(){
        var files = dirhelper.files
        var imageNumber = files.length
        console.log(imageNumber)
        for(var i = 0; i < 5; i++) {
            console.log("Loading: ", files[i])
            var component = Qt.createComponent("ImageViewer.qml")
            var page = component.createObject(swipeView,{"width" : 640, "height": 480 })
            page.source = "file://" + files[i]
        }
    }

    DirHelper {
        id: dirhelper
        path: ""
    }

    Button {
        id: setpath
        x: 12
        y: 24
        text: qsTr("Set path")
    }

    TextField {
        id: textField
        x: 118
        y: 24
        width: 367
        height: 40
        placeholderText: qsTr("Text Field")
    }

    Button {
        id: runviewer
        x: 12
        y: 74
        text: qsTr("Run")
    }

    Image {
        id: image
        x: 497
        y: 5
        width: 137
        height: 99
        source: "qrc:/images/images/logo.png"
        fillMode: Image.PreserveAspectFit
    }

    QConfig{
        id: qconfig
        onImagePathChanged:{
            dirhelper.path = imagePath
            console.log(dirhelper.path)
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        anchors.topMargin: 120
        currentIndex: tabBar.currentIndex
    }

    Connections {
        target: setpath
        onClicked: qconfig.dataPath = textField.text
    }

    Connections {
        target: runviewer
        onClicked: imageLoad()
    }


}
