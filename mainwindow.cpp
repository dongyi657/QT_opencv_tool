#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "QDebug"
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>

#ifdef LIB4OPENCVTOOLMANAGER_H
extern QStringList includeMethods();
#endif
extern QImage MatToQImage(const cv::Mat& mat);
extern cv::Mat QImageToMat(QImage image);

simple_action MainWindow::Winaction={0,0,1,0};
static bool press=false;
static QPoint PreDot;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Qt_OpenCV学习工具"));

    //初始化全局变量
    m_isOpenFile = false;

    //初始化控件
#ifdef LIB4OPENCVTOOLMANAGER_H
        ui->SUR_listWidget->addItems(includeMethods());
#endif

    //鼠标事件
    statusLabel=new QLabel;
    statusLabel->setText(tr("操作状态栏"));
    statusLabel->setFixedWidth(200);
    MousePosLabel=new QLabel;
    MousePosLabel->setText(tr(""));
    MousePosLabel->setFixedWidth(100);

    //在QMainWindow的状态栏中加入控件
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(MousePosLabel);
    //设置窗体追踪鼠标
    //this->setMouseTracking(true);


    ui->graphicsView->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::IsInPic(int x, int y){

    if(x > ui->label_OriginalImg->geometry().left() && x < ui->label_OriginalImg->geometry().right() && \
            y > ui->label_OriginalImg->geometry().top() && y < ui->label_OriginalImg->geometry().bottom())
        return true;

    if(x > ui->label_ProcessedImg->geometry().left() && x < ui->label_ProcessedImg->geometry().right() && \
        y > ui->label_ProcessedImg->geometry().top() && y < ui->label_ProcessedImg->geometry().bottom())
        return true;

    return false;
}


bool MainWindow::event(QEvent * event){

    if(event->type() == QEvent::MouseButtonPress )
    {
           QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

           //判断鼠标是否是左键按下,且鼠标位置是否在绘画区域
           if(mouse->button()==Qt::LeftButton && IsInPic(mouse->pos().x(),mouse->pos().y()))
           {
               press=true;
               QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式
               MousePosLabel->setText("("+QString::number(mouse->pos().x())+","+QString::number(mouse->pos().y())+")");
               PreDot = mouse->pos();
           }
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

        //判断鼠标是否是左键释放,且之前是在绘画区域
        if(mouse->button()==Qt::LeftButton && press )
        {
            QApplication::setOverrideCursor(Qt::ArrowCursor); //改回鼠标样式
            Winaction.offsetX=Winaction.offsetX+ mouse->x() - PreDot.x();
            Winaction.offsetY=Winaction.offsetY+ mouse->y() - PreDot.y();
            Winaction.action = 1;
            Show_image(&Winaction);
            press=false;
        }
    }
    return QWidget::event(event);
}

//鼠标滚轮事件响应
void MainWindow::wheelEvent(QWheelEvent* event)     //鼠标滑轮事件
{
    //qDebug()<<event->delta();
     if (IsInPic(event->x(), event->y())){
         if (event->delta()>0 ) {      //上滑,缩小
           // event->x(),event->y()
            statusBar()->showMessage(tr("缩小"));
            Winaction.action=2;
         } else {                    //下滑,放大
            statusBar()->showMessage(tr("放大"));
            Winaction.action=3;
         }
         double tmp=Winaction.scaling_factor*pow(1.1,(event->delta()/120));
         if(tmp<5 && tmp>0.01)Winaction.scaling_factor=tmp;
         Show_image(&Winaction);
     }
     event->accept();
}

//打开
void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty())
    {
        return;
    }

    m_srcImage = imread(fileName.toLatin1().data());//读取图片数据
    if (!m_srcImage.data)
    {
        m_isOpenFile = false;
        QMessageBox box(QMessageBox::Critical, "打开图像", "读取图像文件失败！请重新打开．");
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, QString("确定"));
        box.exec();
        return;
    }
    m_isOpenFile = true;//修改打开标志

    Mat disImageTemp;
    cvtColor(m_srcImage, disImageTemp, COLOR_BGR2RGB);//图像格式转换
    QImage disImage = QImage((const unsigned char*)(disImageTemp.data),disImageTemp.cols,disImageTemp.rows,QImage::Format_RGB888);
    ui->label_OriginalImg->setPixmap(QPixmap::fromImage(disImage.scaled(ui->label_OriginalImg->width(), ui->label_OriginalImg->height(), Qt::KeepAspectRatio)));
}

