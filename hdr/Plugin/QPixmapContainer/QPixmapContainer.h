#ifndef QPIXMAPCONTAINER_H
#define QPIXMAPCONTAINER_H

#include <QObject>
#include <QPixmap>

class QPixmapContainer : public QObject
{
    Q_OBJECT
public:
    explicit QPixmapContainer(QObject *parent = nullptr);
    QPixmap m_pixmap;

signals:

};

#endif // QPIXMAPCONTAINER_H
