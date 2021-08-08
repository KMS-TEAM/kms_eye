import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

import kms.team.dirhelper 1.0
import kms.team.qconfig 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("KMS EyE")

    property string state: "stop"
    property int currentImage: 0
    property var files
    property var imageNumber
    property string currentImagePath: ""

    function imageLoad(){
        files = dirhelper.files
        imageNumber = files.length
        console.log(imageNumber)

    }

    function imagePlay(){
        if (state === "resume" || state === "running"){
            while(currentImage < imageNumber){
                console.log("Loading: ", files[i])
                root.currentImagePath = "file://" + files[i]
                currentImage++;
            }
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
        y: 24
        width: 367
        height: 40
        anchors.left: setpath.right
        anchors.leftMargin: 10
        placeholderText: qsTr("Text Field")
    }

    Button {
        id: runviewer
        x: 12
        text: qsTr("Run")
        anchors.top: setpath.bottom
        anchors.topMargin: 10
    }

    Button {
        id: pauseviewer
        y: 74
        text: qsTr("Pause")
        anchors.left: runviewer.right
        anchors.leftMargin: 40
    }

    Button {
        id: resumeviewer
        y: 74
        text: qsTr("Resume")
        anchors.left: pauseviewer.right
        anchors.leftMargin: 40
    }

    Image {
        id: image
        x: 497
        y: 5
        width: 137
        height: 99
        anchors.right: parent.right
        source: "qrc:/images/images/logo.png"
        anchors.rightMargin: 5
        fillMode: Image.PreserveAspectFit
    }

    QConfig{
        id: qconfig
        onImagePathChanged:{
            dirhelper.path = imagePath
            console.log(dirhelper.path)
        }
    }

    Connections {
        target: setpath
        onClicked: qconfig.dataPath = textField.text
    }

    Connections {
        target: runviewer
        onClicked:{
            root.currentImage = 0
            root.state = "running"
            imageLoad()
        }
    }

    Connections{
        target: pauseviewer
        onClicked: {
            root.state = "pause"
            imageLoad()
        }
    }

    Connections{
        target: resumeviewer
        onClicked: {
            root.state = "resume"
            imageLoad()
        }
    }

    Image {
        id: imageViewer
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 120

        fillMode: Image.PreserveAspectFit
        source: root.currentImagePath
    }
}
