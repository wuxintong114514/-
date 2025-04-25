#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlError>
#include <QFileDialog>
#include <QDir>

// 数据增强函数
cv::Mat augmentImage(const cv::Mat& img) {
    cv::Mat augmentedImg;
    // 随机旋转
    cv::Point2f center(img.cols / 2.0, img.rows / 2.0);
    double angle = (rand() % 30) - 15; // 随机旋转 -15 到 15 度
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(img, augmentedImg, rot, img.size());
    return augmentedImg;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
    model = new QSqlTableModel(this);
    model->setTable("basic_inf");//指定要访问的视图名
    if (!model->select()) {
        qDebug() << "Failed to load basic_inf table: " << model->lastError().text();
    }
    model->select();//加载视图数据
    model_d = new QSqlTableModel(this);
    model_d->setTable("details_inf");
    if (!model_d->select()) {
        qDebug() << "Failed to load details_inf table: " << model_d->lastError().text();
    }
    model_d->select();
    ui->basicTableView->setModel(model);
    if (!face_cascade.load(cv::samples::findFile("haarcascade_frontalface_alt.xml"))) {
        qDebug() << "Error loading face cascade";
    }

    // 初始化人脸识别器，使用 LBPH 算法
    recognizer = cv::face::LBPHFaceRecognizer::create();

    // 加载训练数据
    loadTrainingData();
    // 检查训练数据是否为空
    if (training_images.empty() || training_labels.empty()) {
        qDebug() << "Training data is empty";
    }

    // 训练人脸识别器
    if (!training_images.empty() && !training_labels.empty()) {
        recognizer->train(training_images, training_labels);
    }

    // 连接开始识别按钮的点击信号到槽函数
    connect(ui->startPushButton, &QPushButton::clicked, this, &MainWindow::on_startPushButton_clicked);
}