//保存
void MainWindow::on_pushButton_SAVE_clicked()
{
    if(m_isOpenFile){
        QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Image File(*.png *.jpg *.jpeg *.bmp)")); //选择路径
        cvtColor(m_dstImage, m_dstImage, COLOR_RGB2BGR);//图像格式转换
        if (filename.isEmpty())
        {
            return;
        }
        imwrite(filename.toLatin1().data(), m_dstImage);
    }
}

int MainWindow::Image_transform(bool Viewtransform)
{

    Mat srcImage;
    m_srcImage.copyTo(srcImage);
    Mat dstImage;
    int ret;
    int i=0;
    int count;
    isTransform=false;
    if  (Viewtransform) {
        count=ui->EXE_listWidget->currentRow()+1;
    } else {
        count=ui->EXE_listWidget->count();
    }
    if (count <=0)
        return -2;
    foreach(args_info step,EXE_args_info){
        if(i>=count)break;
        i++;
        lib4opencvtool* Method = Lib4opencvtoolManager::create(&step);
        ret=Method->MatTransform(&srcImage, &dstImage);
        Lib4opencvtoolManager::destroy(Method);
        if (ret<0)
            return ret;
        srcImage=dstImage;
    }
    m_dstImage=dstImage;
    isTransform=true;
    return i;
}

void MainWindow::Image_move(Mat *src, Mat *dst){
    CV_Assert(src->depth() == CV_8U);
/* 水平镜像
 *
    dst->create( src->size(), src->type());
    Mat map_x;
    Mat map_y;
    map_x.create( src->size(), CV_32FC1);
    map_y.create( src->size(), CV_32FC1);
    for( int i = 0; i < src->rows; ++i)
    {
        for( int j = 0; j < src->cols; ++j)
        {
            map_x.at<float>(i, j) = (float) (src->cols - j) ;
            map_y.at<float>(i, j) = (float) i ;  //水平
            //map_x.at<float>(i, j) = (float) j ;
            //map_y.at<float>(i, j) = (float) (src.rows - i) ;  //垂直
        }
    }
    cv::remap(*src, *dst, map_x, map_y, CV_INTER_LINEAR);
    cv::imshow("test",*dst);
*/
    Mat temp;
    //缩放
    cv::resize(*src, temp, Size(),Winaction.scaling_factor, Winaction.scaling_factor);
    //平移

    const int rows = ui->label_ProcessedImg->height() ; //输出图像的大小
    const int cols = ui->label_ProcessedImg->width() ;
    int dx = Winaction.offsetX;
    int dy = Winaction.offsetY;
    dst->create(rows, cols, temp.type());
  //  qDebug()<<rows<<cols<<temp.type();
    if(temp.type() == CV_8UC3){
        Vec3b *p;
        for (int i = 0; i < rows; i++)
        {
            p = dst->ptr<Vec3b>(i);
            for (int j = 0; j < cols; j++)
            {
                int x = j - dx;
                int y = i - dy;

                if (x >= 0 && y >= 0 && x < temp.cols && y < temp.rows)
                    p[j] = temp.ptr<Vec3b>(y)[x];
            }
        }
    }

    if(temp.type() == CV_8UC1){
        uchar *p;
        for (int i = 0; i < rows; i++)
        {
            p = dst->ptr<uchar>(i);
            for (int j = 0; j < cols; j++)
            {
                int x = j - dx;
                int y = i - dy;

                if (x >= 0 && y >= 0 && x < temp.cols && y < temp.rows)
                    p[j] = temp.ptr<uchar>(y)[x];
            }
        }
    }
}

