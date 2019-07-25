#include "opencv_hough.h"
#include <QDebug>
#include <QVector>
#define HOUGH_ARGSUSE 0b100110000010

static QStringList opencv_hough_boxs1;
//static QStringList opencv_hough_boxs2;
static box_info hou_defualt_boxinfo[BOX_NUM]={
    {0, opencv_hough_boxs1 << "line1" << "line2"<< "circle"},
    {},
    {}
};

static slider_info hou_defualt_slider[SLIDER_NUM]={
    {"line width", 1, 9, 1, 0, 3},
    {"Precision", 1,201,10,1,10},
    {}
};

static line_info hou_defualt_line[LINE_NUM]={
    {},
    {},
    {},
    {},
    {"color", ""},
    {}
};

opencv_hough::opencv_hough(args_info *_arginfo):arginfo(_arginfo)
{
    arginfo->argsuse = HOUGH_ARGSUSE;
}

opencv_hough::~opencv_hough()
{
    memset(arginfo, 0, sizeof(*arginfo));
}

int16_t opencv_hough::GetArgsUse(){
    return arginfo->argsuse;
}

QString opencv_hough::Getmethod(){
    return Methodname;
}

void opencv_hough::GetDefualtInfo(args_info *readargsinfo){
    for (int i=0;i < BOX_NUM; i++)
        readargsinfo->boxinfo[i]=hou_defualt_boxinfo[i];
    for (int i=0;i < SLIDER_NUM; i++)
        readargsinfo->sliderinfo[i]=hou_defualt_slider[i];
    for (int i=0;i < LINE_NUM; i++)
        readargsinfo->lineinfo[i]=hou_defualt_line[i];
    readargsinfo->argsuse = arginfo->argsuse;
}

QString opencv_hough::ReadInfo(args_info readargsinfo){
    readargsinfo=*arginfo;
    QString s=Methodname+":"+arginfo->boxinfo[0].s.at(arginfo->boxinfo[0].num)+" "+ QString::number(arginfo->sliderinfo[1].value)+" "+ QString::number(arginfo->sliderinfo[0].value);
    return s;
}

int opencv_hough::WriteInfo(args_info writearginfo){
    for (int i=0;i < BOX_NUM; i++)
          arginfo->boxinfo[i].num=writearginfo.boxinfo[i].num;
    for (int i=0;i < SLIDER_NUM; i++)
         arginfo->sliderinfo[i].value=writearginfo.sliderinfo[i].value;
    for (int i=0;i < LINE_NUM; i++)
          arginfo->lineinfo[i].line=writearginfo.lineinfo[i].line;
    return 1;
}

Point opencv_hough::QSting2Point(QString str){
    qDebug()<<str;
    QStringList point_str=str.split(",");
    Point p={0, 0};
    if (point_str.count()<2) return p;
    p.x=point_str[0].toInt();
    p.y=point_str[1].toInt();
    return p;
}

bool opencv_hough::MatTransform(Mat *srcMat,  Mat *dstMat){
    int typeSel = arginfo->boxinfo[0].num;
    int line_width = arginfo->sliderinfo[0].value;
    int Precision= arginfo->sliderinfo[1].value;
    QStringList clorstr=arginfo->lineinfo[4].line.split(",");
    Scalar color=Scalar(0, 0, 0);
    //background_color = QColor();
    if (clorstr.count()==3){
        color = Scalar(clorstr[2].toInt(), clorstr[1].toInt(), clorstr[0].toInt());
    }

    Mat midImage;
    // 定义矢量结构存放检测出来的直线
    std::vector<Vec2f> lines2f;
    std::vector<Vec4i> lines4i;
    std::vector<Vec3f> circles3f;
    qDebug()<<"霍夫变换:"<<"方法"<<arginfo->boxinfo[0].s.at(typeSel)<<":精度"<<Precision<<":线粗"<<line_width;
    switch (typeSel) {
        case 0://line 角度，半径
            //边缘检测
            Canny(*srcMat, midImage, 100, 200, 3);
            //灰度化
            cvtColor(midImage, *dstMat, CV_GRAY2BGR);
            //通过这个函数，我们就可以得到检测出来的直线集合了
            HoughLines(midImage, lines2f, 1, CV_PI / 180, Precision, 0, 0);
            //这里注意第五个参数，表示阈值，阈值越大，表明检测的越精准，速度越快，得到的直线越少（得到的直线都是很有把握的直线）
            //这里得到的lines是包含rho和theta的，而不包括直线上的点，所以下面需要根据得到的rho和theta来建立一条直线

            //依次画出每条线段
            for (size_t i = 0; i < lines2f.size(); i++)
            {
                float rho = lines2f[i][0]; //就是圆的半径r
                float theta = lines2f[i][1]; //就是直线的角度
                Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a*rho, y0 = b*rho;
                pt1.x = cvRound(x0 + 1000 * (-b));
                pt1.y = cvRound(y0 + 1000*(a));
                pt2.x = cvRound(x0 - 1000*(-b));
                pt2.y = cvRound(y0 - 1000 * (a));

                line(*dstMat, pt1, pt2, color, line_width, LINE_AA); //Scalar函数用于调节线段颜色，就是你想检测到的线段显示的是什么颜色
            }
            break;
         case 1://line  两点
                Canny(*srcMat, midImage, 100, 200, 3);
                cvtColor(midImage, *dstMat, CV_GRAY2BGR);

                //与HoughLines不同的是，HoughLinesP得到lines的是含有直线上点的坐标的，所以下面进行划线时就不再需要自己求出两个点来确定唯一的直线了
                HoughLinesP(midImage, lines4i, 1, CV_PI / 180, Precision, 50, 10);//注意第五个参数，为阈值

                //依次画出每条线段
                for (size_t i = 0; i < lines4i.size(); i++)
                {
                    Vec4i l = lines4i[i];

                    line(*dstMat , Point(l[0], l[1]), Point(l[2], l[3]), color, line_width, LINE_AA); //Scalar函数用于调节线段颜色

                }
            break;
        case 2://circle

            //【3】转为灰度图，进行图像平滑
            cvtColor(*srcMat, midImage, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
            GaussianBlur(midImage, *dstMat, Size(9, 9), 2, 2);

            //【4】进行霍夫圆变换
            HoughCircles(midImage, circles3f, CV_HOUGH_GRADIENT, 1.5, 10, 200, Precision, 0, 0); //注意第七的参数为阈值，可以自行调整，值越大，检测的圆更精准

            //【5】依次在图中绘制出圆
            for (size_t i = 0; i < circles3f.size(); i++)
            {
                Point center(cvRound(circles3f[i][0]), cvRound(circles3f[i][1]));
                int radius = cvRound(circles3f[i][2]);
                //绘制圆心
                circle(*dstMat, center, 3, color, -1, 8, 0);
                //绘制圆轮廓
                circle(*dstMat, center, radius, color, line_width, 8, 0);
            }
            break;
        case 3:
            break;
        default:
            break;
    }

    return true;
}

