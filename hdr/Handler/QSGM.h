#ifndef QSGM_H
#define QSGM_H

#include <QObject>
#include "Common.h"
#include "QConfig.h"
#include "AppConstant.h"

#include <iostream>
#include <numeric>
#include <sys/time.h>
#include <vector>
#include <stdlib.h>
#include <typeinfo>
#include <opencv2/opencv.hpp>

#include <numeric>
#include <stdlib.h>
#include <ctime>
#include <sys/types.h>
#include <stdint.h>
#include <linux/limits.h>
#include <dirent.h>
#include <iostream>
#include <fstream>

class QSGM : public QObject
{
    Q_OBJECT
public:
    explicit QSGM(QObject *parent = nullptr);

    cv::Mat sgm(QStringList imagePath, QConfig* config, int imageNumber) const;

    static void saveXYZ(const char* filename, const cv::Mat& mat);

signals:

private:

};

#endif // QSGM_H
