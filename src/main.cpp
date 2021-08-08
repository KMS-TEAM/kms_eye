#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "hdr/QConfig.h"
#include <string>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QConfig fsSetting;
    fsSetting.setParameterFile("/home/lacie/Github/kms_eye/data/EurocStereoVIO.yaml");
    std::string camera_fx("Camera.fx");
    bool check = fsSetting.ParseCamParam();


    return app.exec();


}
