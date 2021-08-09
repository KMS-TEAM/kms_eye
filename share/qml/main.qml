import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2

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
    property string currentImagePath: "qrc:/images/images/logo.png"

    function imageLoad(){
        files = dirhelper.files
        imageNumber = files.length
        console.log(imageNumber)

    }

    function imagePlay(){
        imageViewer.source = "file://" + files[currentImage]
//        if (state === "resume" || state === "running"){
//            while(currentImage < imageNumber){
//                console.log("Loading: ", files[currentImage])
//                imageViewer.source = "file://" + files[currentImage]
//                currentImage++;
//            }
//        }
    }

    DirHelper {
        id: dirhelper
        path: ""
    }

    Button {
        id: setpath
        x: 12
        y: 24
        text: qsTr("Setting path")
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

    FileDialog {
        id: openDialog
        title: "Please choose setting file"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: true

        onAccepted: {
            qconfig.dataPath = openDialog.fileUrl
            console.log(qconfig.dataPath)
            // textField.displayText = qconfig.dataPath
        }
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
        text: qsTr("Next")
        anchors.left: runviewer.right
        anchors.leftMargin: 49
    }

    Button {
        id: resumeviewer
        y: 74
        text: qsTr("Previous")
        anchors.left: pauseviewer.right
        anchors.leftMargin: 49
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
            console.log("Check")
            console.log(dirhelper.path)
        }
    }

    Connections {
        target: setpath
        onClicked: openDialog.open()
    }

    Connections {
        target: runviewer
        onClicked:{
            console.log("Start Viewer")
            root.currentImage = 0
            root.state = "running"
            imageLoad()
            imagePlay()
        }
    }

    Connections{
        target: pauseviewer
        onClicked: {
            root.state = "next"
            root.currentImage++
            imagePlay()
        }
    }

    Connections{
        target: resumeviewer
        onClicked: {
            root.state = "previous"
            root.currentImage--
            imagePlay()
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
        source: "qrc:/images/images/logo.png"
    }
}
