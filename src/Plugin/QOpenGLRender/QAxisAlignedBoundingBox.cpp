#include "QOpenGLRender/QAxisAlignedBoundingBox.h"

#include <QDebug>

QAxisAlignedBoundingBox::QAxisAlignedBoundingBox()
    : m_center()
    , m_radii()
{

}

QAxisAlignedBoundingBox::QAxisAlignedBoundingBox(const QVector<QVector3D> &points)
{
    update(points);
}

void QAxisAlignedBoundingBox::update(const QVector<QVector3D> &points)
{
    if (points.isEmpty()) {
            m_center = QVector3D();
            m_radii = QVector3D();
            return;
        }

        QVector3D minPoint = points.at( 0 );
        QVector3D maxPoint = points.at( 0 );

        for ( int i = 1; i < points.size(); ++i )
        {
            const QVector3D& point = points.at( i );
            if (point.x() > maxPoint.x())
            {
                maxPoint.setX(point.x());
            }
            if (point.y() > maxPoint.y())
            {
                maxPoint.setY(point.y());
            }
            if (point.z() > maxPoint.z())
            {
                maxPoint.setZ(point.z());
            }
            if (point.x() < minPoint.x())
            {
                minPoint.setX(point.x());
            }
            if (point.y() < minPoint.y())
            {
                minPoint.setY(point.y());
            }
            if (point.z() < minPoint.z())
            {
                minPoint.setZ(point.z());
            }
        }

        m_center = 0.5 * ( minPoint + maxPoint );
        m_radii = 0.5 * ( maxPoint - minPoint );
    #if 0
        qDebug() << "AABB:";
        qDebug() << "    min =" << minPoint;
        qDebug() << "    max =" << maxPoint;
        qDebug() << " center =" << m_center;
        qDebug() << "  radii =" << m_radii;
    #endif
}

QVector3D QAxisAlignedBoundingBox::center() const
{
    return m_center;
}

QVector3D QAxisAlignedBoundingBox::radii() const
{
    return m_radii;
}

QVector3D QAxisAlignedBoundingBox::minPoint() const
{
    return m_center - m_radii;
}

QVector3D QAxisAlignedBoundingBox::maxPoint() const
{
    return m_center + m_radii;
}

float QAxisAlignedBoundingBox::xExtent() const
{
    return 2.0f * m_radii.x();
}

float QAxisAlignedBoundingBox::yExtent() const
{
    return 2.0f * m_radii.y();
}

float QAxisAlignedBoundingBox::zExtent() const
{
    return 2.0f * m_radii.z();
}

float QAxisAlignedBoundingBox::minExtent() const
{
    return qMax( xExtent(), qMax( yExtent(), zExtent() ) );
}

float QAxisAlignedBoundingBox::maxExtent() const
{
    return qMin( xExtent(), qMin( yExtent(), zExtent() ) );
}

QDebug &operator<<(QDebug &stream, const QAxisAlignedBoundingBox &bbox)
{
    stream << "AABB: min=" << bbox.minPoint() << ", max=" << bbox.maxPoint();
    return stream;
}
