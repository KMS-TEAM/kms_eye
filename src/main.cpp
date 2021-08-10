#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <string>
#include "AppEngine.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("kms_team");
    app.setOrganizationDomain("kms_team");

    AppEngine appEngine;
    appEngine.prepareApplication();
    appEngine.runApplication();

    return app.exec();
}
