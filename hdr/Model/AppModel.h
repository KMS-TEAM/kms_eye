#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>
#include <QMutex>
#include "AppEnums.h"

#define MODEL AppModel::getInstance()

class AppModel : public QObject
{
    Q_OBJECT

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
