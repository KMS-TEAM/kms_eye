#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QVector>

#include "Common.h"
#include "QConfig.h"
#include "AppEnums.h"

#define MODEL AppModel::instance()

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString settingPath READ settingPath WRITE setSettingPath NOTIFY settingPathChanged)
//    Q_PROPERTY(cv::Mat disparityMap READ disparityMap WRITE setdisparityMap NOTIFY disparityMapChanged)
    Q_PROPERTY(int currentImageNumber READ currentImageNumber WRITE setCurrentImageNumber NOTIFY currentImageNumberChanged)
    Q_PROPERTY(QStringList currentImagePath READ currentImagePath WRITE setCurrentImagePath NOTIFY currentImagePathChanged)
    Q_PROPERTY(AppEnums::APP_STATE state READ state WRITE setState NOTIFY stateChanged)

public:

    int m_currentImageNumber;

    static AppModel *instance();
    int currentImageNumber() const;
    QStringList currentImagePath() const;
    QString settingPath() const;
    AppEnums::APP_STATE state() const;

    void setListImage();
    QVector<QStringList> getListImages() const;

public slots:
    void setCurrentImagePath(QStringList currentImagePath);
    void setCurrentImageNumber(int currentImageNumber);
    void setSettingPath(QString settingPath);
    void setState(AppEnums::APP_STATE state);

signals:
    void currentImagePathChanged();
    void currentImageNumberChanged();
    void settingPathChanged();
    void stateChanged();

private:
    AppModel(QObject* parent = nullptr);
    AppModel(const AppModel& _other) = delete;
    void operator =(const AppModel& _other) = delete;

private:
    static AppModel* m_instance;
    static QMutex m_lock;

    static QConfig* m_config;

    QVector<QStringList> m_listImage;
    QStringList m_currentImagePath;
    AppEnums::APP_STATE m_state;
    QString m_settingPath;
};

#endif // APPMODEL_H
