#ifndef APPENUMS_H
#define APPENUMS_H

#include <QObject>

class AppEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(APP_STATE)
    Q_ENUMS(ALGORITHM)
    Q_ENUMS(EVENT_t)
    Q_ENUMS(VIEW_SCREEN)
public:
    enum EVENT_t{
        EVT_NONE = 0,   // do not add enum above this

        EVT_CLICK_IMAGE_SCREEN,
        EVT_CLICK_PCL_SCREEN,
        EVT_CLICK_SETTING_PATH,
        EVT_CLICK_RECONSTRUCTION_SETTING_PATH,
        EVT_CLICK_RUN,
        EVT_CLICK_RUN_RECONSTRUCTION,
        EVT_CLICK_STOP,
        EVT_CLICK_NEXT_IMAGE,
        EVT_CLICK_PREVIOUS_IMAGE,
        EVT_CLICK_PCL_VIEW,
        EVT_CLICK_RESET,

        EVT_MAX, // do not add enum under this
    };

    enum APP_STATE{
        STATE_NONE = 0,

        STATE_RUNNING ,
        STATE_STOP,
        STATE_RESET,
    };

    enum VIEW_SCREEN{
        IMAGE_VIEWER_SCREEN = 0,
        PCL_VIEWER_SCREEN,
    };

    enum ALGORITHM{
        SGBM = 0,
    };

private:
    AppEnums(const AppEnums& _other) = delete;
    void operator =(const AppEnums& _other) = delete;
};

#endif // APPENUMS_H
