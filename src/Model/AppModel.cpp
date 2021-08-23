#include "AppModel.h"
#include "AppConstant.h"
#include <QDebug>

AppModel* AppModel::m_instance = nullptr;
QMutex AppModel::m_lock;

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

QStringList AppModel::currentImagePath() const
{

}

QString AppModel::settingPath() const
{

}

AppEnums::APP_STATE AppModel::state() const
{

}

void AppModel::setListImage()
{

}

QVector<QStringList> AppModel::getListImages() const
{

}

void AppModel::setCurrentImagePath(QStringList currentImagePath)
{

}

void AppModel::setSettingPath(QString settingPath)
{

}

void AppModel::setState(AppEnums::APP_STATE state)
{

}



//QString AppModel::currentPath() const
//{
//    return m_currentPath;
//}

//void AppModel::setListImages()
//{
//    // do somthing here to set list images
//}

//QStringList AppModel::getListImages() const
//{
//    // do something here
//    return m_listImages;
//}

//void AppModel::setCurrentPath(QString currentPath)
//{
//    CONSOLE << currentPath;
//    if (m_currentPath == currentPath)
//        return;
//    m_currentPath = currentPath;
//    emit currentPathChanged();
//}
