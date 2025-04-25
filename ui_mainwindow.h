/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *CT_Img_Label;
    QLabel *label_3;
    QTimeEdit *timeEdit;
    QLCDNumber *yearLcdNumber;
    QLCDNumber *monthLcdNumber;
    QLCDNumber *dayLcdNumber;
    QPushButton *startPushButton;
    QTableView *basicTableView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(984, 803);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, -10, 591, 81));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\226\260\351\255\217"));
        font.setPointSize(26);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        CT_Img_Label = new QLabel(centralwidget);
        CT_Img_Label->setObjectName(QString::fromUtf8("CT_Img_Label"));
        CT_Img_Label->setGeometry(QRect(280, 90, 400, 400));
        CT_Img_Label->setFrameShape(QFrame::Box);
        CT_Img_Label->setFrameShadow(QFrame::Sunken);
        CT_Img_Label->setScaledContents(true);
        CT_Img_Label->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(290, 70, 81, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\215\216\346\226\207\344\273\277\345\256\213"));
        font1.setPointSize(12);
        label_3->setFont(font1);
        timeEdit = new QTimeEdit(centralwidget);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setEnabled(false);
        timeEdit->setGeometry(QRect(390, 60, 118, 22));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Times New Roman"));
        font2.setPointSize(10);
        timeEdit->setFont(font2);
        timeEdit->setAlignment(Qt::AlignCenter);
        timeEdit->setReadOnly(true);
        yearLcdNumber = new QLCDNumber(centralwidget);
        yearLcdNumber->setObjectName(QString::fromUtf8("yearLcdNumber"));
        yearLcdNumber->setGeometry(QRect(530, 60, 64, 23));
        yearLcdNumber->setDigitCount(4);
        yearLcdNumber->setProperty("value", QVariant(1900.000000000000000));
        monthLcdNumber = new QLCDNumber(centralwidget);
        monthLcdNumber->setObjectName(QString::fromUtf8("monthLcdNumber"));
        monthLcdNumber->setGeometry(QRect(610, 60, 31, 23));
        monthLcdNumber->setDigitCount(2);
        monthLcdNumber->setSegmentStyle(QLCDNumber::Flat);
        monthLcdNumber->setProperty("value", QVariant(1.000000000000000));
        dayLcdNumber = new QLCDNumber(centralwidget);
        dayLcdNumber->setObjectName(QString::fromUtf8("dayLcdNumber"));
        dayLcdNumber->setGeometry(QRect(640, 60, 31, 23));
        dayLcdNumber->setDigitCount(2);
        dayLcdNumber->setSegmentStyle(QLCDNumber::Flat);
        dayLcdNumber->setProperty("value", QVariant(1.000000000000000));
        startPushButton = new QPushButton(centralwidget);
        startPushButton->setObjectName(QString::fromUtf8("startPushButton"));
        startPushButton->setGeometry(QRect(440, 500, 93, 41));
        startPushButton->setFont(font1);
        basicTableView = new QTableView(centralwidget);
        basicTableView->setObjectName(QString::fromUtf8("basicTableView"));
        basicTableView->setGeometry(QRect(170, 560, 651, 171));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 984, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\344\272\272\350\204\270\350\257\206\345\210\253\347\263\273\347\273\237", nullptr));
        CT_Img_Label->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "\344\272\272\350\204\270\345\233\276\345\203\217", nullptr));
        timeEdit->setDisplayFormat(QCoreApplication::translate("MainWindow", "HH:mm:ss", nullptr));
        startPushButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\350\257\206\345\210\253", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
