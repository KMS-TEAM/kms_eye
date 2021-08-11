#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>
#include <QMutex>

#include "Common.h"
#include "AppEnums.h"

#define MODEL AppModel::getInstance()

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString settingPath READ settingPath WRITE setSettingPath NOTIFY settingPathChanged)
    Q_PROPERTY(cv::Mat disparityMap READ disparityMap WRITE setdisparityMap NOTIFY disparityMapChanged)
    Q_PROPERTY(int imagePath READ imagePath WRITE setimagePath NOTIFY imagePathChanged)
    Q_PROPERTY(int reset READ reset WRITE setResetState NOTIFY resetStateChanged)

public:
    static AppModel *getInstance();

    Q_INVOKABLE void qmlEventHandler(int event);

public slots:

signals:

private:
    AppModel(QObject* parent = nullptr);
    AppModel(const AppModel& ) = delete;
    void operator =(const AppModel& ) = delete;

    static AppModel* m_instance;
    static QMutex m_lock;
};

#endif // APPMODEL_H
