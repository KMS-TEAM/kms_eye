#ifndef APPENUMS_H
#define APPENUMS_H

#include <QObject>

class AppEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(E_EVENT_t)

public:
    enum E_EVENT_t{
        UserSettingPath = 0,
        UserClickDisparityMap,
        UserClickNextImage,
        UserClickPreviousImage,
        UserClickReset,
    };
};

#endif // APPENUMS_H
