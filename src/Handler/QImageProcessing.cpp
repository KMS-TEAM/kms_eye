#include "QImageProcessing.h"
#include "QReconstruction.h"

QImageProcessing::QImageProcessing(QObject *parent) : QObject(parent)
{
        m_vo = new QVisualOdometry();

    // Connect signals and slots for visual odometry
    connect(m_vo, &QVisualOdometry::trajectoryChanged, this, &QImageProcessing::setVOTrajectory);
    connect(m_vo, &QVisualOdometry::featureViewChanged, this, &QImageProcessing::setVOFeatureView);

}

void QImageProcessing::SGMAgl(QStringList imagePath, int imageNumber)
{
    cv::Mat disparityPath = m_sgm->sgm(imagePath, m_config, imageNumber);
    QImage result = QImage((const unsigned char*) (disparityPath.data), disparityPath.cols, disparityPath.rows,disparityPath.step, QImage::Format_Indexed8);

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

void QImageProcessing::VisualOdometry()
{
    m_vo->init(m_config);
    bool result = m_vo->runVisualOdometry();

    if (result){
        std::cout << "Nai Xo" << std::endl;
    }
}

void QImageProcessing::setConfig(QConfig *config)
{
    m_config = config;
}

void setVOTrajectory(QImage &trajectory)
{
    m_trajectory = trajectory;

    emit updateTrajectory(m_trajectory);
}

void setVOFeatureView(QImage &featureView)
{
    m_featureView = featureView;

    emit updateFeatureView(m_featureView);
}
