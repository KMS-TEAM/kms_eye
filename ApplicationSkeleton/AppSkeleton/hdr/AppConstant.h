#ifndef APPCONSTANT_H
#define APPCONSTANT_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QUrl>

#ifndef MACROS_DEFINE
#define MACROS_DEFINE

#define CONSOLE qDebug() << "[" << __FUNCTION__ << "] "
#define DEFINE(type, name, value) Q_PROPERTY(type name READ name CONSTANT) \
    public: type name() { return value; }

#endif  // ~MACROS_DEFINE

#define DEF AppConstant::instance()
class AppConstant : public QObject
{
    Q_OBJECT

    /************************* DEFINE QML URL *****************************/
    DEFINE(QUrl     , QML_MAIN_URL               , QUrl(QStringLiteral("qrc:/qml/main.qml")))
    DEFINE(QUrl     , QML_MAIN_SCREEN_URL        , QUrl(QStringLiteral("qrc:/qml/MainScreen.qml")))

    /************************* DEFINE SOMETHINGS *****************************/
    DEFINE(int      , DEFAULT_WIDTH               , 1280)
    DEFINE(int      , DEFAULT_HEIGHT              , 720)
    DEFINE(int      , DEFAULT_BTN_W               , 426)
    DEFINE(int      , DEFAULT_BTN_H               , 150)

    /************************* MAIN CLASS *****************************/
public:
    static AppConstant* instance() {
        if (nullptr == self)
            self = new AppConstant();
        return self;
    }
private:
    // private method
    AppConstant() { CONSOLE << "Init instance"; }
    AppConstant(const AppConstant& _other) = delete;
    void operator =(const AppConstant& _other) = delete;

    // private properties
    static AppConstant* self;
};

#endif // APPCONSTANT_H
