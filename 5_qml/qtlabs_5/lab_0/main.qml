import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Визитное приложение")

    Text {
        id: fullname
        text: "ФИО: Мориков Алексей Михайлович"
        color: "blue"
        // anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true

    }
    Text {
        anchors.top: fullname.bottom
        id: group
        text: "Группа: Б22-511"
    }

    Text {
        anchors.top: group.bottom
        id: additionalinfo
        text: "я умею программировать на C++ & Python"
    }

    Text {
        id: sliderSlave
    }

    Slider {
        id: slide
        from: 0.0
        to: 100.0
        anchors.top: additionalinfo.bottom   
    }

    ColumnLayout {
        id: radiobuttons
        anchors.top: slide.bottom
        RadioButton {
            checked: true
            text: qsTr("First")
        }
        RadioButton {
            text: qsTr("Second")
        }
        RadioButton {
            text: qsTr("Third")
        }
    }

    Rectangle {
        id: rect
        width: 100
        height: 100
        anchors.top: radiobuttons.bottom

        Button {
            id: changeColorBtn
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Change color!"
        }

        Connections {
            target: changeColorBtn
            function onClicked() {
                rect.color = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
            }
        }
    }

    Button {
        id: button
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Exit"
    }

    Connections {
        target: button
        function onClicked() {
            Qt.quit();
        }
    }
}
