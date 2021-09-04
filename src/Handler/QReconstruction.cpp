#include "QReconstruction.h"
#include "QReconstructionBase.h"
#include <QDebug>
#include <QStringList>

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
    CONSOLE << "LOL";
    m_startIndex = m_config->getInt("StartIndex");
    m_endIndex = m_config->getInt("EndIndex");
    m_currIndex = m_startIndex;

    m_currFrame = readFrame(m_currIndex);

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

    // Add the first vertex to globalOptimizer
    g2o::VertexSE3* v = new g2o::VertexSE3();
    v->setId( m_currIndex );
    v->setEstimate( Eigen::Isometry3d::Identity() ); //estimated as identity matrix
    v->setFixed( true ); //The first vertex is fixed, no optimization
    m_globalOptimizer.addVertex( v );

    m_keyframes.push_back( m_currFrame );

    keyframe_threshold = config->getFloat("KeyframeThreshold");
    check_loop_closure = config->getBool("CheckLoopClosure");
}

QString QReconstruction::reconstruction()
{
    // Start Reconstruction
    CONSOLE << "Go Go !!!!";

    for (m_currIndex = m_startIndex + 1; m_currIndex < m_endIndex; m_currIndex++){
        CONSOLE <<"Reading files "<< m_currIndex ;
        FRAME currFrame = readFrame( m_currIndex); // read currFrame
        computeKeyPointsAndDesp( currFrame, detector, descriptor ); //Extract features
        CHECK_RESULT result = checkKeyframes( m_keyframes.back(), currFrame, m_globalOptimizer); //match this frame with the last frame in keyframes

        switch (result)
        {
        case NOT_MATCHED:
            //No match, skip directly
            CONSOLE << RED"Not enough inliers.";
            break;
        case TOO_FAR_AWAY:
            // It’s too close, just jump
            CONSOLE << RED"Too far away, may be an error.";
            break;
        case TOO_CLOSE:
            // too far, maybe something went wrong
            CONSOLE << RESET"Too close, not a keyframe";
            break;
        case KEYFRAME:
            CONSOLE << GREEN"This is a new keyframe";
            // Not far, not near, just right
            // detect loopback
            if (check_loop_closure)
            {
                checkNearbyLoops( m_keyframes, currFrame, m_globalOptimizer );
                checkRandomLoops( m_keyframes, currFrame, m_globalOptimizer );
            }
            m_keyframes.push_back( currFrame );

            break;
        default:
            break;
        }
    }

    // optimize
    CONSOLE << RESET"optimizing pose graph, vertices: " << m_globalOptimizer.vertices().size();
    m_globalOptimizer.save("../result_before.g2o");
    m_globalOptimizer.initializeOptimization();
    m_globalOptimizer.optimize( 100 ); //You can specify the number of optimization steps
    m_globalOptimizer.save( "../result_after.g2o" );
    CONSOLE << "Optimization done.";

    // Splicing point cloud map
    CONSOLE << "saving the point cloud map...";
    PointCloud::Ptr output (new PointCloud()); //Global map
    PointCloud::Ptr tmp (new PointCloud());

    pcl::VoxelGrid<PointT> voxel; // Grid filter, adjust the map resolution
    pcl::PassThrough<PointT> pass; // z-direction interval filter, because the effective depth interval of the rgbd camera is limited, remove those that are too far away
    pass.setFilterFieldName("z");
    pass.setFilterLimits( 0.0, 4.0 ); //Don't need more than 4m

    double gridsize = m_config->getFloat("VoxelGrid"); //The resolution map can be adjusted in parameters.txt
    voxel.setLeafSize( gridsize, gridsize, gridsize );

    for (size_t i = 0; i< m_keyframes.size(); i++)
    {
        // Take a frame from g2o
        g2o::VertexSE3* vertex = dynamic_cast<g2o::VertexSE3*>(m_globalOptimizer.vertex(m_keyframes[i].frameID));
        Eigen::Isometry3d pose = vertex->estimate(); //The optimized pose of this frame
        PointCloud::Ptr newCloud = image2PointCloud(m_keyframes[i].rgb, m_keyframes[i].depth, m_camera); //Convert to point cloud
        // The following is filtering
        voxel.setInputCloud( newCloud );
        voxel.filter( *tmp );
        pass.setInputCloud( tmp );
        pass.filter( *newCloud );
        // Transform the point cloud and add it to the global map
        pcl::transformPointCloud( *newCloud, *tmp, pose.matrix() );
        *output += *tmp;
        tmp->clear();
        newCloud->clear();
    }

    voxel.setInputCloud( output );
    voxel.filter( *tmp );
    //storage
    std::string pclDir = m_config->getDir("PCL") + "/result.pcd";
    pcl::io::savePCDFile(pclDir, *tmp );

    CONSOLE << "Final map is saved.";

    return QString::fromStdString(pclDir);

    // setPclPath(QString::fromStdString(pclDir));
}