//action->scaling_factor*
void MainWindow::Show_image(simple_action *action)
{
    QString action_name;
    switch (action->action) {
        case 1:
            action_name="拖动";
        break;
        case 2:
            if(action->scaling_factor<0.1)break;
            action_name="缩小";
        break;
        case 3:
            if(action->scaling_factor>5)break;
            action_name="放大";
        break;
        default:
        return;
    }
    statusLabel->setText(action_name+" "+ QString::number(action->scaling_factor)+"("+QString::number(action->offsetX)+","+QString::number(action->offsetY)+")");

    if( m_isOpenFile ){
        Mat disImageTemp;
        Image_move(&m_srcImage, &disImageTemp);
        ui->label_OriginalImg->setPixmap(QPixmap::fromImage(MatToQImage(disImageTemp)));
    }
    if( isTransform ){
        Mat disImageTemp;
        Image_move(&m_dstImage, &disImageTemp);
        //cv::imshow("dstImage", m_dstImage);
        ui->label_ProcessedImg->setPixmap(QPixmap::fromImage(MatToQImage(disImageTemp)));
    }
    action->action=0;
}

//添加图像处理方法
void MainWindow::on_pushButton_ADD_clicked()
{
    QString args;
    if (m_isOpenFile)
    {
        if(ui->SUR_listWidget->currentRow()>=0){
            EXE_args_info.append(SUR_args_info);
            lib4opencvtool* Method = Lib4opencvtoolManager::create(&SUR_args_info);
            ui->EXE_listWidget->addItem(Method->ReadInfo(SUR_args_info));
            Lib4opencvtoolManager::destroy(Method);
        }
    }
}

//删除图像处理方法
void MainWindow::on_pushButton_DEL_clicked()
{
    //qDebug()<<ui->EXE_listWidget->currentRow()<<ui->EXE_listWidget->currentItem()->text();
    if(ui->EXE_listWidget->count()==1){
        ui->EXE_listWidget->clear();
        return;
    }
    if( ui->EXE_listWidget->currentRow()>=0 && ui->EXE_listWidget->currentRow()<ui->EXE_listWidget->count()){
        int i = ui->EXE_listWidget->currentRow();
        QListWidgetItem *currentItem = ui->EXE_listWidget->currentItem();
        delete currentItem;
        EXE_args_info.removeAt(i);
        //qDebug()<<EXE_args_info.count();
    }
}

//修改图像处理方法
void MainWindow::on_pushButton_MODIFY_clicked()
{
    QString args;
    if(ui->EXE_listWidget->currentRow()>=0){
        EXE_args_info.replace(ui->EXE_listWidget->currentRow(),SUR_args_info);
        lib4opencvtool* Method = Lib4opencvtoolManager::create(&SUR_args_info);
        ui->EXE_listWidget->currentItem()->setText(Method->ReadInfo(SUR_args_info));
        Lib4opencvtoolManager::destroy(Method);
    }
}

void MainWindow::Disable_Args_Set()
{
    ui->comboBox_Type0->setEnabled(false);
    ui->comboBox_Type1->setEnabled(false);
    ui->comboBox_Type2->setEnabled(false);
    ui->horizontalSlider_KernelValue0->setEnabled(false);
    ui->horizontalSlider_KernelValue1->setEnabled(false);
    ui->horizontalSlider_KernelValue2->setEnabled(false);
    ui->label_KernelValue0->setText("");
    ui->label_KernelValue1->setText("");
    ui->label_KernelValue2->setText("");
    ui->Slider_name0->setText("");
    ui->Slider_name1->setText("");
    ui->Slider_name2->setText("");
    ui->lineEdit_0->setEnabled(false);
    ui->lineEdit_1->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->label_0->setText("");
    ui->label_1->setText("");
    ui->label_2->setText("");
    ui->label_3->setText("");
    ui->label_4->setText("");
    ui->label_5->setText("");
}