void MainWindow::loadTrainingData()
{
    cv::Size targetSize(200, 200); // 定义目标尺寸，可根据需要调整

    for (int i = 0; i < model_d->rowCount(); i++) {
        QModelIndex index = model_d->index(i, 0);
        QString name = model_d->data(index).toString();
        names.append(name);

        index = model_d->index(i, 2);
        QByteArray photoData = model_d->data(index).toByteArray();
        if (!photoData.isEmpty()) {
            cv::Mat photo = cv::imdecode(std::vector<uchar>(photoData.begin(), photoData.end()), cv::IMREAD_GRAYSCALE);
            if (!photo.empty()) {
                // 调整图像尺寸
                cv::Mat resizedPhoto;
                cv::resize(photo, resizedPhoto, targetSize);

                // 检查调整后的图像尺寸
                if (resizedPhoto.size() == targetSize) {
                    training_images.push_back(resizedPhoto);
                    training_labels.push_back(i);

                    // 数据增强
                    cv::Mat augmentedPhoto = augmentImage(resizedPhoto);
                    training_images.push_back(augmentedPhoto);
                    training_labels.push_back(i);
                } else {
                    qDebug() << "Failed to resize photo for " << name;
                }
            } else {
                qDebug() << "Failed to decode photo for " << name;
            }
        } else {
            qDebug() << "Photo data is empty for " << name;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    QString ctImgPath = "D:/Work/SUT/FY25/CPlus/Telemedicine/liuenyi.png";
    Mat ctImg = imread(ctImgPath.toLatin1().data());
    cvtColor(ctImg,ctImg,COLOR_BGR2RGB);
    myCtImg = ctImg;
    myCtQImage = QImage((const unsigned char*)(ctImg.data),ctImg.cols,ctImg.rows,QImage::Format_BGR888);
    ctImgShow();
    QDate date = QDate::currentDate();
    int year = date.year();
    ui->yearLcdNumber->display(year);
    int month = date.month();
    ui->monthLcdNumber->display(month);
    int day = date.day();
    ui->dayLcdNumber->display(day);
    myTimer = new QTimer();
    myTimer->setInterval(1000);
    myTimer->start();
    connect(myTimer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

void MainWindow::ctImgRead()
{
    QString ctImgName = QFileDialog::getOpenFileName(this,"载入CT相片",".","Image File(*.png *.jpg *.jpeg *.bmp)");
    if(ctImgName.isEmpty())
    {
        return;
    }
    Mat ctRgbImg,ctGrayImg;
    Mat ctImg = imread(ctImgName.toLatin1().data());
    cvtColor(ctImg,ctRgbImg,COLOR_BGR2RGB);
    cvtColor(ctRgbImg,ctGrayImg,COLOR_BGR2GRAY);
    myCtImg = ctRgbImg;
    myCtGrayImg = ctGrayImg;
    myCtQImage = QImage((const unsigned char*)(ctRgbImg.data),ctRgbImg.cols,ctRgbImg.rows,QImage::Format_BGR888);
    ctImgShow();
}

void MainWindow::detectAndRecognizeFaces(const cv::Mat& img)
{
    std::vector<cv::Rect> faces;
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // 高斯模糊以减少噪声
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    // 直方图均衡化增强对比度
    cv::equalizeHist(gray, gray);

    // 调整人脸检测参数
    face_cascade.detectMultiScale(gray, faces, 1.05, 5, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    if (faces.empty()) {
        QMessageBox::information(this, tr("识别结果"), tr("未检测到人脸"));
        return;
    }

    cv::Mat resultImg = img.clone();
    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::rectangle(resultImg, faces[i], cv::Scalar(255, 0, 0), 2);

        // 人脸特征提取
        cv::Mat faceROI = gray(faces[i]);

        // 调整人脸区域的尺寸与训练图像一致
        cv::Mat resizedFaceROI;
        cv::Size targetSize(200, 200); // 假设训练图像的尺寸为 200x200
        cv::resize(faceROI, resizedFaceROI, targetSize);

        // 人脸识别
        int label;
        double confidence;
        recognizer->predict(resizedFaceROI, label, confidence);

        // 在图像上显示识别结果
        QString label_text = "";
        if (label >= 0 && label < names.size()) {
            label_text = names[label];
        }
        // 显示识别信息
        QMessageBox::information(this, tr("识别结果"), tr("识别出的人脸是: %1，置信度: %2").arg(label_text).arg(confidence));
    }

    // 显示识别后的图像
    cvtColor(resultImg, resultImg, cv::COLOR_BGR2RGB);
    myCtImg = resultImg;
    myCtQImage = QImage((const unsigned char*)(resultImg.data), resultImg.cols, resultImg.rows, QImage::Format_BGR888);
    ctImgShow();
}

bool MainWindow::createMySqlConn()
{
    QSqlDatabase sqldb = QSqlDatabase::addDatabase("QMYSQL");
    sqldb.setHostName("localhost");
    sqldb.setDatabaseName("patient");
    sqldb.setUserName("root");
    sqldb.setPassword("114514");
    if (!sqldb.open())
    {
        QMessageBox::critical(0, QObject::tr("false"), "false to connect: " + sqldb.lastError().text(), QMessageBox::Cancel);
        return false;
    }
    else
    {
        QMessageBox::information(0, QObject::tr("true"), "true to connect", QMessageBox::Cancel);
    }
//    QSqlQuery query(sqldb);
//            QString photoPath = "D:/Work/SUT/FY25/CPlus/Telemedicine/kl2.png";
//            QFile photoFile(photoPath);
//            if(photoFile.exists())
//            {
//                if (!photoFile.open(QIODevice::ReadOnly)) {
//                    QMessageBox::information(0,QObject::tr("false"),"Failed to open photo file");
//                    sqldb.close();
//                    return false;
//                }
//                QByteArray picdata = photoFile.readAll();
//                photoFile.close();

//                QVariant var(picdata);
//                QString sqlstr = "UPDATE user_profile set picture=? where name='库里'";
//                query.prepare(sqlstr);
//                query.addBindValue(var);
//                if(!query.exec())
//                {
//                    QMessageBox::information(0,QObject::tr("false"),"photo false: " + query.lastError().text());
//                }
//                else
//                {
//                    QMessageBox::information(0,QObject::tr("success"),"photo success");
//                }
//            }
//            else
//            {
//                QMessageBox::information(0,QObject::tr("false"),"Photo file does not exist");
//            }

    sqldb.close();
    return true;
}

void MainWindow::ctImgSave()
{
    QFile image("D:/Work/SUT/FY25/CPlus/Telemedicine/Tumor_1.png");
    if(!image.open(QIODevice::ReadWrite)) return;
    QByteArray qba;
    QBuffer buf(&qba);
    buf.open(QIODevice::WriteOnly);
    myCtQImage.save(&buf,"PNG");
    image.write(qba);
}

void MainWindow::ctImgShow()
{
    ui->CT_Img_Label->setPixmap(QPixmap::fromImage(myCtQImage.scaled(ui->CT_Img_Label->size(),Qt::KeepAspectRatio)));
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 1)
    {
        QModelIndex index;
        bool found = false;
        for(int i = 0; i < model_d->rowCount(); i++)
        {
            index = model_d->index(i,0);
            QString current_name = model_d->data(index).toString();
        }
    }
}

#include <QDesktopServices>
#include <QUrl>

#include <QFileDialog>

void MainWindow::on_startPushButton_clicked()
{
    QString folderPath = "D:/Work/SUT/FY25/CPlus/Telemedicine";
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择图片文件"), folderPath, tr("图像文件 (*.png *.jpg *.jpeg *.bmp)"));

    if (!filePath.isEmpty()) {
        cv::Mat img = cv::imread(filePath.toStdString());
        if (!img.empty()) {
            detectAndRecognizeFaces(img);
        } else {
            QMessageBox::information(this, tr("错误"), tr("无法读取图片文件: %1").arg(filePath));
        }
    }
}

void MainWindow::on_basicTableView_clicked(const QModelIndex &index)
{
}

void MainWindow::onTimeOut()
{

}
