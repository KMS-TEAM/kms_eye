#ifndef QIMAGEITEM_H
#define QIMAGEITEM_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class QImageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    explicit QImageItem(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setImage(const QImage &image);
    void paint(QPainter *painter);
    QImage image() const;

signals:
    void imageChanged();

private:
    QImage current_image;
};

#endif // QIMAGEITEM_H
