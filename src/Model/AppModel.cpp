#include "AppModel.h"
#include "AppConstant.h"
#include <QDebug>
#include <QString>

AppModel* AppModel::m_instance = nullptr;
QMutex AppModel::m_lock;
QConfig* AppModel::m_config = new QConfig(nullptr);

AppModel::AppModel(QObject *parent) :
    QObject(parent), m_currentImagePath("qrc:/images/images/logo.png")
{
    CONSOLE << "Init instance";
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


