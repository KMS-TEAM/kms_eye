#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QVector>
#include <QTimer>
#include <QImage>

#include "Common.h"
#include "QConfig.h"
#include "AppEnums.h"
#include "QImageProcessing.h"


#define MODEL AppModel::instance()

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString settingPath READ settingPath WRITE setSettingPath NOTIFY settingPathChanged)
    Q_PROPERTY(QString disparityMap READ disparityMap WRITE setdisparityMap NOTIFY disparityMapChanged)
    Q_PROPERTY(int currentImageNumber READ currentImageNumber WRITE setCurrentImageNumber NOTIFY currentImageNumberChanged)
    Q_PROPERTY(QStringList currentImagePath READ currentImagePath WRITE setCurrentImagePath NOTIFY currentImagePathChanged)
    Q_PROPERTY(AppEnums::APP_STATE state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QImage disparityImage READ disparityImage WRITE setDisparityImage NOTIFY disparityImageChanged)

public:

    int m_currentImageNumber;

    static AppModel *instance();
    int currentImageNumber() const;
    QStringList currentImagePath() const;
    QString settingPath() const;
    AppEnums::APP_STATE state() const;
    QString disparityMap() const;
    QImage disparityImage() const;

    void setListImage();
    QVector<QStringList> getListImages() const;

    void imageProcessing(AppEnums::ALGORITHM algo);

public slots:
    void setCurrentImagePath(QStringList currentImagePath);
    void setCurrentImageNumber(int currentImageNumber);
    void setSettingPath(QString settingPath);
    void setState(AppEnums::APP_STATE state);
    void setdisparityMap(QString disparityMap);
    void increaseIndex();
    void setDisparityImage(QImage disparityImage);

signals:
    void currentImagePathChanged();
    void currentImageNumberChanged();
    void settingPathChanged();
    void stateChanged();
    void disparityMapChanged();
    void disparityImageChanged();
    void runImageProcessing(QStringList &currentImagePath, int &currentImageNumber);

private:
    AppModel(QObject* parent = nullptr);
    AppModel(const AppModel& _other) = delete;
    void operator =(const AppModel& _other) = delete;

private:
    static AppModel* m_instance;
    static QMutex m_lock;

    static QConfig* m_config;
    static QImageProcessing* m_imageprocessing;
    static AppEnums::APP_STATE m_state;

    QImage m_disparityImage;
    QVector<QStringList> m_listImage;
    QStringList m_currentImagePath;
    QString m_disparityMap;
    QString m_settingPath;

    QTimer m_timer;
};

#endif // APPMODEL_H
