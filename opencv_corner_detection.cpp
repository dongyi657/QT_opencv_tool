#include "opencv_corner_detection.h"
#include <QDebug>

#define opencv_corner_detection_ARGSUSE 0b100111000010
static QStringList opencv_corner_detection_boxs1;
static QStringList opencv_corner_detection_boxs2;
//static QStringList opencv_corner_detection_boxs2;
static box_info opencv_corner_detection_defualt_boxinfo[BOX_NUM]={
    {0, opencv_corner_detection_boxs1 << "Harris" << "Shi-Tomasi" << "相关" <<"标准相关" << "正负相关" <<"标准正负相关"},
    {},
    {}
};

static slider_info opencv_corner_detection_defualt_slider[SLIDER_NUM]={
    {"Precision", 1, 175, 1, 30, 30},
    {"radius", 1, 5, 1, 3, 3},
    {"find number", 1, 100, 10, 10, 10}
};

static line_info opencv_corner_detection_defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {"color", ""},
    {}
};

opencv_corner_detection::opencv_corner_detection(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = opencv_corner_detection_ARGSUSE;
}

opencv_corner_detection::~opencv_corner_detection()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_corner_detection::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_corner_detection::Getmethod(){
    return Methodname;
}

void opencv_corner_detection::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=opencv_corner_detection_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=opencv_corner_detection_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=opencv_corner_detection_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_corner_detection::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num)+
            ":精度"+QString::number(arginfo->sliderinfo[1].value)+
            ":线粗"+QString::number(arginfo->sliderinfo[0].value)+
            ":num"+QString::number(arginfo->sliderinfo[2].value);
    return s;
}

int opencv_corner_detection::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

bool opencv_corner_detection::MatTransform(Mat *srcMat,  Mat *dstMat){
    int typeSel = arginfo->boxinfo[0].num;
    Scalar color=QSting2Scalar(arginfo->lineinfo[4].line);
    double thresh = arginfo->sliderinfo[0].value;
    int radius = arginfo->sliderinfo[1].value;

  //  RNG rng(12345);  //RNG：random number generator，随机数产生器
    Mat  g_grayImage, outimage, normImage, scaledImage;//灰度图, 归一化后的图, 线性变换后的八位无符号整型的图

    int maxCorners = 23;

    //Harris 参数
    int kernel_size = 3;

    //初始化 Shi-Tomasi algorithm的一些参数
    std::vector<Point2f> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    *dstMat = srcMat->clone();
    cvtColor(*srcMat, g_grayImage, CV_BGR2GRAY);
    switch (typeSel) {
        case 0://Harris
            //进行角点检测
            //第三个参数表示邻域大小，第四个参数表示Sobel算子孔径大小，第五个参数表示Harris参数
            cornerHarris(g_grayImage, outimage, blockSize, kernel_size, k, BORDER_DEFAULT);

            // 归一化与转换
            normalize(outimage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
        //    convertScaleAbs(normImage, scaledImage);//将归一化后的图线性变换成8位无符号整型

            // 将检测到的，且符合阈值条件的角点绘制出来
            for (int j = 0; j < normImage.rows; j++)
            {
                for (int i = 0; i < normImage.cols; i++)
                {
                    //Mat::at<float>(j,i)获取像素值，并与阈值比较
                    if ((int)normImage.at<float>(j, i) > thresh + 80)
                    {
                        circle(*dstMat, Point(i, j), radius, color, 1, 8, 0);
                     //   circle(scaledImage, Point(i, j), radius, color, 2, 8, 0);
                    }
                }
            }
            break;
        case 1://Shi-Tomasi
            // 角点检测
//        第一个参数是输入图像（8位或32位单通道图）。
//        第二个参数是检测到的所有角点，类型为vector或数组，由实际给定的参数类型而定。
//                  如果是vector，那么它应该是一个包含cv::Point2f的vector对象；
//                  如果类型是cv::Mat,那么它的每一行对应一个角点，点的x、y位置分别是两列。
//        第三个参数用于限定检测到的点数的最大值。
//        第四个参数表示检测到的角点的质量水平（通常是0.10到0.01之间的数值，不能大于1.0）。
//        第五个参数用于区分相邻两个角点的最小距离（小于这个距离得点将进行合并）。
//        第六个参数是mask，如果指定，它的维度必须和输入图像一致，且在mask值为0处不进行角点检测。
//        第七个参数是blockSize，表示在计算角点时参与运算的区域大小，
//        常用值为3，但是如果图像的分辨率较高则可以考虑使用较大一点的值。
//        第八个参数用于指定角点检测的方法，如果是true则使用Harris角点检测，false则使用Shi Tomasi算法。
//        第九个参数是在使用Harris算法时使用，最好使用默认值0.04

            goodFeaturesToTrack(g_grayImage, corners, maxCorners, qualityLevel,
                                minDistance, Mat(), blockSize, useHarrisDetector, k);

            //画出检测到的角点
            qDebug() << "** Number of corners detected: " << corners.size() << endl;
            for (int i = 0; i < corners.size(); i++)
            {
                circle(*dstMat, corners[i], radius, color, 1, 8, 0);
            }
            break;
        default:
            break;
    }
    return true;
}
