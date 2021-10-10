import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import MyImage 1.0
import AppEnums 1.0

Item {
    id: visualViewer
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
        id: test
        anchors.fill: parent
        color: "grey"
        Rectangle{
            id: voRaw
            anchors.fill: parent
            height: 320
            width: 640

            GridLayout {
                id: grid_VO
                anchors.fill: parent
                columns: 2
                rows: 2

                Image {
                    id: vO
                    width: (voRaw.width-10)/2
                    height: (voRaw.height-10)/2
                    source: pathpreprocess(visualViewer.leftImage)
                    fillMode: Image.PreserveAspectFit
                    Layout.column: 0
                    Layout.row: 0

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
                }
            }
        }
    }

