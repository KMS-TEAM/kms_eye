#include "QImageProcessing.h"

QImageProcessing::QImageProcessing(QObject *parent) : QObject(parent)
{

}

void QImageProcessing::SGMAgl(QStringList imagePath, int imageNumber)
{
    QString disparityPath = m_sgm->sgm(imagePath, m_config, imageNumber);
    emit finish(disparityPath);
}

void QImageProcessing::setConfig(QConfig *config)
{
    m_config = config;
}