FRAME QReconstruction::readFrame(int index)
{
    // TODO: Need to change for stereo camera
    FRAME f;

    if (m_config->getDir("Type") == "stereo"){
        string leftImage = m_config->getDir("LeftFolder");
        string rightImage = m_config->getDir("RightFolder");

        string rgbExt = m_config->getDir("ImageExtension");

        QStringList imageList;

        // Left Image
        stringstream ss;
        ss << leftImage << index << rgbExt;
        string filename;
        ss >> filename;
        cv::Mat left = cv::imread(filename);
        f.rgb = left;
        imageList.append(QString::fromStdString(filename));

        // Right Image
        ss.clear();
        filename.clear();
        ss << rightImage << index << rgbExt;
        ss>>filename;
        cv::Mat right = cv::imread(filename);
        imageList.append(QString::fromStdString(filename));

        cv::Mat depth = m_sgm->sgm(imageList, m_config, index);

        f.depth = depth;
        //std::cout << filename << std::endl;
        f.frameID = index;
        return f;
    }
    if (m_config->getDir("Type") == "RGB-D"){
        CONSOLE << QString::fromStdString(m_config->getDir("Type"));
        FRAME f;
        string rgbDir =  m_config->getDir("rgb_dir");
        string depthDir =  m_config->getDir("depth_dir");

        string rgbExt =  "." + m_config->getDir("rgb_extension");
        string depthExt =  "." + m_config->getDir("depth_extension");

        stringstream ss;
        ss << rgbDir << index << rgbExt;
        string filename;
        ss >> filename;
        //std::cout << filename << std::endl;
        f.rgb = cv::imread(filename);
        //std::cout << filename << std::endl;

        ss.clear();
        filename.clear();
        ss << depthDir << index << depthExt;
        ss >> filename;

        //std::cout << filename << std::endl;
        f.depth = cv::imread( filename, -1 );
        //std::cout << filename << std::endl;
        f.frameID = index;
        return f;
    }
}

double QReconstruction::normofTransform(cv::Mat rvec, cv::Mat tvec)
{
    return fabs(min(cv::norm(rvec), 2*M_PI-cv::norm(rvec)))+ fabs(cv::norm(tvec));
}

