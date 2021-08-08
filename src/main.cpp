#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <string>

#include "QConfig.h"
#include "DirHelper.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<DirHelper>("kms.team.dirhelper",1,0,"DirHelper");
    qmlRegisterType<QConfig>("kms.team.qconfig",1,0,"QConfig");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //    QConfig fsSetting;
    //    fsSetting.setParameterFile("/home/lacie/Github/kms_eye/data/EurocStereoVIO.yaml");

    return app.exec();
}
