import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Window {
    id: window
    visible: true

    width: 800
    height: 600
    title: qsTr("Plot generator")

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: settingsLayout
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            Layout.margins: 10

            Label {
                text: "Theme:"
            }

            ComboBox {
                textRole: "text"
                valueRole: "value"
                onActivated: chart.currentTheme = currentValue
                Component.onCompleted: currentIndex = indexOfValue(chart.currentTheme)
                model: [
                    { value: ChartView.ChartThemeLight        , text: qsTr("Light")        },
                    { value: ChartView.ChartThemeBlueCerulean , text: qsTr("BlueCerulean") },
                    { value: ChartView.ChartThemeDark         , text: qsTr("Dark")         },
                    { value: ChartView.ChartThemeBrownSand    , text: qsTr("BrownSand")    },
                    { value: ChartView.ChartThemeBlueNcs      , text: qsTr("BlueNcs")      },
                    { value: ChartView.ChartThemeHighContrast , text: qsTr("HighContrast") },
                    { value: ChartView.ChartThemeBlueIcy      , text: qsTr("BlueIcy")      },
                    { value: ChartView.ChartThemeQt           , text: qsTr("Qt")           }
                ]
            }
        }

        ChartView {
            id: chart

            property int currentTheme: ChartView.ChartThemeDark

            legend.visible: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            theme: currentTheme
            animationOptions: ChartView.NoAnimation
            antialiasing: false
            Component.onCompleted: dataDispatcher.setSeries(scatterSeries)

            ValueAxis {
                id: axisY
                min: -2
                max: 2
            }

            ValueAxis {
                id: axisX
                min: 0
                max: 1024
            }

            ScatterSeries {
                id: scatterSeries
                axisX: axisX
                axisY: axisY
                useOpenGL: true
                markerSize: 4
            }
        }

        RowLayout {
            id: buttonsPanel
            Layout.preferredWidth: 360
            Layout.preferredHeight: 64
            Layout.alignment: Qt.AlignCenter

            function onReadyToStart() {
                buttonStart.enabled = true;
                buttonPause.enabled = false;
                buttonStop.enabled = false;
                buttonPause.text = "Pause"
            }

            function onStarted() {
                buttonStart.enabled = false;
                buttonPause.enabled = true;
                buttonStop.enabled = true;
            }

            function onPause() {
                buttonStart.enabled = true;
                buttonPause.enabled = true;
                buttonStop.enabled = true;
                buttonPause.text = "Resume"
            }

            function onResume() {
                onStarted();
                buttonPause.text = "Pause"
            }

            Component.onCompleted: {
                dataDispatcher.readyToStart.connect(onReadyToStart)
                dataDispatcher.started.connect(onStarted)
                dataDispatcher.pauseGenerator.connect(onPause)
                dataDispatcher.resumeGenerator.connect(onResume)
            }

            Button {
                id: buttonStart
                text: qsTr("Start")
                enabled: false
                onClicked: dataDispatcher.start()
            }

            Button {
                id: buttonPause
                text: qsTr("Pause")
                enabled: false
                onClicked: dataDispatcher.pause()
            }

            Button {
                id: buttonStop
                text: qsTr("Stop")
                enabled: false
                onClicked: dataDispatcher.stop()
            }
        }
    }
}
