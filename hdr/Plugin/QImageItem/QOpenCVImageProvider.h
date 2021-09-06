#ifndef QOPENCVIMAGEPROVIDER_H
#define QOPENCVIMAGEPROVIDER_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>
#include <QFuture>

class QOpenCVImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit QOpenCVImageProvider(QObject *parent = nullptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

public slots:
    void updateImage(const QImage &image);

signals:
    void imageChanged();

private:
    QImage image;
};

#endif // QOPENCVIMAGEPROVIDER_H
