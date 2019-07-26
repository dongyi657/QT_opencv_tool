#ifndef OPENCV_TEMPLATE_MATCH_H
#define OPENCV_TEMPLATE_MATCH_H
#include "lib4opencvtool.h"

const static QString OPENCV_TEMPLATE_MATCH_METHOD_NAME="模板匹配";

class opencv_template_match:public lib4opencvtool
{
private:
     args_info *arginfo;
public:
    const QString Methodname = OPENCV_TEMPLATE_MATCH_METHOD_NAME;
    opencv_template_match(args_info *_arginfo);
    ~opencv_template_match();
    virtual int16_t GetArgsUse();
    virtual QString Getmethod();
    virtual void GetDefualtInfo( args_info *readargsinfo);
    virtual QString ReadInfo( args_info readargsinfo);
    virtual int WriteInfo(args_info writearginfo);
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat);
};

#endif // OPENCV_TEMPLATE_MATCH_H
