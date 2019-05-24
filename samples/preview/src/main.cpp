#include <QSize>
#include <CVCamera.h>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //Instantiate Camera.
    CVCamera camera;
    camera.setSize(QSize(640,480));



    auto qmlContext = engine.rootContext();
    qmlContext->setContextProperty("camerax", &camera);

    engine.load(QUrl(QStringLiteral("qrc:/src/main.qml")));

    return app.exec();
}
