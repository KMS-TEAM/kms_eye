import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import MyImage 1.0
import AppEnums 1.0

Item{
    id: imageViewer

    property string leftImage: "qrc:/images/images/logo.png"
    property string rightImage: "qrc:/images/images/logo.png"
    property string leftDepth: "qrc:/images/images/logo.png"
    property string rightDepth: "qrc:/images/images/logo.png"

    function pathpreprocess(path){
        if (path[0] === "q"){
            return path
        }
        else{
            console.log(path)
            return qsTr("file:///" + path)
        }
    }

    Rectangle{
        id: bg
        anchors.fill: parent
        color: "grey"
        // opacity: 0.5

        Rectangle{
            id: imageRaw
            anchors.fill: parent
            height: 320
            width: 640
//            anchors.rightMargin: 320
//            anchors.leftMargin: 0
//            anchors.bottomMargin: 320

            Grid {
                id: grid
                columns: 2
                spacing: 0.5
                anchors.fill: imageRaw

                Image {
                    id: imageleft
                    width: imageRaw.width / 2
                    height: imageRaw.height /2
                    anchors.left: parent.left
                    anchors.top: parent.top
                    source: pathpreprocess(imageViewer.leftImage)
                    anchors.topMargin: 0
                    anchors.leftMargin: 0
                    fillMode: Image.PreserveAspectFit
                }

                Image {
                    id: leftdepth
                    width: imageRaw.width / 2
                    height: imageRaw.height /2
                    anchors.right: parent.right
                    anchors.top: parent.top
                    source: pathpreprocess(imageViewer.leftDepth)
                    anchors.topMargin: 0
                    anchors.rightMargin: 0
                    fillMode: Image.PreserveAspectFit
                }

                Image {
                    id: imageright
                    width: imageRaw.width / 2
                    height: imageRaw.height /2
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    source: pathpreprocess(imageViewer.rightImage)
                    anchors.leftMargin: 0
                    anchors.bottomMargin: 0
                    fillMode: Image.PreserveAspectFit
                }

//                Image {
//                    id: rightdepth
//                    width: imageRaw.width / 2
//                    height: imageRaw.height /2
//                    anchors.right: parent.right
//                    anchors.bottom: parent.bottom
//                    source: imageViewer.rightDepth
//                    anchors.bottomMargin: 0
//                    anchors.rightMargin: 0
//                    fillMode: Image.PreserveAspectFit
//                }

                QImageItem{
                    id: imageItem
                    width: imageRaw.width / 2 - 20
                    height: imageRaw.height /2
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.rightMargin: 0
                }

            }
        }
    }

    Connections{
        target: QmlModel
        onDisparityImageChanged: imageItem.setImage(QmlModel.disparityImage)
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
