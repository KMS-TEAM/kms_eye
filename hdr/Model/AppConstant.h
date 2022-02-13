#ifndef APPCONSTANT_H
#define APPCONSTANT_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QColor>
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
    DEF_CONST(QUrl     , QML_PCL_VIEWER_URL         , QUrl(QStringLiteral("qrc:/qml/qml/Screen/PCLScreen/PCLScreen.qml")))
    DEF_CONST(QUrl     , QML_IMAGE_VIEWER_URL       , QUrl(QStringLiteral("qrc:/qml/qml/Screen/ImageScreen/ImageScreen.qml")))
    DEF_CONST(QUrl     , QML_CAMERA_VIEWER_URL      , QUrl(QStringLiteral("qrc:/qml/qml/Screen/CameraScreen/CameraScreen.qml")))
    DEF_CONST(QUrl     , QML_PCL_LOADER             , QUrl(QStringLiteral("qrc:/qml/qml/Screen/PCLScreen/PCLViewer.qml")))
    DEF_CONST(QUrl     , QML_OPENGL_RENDER_URL      , QUrl(QStringLiteral("qrc:/qml/qml/Screen/OpenGLScreen/OpenGLViewer.qml")))

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

    /******************************************* RESOURCE ************************************************/

    // QML

    // IMAGES
    DEF_CONST(QString, IMAGE_FOLDER , "qrc:/res/res/")
    DEF_CONST(QString, SEARCH_IMG   , "search.svg"  )
    DEF_CONST(QString, HOME_IMG     , "home.svg"    )
    DEF_CONST(QString, CONTROL_IMG  , "control.svg" )
    DEF_CONST(QString, MAP_IMG      , "map.svg"     )
    DEF_CONST(QString, USER_IMG     , "user.svg"    )

    /********************************************** Point Cloud Resource ********************************/

    DEF_CONST(QString, PHONG_VERTEX     , "/home/lacie/Github/kms_eye/share/shader/phong.vert" )
    DEF_CONST(QString, PHONG_FRAGMENT   , "/home/lacie/Github/kms_eye/share/shader/phong.frag" )
    DEF_CONST(QString, OPENGL_EXAMPLE   , "/home/lacie/Github/kms_eye/share/shader/trefoil.obj")

    /********************************************** Camera **********************************************/

    DEF_CONST(QString, LEFT_CAMERA , "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink")
    DEF_CONST(QString, RIGHT_CAMERA, "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink")
    DEF_CONST(QString, VIDEO_EXAMPLE_1, "/home/lacie/Videos/example1.mp4")
    DEF_CONST(QString, VIDEO_EXAMPLE_2, "/home/lacie/Videos/example2.mp4")

    /********************************************** GENERAL **********************************************/
    DEF_CONST(int, MAX_WIDTH        , 1280  )
    DEF_CONST(int, MAX_HEIGHT       , 680   )
    DEF_CONST(int, MENU_BAR_WIDTH   , 80    )

    DEF_CONST(QString   , EMPTY_STRING , ""    )

    // search screen constant
    DEF_CONST(int, INPUT_BOX_WIDTH      , 1100  )
    DEF_CONST(int, INPUT_BOX_HEIGHT     , 60    )
    DEF_CONST(int, INPUT_BOX_TOP_MARGIN , 15    )

    // constant color
    DEF_CONST(QColor, COLOR_SWITCH_OFF      , "#ADADAD")
    DEF_CONST(QColor, COLOR_SWITCH_ON       , "#00CCD9")
    DEF_CONST(QColor, COLOR_SWITCH_BTN_NOOD , "#F0F0F0")
    DEF_CONST(QColor, COLOR_BORDER_DARK     , "#777777")

    DEF_CONST(QColor, COLOR_MENU_BAR        , "#00AF2A")
    DEF_CONST(QColor, COLOR_MENU_BAR_FOCUS  , "#DDDDDD")
    DEF_CONST(QColor, COLOR_BACK_GROUND     , COLOR_MENU_BAR_FOCUS())

    DEF_CONST(QString, COLOR_INVISIBLE      , "transparent")

    // QChart define
    DEF_CONST(int, CHART_DRAW_OFFSET, 10)
};


#endif // APPCONSTANT_H