void MainWindow::Controlinfo(args_info default_info){
     //qDebug()<<QString("%1").arg(default_info.argsuse,4,16,QLatin1Char('0'));
    if(default_info.argsuse & 0x800){
        ui->comboBox_Type0->clear();
        ui->comboBox_Type0->insertItems(0,default_info.boxinfo[0].s);
        ui->comboBox_Type0->setCurrentIndex(default_info.boxinfo[0].num);
        ui->comboBox_Type0->setEnabled(true);
    }
    if(default_info.argsuse & 0x400){
        ui->comboBox_Type1->clear();
        ui->comboBox_Type1->insertItems(0,default_info.boxinfo[1].s);
        ui->comboBox_Type1->setCurrentIndex(default_info.boxinfo[1].num);
        ui->comboBox_Type1->setEnabled(true);
    }
    if(default_info.argsuse & 0x200){
        ui->comboBox_Type2->clear();
        ui->comboBox_Type2->insertItems(0,default_info.boxinfo[2].s);
        ui->comboBox_Type2->setCurrentIndex(default_info.boxinfo[2].num);
        ui->comboBox_Type2->setEnabled(true);
    }
    if(default_info.argsuse & 0x100){
        ui->Slider_name0->setText(default_info.sliderinfo[0].label_name);
        ui->horizontalSlider_KernelValue0->setMinimum(default_info.sliderinfo[0].min);
        ui->horizontalSlider_KernelValue0->setMaximum(default_info.sliderinfo[0].max);
        ui->horizontalSlider_KernelValue0->setSingleStep(default_info.sliderinfo[0].SingleStep);
        ui->horizontalSlider_KernelValue0->setValue(default_info.sliderinfo[0].value);
        ui->label_KernelValue0->setText(QString::number(default_info.sliderinfo[0].value));
        ui->horizontalSlider_KernelValue0->setEnabled(true);
    }
    if(default_info.argsuse & 0x080){
        ui->Slider_name1->setText(default_info.sliderinfo[1].label_name);
        ui->horizontalSlider_KernelValue1->setMinimum(default_info.sliderinfo[1].min);
        ui->horizontalSlider_KernelValue1->setMaximum(default_info.sliderinfo[1].max);
        ui->horizontalSlider_KernelValue1->setSingleStep(default_info.sliderinfo[1].SingleStep);
        ui->horizontalSlider_KernelValue1->setValue(default_info.sliderinfo[1].value);
        ui->label_KernelValue1->setText(QString::number(default_info.sliderinfo[1].value));
        ui->horizontalSlider_KernelValue1->setEnabled(true);
    }
    if(default_info.argsuse & 0x040){
        ui->Slider_name2->setText(default_info.sliderinfo[2].label_name);
        ui->horizontalSlider_KernelValue2->setMinimum(default_info.sliderinfo[2].min);
        ui->horizontalSlider_KernelValue2->setMaximum(default_info.sliderinfo[2].max);
        ui->horizontalSlider_KernelValue2->setSingleStep(default_info.sliderinfo[2].SingleStep);
        ui->horizontalSlider_KernelValue2->setValue(default_info.sliderinfo[2].value);
        ui->label_KernelValue2->setText(QString::number(default_info.sliderinfo[2].value));
        ui->horizontalSlider_KernelValue2->setEnabled(true);
    }
    if(default_info.argsuse & 0x020){
        ui->lineEdit_0->clear();
        ui->label_0->clear();
        ui->label_0->setText(default_info.lineinfo[0].label);
        ui->lineEdit_0->setText(default_info.lineinfo[0].line);
        ui->label_0->setEnabled(true);
        ui->lineEdit_0->setEnabled(true);
    }
    if(default_info.argsuse & 0x010){
        ui->lineEdit_1->clear();
        ui->label_1->clear();
        ui->label_1->setText(default_info.lineinfo[1].label);
        ui->lineEdit_1->setText(default_info.lineinfo[1].line);
        ui->label_1->setEnabled(true);
        ui->lineEdit_1->setEnabled(true);
    }
    if(default_info.argsuse & 0x008){
        ui->lineEdit_2->clear();
        ui->label_2->clear();
        ui->label_2->setText(default_info.lineinfo[2].label);
        ui->lineEdit_2->setText(default_info.lineinfo[2].line);
        ui->label_2->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
    if(default_info.argsuse & 0x004){
        ui->lineEdit_3->clear();
        ui->label_3->clear();
        ui->label_3->setText(default_info.lineinfo[3].label);
        ui->lineEdit_3->setText(default_info.lineinfo[3].line);
        ui->label_3->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
    }
    if(default_info.argsuse & 0x002){
        ui->lineEdit_4->clear();
        ui->label_4->clear();
        ui->label_4->setText(default_info.lineinfo[4].label);
        ui->lineEdit_4->setText(default_info.lineinfo[4].line);
        ui->label_4->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
    }
    if(default_info.argsuse & 0x001){
        ui->lineEdit_5->clear();
        ui->label_5->clear();
        ui->label_5->setText(default_info.lineinfo[5].label);
        ui->lineEdit_5->setText(default_info.lineinfo[5].line);
        ui->label_5->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
    }
}
//源方法列表选择
void MainWindow::on_SUR_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString currentText = item->text();
    ui->statusBar->showMessage("CUR:" + item->text() + "(default)");
    SUR_args_info.method_name=currentText;
    //qDebug()<<currentText<<SUR_args_info.method_name;
    lib4opencvtool* Method = Lib4opencvtoolManager::create(&SUR_args_info);
    Method->GetDefualtInfo(&SUR_args_info);
    Disable_Args_Set();
    Controlinfo(SUR_args_info);
    Lib4opencvtoolManager::destroy(Method);
}

