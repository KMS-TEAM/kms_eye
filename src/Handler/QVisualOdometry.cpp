#include "QVisualOdometry.h"

QVisualOdometry::QVisualOdometry(QObject *parent) : QObject(parent)
{

}

void QVisualOdometry::init(QConfig *config)
{

}

QImage QVisualOdometry::trajectory() const
{
    return m_trajectory;
}

QImage QVisualOdometry::featureView() const
{
    return m_featureView;
}

bool QVisualOdometry::run()
{
    return true;
}

void QVisualOdometry::setTrajectory(QImage trajectory)
{
    m_trajectory = trajectory;

    emit trajectoryChanged(m_trajectory);
}

void QVisualOdometry::setFeatureView(QImage featureView)
{
    m_featureView = featureView;

    emit featureViewChanged(m_featureView);
}
