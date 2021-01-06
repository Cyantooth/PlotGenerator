#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "datadispatcher.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    DataDispatcher dataDispatcher(&engine);
    engine.rootContext()->setContextProperty("dataDispatcher", &dataDispatcher);

    engine.load(url);

    return app.exec();
}
