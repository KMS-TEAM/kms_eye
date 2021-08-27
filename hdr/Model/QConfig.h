#ifndef QCONFIG_H
#define QCONFIG_H

#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QVector>

#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <memory>

class QConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<QString> imagePath READ ImagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QString dataPath READ DataPath WRITE setDataPath NOTIFY dataPathChanged)
public:
    explicit QConfig(QObject *parent = nullptr);

public:
    QVector<QString> ImagePath();
    void setImagePath(QVector<QString> value);

    QString DataPath();
    void setDataPath(QString value);

    // Set a new config file
    void setParameterFile(const std::string &filename);

    // Get a content by key
    template<typename T>
    static T get(const std::string &key);

    std::string getDir(const std::string &key);

    int getInt(const std::string &key);

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
    QVector<QString> image_path;
    QString data_path;
    QDir dir;

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

    // SGM Setting


signals:
    void imagePathChanged();
    void dataPathChanged();

};


#endif // QCONFIG_H
