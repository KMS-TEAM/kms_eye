#ifndef QAXISALIGNEDBOUNDINGBOX_H
#define QAXISALIGNEDBOUNDINGBOX_H

#include <QVector>
#include <QVector3D>

#include "AppConstant.h"

class QDebug;

class QAxisAlignedBoundingBox
{
public:
    QAxisAlignedBoundingBox();

    QAxisAlignedBoundingBox(const QVector<QVector3D>& points);

    void update(const QVector<QVector3D>& points);

    QVector3D center() const;
    QVector3D radii() const;

    QVector3D minPoint() const;
    QVector3D maxPoint() const;

    float xExtent() const;
    float yExtent() const;
    float zExtent() const;

    float minExtent() const;
    float maxExtent() const;

private:
    QVector3D m_center;
    QVector3D m_radii;
};

QDebug & operator<<(QDebug & stream, const QAxisAlignedBoundingBox & bbox);

#endif // QAXISALIGNEDBOUNDINGBOX_H
