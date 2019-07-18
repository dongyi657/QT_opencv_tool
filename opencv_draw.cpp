#include "opencv_draw.h"
#include <QDebug>
#include <QRgb>
#define DRAW_ARGSUSE 0b110110111111

static QStringList opencv_draw_boxs1;
static QStringList opencv_draw_boxs2;
static box_info dra_defualt_boxinfo[BOX_NUM]={
    {0, opencv_draw_boxs1 << "line" << "rectangle" << "circle" << "ellipse" << "polylines" << "puttext"},
    {0, opencv_draw_boxs2 << "实心" << "空心" },
    {}
};

static slider_info dra_defualt_slider[SLIDER_NUM]={
    {"line width", 1, 9, 1, 0, 3},
    {"radius", 1,201,10,1,10},
    {}
};

static line_info dra_defualt_line[LINE_NUM]={
    {"point1", ""},
    {"point2", ""},
    {"point3", ""},
    {"point4", ""},
    {"color", ""},
    {"text", ""}
};

opencv_draw::opencv_draw(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = DRAW_ARGSUSE;
}

opencv_draw::~opencv_draw()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_draw::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_draw::Getmethod(){
    return Methodname;
}

void opencv_draw::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=dra_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=dra_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=dra_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_draw::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[1].s.at(arginfo->boxinfo[1].num)+" "+ QString::number(arginfo->sliderinfo[0].value)+" "+ QString::number(arginfo->sliderinfo[1].value);
    return s;
}

int opencv_draw::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

Point opencv_draw::QSting2Point(QString str){
    qDebug()<<str;
    QStringList point_str=str.split(",");
    Point p={0, 0};
    if (point_str.count()<2) return p;
    p.x=point_str[0].toInt();
    p.y=point_str[1].toInt();
    return p;
}

bool opencv_draw::MatTransform(Mat *srcMat,  Mat *dstMat){
    int typeSel = arginfo->boxinfo[0].num;
    int SolidorHollow = arginfo->boxinfo[1].num;
    int line_width = arginfo->sliderinfo[0].value;
    int radius= arginfo->sliderinfo[1].value;
    Point p1=QSting2Point(arginfo->lineinfo[0].line);
    Point p2=QSting2Point(arginfo->lineinfo[1].line);
    Point p3=QSting2Point(arginfo->lineinfo[2].line);
    Point p4=QSting2Point(arginfo->lineinfo[3].line);
    std::string str = arginfo->lineinfo[5].line.toStdString();
    QStringList clorstr=arginfo->lineinfo[4].line.split(",");
    Scalar color=Scalar(0, 0, 0);
    //background_color = QColor();
    if (clorstr.count()==3){
        color = Scalar(clorstr[2].toInt(), clorstr[1].toInt(), clorstr[0].toInt());
    }
    Mat DstPic;
    std::vector<cv::Point> vList;
    int font = cv::FONT_HERSHEY_SIMPLEX;

    srcMat->copyTo(DstPic);
    qDebug()<<"绘图:"<<"形状"<<arginfo->boxinfo[0].s.at(typeSel)<<"SH"<<arginfo->boxinfo[1].s.at(SolidorHollow)<<":线粗"<<line_width;
    switch (typeSel) {
        case 0://line
            line(DstPic, p1, p2, color, line_width);
            break;
        case 1://rectangle
            if (SolidorHollow==0)line_width=-1;             //负数实心
            rectangle(DstPic, p1, p2, color, line_width);
            break;
        case 2://circle
            if (SolidorHollow==0)line_width=-1;             //负数实心
            circle(DstPic, p1, radius, color, line_width);
            break;
        case 3://ellipse
            if (SolidorHollow==0)line_width=-1;             //负数实心
            if ( p2.x!=0 && p2.y!=0 ){
                ellipse(DstPic, p1, p2, 0, 0, 360, color, line_width); //绘画图像，椭圆中心，主轴半径，旋转角度，起始角度，结束角度（360度一圈为单位）
            }
            else{
                ellipse(DstPic, p1, cv::Size(100, 50), 0, 0, 360, color, line_width);
            }
            break;
        case 4:         //polylines
    //        Point point1[2][4];
    //        point1[0][0] = cvPoint(200, 40); /* 三角形的点的相关信息 */
    //        point1[0][1] = cvPoint(150, 80);
    //        point1[0][2] = cvPoint(250, 80);
    //        point1[1][0] = cvPoint(150, 100); /* 正方形的相关信息 */
    //        point1[1][1] = cvPoint(250, 100);
    //        point1[1][2] = cvPoint(250, 200);
    //        point1[1][3] = cvPoint(150, 200);
    //        const Point* ppt[2] = { point1[0], point1[1]};
    //        int npts[2] = {3, 4};/* 三角形有三个有效点正方形有四个有效点 */
    //        CvScalar color1 = cvScalar(200, 100, 250);/* 颜色参数 */
    //        //cv::fillPoly(DstPic, ppt, npts, 2, true, color, line_width);
    //        cv::fillPoly(DstPic, ppt, npts, 2, color);

    //        Point point2[2][4];
    //        point2[0][0] = cvPoint(100, 40);
    //        point2[0][1] = cvPoint(50, 80);
    //        point2[0][2] = cvPoint(150, 80);
    //        point2[1][0] = cvPoint(50, 100);
    //        point2[1][1] = cvPoint(150, 100);
    //        point2[1][2] = cvPoint(150, 200); /* 参数的相关含义如上 */
    //        point2[1][3] = cvPoint(50, 200);
    //        const Point* ppt2[2] = { point2[0], point2[1]};
    //        //cv::polylines(DstPic, point2, npts, 2, 1, color, line_width);/* PolyLine 函数的使用*/
    //        cv::polylines(DstPic,ppt2, npts, 1, true, color, line_width);

            vList.push_back(p1);// 点0
            vList.push_back(p2);// 点1
            vList.push_back(p3);// 点2
            vList.push_back(p4); // 点3
            if (SolidorHollow==0)line_width=-1;
            polylines(DstPic, vList, true, color, line_width);
            break;
        case 5:         //puttext
        if (SolidorHollow==0)line_width=-1;
            putText(DstPic, str, p1, font, 4, color, line_width);
            break;
        default:
            break;
    }

    *dstMat=DstPic;
    return true;
}
