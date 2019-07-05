#include "opencv_morphology.h"
#include <QDebug>

#define MORPHOLOGY_ARGSUSE 0x900
static QStringList morphology_boxs1;
static box_info defualt_boxinfo[BOX_NUM]={
    {1,morphology_boxs1<<"膨胀" << "腐蚀" << "开运算" << "闭运算" << "形态学梯度" << "顶帽" << "黑帽"},
    {},
    {}
};

static slider_info defualt_slider[SLIDER_NUM]={
    {"kernel size", 0, 40, 1, 15, 5},
    {},
    {}
};

static line_info defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {},
    {}
};

opencv_morphology::opencv_morphology(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = MORPHOLOGY_ARGSUSE;
}

opencv_morphology::~opencv_morphology()
{
    for (int i=0;i < BOX_NUM; i++)
        arginfo->boxinfo[i]=defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        arginfo->sliderinfo[i]=defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        arginfo->lineinfo[i]=defualt_line[i];
    arginfo->argsuse = 0;
}

int16_t opencv_morphology::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_morphology::Getmethod(){
    return Methodname;
}

void opencv_morphology::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_morphology::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+QString::number(  arginfo->boxinfo[0].num)+" "+ QString::number(  arginfo->sliderinfo[0].value);
    return s;
}

int opencv_morphology::WriteInfo(const args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

bool opencv_morphology::MatTransform(Mat *srcMat,  Mat *dstMat){

    int typeSel=  arginfo->boxinfo[0].num;
    int m_KernelValue=  arginfo->sliderinfo[0].value;
    //获取内核形状和尺寸
    Mat element = getStructuringElement(MORPH_RECT, Size(m_KernelValue * 2 + 1, m_KernelValue * 2 + 1), Point(m_KernelValue, m_KernelValue));

    qDebug()<<"形态学处理:"<<"形态学方法"<<arginfo->boxinfo[0].s.at(typeSel)<<":核大小"<<m_KernelValue;
    //
    switch (typeSel) {
    case 0://膨胀
        morphologyEx(*srcMat, *dstMat, MORPH_DILATE, element);
        break;
    case 1://腐蚀
        morphologyEx(*srcMat, *dstMat, MORPH_ERODE, element);
        break;
    case 2://开运算
        morphologyEx(*srcMat, *dstMat, MORPH_OPEN, element);
        break;
    case 3://闭运算
        morphologyEx(*srcMat, *dstMat, MORPH_CLOSE, element);
        break;
    case 4://形态学梯度
        morphologyEx(*srcMat, *dstMat, MORPH_GRADIENT, element);
        break;
    case 5://顶帽
        morphologyEx(*srcMat, *dstMat, MORPH_TOPHAT, element);
        break;
    case 6://黑帽
        morphologyEx(*srcMat, *dstMat, MORPH_BLACKHAT, element);
        break;
    default:
        break;
    }
    return true;
}
