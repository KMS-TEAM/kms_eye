#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QVector>

#include "Common.h"
#include "AppEnums.h"

#define MODEL AppModel::instance()

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString settingPath READ settingPath WRITE setSettingPath NOTIFY settingPathChanged)
//    Q_PROPERTY(cv::Mat disparityMap READ disparityMap WRITE setdisparityMap NOTIFY disparityMapChanged)
    Q_PROPERTY(QStringList currentImagePath READ currentImagePath WRITE setCurrentImagePath NOTIFY currentImagePathChanged)
    Q_PROPERTY(AppEnums::APP_STATE state READ state WRITE setState NOTIFY stateChanged)

public:
    static AppModel *instance();
    QStringList currentImagePath() const;
    QString settingPath() const;
    AppEnums::APP_STATE state() const;

    void setListImage();
    QVector<QStringList> getListImages() const;

public slots:
    void setCurrentImagePath(QStringList currentImagePath);
    void setSettingPath(QString settingPath);
    void setState(AppEnums::APP_STATE state);

signals:
    void currentImagePathChanged();
    void settingPathChanged();
    void stateChanged();

private:
    AppModel(QObject* parent = nullptr);
    AppModel(const AppModel& _other) = delete;
    void operator =(const AppModel& _other) = delete;

private:
    static AppModel* m_instance;
    static QMutex m_lock;

    QVector<QStringList> m_listImage;
    QStringList m_currentImagePath;
    QString m_settingPath;
    AppEnums::APP_STATE m_state;
};

#endif // APPMODEL_H
