#include "QImageItem/QImageItem.h"
#include "AppConstant.h"
#include <QDebug>

QImageItem::QImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    this->current_image = QImage(":/images/images/logo.png");
}

void QImageItem::paint(QPainter *painter)
{
    QRectF bounding_rect = boundingRect();
    CONSOLE << bounding_rect.height() << " " << bounding_rect.width();
    QImage scaled = this->current_image.scaledToHeight(bounding_rect.height());
    CONSOLE << "Bug";
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
   painter->drawImage(center, scaled);
}

QImage QImageItem::image() const
{    return this->current_image;
}

void QImageItem::setImage(const QImage &image)
{
    this->current_image = image;
    update();
}
