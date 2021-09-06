#ifndef APPCONSTANT_H
#define APPCONSTANT_H

#include <QObject>
#include <QString>
#include <QUrl>
//#include <QColor>
#include <QMutex>
#include <QCoreApplication>

#ifndef MACRO_DEFINE
#define MACRO_DEFINE

#define CONSOLE qDebug() << "[" << __FUNCTION__ << "] "
#define DEF_VAR(type, name, value) Q_PROPERTY(type name READ name NOTIFY dataUpdate) \
    public: type name() { return value; }

#define DEF_CONST(type, name, value) Q_PROPERTY(type name READ name CONSTANT) \
    public: type name() const { return value; }

#endif

#define DEFS AppConstant::instance()
#define DELETE_DEFS AppConstant::DestroyInstance()

#ifndef BUILD_DIR
#define BUILD_DIR QCoreApplication::applicationDirPath()
#endif

#ifndef IMAGE_DEF
#define IMAGE_DEF "qrc:/images/images/logo.png"
#endif

class AppConstant : public QObject
{
    Q_OBJECT

    /************************* DEFINE QML URL *****************************/
    DEF_CONST(QUrl     , QML_MAIN_URL               , QUrl(QStringLiteral("qrc:/qml/qml/main.qml")))
    DEF_CONST(QUrl     , QML_MAIN_SCREEN_URL        , QUrl(QStringLiteral("qrc:/qml/qml/MainScreen.qml")))
    DEF_CONST(QUrl     , QML_PCL_VIEWER_URL         , QUrl(QStringLiteral("qrc:/qml/qml/PCLViewer.qml")))

    /************************* DEFINE SOMETHINGS *****************************/
    DEF_CONST(int      , DEFAULT_WIDTH               , 1280)
    DEF_CONST(int      , DEFAULT_HEIGHT              , 720)
    DEF_CONST(int      , DEFAULT_BTN_W               , 426)
    DEF_CONST(int      , DEFAULT_BTN_H               , 150)

signals:
    void dataUpdated();

public:
    static AppConstant* instance()
    {
        m_lock.lock();
        if(nullptr == m_instance){
            m_instance = new AppConstant();
        }
        m_lock.unlock();
        return m_instance;
    }

    static void DestroyInstance()
    {
        m_lock.lock();
        if (nullptr != m_instance){
            delete m_instance;
        }
        m_instance = nullptr;
        m_lock.unlock();
    }

private:
    explicit AppConstant(QObject* parent = nullptr) : QObject { parent } {}
    ~AppConstant() {}
    AppConstant(const AppConstant&) = delete;
    bool operator= (const AppConstant&) = delete;

    static AppConstant* m_instance;
    static QMutex m_lock;
};


#endif // APPCONSTANT_H
