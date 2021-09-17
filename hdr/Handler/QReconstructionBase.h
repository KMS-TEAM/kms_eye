#ifndef QRECONSTRUCTIONBASE_H
#define QRECONSTRUCTIONBASE_H

//#include <QObject>
//#include "Common/Common.h"
//#include "AppConstant.h"
//#include "QConfig.h"

//// C++ Standard Library
//#include <fstream>
//#include <vector>
//#include <map>

//// Eigen
//#include <Eigen/Core>
//#include <Eigen/Geometry>

//// OpenCV
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/core/eigen.hpp>

//// PCL
//#include <pcl/io/pcd_io.h>
//#include <pcl/point_types.h>
//#include <pcl/common/transforms.h>
//#include <pcl/visualization/cloud_viewer.h>
//#include <pcl/filters/voxel_grid.h>

//using namespace std;
//// Type definition
//typedef pcl::PointXYZRGBA PointT;
//typedef pcl::PointCloud<PointT> PointCloud;

//// Camera internal parameter structure
//struct CAMERA_INTRINSIC_PARAMETERS
//{
//    double cx, cy, fx, fy, scale;
//};

//// frame structure
//struct FRAME
//{
//    int frameID;
//    cv::Mat rgb, depth; //The color image and depth image corresponding to the frame
//    cv::Mat desp; //Feature Descriptor
//    vector<cv::KeyPoint> kp; //Key point
//};

//// PnP result
//struct RESULT_OF_PNP
//{
//    cv::Mat rvec, tvec;
//    int inliers;
//};


//CAMERA_INTRINSIC_PARAMETERS getCameraParameters(QConfig* config);

//// Function interface
//// image2PonitCloud converts rgb image to point cloud
//PointCloud::Ptr image2PointCloud( cv::Mat& rgb, cv::Mat& depth, CAMERA_INTRINSIC_PARAMETERS& camera );

//// point2dTo3d converts a single point from image coordinates to space coordinates
//// input: 3-dimensional point Point3f (u,v,d)
//cv::Point3f point2dTo3d( cv::Point3f& point, CAMERA_INTRINSIC_PARAMETERS& camera );

//// computeKeyPointsAndDesp extracts key points and feature descriptors at the same time
//void computeKeyPointsAndDesp( FRAME& frame, string detector, string descriptor );

//// estimateMotion calculates the motion between two frames
//// Input: frame 1 and frame 2, camera internal parameters
//RESULT_OF_PNP estimateMotion( FRAME& frame1, FRAME& frame2, CAMERA_INTRINSIC_PARAMETERS& camera, QConfig *config);

//// cvMat2Eigen
//Eigen::Isometry3d cvMat2Eigen( cv::Mat& rvec, cv::Mat& tvec );

//// joinPointCloud
//PointCloud::Ptr joinPointCloud( PointCloud::Ptr original, FRAME& newFrame, Eigen::Isometry3d T, CAMERA_INTRINSIC_PARAMETERS& camera, QConfig *config);


////the following are UBUNTU/LINUX ONLY terminal color
//#define RESET "\033[0m"
//#define BLACK "\033[30m" /* Black */
//#define RED "\033[31m" /* Red */
//#define GREEN "\033[32m" /* Green */
//#define YELLOW "\033[33m" /* Yellow */
//#define BLUE "\033[34m" /* Blue */
//#define MAGENTA "\033[35m" /* Magenta */
//#define CYAN "\033[36m" /* Cyan */
//#define WHITE "\033[37m" /* White */
//#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */
//#define BOLDRED "\033[1m\033[31m" /* Bold Red */
//#define BOLDGREEN "\033[1m\033[32m" /* Bold Green */
//#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
//#define BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
//#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
//#define BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
//#define BOLDWHITE "\033[1m\033[37m" /* Bold White */

#endif // QRECONSTRUCTIONBASE_H
