#include "QVisualOdometry.h"
#include "AppConstant.h"
#include <QDebug>

QVisualOdometry::QVisualOdometry(QObject *parent) : QObject(parent)
{

}

void QVisualOdometry::init(QConfig *config)
{
    m_config = config;
    display_ground_truth = m_config->getBool("display_ground_truth");

    if (display_ground_truth){
        std::string filename_pose = m_config->getDir("ground_truth");
        pose_matrix_gt = loadPoses(filename_pose);
    }

    datasetPath = m_config->getDir("Dataset");
    startIndex = m_config->getInt("startIndex");
    stopIndex = m_config->getInt("stopIndex");

    float fx = m_config->getFloat("Camera.fx");
    float fy = m_config->getFloat("Camera.fy");
    float cx = m_config->getFloat("Camera.cx");
    float cy = m_config->getFloat("Camera.cy");
    float bf = m_config->getFloat("Camera.bf");

    projMattrixLeft  = (cv::Mat_<float>(3, 4) << fx, 0., cx, 0., 0., fy, cy, 0., 0,  0., 1., 0.);
    projMattrixRight = (cv::Mat_<float>(3, 4) << fx, 0., cx, 0., 0., fy, cy, 0., 0,  0., 1., 0.);

    rotation = cv::Mat::eye(3, 3, CV_64F);
    translation = cv::Mat::zeros(3, 1, CV_64F);

    pose = cv::Mat::zeros(3, 1, CV_64F);
    Rpose = cv::Mat::eye(3, 3, CV_64F);

    frame_pose = cv::Mat::eye(4, 4, CV_64F);
    frame_pose32 = cv::Mat::eye(4, 4, CV_32F);

    cvtrajectory = cv::Mat::zeros(600, 1200, CV_8UC3);

    // Load first image
    init_frame_id = startIndex;
    cv::Mat imageLeft_t0_color;
    loadImageLeft(imageLeft_t0_color, imageLeft_t0, init_frame_id, datasetPath);
    cv::Mat imageRight_t0_color;
    loadImageRight(imageRight_t0_color, imageRight_t0, init_frame_id, datasetPath);
}

QImage QVisualOdometry::trajectory() const
{
    return m_trajectory;
}

QImage QVisualOdometry::featureView() const
{
    return m_featureView;
}

bool QVisualOdometry::runVisualOdometry()
{
    bool isSuccess = true;

    int frame_id = 0;
    for (frame_id = init_frame_id + 1; frame_id < stopIndex; frame_id++){
        CONSOLE << "Frame ID: " << frame_id;

        cv::Mat imageRight_t1,  imageLeft_t1;

        // Load Image
        cv::Mat imageLeft_t1_color;
        loadImageLeft(imageLeft_t1_color,  imageLeft_t1, frame_id, datasetPath);
        cv::Mat imageRight_t1_color;
        loadImageRight(imageRight_t1_color, imageRight_t1, frame_id, datasetPath);

        t_a = clock();
        std::vector<cv::Point2f> oldPointsLeft_t0 = currentVOFeatures.points;

        std::vector<cv::Point2f> pointsLeft_t0, pointsRight_t0, pointsLeft_t1, pointsRight_t1;
        matchingFeatures( imageLeft_t0, imageRight_t0,
                          imageLeft_t1, imageRight_t1,
                          currentVOFeatures,
                          pointsLeft_t0,
                          pointsRight_t0,
                          pointsLeft_t1,
                          pointsRight_t1);

        imageLeft_t0 = imageLeft_t1;
        imageRight_t0 = imageRight_t1;

        std::vector<cv::Point2f>& currentPointsLeft_t0 = pointsLeft_t0;
        std::vector<cv::Point2f>& currentPointsLeft_t1 = pointsLeft_t1;

        std::vector<cv::Point2f> newPoints;
        std::vector<bool> valid; // valid new points are ture

        // Triagulate 3D Points
        cv::Mat points3D_t0, points4D_t0;
        cv::triangulatePoints( projMattrixLeft,  projMattrixRight,  pointsLeft_t0,  pointsRight_t0,  points4D_t0);
        cv::convertPointsFromHomogeneous(points4D_t0.t(), points3D_t0);

        // Tracking transfomation
        clock_t tic_gpu = clock();
        trackingFrame2Frame(projMattrixLeft, projMattrixRight, pointsLeft_t0, pointsLeft_t1, points3D_t0, rotation, translation, false);
        clock_t toc_gpu = clock();
        CONSOLE << "tracking frame 2 frame: " << float(toc_gpu - tic_gpu)/CLOCKS_PER_SEC*1000 << "ms";
        displayTracking(imageLeft_t1, pointsLeft_t0, pointsLeft_t1);

        // Intergrating Visual Odometry
        cv::Vec3f rotation_euler = rotationMatrixToEulerAngles(rotation);

        cv::Mat rigid_body_transformation;

        if(abs(rotation_euler[1])<0.1 && abs(rotation_euler[0])<0.1 && abs(rotation_euler[2])<0.1)
        {
            integrateOdometryStereo(frame_id, rigid_body_transformation, frame_pose, rotation, translation);

        } else {

            std::cout << "Too large rotation"  << std::endl;
        }
        t_b = clock();
        float frame_time = 1000*(double)(t_b-t_a)/CLOCKS_PER_SEC;
        float fps = 1000/frame_time;
        std::cout << "[Info] frame times (ms): " << frame_time << std::endl;
        std::cout << "[Info] FPS: " << fps << std::endl;

        cv::Mat xyz = frame_pose.col(3).clone();
        // display(frame_id, cvtrajectory, xyz, pose_matrix_gt, fps, display_ground_truth);
        QDisplayTrajectory(frame_id, cvtrajectory, xyz, pose_matrix_gt, fps, display_ground_truth);
    }

    if (frame_id != stopIndex){
        isSuccess = false;
    }
    return isSuccess;
}

