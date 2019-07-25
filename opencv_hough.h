#ifndef OPENCV_HOUGH_H
#define OPENCV_HOUGH_H

#include "lib4opencvtool.h"

const static QString OPENCV_HOUGH_METHOD_NAME="霍夫变换";

class opencv_hough:public lib4opencvtool
{
private:
     args_info *arginfo;
     Point QSting2Point(QString str);
public:
    const QString Methodname = OPENCV_HOUGH_METHOD_NAME;
    opencv_hough(args_info *_arginfo);
    ~opencv_hough();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};

#endif // OPENCV_HOUGH_H

