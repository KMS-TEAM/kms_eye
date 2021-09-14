#ifndef QVISUALODOMETRY_H
#define QVISUALODOMETRY_H

#include <QObject>
#include <QString>
#include <QImage>

#include "QConfig.h"

#include "Common.h"
#include "visual_odom/feature.h"
#include "visual_odom/utils.h"
#include "visual_odom/evaluate_odometry.h"
#include "visual_odom/frame.h"
#include "visual_odom/camera.h"
#include "visual_odom/rgbd_standalone.h"


class QVisualOdometry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage trajectory READ trajectory WRITE setTrajectory NOTIFY trajectoryChanged)
    Q_PROPERTY(QImage featureView READ featureView WRITE setFeatureView NOTIFY featureViewChanged)

public:
    explicit QVisualOdometry(QObject *parent = nullptr);

    void init(QConfig* config);

    QImage trajectory() const;
    QImage featureView() const;

    bool run();

public slots:
    void setTrajectory(QImage trajectory);
    void setFeatureView(QImage featureView);

signals:
    void trajectoryChanged(QImage &trajectory);
    void featureViewChanged(QImage &featureView);

private:
    bool display_ground_truth;
    bool use_intel_rgbd;
    std::vector<Matrix> pose_atrix_gt;

    cv::Mat projMattrixLeft;
    cv::Mat projMattrixRight;

    cv::Mat rotation;
    cv::Mat translation;

    cv::Mat pose;
    cv::Mat Rpose;

    cv::Mat frame_pose;
    cv::Mat frame_pose32;

    std::vector<FeaturePoint> oldFeaturePointsLeft;
    std::vector<FeaturePoint> currentFeaturePointsLeft;

    cv::Mat cvtrajectory;
    FeatureSet currentVOFeatures;
    cv::Mat points4D, points3D;
    int init_frame_id;

    cv::Mat imageRight_t0;
    cv::Mat imageLeft_t0;

    CameraBase *pCamera;

    QImage m_trajectory;
    QImage m_featureView;
};

#endif // QVISUALODOMETRY_H
