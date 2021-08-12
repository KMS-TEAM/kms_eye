import QtQuick 2.12
import QtQuick.Window 2.12
import AppEnums 1.0

Window {
    id: root
    visible: true
    width: QmlConst.DEFAULT_WIDTH
    height: QmlConst.DEFAULT_HEIGHT
    maximumHeight: QmlConst.DEFAULT_HEIGHT
    minimumHeight: QmlConst.DEFAULT_HEIGHT
    maximumWidth: QmlConst.DEFAULT_WIDTH
    minimumWidth: QmlConst.DEFAULT_WIDTH

    title: qsTr("STR_HELLO")
    Loader {
        id: loader
        anchors.fill: parent
        source: QmlConst.QML_MAIN_SCREEN_URL
    }
}
