#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QVector>
#include <QTimer>
#include <QImage>
#include <QQmlApplicationEngine>


#include "Common.h"
#include "QConfig.h"
#include "AppEnums.h"
#include "QImageProcessing.h"
#include "QCameraCapture/QCameraCapture.h"

#define MODEL AppModel::instance()

class AppModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentScreenID READ currentScreenID WRITE setCurrentScreenID NOTIFY currentScreenIDChanged)
    Q_PROPERTY(QString settingPath READ settingPath WRITE setSettingPath NOTIFY settingPathChanged)
    Q_PROPERTY(QString disparityMap READ disparityMap WRITE setdisparityMap NOTIFY disparityMapChanged)
    Q_PROPERTY(int currentImageNumber READ currentImageNumber WRITE setCurrentImageNumber NOTIFY currentImageNumberChanged)
    Q_PROPERTY(QStringList currentImagePath READ currentImagePath WRITE setCurrentImagePath NOTIFY currentImagePathChanged)
    Q_PROPERTY(AppEnums::APP_STATE state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QImage disparityImage READ disparityImage WRITE setDisparityImage NOTIFY disparityImageChanged)
    Q_PROPERTY(QString pclPath READ pclPath WRITE setPclPath NOTIFY pclPathChanged)

public:

    int m_currentImageNumber;

    static AppModel *instance();
    int currentImageNumber() const;
    QStringList currentImagePath() const;
    QString settingPath() const;
    AppEnums::APP_STATE state() const;
    QString disparityMap() const;
    QImage disparityImage() const;
    QString pclPath() const;
    int currentScreenID() const;

    void setListImage();
    QVector<QStringList> getListImages() const;

    void imageProcessing(AppEnums::ALGORITHM algo);

    void cameraRun(QString path1, QString path2);

public slots:
    void setCurrentImagePath(QStringList currentImagePath);
    void setCurrentImageNumber(int currentImageNumber);
    void setSettingPath(QString settingPath);
    void setState(AppEnums::APP_STATE state);
    void setdisparityMap(QString disparityMap);
    void increaseIndex();
    void setDisparityImage(QImage disparityImage);
    void setPclPath(QString pclPath);
    void setCurrentScreenID(int currentScreenID);
    void setCurrentLeftFrame(cv::Mat *frame);
    void setCurrentRightFrame(cv::Mat *frame);

signals:
    void currentImagePathChanged();
    void currentImageNumberChanged();
    void settingPathChanged();
    void stateChanged();
    void disparityMapChanged();
    void disparityImageChanged(QImage &image);
    void runImageProcessing(QStringList &currentImagePath, int &currentImageNumber);
    void pclPathChanged();
    void runReconstruction();
    void currentScreenIDChanged(int currentScreenID);
    void currentLeftFrameChanged(QImage &image);
    void currentRightFrameChanged(QImage &image);

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

    static QCameraCapture* m_leftCamcapture;
    static QCameraCapture* m_rightCamcapture;
    QImage m_currentRightFrame;
    QImage m_currentLeftFrame;

    QImage m_disparityImage;
    QVector<QStringList> m_listImage;
    QStringList m_currentImagePath;
    QString m_disparityMap;
    QString m_settingPath;
    QString m_pclPath;

    QTimer m_timer;
    int m_currentScreenID;
};

#endif // APPMODEL_H
