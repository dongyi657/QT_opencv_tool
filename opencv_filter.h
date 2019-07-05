#ifndef OPENCV_FILTER_H
#define OPENCV_FILTER_H
#include "lib4opencvtool.h"

const static QString OPENCV_FILTER_METHOD_NAME="滤波";

class opencv_filter:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_FILTER_METHOD_NAME;
    opencv_filter(args_info *_arginfo);
    ~opencv_filter();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(const args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};

#endif // OPENCV_FILTER_H
