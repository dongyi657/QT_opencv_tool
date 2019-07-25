#ifndef OPENCV_REMAP_H
#define OPENCV_REMAP_H
#include "lib4opencvtool.h"

const static QString OPENCV_REMAP_METHOD_NAME="重映射，仿射变换";

class opencv_remap:public lib4opencvtool
{
private:
     args_info *arginfo;
     Point QSting2Point(QString str);
     Scalar QSting2Scalar(QString str);
public:
    const QString Methodname = OPENCV_REMAP_METHOD_NAME;
    opencv_remap(args_info *_arginfo);
    ~opencv_remap();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};

#endif // OPENCV_REMAP_H

