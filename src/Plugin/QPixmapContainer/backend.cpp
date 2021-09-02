#include <QQmlEngine>
#include "QPixmapContainer/backend.h"
#include "QPixmapContainer/QPixmapContainer.h"

Backend* Backend::m_instance = nullptr;
QMutex Backend::m_lock;

Backend::Backend(QObject *parent) : QObject(parent)
{
}

QObject *Backend::getPixmapContainer() const
{
    QPixmapContainer *pc = new QPixmapContainer();
    pc->m_pixmap.load("qrc:/images/images/logo.png");
    Q_ASSERT((!pc->m_pixmap.isNull()));
    QQmlEngine::setObjectOwnership(pc, QQmlEngine::JavaScriptOwnership);

    return pc;
}

Backend *Backend::instance(){
    m_lock.lock();
    if (nullptr == m_instance){
        m_instance = new Backend();
    }
    m_lock.unlock();
    return m_instance;
}




