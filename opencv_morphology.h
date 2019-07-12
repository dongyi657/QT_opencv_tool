#ifndef OPENCV_MORPHOLOGY_H
#define OPENCV_MORPHOLOGY_H
#include "lib4opencvtool.h"

const static QString OPENCV_MORPHOLOGY_METHOD_NAME="形态学运算";

class opencv_morphology:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_MORPHOLOGY_METHOD_NAME;
    opencv_morphology(args_info *_arginfo);
    ~opencv_morphology();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};

#endif // OPENCV_MORPHOLOGY_H
