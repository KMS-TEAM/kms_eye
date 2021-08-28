#ifndef QIMAGEPROCESSING_H
#define QIMAGEPROCESSING_H

#include <QObject>
#include <QStringList>
#include "QConfig.h"
#include "QSGM.h"

class QImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit QImageProcessing(QObject *parent = nullptr);

    QString SGMAgl(QStringList imagePath, int imageNumber);
    void setConfig(QConfig* config);

signals:

private:
    QSGM* m_sgm;
    QConfig* m_config;
};

#endif // QIMAGEPROCESSING_H
