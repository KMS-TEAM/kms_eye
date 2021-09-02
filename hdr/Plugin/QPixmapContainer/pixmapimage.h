#ifndef PIXMAPIMAGE_H
#define PIXMAPIMAGE_H

#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>
#include "QPixmapContainer/QPixmapContainer.h"

class PixmapImage : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit PixmapImage(QQuickItem *parent = Q_NULLPTR);
    Q_INVOKABLE void setImage(QObject *pixmapContainer);

protected:
    virtual void paint(QPainter *painter) Q_DECL_OVERRIDE;

signals:

private:
    QPixmapContainer m_pixmapContianer;
};

#endif // PIXMAPIMAGE_H
