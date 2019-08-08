#ifndef OPENCV_ROI_PROCESSING_H
#define OPENCV_ROI_PROCESSING_H
#include "lib4opencvtool.h"

const static QString OPENCV_ROI_PROCESSING_METHOD_NAME="感兴趣区域处理";

class opencv_ROI_processing:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_ROI_PROCESSING_METHOD_NAME;
    opencv_ROI_processing(args_info *_arginfo);
    ~opencv_ROI_processing();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};


#endif // opencv_ROI_processing_H
