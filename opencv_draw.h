#ifndef OPENCV_DRAW_H
#define OPENCV_DRAW_H
#include "lib4opencvtool.h"

const static QString OPENCV_DRAW_METHOD_NAME="绘图";

class opencv_draw:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_DRAW_METHOD_NAME;
    opencv_draw(args_info *_arginfo);
    ~opencv_draw();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};

#endif // OPENCV_DRAW_H
