#ifndef APPENUMS_H
#define APPENUMS_H

#include <QObject>

class AppEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(APP_STATE)
public:
    enum EVENT_t{
        EVT_NONE = 0,   // do not add enum above this

        EVT_CLICK_SETTING_PATH,
        EVT_CLICK_DISPARITY_MAP,
        EVT_CLICK_NEXT_IMAGE,
        EVT_CLICK_PREVIOUS_IMAGE,
        EVT_CLICK_RESET,

        EVT_MAX, // do not add enum under this
    };
    Q_ENUMS(EVENT_t)

    enum APP_STATE{
        STATE_RUNNING = 0,
        STATE_RESET = 1,
    };

private:
    AppEnums(const AppEnums& _other) = delete;
    void operator =(const AppEnums& _other) = delete;
};

#endif // APPENUMS_H
