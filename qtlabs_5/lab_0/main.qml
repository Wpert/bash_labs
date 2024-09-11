import QtQuick 2.15

Rectangle {
    id: window
    width: 320; height: 480
    color: "lightgray"

    Text {
        id: mainText
        text: "I've did first lab !!!"
	y: 30
	color: "blue"
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }
}
