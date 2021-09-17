import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
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

            GridLayout {
                id: grid
                anchors.fill: parent
                columns: 2
                rows: 2

                Image {
                    id: image
                    width: (imageRaw.width-10)/2
                    height: (imageRaw.height-10)/2
                    source: pathpreprocess(imageViewer.leftImage)
                    fillMode: Image.PreserveAspectFit
                    Layout.column: 0
                    Layout.row: 0

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Image {
                    id: image1
                    width: (imageRaw.width-10)/2
                    height: (imageRaw.height-10)/2
                    source: pathpreprocess(imageViewer.leftDepth)
                    fillMode: Image.PreserveAspectFit
                    Layout.column: 1
                    Layout.row: 0

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Image {
                    id: image2
                    width: (imageRaw.width-10)/2
                    height: (imageRaw.height-10)/2
                    source: pathpreprocess(imageViewer.rightImage)
                    fillMode: Image.PreserveAspectFit
                    Layout.column: 0
                    Layout.row: 1

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Image {
                    id: image3
                    width: (imageRaw.width-10)/2
                    height: (imageRaw.height-10)/2
                    source: "image://live/image"
                    fillMode: Image.PreserveAspectFit
                    property bool counter: false
                    asynchronous: false
                    cache: false
                    // visible: false
                    // anchors.right: parent.right
                    // anchors.bottom: parent.bottom

                    Layout.column: 1
                    Layout.row: 1

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    function reload()
                    {
                        console.log("Update counter")
                        counter = !counter
                        source = "image://live/image?id=" + counter
                        console.log(source)
                    }
                }

                Connections{
                    target: liveImageProvider

                    function onImageChanged()
                    {
                        image3.reload()
                    }

                }
//                Connections{
//                    target: QmlModel
//                    onDisparityImageChanged: imageItem.setImage(QmlModel.disparityImage)
//                }

            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
