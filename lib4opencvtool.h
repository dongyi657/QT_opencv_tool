#ifndef LIB4OPENCVTOOL_H
#define LIB4OPENCVTOOL_H
#include <QStringList>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/types_c.h>
using namespace cv;

#define BOX_NUM     3
#define SLIDER_NUM  3
#define LINE_NUM    6

#define USE_BOX_NUM     (2^BOX_NUM-1)<<(SLIDER_NUM+LINE_NUM)
#define USE_SLIDER_NUM  (2^SLIDER_NUM-1)<<LINE_NUM
#define USE_LINE_NUM    (2^LINE_NUM-1)

struct box_info{
    int num;
    QStringList s;
};

struct slider_info{
    QString label_name;
    int min;
    int max;
    int SingleStep;
    int defualt;
    int value;
};

struct line_info{
    QString label;
    QString line;
};

struct args_info{
    int16_t argsuse;
    QString method_name;
    struct  box_info boxinfo[BOX_NUM];
    struct slider_info sliderinfo[SLIDER_NUM];
    struct line_info lineinfo[LINE_NUM];
};

class lib4opencvtool
{
public:
    lib4opencvtool();
   // args_info arginfo;
    int Controlcnt(int num){
        int i=0;
        for(int j=num;j==0;j=j>>1)
            if(j&1)i++;
        return i;
    }
    virtual int16_t GetArgsUse()=0;//参数设置表格使能
    virtual QString Getmethod()=0;//获取默认参数
    virtual void GetDefualtInfo( args_info *readargsinfo)=0;//获取默认参数
    virtual QString ReadInfo( args_info readargsinfo)=0;//读取参数信息
    virtual int WriteInfo(const args_info writearginfo)=0;//写入参数信息
    virtual bool MatTransform(Mat *srcMat,  Mat *dstMat)=0;//图片转换
};

Point QSting2Point(QString str);
Scalar QSting2Scalar(QString str);
#endif // LIB4OPENCVTOOL_H
