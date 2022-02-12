import QtQuick 2.4

Item {
    id: root

    width: 400
    height: 400

    QCameraControls {
        camera: _camera

        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
    }
}
