#include "QReconstruction.h"
#include <QDebug>

QReconstruction::QReconstruction(QObject *parent) : QObject(parent)
{

}

QString QReconstruction::pclPath() const
{
    return m_pclPath;
}

void QReconstruction::init(QConfig *config)
{
    CONSOLE << "Reconstrcutor Initializing...";
    m_config = config;
    m_startIndex = m_config->getInt("StartIndex");
    m_endIndex = m_config->getInt("EndIndex");
    m_currIndex = m_startIndex;
    m_currFrame = readFrame(m_currIndex, m_config);

    m_camera = getCameraParameters(m_config);
    detector = "ORB";
    descriptor = "ORB";

    computeKeyPointsAndDesp(m_currFrame, detector, descriptor);

    std::unique_ptr<g2o::BlockSolver_6_3::LinearSolverType> linearSolver
            (new g2o::LinearSolverEigen<g2o::BlockSolver_6_3::PoseMatrixType>());

    std::unique_ptr<g2o::BlockSolver_6_3> solver_ptr(new g2o::BlockSolver_6_3(std::move(linearSolver)));
    g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg(std::move(solver_ptr));

    m_globalOptimizer.setAlgorithm(solver);
    m_globalOptimizer.setVerbose(false);
}

void QReconstruction::reconstruction()
{

}

FRAME QReconstruction::readFrame(int index, QConfig *config)
{

}

double QReconstruction::normofTransform(cv::Mat rvec, cv::Mat tvec)
{

}

QReconstruction::CHECK_RESULT QReconstruction::checkKeyframes(FRAME &f1, FRAME &f2, g2o::SparseOptimizer &opti, bool is_loops)
{

}

void QReconstruction::checkNearbyLoops(std::vector<FRAME> &frames, FRAME &currFrame, g2o::SparseOptimizer &opti)
{

}

void QReconstruction::checkRandomLoops(std::vector<FRAME> &frames, FRAME &currFrame, g2o::SparseOptimizer &opti)
{

}

void QReconstruction::setPclPath(QString path)
{

}
