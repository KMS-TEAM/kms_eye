#ifndef FEATURE_DETECTOR_BASE_H
#define FEATURE_DETECTOR_BASE_H

#include <Eigen/Core>
#include <opencv2/opencv.hpp>
#include "FeatureDescriptor.h"

class FeatureDetectorBase
{
public:
    FeatureDetectorBase(){};
    virtual ~FeatureDetectorBase(){};

    virtual void Init(int width, int height, int max_number_of_features) = 0;

    virtual void Detect(const cv::Mat &image,
                        Eigen::Matrix2Xd *current_measurements,
                        std::vector<float> *current_feature_orientations,
                        std::vector<float> *current_feature_scales,
                        FeatureDescriptoru *current_feature_descriptors) {}

    virtual void Detect(const cv::Mat &image,
                        Eigen::Matrix2Xf *current_measurements,
                        FeatureDescriptorf *current_feature_descriptors) {}

    virtual bool IsInitialized() const = 0;
    virtual int GetHeight() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetMaxNumberOfFeatures() const = 0;

private:
    FeatureDetectorBase(const FeatureDetectorBase &fd) = delete;
    FeatureDetectorBase &operator=(const FeatureDetectorBase &fd) = delete;
};

#endif