#include "lib4opencvtool.h"
//#include <QDebug>
#include <QImage>
lib4opencvtool::lib4opencvtool()
{

}

cv::Mat QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        //qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}


//QImage->IplImage *
static IplImage *ConvertToIplImage(const QImage &img)
{
    int nChannel=0;
    if(img.format()==QImage::Format_RGB888)nChannel=3;
    if(img.format()==QImage::Format_ARGB32)nChannel=4;
    if( nChannel==0 )return NULL;

    IplImage *iplImg=cvCreateImageHeader( cvSize(img.width(), img.height()),8,nChannel );
    iplImg->imageData=(char*)img.bits();

    if(nChannel==3)
        cvConvertImage(iplImg,iplImg,CV_CVTIMG_SWAP_RB);

    return iplImg;
}

//Mat->QImage
static QImage ConvertToQImage(cv::Mat &mat)
{
    QImage img;
    int nChannel=mat.channels();
    if(nChannel==3)
    {
        cv::cvtColor(mat,mat,CV_BGR2RGB);
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_RGB888);
    }
    else if(nChannel==4||nChannel==1)
    {
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_ARGB32);
    }

    return img;
}

//IplImage *->QImage
static QImage ConvertToQImage(IplImage *iplImg)
{
    QImage img;
    int nChannel=iplImg->nChannels;
    if(nChannel==3)
    {
        cvConvertImage(iplImg,iplImg,CV_CVTIMG_SWAP_RB);
        img = QImage((const unsigned char*)iplImg->imageData,iplImg->width,iplImg->height,QImage::Format_RGB888);
    }
    else if(nChannel==4||nChannel==1)
    {
        img = QImage((const unsigned char*)iplImg->imageData,iplImg->width,iplImg->height,QImage::Format_ARGB32);
    }

    return img;
}

//Rect convert to QRect
static QRect Rect2QRect(Rect rect)
{
    QRect qrect(rect.x,rect.y,rect.width-1,rect.height-1);
    return qrect;
}

//QRect convert to Rect
static Rect QRect2Rect(QRect qrect)
{
    Rect rect(qrect.x(),qrect.y(),qrect.width()+1,qrect.height()+1);
    return rect;
}

Point QSting2Point(QString str){
    QStringList point_str=str.split(",");
    Point p={0, 0};
    if (point_str.count()<2) return p;
    p.x=point_str[0].toInt();
    p.y=point_str[1].toInt();
    return p;
}

Scalar QSting2Scalar(QString str){
    Scalar color=Scalar(0, 0, 0);//black
    QStringList clorstr=str.split(",");
    //background_color = QColor();
    if (clorstr.count()==3){
        color = Scalar(clorstr[2].toInt(), clorstr[1].toInt(), clorstr[0].toInt());
    }
    return color;
}
