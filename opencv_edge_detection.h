#ifndef OPENCV_EDGE_DETECTION_H
#define OPENCV_EDGE_DETECTION_H
#include "lib4opencvtool.h"

const static QString OPENCV_EDGE_DETECT_METHOD_NAME="边缘检测";

class opencv_edge_detection:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_EDGE_DETECT_METHOD_NAME;
    opencv_edge_detection(args_info *_arginfo);
    ~opencv_edge_detection();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(const args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};



#endif // OPENCV_EDGE_DETECTION_H
