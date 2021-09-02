#include "QPixmapContainer/pixmapimage.h"

PixmapImage::PixmapImage(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

void PixmapImage::setImage(QObject *pixmapContainer)
{
    QPixmapContainer *pc = qobject_cast<QPixmapContainer*>(pixmapContainer);
    Q_ASSERT(pc);
    m_pixmapContianer.m_pixmap = pc->m_pixmap;
    update();
}

void PixmapImage::paint(QPainter *painter)
{
    painter->drawPixmap(0, 0, width(), height(), m_pixmapContianer.m_pixmap);
}
