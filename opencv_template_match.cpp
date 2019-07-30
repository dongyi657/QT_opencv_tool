#include "opencv_template_match.h"
#include <QDebug>
#define TEMPLATE_MATCH_ARGSUSE 0b100110000010

static QStringList opencv_template_match_boxs1;
//static QStringList opencv_template_match_boxs2;
static box_info match_defualt_boxinfo[BOX_NUM]={
    {0, opencv_template_match_boxs1 << "平方差" << "标准平方差" << "相关" <<"标准相关" << "正负相关" <<"标准正负相关"},
    {},
    {}
};

static slider_info match_defualt_slider[SLIDER_NUM]={
    {"line width", 1, 9, 1, 0, 3},
    {"Precision", 0, 100, 10, 90, 90},
    {}
};

static line_info match_defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {"color", ""},
    {}
};

opencv_template_match::opencv_template_match(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = TEMPLATE_MATCH_ARGSUSE;
}

opencv_template_match::~opencv_template_match()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_template_match::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_template_match::Getmethod(){
    return Methodname;
}

void opencv_template_match::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=match_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=match_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=match_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_template_match::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num)+":精度"+QString::number(arginfo->sliderinfo[1].value)+":线粗"+QString::number(arginfo->sliderinfo[0].value);
    return s;
}

int opencv_template_match::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

bool opencv_template_match::MatTransform(Mat *srcMat,  Mat *dstMat){
    int typeSel = arginfo->boxinfo[0].num;
    int line_width = arginfo->sliderinfo[0].value;
    int Precision= arginfo->sliderinfo[1].value;
    Scalar color=QSting2Scalar(arginfo->lineinfo[4].line);
    qDebug()<<"图像匹配:"<<"方式"<<arginfo->boxinfo[0].s.at(typeSel)<<":精度"<<Precision<<":线粗"<<line_width;
    Mat  templ=*dstMat, result;
    int result_cols = srcMat->cols - dstMat->cols + 1;
    int result_rows = srcMat->rows - dstMat->rows + 1;
    result.create(result_cols, result_rows, CV_32FC1);

    double minVal = -1;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;
    switch (typeSel) {
        case 0://平方差
            matchTemplate(*srcMat, templ, result, CV_TM_SQDIFF);//数值越小匹配度越好,0最好break;
            break;
        case 1://标准平方差匹配
            matchTemplate(*srcMat, templ, result, CV_TM_SQDIFF_NORMED);//数值越小匹配度越好,0最好
            break;
        case 2://相关匹配
            matchTemplate(*srcMat, templ, result, CV_TM_CCORR);//数值越大匹配度越好,0最坏
            break;
        case 3://标准相关匹配
            matchTemplate(*srcMat, templ, result, CV_TM_CCORR_NORMED);//数值越大匹配度越好,0最坏
            break;
        case 4://正负相关匹配
            matchTemplate(*srcMat, templ, result, CV_TM_CCOEFF);//1完美匹配，-1糟糕匹配，0没有任何相关性
            break;
        case 5://标准正负相关匹配
            matchTemplate(*srcMat, templ, result, CV_TM_CCOEFF_NORMED);//1完美匹配，-1糟糕匹配，0没有任何相关性
            break;
        default:
            break;
    }
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    qDebug() << "匹配度：" << minVal<<maxVal;
    if (typeSel == 0 || typeSel == 1)
    {
        matchLoc = minLoc;
    }
    else
    {
        matchLoc = maxLoc;
    }
    *dstMat=*srcMat;
    rectangle(*dstMat, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), color, line_width, 8, 0);
    return true;
}
