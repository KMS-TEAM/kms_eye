#include "sgm/QSGM.h"
#include <string>

QSGM::QSGM(QObject *parent) : QObject(parent)
{

}

QString QSGM::sgm(QStringList imagePath, QConfig *config, int imageNumber) const
{
    cv::Mat imageLeft = cv::imread(imagePath[0].toStdString(), cv::IMREAD_COLOR);
    cv::Mat imageRight = cv::imread(imagePath[1].toStdString(), cv::IMREAD_COLOR);

    std::string disparity_dir = config->get<std::string>("Disparity");
    std::string tail = ".png";
    disparity_dir = disparity_dir + std::to_string(imageNumber) + tail;

    int n = 0;
    int n_err = 0;
    std::vector<float> times;

    uint8_t p1, p2;
    p1 = config->get<uint8_t>("p1");
    p2 = config->get<uint8_t>("p2");

    init_disparity_method(p1, p2);

    // Convert images to grayscale
    cv::cvtColor(imageLeft, imageLeft, cv::COLOR_RGB2GRAY);
    cv::cvtColor(imageRight, imageRight, cv::COLOR_RGB2GRAY);


    // Compute disparity
    float elapsed_time_ms;
    cv::Mat disparity_im = compute_disparity_method(imageLeft, imageRight, &elapsed_time_ms);

    const int type = disparity_im.type();
        const uchar depth = type & CV_MAT_DEPTH_MASK;
        if(depth == CV_8U) {
            cv::imwrite(disparity_dir, disparity_im);
        } else {
            cv::Mat disparity16(disparity_im.rows, disparity_im.cols, CV_16UC1);
            for(int i = 0; i < disparity_im.rows; i++) {
                for(int j = 0; j < disparity_im.cols; j++) {
                    const float d = disparity_im.at<float>(i, j)*256.0f;
                    disparity16.at<uint16_t>(i, j) = (uint16_t) d;
                }
            }
            cv::imwrite(disparity_dir, disparity16);
        }
    QString disparityPath = QString::fromStdString(disparity_dir);
    return disparityPath;
}
