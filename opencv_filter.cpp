#include "opencv_filter.h"
#include <QDebug>

#define FILTER_ARGSUSE 0b100100000000
static QStringList filter_boxs1;
static box_info fil_defualt_boxinfo[BOX_NUM]={
    {0, filter_boxs1<<"方框滤波" << "均值滤波" << "高斯滤波" << "中值滤波" << "双边滤波"},
    {},
    {}
};

static slider_info fil_defualt_slider[SLIDER_NUM]={
    {"K(2n+1)", 0, 4, 1, 1, 1},
    {},
    {}
};

static line_info fil_defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {},
    {}
};

opencv_filter::opencv_filter(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = FILTER_ARGSUSE;
}

opencv_filter::~opencv_filter()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_filter::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_filter::Getmethod(){
    return Methodname;
}

void opencv_filter::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=fil_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=fil_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=fil_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_filter::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num)+" "+ QString::number(arginfo->sliderinfo[0].value*2+1);
    return s;
}

int opencv_filter::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

bool opencv_filter::MatTransform(Mat *srcMat,  Mat *dstMat){

    int typeSel = arginfo->boxinfo[0].num;
    int f_KernelValue = arginfo->sliderinfo[0].value*2+1;

    qDebug()<<"滤波操作:"<<"滤波方法"<<arginfo->boxinfo[0].s.at(typeSel)<<":核大小"<<f_KernelValue;
    switch (typeSel) {
    case 0://方框滤波
        boxFilter(*srcMat, *dstMat, -1, Size(f_KernelValue, f_KernelValue));
        break;
    case 1://均值
        blur(*srcMat, *dstMat, Size(f_KernelValue, f_KernelValue));
        break;
    case 2://高斯滤波
        GaussianBlur(*srcMat, *dstMat, Size(f_KernelValue, f_KernelValue), 0, 0);
        break;
    case 3://中值滤波
        medianBlur(*srcMat, *dstMat, f_KernelValue);
        break;
    case 4://双边滤波
        bilateralFilter(*srcMat, *dstMat, f_KernelValue*f_KernelValue, f_KernelValue*f_KernelValue*2, f_KernelValue*f_KernelValue/2);
        break;
    default:
        break;
    }
    return true;
}
