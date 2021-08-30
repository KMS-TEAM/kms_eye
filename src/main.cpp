#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <string>
#include <QSurfaceFormat>
#include "AppEngine.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QSurfaceFormat::CoreProfile );
    QSurfaceFormat::setDefaultFormat( fmt );

    app.setOrganizationName("kms_team");
    app.setOrganizationDomain("kms_team");

    AppEngine appEngine;
    appEngine.initEngine();
    appEngine.startEngine();

    return app.exec();
}
