#include "QImageProcessing.h"
#include "QReconstruction.h"

QImageProcessing::QImageProcessing(QObject *parent) : QObject(parent)
{

}

void QImageProcessing::SGMAgl(QStringList imagePath, int imageNumber)
{
    cv::Mat disparityPath = m_sgm->sgm(imagePath, m_config, imageNumber);
    QImage result = QImage((const unsigned char*) (disparityPath.data), disparityPath.cols, disparityPath.rows, QImage::Format_RGB888);

    emit finishCompute(result);
    // emit finishDisparity(disparityPath);
}

void QImageProcessing::Reconstrction()
{
    QReconstruction* m_rec = new QReconstruction();
    m_rec->init(m_config);
    QString pclPath = m_rec->reconstruction();

    emit finishReconstruction(pclPath);
}

void QImageProcessing::setConfig(QConfig *config)
{
    m_config = config;
}
