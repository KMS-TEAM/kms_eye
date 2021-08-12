#include "AppModel.h"
#include "AppConstant.h"

AppModel* AppModel::self = nullptr;
AppModel *AppModel::instance()
{
    if (nullptr == self)
        self = new AppModel();
    return self;
}

AppModel::AppModel()
    : m_currentPath{"000/000/000"}
{
    CONSOLE << "Init instance";
}

QString AppModel::currentPath() const
{
    return m_currentPath;
}

void AppModel::setListImages()
{
    // do somthing here to set list images
}

QStringList AppModel::getListImages() const
{
    // do something here
    return m_listImages;
}

void AppModel::setCurrentPath(QString currentPath)
{
    CONSOLE << currentPath;
    if (m_currentPath == currentPath)
        return;
    m_currentPath = currentPath;
    emit currentPathChanged();
}

