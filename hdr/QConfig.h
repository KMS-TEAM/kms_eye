#ifndef QCONFIG_H
#define QCONFIG_H

#include <QObject>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <memory>

class QConfig : public QObject
{
    Q_OBJECT
public:
    explicit QConfig(QObject *parent = nullptr);

public:
    /// Set a new config file
    static void setParameterFile(const std::string &filename);

    /// Get a content by key
    template<typename T>
    static T get(const std::string &key);

    /// Get a vector of content by key
    template<typename T>
    static std::vector<T> getVector(const std::string &key);

    /** Get a content by key
             * The content is of type string ("true", "false").
             * It's then convertd to bool and returned.
             */
    static bool getBool(const std::string &key);

    ~QConfig();

    // Parse the config file
    bool ParseCamParam(bool is_stereo = true, bool is_imu_stereo = true);
    bool ParseFeatureParam();  // ORB Feature
    bool ParseIMUParam();

    bool GetParamFromGUI();

private:
    static std::shared_ptr<QConfig> config_;
    cv::FileStorage file_;

    /** @brief: Get content by key
             * If key doesn't exist, throw runtime error;
             */
    static cv::FileNode get_(const std::string &key);

protected:
    std::string sCameraName;

    //Calibration matrix
    int mbRGB;
    cv::Mat mK;
    cv::Mat mDistCoef;
    float mbf;
    std::vector<float> vCamCalib;

    // Threshold close/far points
    // Points seen as close by the stereo/RGBD sensor are considered reliable
    // and inserted from just one frame. Far points requiere a match in two keyframes.
    float mThDepth;

    // ORB Setting
    int nFeatures;
    int nLevels;
    int fIniThFAST;
    int fMinThFAST;
    float fScaleFactor;

    // IMU Setting
    cv::Mat Tbc;
    float freq;
    float Ng;
    float Na;
    float Ngw;
    float Naw;

    // Setreo camera
    cv::Mat mTlr;
    std::vector<float> vCamCalib2;

signals:

};

/// Get a content of cv::FileNode. Convert to type T
template<typename T>
T QConfig::get(const std::string &key) {
    cv::FileNode content = QConfig::get_(key);
    return static_cast<T>(content);
}

/// Get a content of cv::FileNode. Convert to type vector<T>
template<typename T>
std::vector<T> QConfig::getVector(const std::string &key) {
    cv::FileNode content = QConfig::get_(key);
    std::vector<T> res;
    content >> res;
    return res;
}

#endif // QCONFIG_H
