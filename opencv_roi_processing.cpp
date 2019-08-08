#include "opencv_ROI_processing.h"
#include <QDebug>

#define opencv_ROI_processing_ARGSUSE 0b110101110001
#define MAX_MATCH_NUM 20
static QStringList opencv_ROI_processing_boxs1;
static QStringList opencv_ROI_processing_boxs2;
//static QStringList opencv_ROI_processing_boxs2;
static box_info opencv_ROI_processing_defualt_boxinfo[BOX_NUM]={
    {0, opencv_ROI_processing_boxs1 << "ROI裁剪" << "LOGO添加" << "图像修复" <<"图片掩模"},
    {0, opencv_ROI_processing_boxs2 << "LOGO添加alphaValue" << "LOGO添加覆盖" },
    {}
};

static slider_info opencv_ROI_processing_defualt_slider[SLIDER_NUM]={
    {"inpaint Radius", 1, 9, 1, 3, 3},
    {"kernel Size", 1, 9, 1, 3, 3},
    {"alpha Value", 1, 10, 1, 5, 5}
};

static line_info opencv_ROI_processing_defualt_line[LINE_NUM]={
    {"Point", ""},
    {"w,h", ""},
    {},
    {},
    {"", ""},
    {"file_name", ""}
};

opencv_ROI_processing::opencv_ROI_processing(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = opencv_ROI_processing_ARGSUSE;
}

opencv_ROI_processing::~opencv_ROI_processing()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_ROI_processing::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_ROI_processing::Getmethod(){
    return Methodname;
}

void opencv_ROI_processing::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=opencv_ROI_processing_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=opencv_ROI_processing_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=opencv_ROI_processing_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_ROI_processing::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num)+
            ":精度"+QString::number(arginfo->sliderinfo[1].value)+
            ":线粗"+QString::number(arginfo->sliderinfo[0].value)+
            ":num"+QString::number(arginfo->sliderinfo[2].value);
    return s;
}

int opencv_ROI_processing::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

bool opencv_ROI_processing::MatTransform(Mat *srcMat,  Mat *dstMat){
    int typeSel = arginfo->boxinfo[0].num;
    int LOGO_add_method = arginfo->boxinfo[1].num;
    int inpaintRadius= arginfo->sliderinfo[0].value;
    int kernelSize=arginfo->sliderinfo[1].value;
    double alphaValue=arginfo->sliderinfo[2].value;
    Point p1=QSting2Point(arginfo->lineinfo[0].line);
    Point p2=QSting2Point(arginfo->lineinfo[1].line);
    Mat logo_image = imread(arginfo->lineinfo[5].line.toLatin1().data());
    if (!logo_image.data && typeSel==1){
        qDebug()<<"读取原始logo图失败！";
        return false;
    }
    Mat imageMask;
    qDebug()<<"ROI:"<<"方式"<<arginfo->boxinfo[0].s.at(typeSel)<<":方式"<<arginfo->boxinfo[1].s.at(LOGO_add_method)<<":修复半径"<<inpaintRadius<<":alphaValue"<<alphaValue;
    Mat Kernel = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
    if (p2.x==0 && p2.y==0 && logo_image.data){
        p2.x=logo_image.cols;
        p2.y=logo_image.rows;
    }
    Mat ROI,imageGray;
    switch (typeSel){
        case 0://ROI裁剪
            ROI.copyTo(*dstMat);
            break;
        case 1://LOGO添加
            ROI = (*srcMat)(Rect(p1.x, p1.y, logo_image.cols, logo_image.rows));
            imshow("ROI",ROI);
            switch (LOGO_add_method) {
                case 0://LOGO添加alphaValue
                    qDebug()<<":alphaValue"<<alphaValue;
                    addWeighted(ROI, 1-alphaValue/10, logo_image, alphaValue, 0., ROI);//dst = src1[I]*alpha+ src2[I]*beta + gamma；第一第四个参数就是各自权重，第5个参数就是公式中的偏执因子gamma。
                    break;
                case 1://LOGO添加覆盖
                    logo_image.copyTo(ROI, imageMask);
                    break;
                default:
                    break;
            }
            *dstMat=*srcMat;
            break;
        case 2://图像修复
            //转换为灰度图
            ROI = (*srcMat)(Rect(p1, p2));
            cvtColor(ROI, imageGray, CV_RGB2GRAY, 0);
            imageMask = Mat(ROI.size(), CV_8UC1, Scalar::all(0));

            //通过阈值处理生成Mask
            threshold(ROI, imageMask, 240, 255, CV_THRESH_BINARY);
            //对Mask膨胀处理，增加Mask面积
            dilate(imageMask, imageMask, Kernel);

            //图像修复
            inpaint(ROI, imageGray, ROI, inpaintRadius, INPAINT_TELEA);
            *dstMat=*srcMat;
            break;
        case 3://图片掩模
            {
                Rect r1(p1, p2);
                Mat img1, img2, img3, img4;
                imageMask = Mat::zeros(srcMat->size(), CV_8UC1);
                (*srcMat)(r1).setTo(255);
                img1 = (*srcMat)(r1);
                srcMat->copyTo(img2, imageMask);

                srcMat->copyTo(img3);
                img3.setTo(0, imageMask);


                imshow("Image sequence", *srcMat);
                imshow("img1", img1);
                imshow("img2", img2);
                imshow("img3", img3);
                imshow("mask", imageMask);
                waitKey();
            }
            break;
        default:
            break;
    }
    return true;
}