//执行列表选择
void MainWindow::on_EXE_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->statusBar->showMessage("EXE:"+QString::number(ui->EXE_listWidget->currentRow())+" INFO:" + item->text());
    //SUR_args_info=EXE_args_info.at(ui->EXE_listWidget->currentRow());
    Disable_Args_Set();
    Controlinfo(EXE_args_info.at(ui->EXE_listWidget->currentRow()));
}

//断点转换
void MainWindow::on_pushButton_EXE_clicked()
{
    Winaction.action=1;
    if(m_isOpenFile && Image_transform(true)>0){
        Show_image(&Winaction);
    }
}

//完全转换
void MainWindow::on_pushButton_EXED_clicked()
{
    Winaction={0, 0, 1, 1};
    qDebug()<<111111111111111;
    if(m_isOpenFile && Image_transform(false)>0)Show_image(&Winaction);
}

void MainWindow::on_comboBox_Type0_currentIndexChanged(int index)
{
    if (m_isOpenFile)
    {
        SUR_args_info.boxinfo[0].num=index;
    }
}

void MainWindow::on_comboBox_Type1_currentIndexChanged(int index)
{
    if (m_isOpenFile)
    {
        SUR_args_info.boxinfo[1].num=index;
    }
}

void MainWindow::on_comboBox_Type2_currentIndexChanged(int index)
{
    if (m_isOpenFile)
    {
        SUR_args_info.boxinfo[2].num=index;
    }
}

void MainWindow::on_horizontalSlider_KernelValue0_valueChanged(int value)
{
    if (m_isOpenFile)
    {
        SUR_args_info.sliderinfo[0].value=value;
        ui->label_KernelValue0->setText(QString::number(value));
    }

}

void MainWindow::on_horizontalSlider_KernelValue1_valueChanged(int value)
{
    if (m_isOpenFile)
    {
        SUR_args_info.sliderinfo[1].value=value;
        ui->label_KernelValue1->setText(QString::number(value));
    }
}


void MainWindow::on_horizontalSlider_KernelValue2_valueChanged(int value)
{
    if (m_isOpenFile)
    {
        SUR_args_info.sliderinfo[2].value=value;
        ui->label_KernelValue2->setText(QString::number(value));
    }
}

void MainWindow::on_lineEdit_0_textEdited(const QString &arg1)
{
    if (m_isOpenFile)
    {
        SUR_args_info.lineinfo[0].line=arg1;
    }
}

void MainWindow::on_lineEdit_1_textEdited(const QString &arg1)
{
    if (m_isOpenFile)
    {
        SUR_args_info.lineinfo[1].line=arg1;
    }
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    if (m_isOpenFile)
    {
        SUR_args_info.lineinfo[2].line=arg1;
    }

}

void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    if (m_isOpenFile)
    {
        SUR_args_info.lineinfo[3].line=arg1;
    }

}

void MainWindow::on_lineEdit_4_textEdited(const QString &arg1)
{
    if (m_isOpenFile)
    {
        SUR_args_info.lineinfo[4].line=arg1;
    }

}

void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)
{
    if (m_isOpenFile)
    {
        SUR_args_info.lineinfo[5].line=arg1;
    }
}
