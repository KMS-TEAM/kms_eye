#ifndef QIMAGEPROCESSING_H
#define QIMAGEPROCESSING_H

#include <QObject>
#include <QStringList>
#include <QImage>
#include "QConfig.h"
#include "QSGM.h"
#include "QReconstruction.h"
#include "QVisualOdometry.h"
#include "AppEnums.h"

class QImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit QImageProcessing(QObject *parent = nullptr);

    void setConfig(QConfig* config);

public slots:
    void SGMAgl(QStringList imagePath, int imageNumber);
    void Reconstrction();
    void VisualOdometry();

signals:
    void finishDisparity(QString &result);
    void finishCompute(QImage &result);
    void finishReconstruction(QString &result);
    void updateTrajectory(QImage &trajectory);
    void updateFeatureView(QImage &featureView);
private:
    QSGM* m_sgm;
    QVisualOdometry* m_vo;
    QConfig* m_config;
};

#endif // QIMAGEPROCESSING_H
