#ifndef QSGM_H
#define QSGM_H

#include <QObject>
#include "Common.h"
#include "QConfig.h"

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
#include "disparity_method.h"

class QSGM : public QObject
{
    Q_OBJECT
public:
    explicit QSGM(QObject *parent = nullptr);

    std::vector<cv::Mat> sgm(QStringList imagePath, QConfig* config) const;

signals:

private:

};

#endif // QSGM_H
