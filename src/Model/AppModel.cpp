#include "AppModel.h"
#include "AppConstant.h"
#include <QDebug>
#include <QString>
#include <QThread>

AppModel* AppModel::m_instance = nullptr;
QMutex AppModel::m_lock;
QConfig* AppModel::m_config = new QConfig(nullptr);
QImageProcessing* AppModel::m_imageprocessing = new QImageProcessing(nullptr);
QCameraCapture* AppModel::m_camcapture = new QCameraCapture(nullptr);
AppEnums::APP_STATE AppModel::m_state = AppEnums::APP_STATE::STATE_NONE;

AppModel::AppModel(QObject *parent) :
    QObject(parent),
    m_currentScreenID{static_cast<int>(AppEnums::IMAGE_VIEWER_SCREEN)}
{
    CONSOLE << "Init instance";
    QStringList image_def;
    image_def.append(IMAGE_DEF);
    image_def.append(IMAGE_DEF);
    setCurrentImagePath(image_def);
    setdisparityMap(IMAGE_DEF);

    m_pclPath = "/home/lacie/Github/kms_eye/data/bunny.pcd";

    m_timer.setInterval(500);

    connect(&m_timer, &QTimer::timeout, this, &AppModel::increaseIndex);
    connect(this, &AppModel::runImageProcessing, m_imageprocessing, &QImageProcessing::SGMAgl);
    // connect(m_imageprocessing, &QImageProcessing::finishDisparity, this, &AppModel::setdisparityMap);
    connect(m_imageprocessing, &QImageProcessing::finishCompute, this, &AppModel::setDisparityImage);
    connect(this, &AppModel::runReconstruction, m_imageprocessing, &QImageProcessing::Reconstrction);
    connect(m_imageprocessing, &QImageProcessing::finishReconstruction, this, &AppModel::setPclPath);
    connect(m_camcapture, &QCameraCapture::frameCaptured, this, &AppModel::setCurrentFrame);
}

AppModel *AppModel::instance(){
    m_lock.lock();
    if (nullptr == m_instance){
        m_instance = new AppModel();
    }
    m_lock.unlock();
    return m_instance;
}

int AppModel::currentImageNumber() const
{
    return m_currentImageNumber;
}

QStringList AppModel::currentImagePath() const
{
    return m_currentImagePath;
}

QString AppModel::settingPath() const
{
    return m_settingPath;
}

AppEnums::APP_STATE AppModel::state() const
{
    return m_state;
}

QString AppModel::disparityMap() const
{
    return m_disparityMap;
}

QImage AppModel::disparityImage() const
{
    return m_disparityImage;
}

QString AppModel::pclPath() const
{
    return m_pclPath;
}

int AppModel::currentScreenID() const
{
    return m_currentScreenID;
}

void AppModel::setListImage()
{
    QVector<QString> imagePath = m_config->ImagePath();

    qDebug() << imagePath[0];
    qDebug() << imagePath[1];

    QDir dir;

    // Left path
    dir.setPath(imagePath[0]);
    QFileInfoList listLeft = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Files);
    QStringList filelistLeft;

    foreach(QFileInfo info, listLeft){
        filelistLeft.append(info.filePath());
        // qDebug() << info.filePath();
    }

    // Right path
    dir.setPath(imagePath[1]);
    QFileInfoList listRight = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Files);
    QStringList filelistRight;

    foreach(QFileInfo info, listRight){
        filelistRight.append(info.filePath());
        // qDebug() << info.filePath();
    }

    m_listImage.append(filelistLeft);
    m_listImage.append(filelistRight);

    m_currentImageNumber = 0;

    setCurrentImageNumber(m_currentImageNumber);

}

QVector<QStringList> AppModel::getListImages() const
{
    return m_listImage;
}

void AppModel::imageProcessing(AppEnums::ALGORITHM algo)
{
    if (m_state == AppEnums::APP_STATE::STATE_RUNNING){
        m_timer.start();
    }

    if (m_state == AppEnums::APP_STATE::STATE_STOP){
        m_timer.stop();
    }
}

void AppModel::cameraRun(QString path)
{
    m_camcapture->initCamera(path, &m_lock);
    m_camcapture->start();
}

void AppModel::setCurrentImagePath(QStringList currentImagePath)
{
    m_currentImagePath = currentImagePath;
//    imageProcessing(AppEnums::ALGORITHM::SGBM);
    emit runImageProcessing(m_currentImagePath, m_currentImageNumber);
    emit currentImagePathChanged();
}

void AppModel::setCurrentImageNumber(int currentImageNumber)
{
    QStringList currentImagePath;
    currentImagePath << m_listImage.at(0)[currentImageNumber] << m_listImage.at(1)[currentImageNumber] ;
    CONSOLE << currentImagePath;
    setCurrentImagePath(currentImagePath);

    // emit currentImageNumberChanged();
}

void AppModel::setSettingPath(QString settingPath)
{
    m_settingPath = settingPath;
    m_config->setDataPath(m_settingPath);
    m_imageprocessing->setConfig(m_config);
    m_currentImageNumber = 0;
    m_state = AppEnums::APP_STATE::STATE_STOP;
    setListImage();
}

void AppModel::setState(AppEnums::APP_STATE state)
{
    m_state = state;

    emit stateChanged();
}

void AppModel::setdisparityMap(QString disparityMap)
{
    m_disparityMap = disparityMap;

    emit disparityMapChanged();
}

void AppModel::increaseIndex()
{
    m_currentImageNumber++;
    setCurrentImageNumber(m_currentImageNumber);

    emit runImageProcessing(m_currentImagePath, m_currentImageNumber);
}

void AppModel::setDisparityImage(QImage disparityImage)
{
    CONSOLE << "Bug";
    m_disparityImage = disparityImage;

    emit disparityImageChanged(m_disparityImage);
}

void AppModel::setPclPath(QString pclPath)
{
    CONSOLE << pclPath;
    m_pclPath = pclPath;
    CONSOLE << m_pclPath;
    emit pclPathChanged();
}

void AppModel::setCurrentScreenID(int currentScreenID)
{
    if(m_currentScreenID == currentScreenID){
        return;
    }

    m_currentScreenID = currentScreenID;
    emit currentScreenIDChanged(m_currentScreenID);
}

void AppModel::setCurrentFrame(cv::Mat *frame)
{
    // CONSOLE << frame->cols << " " << frame->rows;

    QImage img = QImage(frame->data,frame->cols,frame->rows,QImage::Format_RGB888).rgbSwapped();

    m_currentFrame = img;

    CONSOLE << m_currentFrame.width() << " " << m_currentFrame.height();

    emit currentFrameChanged(m_currentFrame);
}


