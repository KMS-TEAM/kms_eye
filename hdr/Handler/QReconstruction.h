#ifndef QRECONSTRUCTION_H
#define QRECONSTRUCTION_H

#include <QObject>
#include <QString>

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
#include "AppConstant.h"
#include "QSGM.h"

// Put the definition of g2o to the front
typedef g2o::BlockSolver_6_3 SlamBlockSolver;
typedef g2o::LinearSolverEigen< SlamBlockSolver::PoseMatrixType> SlamLinearSolver;

class QReconstruction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pclPath READ pclPath WRITE setPclPath NOTIFY pclPathChanged)

    // detect two frames and define the result
    enum CHECK_RESULT{
        NOT_MATCHED = 0,
        TOO_FAR_AWAY,
        TOO_CLOSE,
        KEYFRAME,
    };

public:
    explicit QReconstruction(QObject *parent = nullptr);
    QString pclPath() const;

    void init(QConfig* config);
    QString reconstruction();

private:
    // Givent index, and read a frame of data
    FRAME readFrame(int index);
    // Estimate the size of a movement
    double normofTransform(cv::Mat rvec, cv:: Mat tvec);

    // Function declaration
    CHECK_RESULT checkKeyframes(FRAME& f1, FRAME& f2, g2o::SparseOptimizer& opti, bool is_loops = false);
    // Detect short-range loops
    void checkNearbyLoops(std::vector<FRAME>& frames, FRAME& currFrame, g2o::SparseOptimizer& opti);
    // Random detection loop
    void checkRandomLoops(std::vector<FRAME>& frames, FRAME& currFrame, g2o::SparseOptimizer& opti);

public slots:
    void setPclPath(QString path);
signals:
    void pclPathChanged(QString path);

private:
    QConfig* m_config;
    QSGM* m_sgm;
    g2o::SparseOptimizer m_globalOptimizer;

    std::vector<FRAME> m_keyframes;
    FRAME m_currFrame;
    double keyframe_threshold;
    bool check_loop_closure;

    int m_startIndex;
    int m_endIndex;
    int m_currIndex;

    QString m_pclPath;
    std::string detector;
    std::string descriptor;

    CAMERA_INTRINSIC_PARAMETERS m_camera;
};

#endif // QRECONSTRUCTION_H
