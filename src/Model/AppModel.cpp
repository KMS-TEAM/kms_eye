#include "AppModel.h"
#include "AppConstant.h"
#include <QDebug>
#include <QString>

AppModel* AppModel::m_instance = nullptr;
QMutex AppModel::m_lock;
QConfig* AppModel::m_config = new QConfig(nullptr);
QImageProcessing* AppModel::m_imageprocessing = new QImageProcessing(nullptr);
AppEnums::APP_STATE AppModel::m_state = AppEnums::APP_STATE::STATE_NONE;

AppModel::AppModel(QObject *parent) :
    QObject(parent)
{
    CONSOLE << "Init instance";
    QStringList image_def;
    image_def.append(IMAGE_DEF);
    image_def.append(IMAGE_DEF);
    setCurrentImagePath(image_def);
    setdisparityMap(IMAGE_DEF);
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
    CONSOLE << "Algorithm : " << algo;
    switch (algo) {
    case static_cast<int>(AppEnums::ALGORITHM::SGBM):{
        CONSOLE << m_currentImagePath[0];
        CONSOLE << m_currentImagePath[1];
        QString results = m_imageprocessing->SGMAgl(m_currentImagePath, m_currentImageNumber);
        setdisparityMap(results);
        break;
    }
    default:
        break;
    }

}

void AppModel::setCurrentImagePath(QStringList currentImagePath)
{
    m_currentImagePath = currentImagePath;
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


