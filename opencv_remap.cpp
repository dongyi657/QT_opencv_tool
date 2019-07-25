#include "opencv_remap.h"
#include <QDebug>
#include <QRgb>
#define REMAP_ARGSUSE 0b100110111010

static QStringList opencv_remap_boxs1;
//static QStringList opencv_remap_boxs2;
static box_info rem_defualt_boxinfo[BOX_NUM]={
    {0, opencv_remap_boxs1 << "重映射" << "src定点转三点仿射" << "转dst定点三点仿射" <<"角度比例仿射"},
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

opencv_remap::opencv_remap(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = REMAP_ARGSUSE;
}

opencv_remap::~opencv_remap()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_remap::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_remap::Getmethod(){
    return Methodname;
}

void opencv_remap::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=rem_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=rem_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=rem_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_remap::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num);
    return s;
}

int opencv_remap::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

Point opencv_remap::QSting2Point(QString str){
    qDebug()<<str;
    QStringList point_str=str.split(",");
    Point p={0, 0};
    if (point_str.count()<2) return p;
    p.x=point_str[0].toInt();
    p.y=point_str[1].toInt();
    return p;
}

Scalar opencv_remap::QSting2Scalar(QString str){

    Scalar color=Scalar(0, 0, 0);//black
    QStringList clorstr=str.split(",");
    //background_color = QColor();
    if (clorstr.count()==3){
        color = Scalar(clorstr[2].toInt(), clorstr[1].toInt(), clorstr[0].toInt());
    }
    return color;
}

bool opencv_remap::MatTransform(Mat *srcMat,  Mat *dstMat){
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
            dstMat->create(srcMat->size(), srcMat->type());//创建和原图一样的效果图
            map_x.create(srcMat->size(), CV_32FC1);
            map_y.create(srcMat->size(), CV_32FC1);
            //遍历每一个像素点，改变map_x & map_y的值,实现翻转180度
            for (int j = 0; j < srcMat->rows; j++)
            {
                for (int i = 0; i < srcMat->cols; i++)
                {
                    map_x.at<float>(j, i) = static_cast<float>(i);
                    map_y.at<float>(j, i) = static_cast<float>(srcMat->rows - j);
                }
            }
            //进行重映射操作
            remap(*srcMat, *dstMat, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, color);
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
