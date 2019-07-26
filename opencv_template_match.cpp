#include "opencv_template_match.h"
#include <QDebug>
#define TEMPLATE_MATCH_ARGSUSE 0b100110111010

static QStringList opencv_template_match_boxs1;
//static QStringList opencv_template_match_boxs2;
static box_info rem_defualt_boxinfo[BOX_NUM]={
    {0, opencv_template_match_boxs1 << "平方差" << "标准平方差" << "相关" <<"标准相关" << "正负相关" <<"标准正负相关"},
    {},
    {}
};

static slider_info rem_defualt_slider[SLIDER_NUM]={
    {"旋转角度", 0, 360, 30, 0, 0},
    {"缩放比例", 1,100,10,1,10},
    {}
};

static line_info rem_defualt_line[LINE_NUM]={
    {"point1", ""},
    {"point2", ""},
    {"point3", ""},
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
        readargsinfo->boxinfo[i]=rem_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=rem_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=rem_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_template_match::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num);
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
    Point2f p1=QSting2Point(arginfo->lineinfo[0].line);
    Point2f p2=QSting2Point(arginfo->lineinfo[1].line);
    Point2f p3=QSting2Point(arginfo->lineinfo[2].line);
    int angle= arginfo->sliderinfo[0].value;//逆时针旋转45度
    double scale= (double)arginfo->sliderinfo[1].value / 10;//缩放比例
    Scalar color=QSting2Scalar(arginfo->lineinfo[4].line);

    qDebug()<<"重映射:"<<"方式"<<arginfo->boxinfo[0].s.at(typeSel) <<angle <<scale;

    Point2f srcPoints[3];//原图中的三点
    Point2f dstPoints[3];//目标图中的三点
    Point2f center(srcMat->cols / 2, srcMat->rows / 2);//旋转中心
    Mat TransformationMatrix;
    Mat map_x, map_y;
    switch (typeSel) {
        case 0://重映射
            matchTemplate()
            break;
        case 1://第一种仿射变换  原图三定点转目标图参数点
            //第一种仿射变换的调用方式：三点法
            //三个点对的值,上面也说了，只要知道你想要变换后图的三个点的坐标，就可以实现仿射变换
            srcPoints[0] = Point2f(0, 0);
            srcPoints[1] = Point2f(0, srcMat->rows - 1);
            srcPoints[2] = Point2f(srcMat->cols - 1, 0);
            //映射后的三个坐标值
            dstPoints[0] = p1;
            dstPoints[1] = p2;
            dstPoints[2] = p3;
            TransformationMatrix = getAffineTransform(srcPoints, dstPoints);//由三个点对计算变换矩阵
            warpAffine(*srcMat, *dstMat, TransformationMatrix, srcMat->size());//仿射变换
            break;
        case 2://第一种仿射变换  原图参数点转目标图三定点
            //第一种仿射变换的调用方式：三点法
            //三个点对的值,上面也说了，只要知道你想要变换后图的三个点的坐标，就可以实现仿射变换
            srcPoints[0] = p1;
            srcPoints[1] = p2;
            srcPoints[2] = p3;
            //映射后的三个坐标值
            dstPoints[0] = Point2f(0, 0);
            dstPoints[1] = Point2f(0, srcMat->rows - 1);
            dstPoints[2] = Point2f(srcMat->cols - 1, 0);
            TransformationMatrix = getAffineTransform(srcPoints, dstPoints);//由三个点对计算变换矩阵
            warpAffine(*srcMat, *dstMat, TransformationMatrix, srcMat->size());//仿射变换
            break;
        case 3://第二种仿射变换
            //第二种仿射变换的调用方式：直接指定角度和比例
            if(p1.x != 0 && p1.y != 0 )center = p1;
            TransformationMatrix = getRotationMatrix2D(center, angle, scale);//计算旋转加缩放的变换矩阵
            warpAffine(*srcMat, *dstMat, TransformationMatrix, srcMat->size());//仿射变换
            break;
        default:
            break;
    }
    return true;
}
