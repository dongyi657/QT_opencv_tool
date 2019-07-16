#include "opencv_edge_detection.h"
#include <QDebug>

#define EDGE_DET_ARGSUSE 0x580
static QStringList edge_detect_boxs1;
static box_info edg_defualt_boxinfo[BOX_NUM]={
    {},
    {0, edge_detect_boxs1<<"canny" << "sobel" << "laplacian"},
    {}
};

static slider_info edg_defualt_slider[SLIDER_NUM]={
    {"filter size", 1, 9, 1, 15, 3},
    {"edge size", 1, 9, 1, 15, 3},
    {}
};

static line_info edg_defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {},
    {}
};

opencv_edge_detection::opencv_edge_detection(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = EDGE_DET_ARGSUSE;
}

opencv_edge_detection::~opencv_edge_detection()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_edge_detection::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_edge_detection::Getmethod(){
    return Methodname;
}

void opencv_edge_detection::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=edg_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=edg_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=edg_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_edge_detection::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[1].s.at(arginfo->boxinfo[1].num)+" "+ QString::number(arginfo->sliderinfo[0].value)+" "+ QString::number(arginfo->sliderinfo[1].value);
    return s;
}

int opencv_edge_detection::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

bool opencv_edge_detection::MatTransform(Mat *srcMat,  Mat *dstMat){
    int typeSel = arginfo->boxinfo[1].num;
    int f_KernelValue = arginfo->sliderinfo[0].value;
    int e_KernelValue = arginfo->sliderinfo[1].value;
    Mat DstPic, grayImage, filterImage;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Mat gray, abs_dst;

    qDebug()<<"边缘检测:"<<"检测方法"<<arginfo->boxinfo[1].s.at(typeSel)<<":滤波核大小"<<f_KernelValue<<":滤波器核大小"<<e_KernelValue;
    switch (typeSel) {
    case 0://canny

        //创建与src同类型和同大小的矩阵
        DstPic.create(srcMat->size(), srcMat->type());
        //将原始图转化为灰度图
        cvtColor(*srcMat, grayImage, COLOR_BGR2GRAY);

        //先使用3*3内核来降噪
        blur(grayImage, filterImage, Size(f_KernelValue, f_KernelValue));

        //运行canny算子
        Canny(filterImage, *dstMat, e_KernelValue, e_KernelValue*e_KernelValue, e_KernelValue);
        break;
    case 1://sobel

        //求x方向梯度
        Sobel(*srcMat, grad_x, CV_16S, 1, 0, e_KernelValue, 1, 1,BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);
       // imshow("x方向soble", abs_grad_x);

        //求y方向梯度
        Sobel(*srcMat, grad_y,CV_16S,0, 1, e_KernelValue, 1, 1, BORDER_DEFAULT);
        convertScaleAbs(grad_y,abs_grad_y);
       // imshow("y向soble", abs_grad_y);

        //合并梯度
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, *dstMat);
        break;
    case 2://laplacian
        //高斯滤波消除噪声
        GaussianBlur(*srcMat, *srcMat, Size(f_KernelValue, f_KernelValue), 0, 0, BORDER_DEFAULT);
        //转换为灰度图
        cvtColor(*srcMat, gray, COLOR_RGB2GRAY);
        //使用Laplace函数
        //第三个参数：目标图像深度；第四个参数：滤波器孔径尺寸；第五个参数：比例因子；第六个参数：表示结果存入目标图
        Laplacian(gray, abs_dst, CV_16S, e_KernelValue, 1, 0, BORDER_DEFAULT);
        //计算绝对值，并将结果转为8位
        convertScaleAbs(abs_dst, *dstMat);

        break;
    default:
        break;
    }
    return true;
}
