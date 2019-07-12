#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStatusBar>
#include <QListWidgetItem>
#include <QMouseEvent>
#include "lib4opencvtoolmanager.h"
#include "opencv2/opencv.hpp"
using namespace cv;


struct simple_action{
    int offsetX;
    int offsetY;
    double scaling_factor;
    int action;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool m_isOpenFile;
    bool isTransform;
    static simple_action Winaction;
    args_info SUR_args_info;
    QList<args_info> EXE_args_info;

    Mat m_srcImage;
    Mat m_dstImage;

public:
    void Show_image(simple_action *action);
//    void Morphology_getArgs(bool reload);
//    void Filter_getArgs(bool reload);
//    void IMAGE_Morphology(QString args, Mat *srcMat,  Mat *dstMat);
//    void IMAGE_Filter(QString args, Mat *srcMat,  Mat *dstMat);
//    void Visible_Args_Select(int typeSel);
    int Image_transform(bool Viewtransform);
    void Disable_Args_Set();
    void Controlinfo(args_info default_info);
    bool IsInPic(int x, int y);
    void Image_move(Mat *src, Mat *dst);
    void QImage_fill_info(QImage *src, QImage *dst, QPoint I_abs_point);
    QPoint W2I_abs_point(QPoint m_point);
    void Draw_Pixel_Info(QPoint m_point);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_SAVE_clicked();
    void on_pushButton_ADD_clicked();
    void on_pushButton_DEL_clicked();
    void on_pushButton_MODIFY_clicked();
//    void on_SUR_listWidget_currentTextChanged(const QString &currentText);
//    void on_EXE_listWidget_currentTextChanged(const QString &currentText);
    void on_pushButton_EXE_clicked();
    void on_pushButton_EXED_clicked();
    void on_horizontalSlider_KernelValue0_valueChanged(int value);
    void on_horizontalSlider_KernelValue1_valueChanged(int value);
    void on_horizontalSlider_KernelValue2_valueChanged(int value);
    void on_comboBox_Type0_currentIndexChanged(int index);
    void on_comboBox_Type1_currentIndexChanged(int index);
    void on_comboBox_Type2_currentIndexChanged(int index);
    void on_lineEdit_0_textEdited(const QString &arg1);
    void on_lineEdit_1_textEdited(const QString &arg1);
    void on_lineEdit_2_textEdited(const QString &arg1);
    void on_lineEdit_3_textEdited(const QString &arg1);
    void on_lineEdit_4_textEdited(const QString &arg1);
    void on_lineEdit_5_textEdited(const QString &arg1);

    void on_SUR_listWidget_itemClicked(QListWidgetItem *item);
    void on_EXE_listWidget_itemClicked(QListWidgetItem *item);
    void on_EXE_listWidget_itemDoubleClicked(QListWidgetItem *item);

protected:
    //重定义了QWidget类的鼠标事件方法
    bool event(QEvent * event);
    void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件
private:
    QLabel *statusLabel;
    QLabel *MousePosLabel;

};

#endif // MAINWINDOW_H
