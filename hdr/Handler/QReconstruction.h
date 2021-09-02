#ifndef QRECONSTRUCTION_H
#define QRECONSTRUCTION_H

#include <QObject>

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>

#include <g2o/types/slam3d/types_slam3d.h>
#include <g2o/core/sparse_optimizer.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/factory.h>
#include <g2o/core/optimization_algorithm_factory.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/core/robust_kernel.h>
#include <g2o/core/robust_kernel_impl.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/eigen_types.h>

#include "QReconstructionBase.h"

class QReconstruction : public QObject
{
    Q_OBJECT
public:
    explicit QReconstruction(QObject *parent = nullptr);

signals:

};

#endif // QRECONSTRUCTION_H