void QVisualOdometry::QDisplayTracking(cv::Mat &imageLeft_t1, std::vector<cv::Point2f> &pointsLeft_t0, std::vector<cv::Point2f> &pointsLeft_t1)
{
    // Display feature racking
    int radius = 2;
    cv::Mat vis;

    cv::cvtColor(imageLeft_t1, vis, cv::COLOR_GRAY2BGR, 3);

    for (int i = 0; i < pointsLeft_t0.size(); i++)
    {
        cv::circle(vis, cv::Point(pointsLeft_t0[i].x, pointsLeft_t0[i].y), radius, CV_RGB(0,255,0));
    }

    for (int i = 0; i < pointsLeft_t1.size(); i++)
    {
        cv::circle(vis, cv::Point(pointsLeft_t1[i].x, pointsLeft_t1[i].y), radius, CV_RGB(255,0,0));
    }

    for (int i = 0; i < pointsLeft_t1.size(); i++)
    {
        cv::line(vis, pointsLeft_t0[i], pointsLeft_t1[i], CV_RGB(0,255,0));
    }

    QImage result = QImage((const unsigned char*)(vis.data), vis.cols, vis.rows, QImage::Format_RGB888);

    setFeatureView(result);

}

void QVisualOdometry::QDisplayTrajectory(int frame_id, cv::Mat &trajectory, cv::Mat &pose, std::vector<Matrix> &pose_matrix_gt, float fps, bool show_gt)
{
    // draw estimated trajectory
    int x = int(pose.at<double>(0)) + 300;
    int y = int(pose.at<double>(2)) + 100;
    circle(trajectory, cv::Point(x, y) ,1, CV_RGB(255,0,0), 2);

    if (show_gt)
    {
        // draw ground truth trajectory
        cv::Mat pose_gt = cv::Mat::zeros(1, 3, CV_64F);

        pose_gt.at<double>(0) = pose_matrix_gt[frame_id].val[0][3];
        pose_gt.at<double>(1) = pose_matrix_gt[frame_id].val[0][7];
        pose_gt.at<double>(2) = pose_matrix_gt[frame_id].val[0][11];
        x = int(pose_gt.at<double>(0)) + 300;
        y = int(pose_gt.at<double>(2)) + 100;
        circle(trajectory, cv::Point(x, y) ,1, CV_RGB(255,255,0), 2);
    }

    QImage result = QImage((const unsigned char*)(trajectory.data), trajectory.cols, trajectory.rows, QImage::Format_RGB888);

    setTrajectory(result);
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