QReconstruction::CHECK_RESULT QReconstruction::checkKeyframes(FRAME &f1, FRAME &f2, g2o::SparseOptimizer &opti, bool is_loops)
{
    static int min_inliers = m_config->getInt("min_inliers");
    static double max_norm = m_config->getFloat("max_norm");
    static double keyframe_threshold = m_config->getFloat("KeyframeThreshold");
    static double max_norm_lp = m_config->getFloat("max_norm_lp");

    // compare f1 and f2
    RESULT_OF_PNP result = estimateMotion( f1, f2, m_camera, m_config);
    if ( result.inliers < min_inliers ) //Inliers are not enough, give up the frame
        return NOT_MATCHED;
    // Calculate whether the range of motion is too large
    double norm = normofTransform(result.rvec, result.tvec);
    if ( is_loops == false )
    {
        if ( norm >= max_norm )
            return TOO_FAR_AWAY;   // too far away, may be error
    }
    else
    {
        if ( norm >= max_norm_lp)
            return TOO_FAR_AWAY;
    }

    if ( norm <= keyframe_threshold )
        return TOO_CLOSE;   // too adjacent frame

    // Add the edge of this vertex to the previous frame to g2o
    // Vertex part
    // Vertex only needs to set id
    if (is_loops == false)
    {
        g2o::VertexSE3 *v = new g2o::VertexSE3();
        v->setId( f2.frameID );
        v->setEstimate( Eigen::Isometry3d::Identity() );
        opti.addVertex(v);
    }
    // Edge part
    g2o::EdgeSE3* edge = new g2o::EdgeSE3();
    // The id of the two vertices connecting this edge
    edge->setVertex( 0, opti.vertex(f1.frameID ));
    edge->setVertex( 1, opti.vertex(f2.frameID ));
    edge->setRobustKernel( new g2o::RobustKernelHuber() );
    // Information matrix
    Eigen::Matrix<double, 6, 6> information = Eigen::Matrix< double, 6,6 >::Identity();

    // The information matrix is the inverse of the covariance matrix,
    // which represents our pre-estimation of the accuracy of the edges
    // Because the pose is 6D, the information matrix is a 6*6 matrix,
    // assuming that the estimation accuracy of position and angle are both 0.1 and independent of each other
    // Then the covariance is a matrix of 0.01 diagonal, and the information matrix is a matrix of 100
    information(0,0) = information(1,1) = information(2,2) = 100;
    information(3,3) = information(4,4) = information(5,5) = 100;
    // You can also set the angle to be larger, which means that the estimation of the angle is more accurate
    edge->setInformation( information );
    // The edge estimation is the result of pnp solution
    Eigen::Isometry3d T = cvMat2Eigen( result.rvec, result.tvec );
    // edge->setMeasurement( T );
    edge->setMeasurement( T.inverse() );
    // Add this edge to the graph
    opti.addEdge(edge);
    return KEYFRAME;
}

void QReconstruction::checkNearbyLoops(std::vector<FRAME> &frames, FRAME &currFrame, g2o::SparseOptimizer &opti)
{
    static int nearby_loops = m_config->getInt("nearby_loops");

    // Just measure the last few in currFrame and frames
    if ( frames.size() <= nearby_loops )
    {
        // no enough keyframes, check everyone
        for (size_t i=0; i<frames.size(); i++)
        {
            checkKeyframes( frames[i], currFrame, opti, true );
        }
    }
    else
    {
        // check the nearest ones
        for (size_t i = frames.size()-nearby_loops; i<frames.size(); i++)
        {
            checkKeyframes( frames[i], currFrame, opti, true );
        }
    }
}

void QReconstruction::checkRandomLoops(std::vector<FRAME> &frames, FRAME &currFrame, g2o::SparseOptimizer &opti)
{
    static int random_loops = m_config->getInt("random_loops");
    srand( (unsigned int) time(NULL) );
    // Randomly take some frames for detection

    if ( frames.size() <= random_loops )
    {
        // no enough keyframes, check everyone
        for (size_t i=0; i<frames.size(); i++)
        {
            checkKeyframes( frames[i], currFrame, opti, true );
        }
    }
    else
    {
        // randomly check loops
        for (int i=0; i<random_loops; i++)
        {
            int index = rand()%frames.size();
            checkKeyframes( frames[index], currFrame, opti, true );
        }
    }
}

void QReconstruction::setPclPath(QString path)
{
    m_pclPath = path;
    emit pclPathChanged(m_pclPath);
}
