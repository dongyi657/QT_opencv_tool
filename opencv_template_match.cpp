#include "opencv_template_match.h"
//截屏
#include <QScreen>
#include <QGuiApplication>
#include <QMainWindow>
//截屏
#include <QDebug>

#define TEMPLATE_MATCH_ARGSUSE 0b110111000011
#define MAX_MATCH_NUM 20
static QStringList opencv_template_match_boxs1;
static QStringList opencv_template_match_boxs2;
//static QStringList opencv_template_match_boxs2;
static box_info match_defualt_boxinfo[BOX_NUM]={
    {0, opencv_template_match_boxs1 << "平方差" << "标准平方差" << "相关" <<"标准相关" << "正负相关" <<"标准正负相关"},
    {0, opencv_template_match_boxs2 << "打开图像中查找" << "屏幕查找" },
    {}
};

static slider_info match_defualt_slider[SLIDER_NUM]={
    {"Line width", 1, 9, 1, 0, 3},
    {"Precision", 0, 100, 10, 90, 90},
    {"Matching number", 1, MAX_MATCH_NUM, 1, 0, 1}
};

static line_info match_defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {"color", ""},
    {"file_name", ""}
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
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num)+
            ":精度"+QString::number(arginfo->sliderinfo[1].value)+
            ":线粗"+QString::number(arginfo->sliderinfo[0].value)+
            ":num"+QString::number(arginfo->sliderinfo[2].value);
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
    int MatchN= arginfo->sliderinfo[2].value;
    Scalar color=QSting2Scalar(arginfo->lineinfo[4].line);
    Mat match_image = imread(arginfo->lineinfo[5].line.toLatin1().data());
    if (!match_image.data){
        return false;
    }
    if(arginfo->boxinfo[0].num==1){
        //截屏
        //问题 Question Problem
        //放在mainwindow.cpp可以截屏，这里不可以，待解！！！！！！！！！！！！！！！！
        //
        Mat pm;
        QScreen *screen = QGuiApplication::primaryScreen();
        pm = QImageToMat(screen->grabWindow(0).toImage());
        if (!srcMat->data){
            qDebug() << "屏幕未捕获";
        }else {
            *srcMat = pm;
        }
        imshow("screen", *srcMat);
        //截屏
    }
    qDebug()<<"图像匹配:"<<"方式"<<arginfo->boxinfo[0].s.at(typeSel)<<":精度"<<Precision<<":线粗"<<line_width<<":num"<<MatchN;
    Mat  templ=match_image, result;
    int result_cols = srcMat->cols - match_image.cols + 1;
    int result_rows = srcMat->rows - match_image.rows + 1;
    result.create(result_cols, result_rows, CV_32FC1);

    double minVal = -1;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc[MAX_MATCH_NUM];
    if ( MatchN>1 && (typeSel%2==0) )
        typeSel=typeSel+1;
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
    int i=0,match_num;
    for (i=0; i<MatchN;i++){
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
        qDebug()<< "("<<matchLoc[i].x<<","<<matchLoc[i].y<<")"<<QString::number(minVal,10,5)<<" "<<QString::number(maxVal,10,5);
        //circle(g_resultImage, matchLocation[i], 3, Scalar(255, 0, 0), 2, 8, 0);
        //cout<< minValue <<minLocation <<maxValue <<maxLocation<<endl;
        if (typeSel == TM_CCORR_NORMED || typeSel == TM_CCOEFF_NORMED)
        {
            if (maxVal*100<Precision)break;
            matchLoc[i] = maxLoc;
            //cout<<QString::number(g_resultImage.ptr<float>(maxLocation.y)[maxLocation.x]).toStdString()<<endl;
            result.ptr<float>(maxLoc.y)[maxLoc.x]=0;
            //rectangle(g_resultImage, matchLocation[i], Point(matchLocation[i].x + g_tempalteImage.cols, matchLocation[i].y + g_tempalteImage.rows), Scalar(0, 0, 0), -1, 8, 0);
        }
        else
        {
            if (minVal*100>(100-Precision))break;
            matchLoc[i] = minLoc;
            //cout<<QString::number(g_resultImage.ptr<float>(minLocation.y)[minLocation.x]).toStdString()<<endl;
            result.ptr<float>(minLoc.y)[minLoc.x]=1;
            //rectangle(g_resultImage, matchLocation[i], Point(matchLocation[i].x + g_tempalteImage.cols, matchLocation[i].y + g_tempalteImage.rows), Scalar(255, 255, 255), -1, 8, 0);
        }

    }
    match_num=i;
    for (i=i-1; i>=0;i--){
        rectangle(*srcMat, matchLoc[i], Point(matchLoc[i].x + match_image.cols, matchLoc[i].y + match_image.rows), color, line_width, 8, 0);
        putText(*srcMat, QString::number(i).toStdString(), Point(matchLoc[i].x-10,matchLoc[i].y+20), cv::FONT_HERSHEY_SIMPLEX, 1, color, line_width);
    }
    *dstMat=*srcMat;
    return true;
}
