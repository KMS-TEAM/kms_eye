#include "QSGM.h"
#include <string>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

QSGM::QSGM(QObject *parent) : QObject(parent)
{

}

QString QSGM::sgm(QStringList imagePath, QConfig *config, int imageNumber) const
{
    CONSOLE << imagePath[0];
    CONSOLE << imagePath[1];
    cv::Mat imageLeft = cv::imread(imagePath[0].toStdString(), cv::IMREAD_GRAYSCALE);
    cv::Mat imageRight = cv::imread(imagePath[1].toStdString(), cv::IMREAD_GRAYSCALE);

    std::string disparity_dir = config->getDir("Disparity");
    std::string tail = ".png";
    disparity_dir = disparity_dir + std::to_string(imageNumber) + tail;

//    int n = 0;
//    int n_err = 0;
    std::vector<float> times;
    int SADWindowSize, numberOfDisparities;
    bool color_display;
    float scale;

//    uint8_t p1, p2;
//    p1 = config->getInt("p1");
//    p2 = config->getInt("p2");

    // Get config params
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, 16, 3);
    numberOfDisparities = config->getInt("Maxdisparity");
    SADWindowSize = config->getInt("Blocksize");
    scale = config->getFloat("Scale");
    color_display = config->getBool("ColorDisplay");

    // Convert images to grayscale
//    cv::cvtColor(imageLeft, imageLeft, cv::COLOR_RGB2GRAY);
//    cv::cvtColor(imageRight, imageRight, cv::COLOR_RGB2GRAY);

    if (scale != 1.f){
        cv::Mat temp1, temp2;
        int method = scale < 1 ? cv::INTER_AREA : cv::INTER_CUBIC;
        resize(imageLeft, temp1, cv::Size(), scale, scale, method);
        imageLeft = temp1;
        resize(imageRight, temp2, cv::Size(), scale, scale, method);
        imageRight = temp2;
    }

    cv::Size img_size = imageLeft.size();

    cv::Rect roi1, roi2;
    cv::Mat Q;

    // Get Stereo Rectification params
    cv::Mat leftD, leftK, leftR, leftP;
    cv::Mat rightD, rightK, rightR, rightP;

    leftD = config->getMat("LEFT.D");
    leftK = config->getMat("LEFT.K");
    leftR = config->getMat("LEFT.R");
    leftP = config->getMat("LEFT.P");

    rightD = config->getMat("RIGHT.D");
    rightK = config->getMat("RIGHT.K");
    rightR = config->getMat("RIGHT.R");
    rightP = config->getMat("RIGHT.P");


    // Undistort Rectify Map
    cv::Mat map11, map12, map21, map22;
    cv::initUndistortRectifyMap(leftK, leftD, leftR, leftR, img_size, CV_16SC2, map11, map12);
    cv::initUndistortRectifyMap(rightK, rightD, rightR, rightP, img_size, CV_16SC2, map21, map22);

    cv::Mat img1r, img2r;
    cv::remap(imageLeft, img1r, map11, map12, cv::INTER_LINEAR);
    cv::remap(imageRight, img2r, map21, map22, cv::INTER_LINEAR);

    imageLeft = img1r;
    imageRight = img2r;

    // SGBM setting
    numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((img_size.width/8) + 15) & -16;
    sgbm->setPreFilterCap(63);
    int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
    sgbm->setBlockSize(sgbmWinSize);

    int cn = imageLeft.channels();
    sgbm->setP1(8*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setP2(32*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(numberOfDisparities);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);

    sgbm->setMode(cv::StereoSGBM::MODE_SGBM);

    // Compute disparity
    cv::Mat disparity_im, disp8;
    float disparity_multiplier = 1.0f;

    sgbm->compute(imageLeft, imageRight, disparity_im);
    if (disparity_im.type() == CV_16S)
        disparity_multiplier = 16.0f;

    disparity_im.convertTo(disp8, CV_8U);

    CONSOLE << "Bug";
    cv::Mat disp8_3c;
    if (color_display)
        cv::applyColorMap(disp8, disp8_3c, cv::COLORMAP_TURBO);

    imwrite(disparity_dir, color_display ? disp8_3c : disp8);

//    if(pointCloud)
//    {
//        printf("storing the point cloud...");
//        fflush(stdout);
//        cv::Mat xyz;
//        cv::Mat floatDisp;
//        disparity_im.convertTo(floatDisp, CV_32F, 1.0f / disparity_multiplier);
//        reprojectImageTo3D(floatDisp, xyz, Q, true);
//        saveXYZ(disparity_dir_pointcloud.c_str(), xyz);
//        printf("\n");
//    }

    QString disparityPath = QString::fromStdString(disparity_dir);
    return disparityPath;
}

void QSGM::saveXYZ(const char *filename, const cv::Mat &mat)
{
    const double max_z = 1.0e4;
    FILE* fp = fopen(filename, "wt");
    for(int y = 0; y < mat.rows; y++)
    {
        for(int x = 0; x < mat.cols; x++)
        {
            cv::Vec3f point = mat.at<cv::Vec3f>(y, x);
            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }
    fclose(fp);
}
