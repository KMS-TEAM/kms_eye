import QtQuick 2.0
import "Component"
import "Component/Common"
import QmlCustomItem 1.0

Item
{
    id: root

    QMenuBar {
        id: menuBar
        width: QmlConst.MENU_BAR_WIDTH
        height: root.height
        anchors.left: root.left
        anchors.top: root.top
    }

    QRec {
        id: mainScreen
        width: root.width - QmlConst.MENU_BAR_WIDTH
        height: root.height
        anchors.top: root.top
        anchors.left: menuBar.right

        color: QmlConst.COLOR_INVISIBLE
    }

//    Loader {
//        id: imageLoader
//        visible: QmlModel.currentScreenID === ENUMS.IMAGE_VIEWER_SCREEN
//        anchors.fill: mainScreen
//        source: QmlConst.QML_IMAGE_VIEWER_URL
//    }

    Loader {
        id: screenLoader
        visible: true
        property int screenID: QmlModel.currentScreenID
        anchors.fill: mainScreen
        source: getScreenUrl(screenID)
    }

    function getScreenUrl(id){
        switch(id){
        case 0:
            return QmlConst.QML_IMAGE_VIEWER_URL
        case 1:
            return QmlConst.QML_PCL_VIEWER_URL
        }
    }

}
