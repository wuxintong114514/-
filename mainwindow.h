#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"//opencv高层GUI和媒体I/O
#include "opencv2/imgproc/imgproc.hpp"//opencv图像处理
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QTimer>
#include <QSqlError>
#include <opencv2/face.hpp> // 引入人脸识别模块
#include <QVector>
using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainWindow();
    void ctImgRead();
    void ctImgProc();
    void ctImgSave();
    void ctImgShow();
    void ctImgHoughCircles();
    void onTableSelectChange(int row);
    void showUserPhoto();
    void loadTrainingData();
    // 声明 detectAndRecognizeFaces 函数，包含参数
    void detectAndRecognizeFaces(const cv::Mat& img);
    static bool createMySqlConn();

private slots:
    void on_startPushButton_clicked();
    void on_basicTableView_clicked(const QModelIndex &index);
    void on_tabWidget_tabBarClicked(int index);
    void onTimeOut();

private:
    Ui::MainWindow *ui;
    Mat myCtImg;
    Mat myCtGrayImg;
    QImage myCtQImage;
    QSqlTableModel *model;
    QSqlTableModel *model_d;
    QTimer *myTimer;

    cv::CascadeClassifier face_cascade; // 人脸检测器
    cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer; // 修改为 LBPHFaceRecognizer 类型
    std::vector<cv::Mat> training_images; // 训练图像
    std::vector<int> training_labels; // 训练标签
    QVector<QString> names; // 姓名对应标签
};
#endif // MAINWINDOW_H
