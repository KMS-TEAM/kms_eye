#ifndef QIMAGEPROCESSING_H
#define QIMAGEPROCESSING_H

#include <QObject>
#include <QStringList>
#include "QConfig.h"
#include "QSGM.h"
#include "AppEnums.h"

class QImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit QImageProcessing(QObject *parent = nullptr);

    void setConfig(QConfig* config);

public slots:
    void SGMAgl(QStringList imagePath, int imageNumber);

signals:
    void finishDisparity(QString &result);
private:
    QSGM* m_sgm;
    QConfig* m_config;
};

#endif // QIMAGEPROCESSING_H
