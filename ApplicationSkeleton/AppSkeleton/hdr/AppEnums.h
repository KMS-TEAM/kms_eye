#ifndef APPENUMS_H
#define APPENUMS_H

#include <QObject>

class AppEnums : public QObject
{
    Q_OBJECT
public:
    enum Event_t{
        EVT_NONE = 0,   // do not add enum above this

        EVT_CLICK_BUTTON_A = 1,
        EVT_CLICK_BUTTON_B = 2,
        EVT_CLICK_BUTTON_C = 3,

        EVT_MAX, // do not add enum under this
    };
    Q_ENUM(Event_t)

private:
    AppEnums(const AppEnums& _other) = delete;
    void operator =(const AppEnums& _other) = delete;
};

#endif // APPENUMS_H
