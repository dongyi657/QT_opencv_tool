#ifndef OPENCV_CORNER_DETECTION_H
#define OPENCV_CORNER_DETECTION_H

#include "lib4opencvtool.h"


const static QString OPENCV_CORNER_DETECTION_METHOD_NAME="角点检测";

class opencv_corner_detection:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_CORNER_DETECTION_METHOD_NAME;
    opencv_corner_detection(args_info *_arginfo);
    ~opencv_corner_detection();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};


#endif // opencv_ROI_processing_H
