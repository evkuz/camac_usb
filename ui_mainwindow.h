/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_camac;
    QSpinBox *A_spinBox;
    QLabel *X_label;
    QPushButton *Clear_All;
    QGroupBox *groupBox;
    QLabel *NAF_Data_Label;
    QLabel *label_4;
    QPushButton *Sel_Bit_04;
    QPushButton *Sel_Bit_09;
    QLabel *X_label_2;
    QPushButton *Sel_Bit_07;
    QLabel *Status_label;
    QSpinBox *F_spinBox;
    QPushButton *Sel_Bit_02;
    QPushButton *Sel_Bit_01;
    QLabel *label_3;
    QLabel *label_5;
    QPushButton *Sel_Bit_06;
    QPushButton *Sel_Bit_11;
    QPushButton *SEND_TO_USB_pushButton;
    QLabel *label_6;
    QPushButton *Sel_Bit_12;
    QLabel *label;
    QPushButton *Sel_Bit_08;
    QPushButton *Sel_Bit_15;
    QPushButton *Sel_Bit_03;
    QLabel *label_7;
    QPushButton *Sel_Bit_10;
    QPushButton *Sel_Bit_16;
    QPushButton *Sel_Bit_13;
    QLabel *label_2;
    QPushButton *Stop_ALL_pushButton;
    QFrame *Frequency_frame;
    QPushButton *pushButton_stop_Sending;
    QLabel *label_41;
    QPushButton *Sel_Bit_14;
    QPushButton *Sel_Bit_05;
    QSpinBox *N_spinBox;
    QComboBox *Frequency_comboBox;
    QFrame *line;
    QWidget *N_23_widget;
    QWidget *N_22_widget;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *N_21_widget;
    QLabel *label_10;
    QWidget *N_01_widget;
    QWidget *N_20_widget;
    QWidget *N_19_widget;
    QWidget *N_18_widget;
    QLabel *label_11;
    QLabel *label_12;
    QWidget *N_17_widget;
    QWidget *N_16_widget;
    QLabel *label_13;
    QWidget *N_15_widget;
    QWidget *N_14_widget;
    QLabel *label_14;
    QWidget *N_13_widget;
    QWidget *N_12_widget;
    QWidget *N_11_widget;
    QWidget *N_10_widget;
    QWidget *N_09_widget;
    QWidget *N_08_widget;
    QWidget *N_07_widget;
    QWidget *N_05_widget;
    QWidget *N_06_widget;
    QWidget *N_04_widget;
    QWidget *N_03_widget;
    QWidget *N_02_widget;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_37;
    QLabel *label_38;
    QPushButton *Start_Aquisition_pushButton;
    QLabel *label_39;
    QPushButton *Load_commands_pushButton;
    QLabel *label_23;
    QWidget *tab_Single_NOT_ADP16;
    QWidget *SINGLE_widget;
    QLabel *label_21;
    QSpinBox *N_Spectral_spinBox_Single;
    QPushButton *Show_Selected_Spectral_Single_pushButton;
    QPushButton *Show_Spectral_Selected_From_File_Single_pushButton;
    QLabel *label_22;
    QSpinBox *N_Spectral_Channel_spinBox_Single;
    MainWindow *tab_ADP16_Alpha_Spectral;
    QWidget *tab_FISSION;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1134, 593);
        QFont font;
        font.setPointSize(13);
        font.setBold(false);
        font.setWeight(50);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral("../0_icons/usb.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setAnimated(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(10, 0, 1121, 831));
        QFont font1;
        font1.setPointSize(13);
        tabWidget->setFont(font1);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setStyleSheet(QStringLiteral(""));
        tabWidget->setDocumentMode(true);
        tab_camac = new QWidget();
        tab_camac->setObjectName(QStringLiteral("tab_camac"));
        tab_camac->setAutoFillBackground(false);
        tab_camac->setStyleSheet(QStringLiteral(""));
        A_spinBox = new QSpinBox(tab_camac);
        A_spinBox->setObjectName(QStringLiteral("A_spinBox"));
        A_spinBox->setGeometry(QRect(250, 60, 151, 51));
        QFont font2;
        font2.setPointSize(22);
        font2.setBold(false);
        font2.setWeight(50);
        A_spinBox->setFont(font2);
        A_spinBox->setMaximum(15);
        X_label = new QLabel(tab_camac);
        X_label->setObjectName(QStringLiteral("X_label"));
        X_label->setGeometry(QRect(30, 310, 16, 21));
        X_label->setFont(font1);
        Clear_All = new QPushButton(tab_camac);
        Clear_All->setObjectName(QStringLiteral("Clear_All"));
        Clear_All->setGeometry(QRect(700, 250, 111, 41));
        Clear_All->setFont(font1);
        groupBox = new QGroupBox(tab_camac);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(350, 280, 311, 101));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMaximumSize(QSize(311, 16777215));
        QFont font3;
        font3.setPointSize(12);
        font3.setUnderline(false);
        font3.setStrikeOut(false);
        font3.setKerning(false);
        groupBox->setFont(font3);
        groupBox->setAutoFillBackground(false);
        groupBox->setStyleSheet(QLatin1String("QGroupBox {\n"
"	background-color: rgb(255, 244, 79);\n"
"    border: 3px solid blue;\n"
"    border-radius: 9px;\n"
"    margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        groupBox->setTitle(QStringLiteral(""));
        groupBox->setFlat(false);
        NAF_Data_Label = new QLabel(groupBox);
        NAF_Data_Label->setObjectName(QStringLiteral("NAF_Data_Label"));
        NAF_Data_Label->setGeometry(QRect(20, 30, 261, 21));
        QFont font4;
        font4.setPointSize(16);
        NAF_Data_Label->setFont(font4);
        NAF_Data_Label->setAutoFillBackground(false);
        NAF_Data_Label->setStyleSheet(QStringLiteral("background-color: rgb(255, 244, 79);"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 50, 261, 20));
        QFont font5;
        font5.setPointSize(11);
        font5.setBold(false);
        font5.setWeight(50);
        label_4->setFont(font5);
        label_4->setStyleSheet(QStringLiteral("background-color: rgb(255, 244, 79);"));
        Sel_Bit_04 = new QPushButton(tab_camac);
        Sel_Bit_04->setObjectName(QStringLiteral("Sel_Bit_04"));
        Sel_Bit_04->setGeometry(QRect(740, 200, 51, 31));
        QFont font6;
        font6.setPointSize(12);
        Sel_Bit_04->setFont(font6);
        Sel_Bit_04->setAutoFillBackground(false);
        Sel_Bit_04->setStyleSheet(QStringLiteral(""));
        Sel_Bit_04->setCheckable(true);
        Sel_Bit_09 = new QPushButton(tab_camac);
        Sel_Bit_09->setObjectName(QStringLiteral("Sel_Bit_09"));
        Sel_Bit_09->setGeometry(QRect(440, 200, 51, 31));
        Sel_Bit_09->setFont(font6);
        Sel_Bit_09->setAutoFillBackground(false);
        Sel_Bit_09->setStyleSheet(QStringLiteral(""));
        Sel_Bit_09->setCheckable(true);
        X_label_2 = new QLabel(tab_camac);
        X_label_2->setObjectName(QStringLiteral("X_label_2"));
        X_label_2->setGeometry(QRect(80, 310, 16, 21));
        X_label_2->setFont(font1);
        Sel_Bit_07 = new QPushButton(tab_camac);
        Sel_Bit_07->setObjectName(QStringLiteral("Sel_Bit_07"));
        Sel_Bit_07->setGeometry(QRect(560, 200, 51, 31));
        Sel_Bit_07->setFont(font6);
        Sel_Bit_07->setAutoFillBackground(false);
        Sel_Bit_07->setStyleSheet(QStringLiteral(""));
        Sel_Bit_07->setCheckable(true);
        Status_label = new QLabel(tab_camac);
        Status_label->setObjectName(QStringLiteral("Status_label"));
        Status_label->setGeometry(QRect(650, 30, 251, 20));
        Status_label->setFont(font1);
        F_spinBox = new QSpinBox(tab_camac);
        F_spinBox->setObjectName(QStringLiteral("F_spinBox"));
        F_spinBox->setGeometry(QRect(450, 60, 151, 51));
        F_spinBox->setFont(font2);
        F_spinBox->setMaximum(31);
        Sel_Bit_02 = new QPushButton(tab_camac);
        Sel_Bit_02->setObjectName(QStringLiteral("Sel_Bit_02"));
        Sel_Bit_02->setGeometry(QRect(860, 200, 51, 31));
        Sel_Bit_02->setFont(font6);
        Sel_Bit_02->setAutoFillBackground(false);
        Sel_Bit_02->setStyleSheet(QStringLiteral(""));
        Sel_Bit_02->setCheckable(true);
        Sel_Bit_01 = new QPushButton(tab_camac);
        Sel_Bit_01->setObjectName(QStringLiteral("Sel_Bit_01"));
        Sel_Bit_01->setGeometry(QRect(920, 200, 51, 31));
        QFont font7;
        font7.setPointSize(12);
        font7.setKerning(false);
        Sel_Bit_01->setFont(font7);
        Sel_Bit_01->setAutoFillBackground(false);
        Sel_Bit_01->setStyleSheet(QStringLiteral(""));
        Sel_Bit_01->setCheckable(true);
        Sel_Bit_01->setChecked(false);
        Sel_Bit_01->setFlat(false);
        label_3 = new QLabel(tab_camac);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(520, 10, 21, 21));
        QFont font8;
        font8.setPointSize(14);
        label_3->setFont(font8);
        label_5 = new QLabel(tab_camac);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(100, 40, 61, 21));
        label_5->setFont(font8);
        label_5->setTextFormat(Qt::RichText);
        label_5->setWordWrap(true);
        Sel_Bit_06 = new QPushButton(tab_camac);
        Sel_Bit_06->setObjectName(QStringLiteral("Sel_Bit_06"));
        Sel_Bit_06->setGeometry(QRect(620, 200, 51, 31));
        Sel_Bit_06->setFont(font6);
        Sel_Bit_06->setAutoFillBackground(false);
        Sel_Bit_06->setStyleSheet(QStringLiteral(""));
        Sel_Bit_06->setCheckable(true);
        Sel_Bit_11 = new QPushButton(tab_camac);
        Sel_Bit_11->setObjectName(QStringLiteral("Sel_Bit_11"));
        Sel_Bit_11->setGeometry(QRect(320, 200, 51, 31));
        Sel_Bit_11->setFont(font6);
        Sel_Bit_11->setAutoFillBackground(false);
        Sel_Bit_11->setStyleSheet(QStringLiteral(""));
        Sel_Bit_11->setCheckable(true);
        SEND_TO_USB_pushButton = new QPushButton(tab_camac);
        SEND_TO_USB_pushButton->setObjectName(QStringLiteral("SEND_TO_USB_pushButton"));
        SEND_TO_USB_pushButton->setGeometry(QRect(670, 82, 171, 41));
        SEND_TO_USB_pushButton->setFont(font6);
        label_6 = new QLabel(tab_camac);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(290, 40, 61, 21));
        label_6->setFont(font8);
        label_6->setTextFormat(Qt::RichText);
        label_6->setWordWrap(true);
        Sel_Bit_12 = new QPushButton(tab_camac);
        Sel_Bit_12->setObjectName(QStringLiteral("Sel_Bit_12"));
        Sel_Bit_12->setGeometry(QRect(260, 200, 51, 31));
        Sel_Bit_12->setFont(font6);
        Sel_Bit_12->setAutoFillBackground(false);
        Sel_Bit_12->setStyleSheet(QStringLiteral(""));
        Sel_Bit_12->setCheckable(true);
        label = new QLabel(tab_camac);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 10, 31, 31));
        label->setFont(font8);
        label->setTextFormat(Qt::RichText);
        label->setWordWrap(true);
        Sel_Bit_08 = new QPushButton(tab_camac);
        Sel_Bit_08->setObjectName(QStringLiteral("Sel_Bit_08"));
        Sel_Bit_08->setGeometry(QRect(500, 200, 51, 31));
        Sel_Bit_08->setFont(font6);
        Sel_Bit_08->setAutoFillBackground(false);
        Sel_Bit_08->setStyleSheet(QStringLiteral(""));
        Sel_Bit_08->setCheckable(true);
        Sel_Bit_15 = new QPushButton(tab_camac);
        Sel_Bit_15->setObjectName(QStringLiteral("Sel_Bit_15"));
        Sel_Bit_15->setGeometry(QRect(80, 200, 51, 31));
        Sel_Bit_15->setFont(font6);
        Sel_Bit_15->setAutoFillBackground(false);
        Sel_Bit_15->setStyleSheet(QStringLiteral(""));
        Sel_Bit_15->setCheckable(true);
        Sel_Bit_03 = new QPushButton(tab_camac);
        Sel_Bit_03->setObjectName(QStringLiteral("Sel_Bit_03"));
        Sel_Bit_03->setGeometry(QRect(800, 200, 51, 31));
        Sel_Bit_03->setFont(font6);
        Sel_Bit_03->setAutoFillBackground(false);
        Sel_Bit_03->setStyleSheet(QStringLiteral(""));
        Sel_Bit_03->setCheckable(true);
        label_7 = new QLabel(tab_camac);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(500, 40, 61, 20));
        label_7->setFont(font8);
        label_7->setTextFormat(Qt::RichText);
        label_7->setWordWrap(true);
        Sel_Bit_10 = new QPushButton(tab_camac);
        Sel_Bit_10->setObjectName(QStringLiteral("Sel_Bit_10"));
        Sel_Bit_10->setGeometry(QRect(380, 200, 51, 31));
        Sel_Bit_10->setFont(font6);
        Sel_Bit_10->setAutoFillBackground(false);
        Sel_Bit_10->setStyleSheet(QStringLiteral(""));
        Sel_Bit_10->setCheckable(true);
        Sel_Bit_16 = new QPushButton(tab_camac);
        Sel_Bit_16->setObjectName(QStringLiteral("Sel_Bit_16"));
        Sel_Bit_16->setGeometry(QRect(20, 200, 51, 31));
        Sel_Bit_16->setFont(font6);
        Sel_Bit_16->setAutoFillBackground(false);
        Sel_Bit_16->setStyleSheet(QStringLiteral(""));
        Sel_Bit_16->setCheckable(true);
        Sel_Bit_13 = new QPushButton(tab_camac);
        Sel_Bit_13->setObjectName(QStringLiteral("Sel_Bit_13"));
        Sel_Bit_13->setGeometry(QRect(200, 200, 51, 31));
        Sel_Bit_13->setFont(font6);
        Sel_Bit_13->setAutoFillBackground(false);
        Sel_Bit_13->setStyleSheet(QStringLiteral(""));
        Sel_Bit_13->setCheckable(true);
        label_2 = new QLabel(tab_camac);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(300, 10, 31, 21));
        label_2->setFont(font8);
        Stop_ALL_pushButton = new QPushButton(tab_camac);
        Stop_ALL_pushButton->setObjectName(QStringLiteral("Stop_ALL_pushButton"));
        Stop_ALL_pushButton->setGeometry(QRect(700, 310, 111, 51));
        Stop_ALL_pushButton->setFont(font1);
        Frequency_frame = new QFrame(tab_camac);
        Frequency_frame->setObjectName(QStringLiteral("Frequency_frame"));
        Frequency_frame->setGeometry(QRect(650, 60, 441, 131));
        Frequency_frame->setFrameShape(QFrame::StyledPanel);
        Frequency_frame->setFrameShadow(QFrame::Plain);
        Frequency_frame->setLineWidth(4);
        pushButton_stop_Sending = new QPushButton(Frequency_frame);
        pushButton_stop_Sending->setObjectName(QStringLiteral("pushButton_stop_Sending"));
        pushButton_stop_Sending->setGeometry(QRect(20, 70, 141, 51));
        label_41 = new QLabel(Frequency_frame);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setGeometry(QRect(250, 70, 151, 21));
        QFont font9;
        font9.setPointSize(10);
        label_41->setFont(font9);
        Sel_Bit_14 = new QPushButton(tab_camac);
        Sel_Bit_14->setObjectName(QStringLiteral("Sel_Bit_14"));
        Sel_Bit_14->setGeometry(QRect(140, 200, 51, 31));
        Sel_Bit_14->setFont(font6);
        Sel_Bit_14->setAutoFillBackground(false);
        Sel_Bit_14->setStyleSheet(QStringLiteral(""));
        Sel_Bit_14->setCheckable(true);
        Sel_Bit_05 = new QPushButton(tab_camac);
        Sel_Bit_05->setObjectName(QStringLiteral("Sel_Bit_05"));
        Sel_Bit_05->setGeometry(QRect(680, 200, 51, 31));
        Sel_Bit_05->setFont(font6);
        Sel_Bit_05->setAutoFillBackground(false);
        Sel_Bit_05->setStyleSheet(QStringLiteral(""));
        Sel_Bit_05->setCheckable(true);
        N_spinBox = new QSpinBox(tab_camac);
        N_spinBox->setObjectName(QStringLiteral("N_spinBox"));
        N_spinBox->setGeometry(QRect(50, 60, 151, 51));
        N_spinBox->setFont(font2);
        N_spinBox->setAutoFillBackground(true);
        N_spinBox->setStyleSheet(QStringLiteral("background-color: rgb(188, 199, 170);"));
        N_spinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        N_spinBox->setMinimum(0);
        N_spinBox->setMaximum(23);
        N_spinBox->setValue(17);
        Frequency_comboBox = new QComboBox(tab_camac);
        Frequency_comboBox->setObjectName(QStringLiteral("Frequency_comboBox"));
        Frequency_comboBox->setGeometry(QRect(870, 81, 201, 41));
        Frequency_comboBox->setFont(font4);
        line = new QFrame(tab_camac);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 500, 1091, 16));
        line->setStyleSheet(QStringLiteral("border-color: rgb(85, 170, 127);"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        N_23_widget = new QWidget(tab_camac);
        N_23_widget->setObjectName(QStringLiteral("N_23_widget"));
        N_23_widget->setGeometry(QRect(1060, 549, 10, 80));
        N_23_widget->setAutoFillBackground(false);
        N_23_widget->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0);"));
        N_22_widget = new QWidget(tab_camac);
        N_22_widget->setObjectName(QStringLiteral("N_22_widget"));
        N_22_widget->setGeometry(QRect(1030, 549, 10, 80));
        N_22_widget->setAutoFillBackground(false);
        N_22_widget->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0);"));
        label_8 = new QLabel(tab_camac);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(1055, 630, 21, 16));
        label_9 = new QLabel(tab_camac);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(1025, 630, 21, 16));
        N_21_widget = new QWidget(tab_camac);
        N_21_widget->setObjectName(QStringLiteral("N_21_widget"));
        N_21_widget->setGeometry(QRect(1000, 549, 10, 80));
        N_21_widget->setAutoFillBackground(false);
        N_21_widget->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0);"));
        label_10 = new QLabel(tab_camac);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(995, 630, 21, 16));
        N_01_widget = new QWidget(tab_camac);
        N_01_widget->setObjectName(QStringLiteral("N_01_widget"));
        N_01_widget->setGeometry(QRect(45, 549, 10, 80));
        N_01_widget->setAutoFillBackground(false);
        N_01_widget->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0);"));
        N_20_widget = new QWidget(tab_camac);
        N_20_widget->setObjectName(QStringLiteral("N_20_widget"));
        N_20_widget->setGeometry(QRect(970, 549, 10, 80));
        N_20_widget->setAutoFillBackground(false);
        N_20_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_19_widget = new QWidget(tab_camac);
        N_19_widget->setObjectName(QStringLiteral("N_19_widget"));
        N_19_widget->setGeometry(QRect(940, 549, 10, 80));
        N_19_widget->setAutoFillBackground(false);
        N_19_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_18_widget = new QWidget(tab_camac);
        N_18_widget->setObjectName(QStringLiteral("N_18_widget"));
        N_18_widget->setGeometry(QRect(910, 549, 10, 80));
        N_18_widget->setAutoFillBackground(false);
        N_18_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        label_11 = new QLabel(tab_camac);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(905, 630, 21, 16));
        label_12 = new QLabel(tab_camac);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(965, 630, 21, 16));
        N_17_widget = new QWidget(tab_camac);
        N_17_widget->setObjectName(QStringLiteral("N_17_widget"));
        N_17_widget->setGeometry(QRect(880, 549, 10, 80));
        N_17_widget->setAutoFillBackground(false);
        N_17_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_16_widget = new QWidget(tab_camac);
        N_16_widget->setObjectName(QStringLiteral("N_16_widget"));
        N_16_widget->setGeometry(QRect(850, 549, 10, 80));
        N_16_widget->setAutoFillBackground(false);
        N_16_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        label_13 = new QLabel(tab_camac);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(845, 630, 21, 16));
        N_15_widget = new QWidget(tab_camac);
        N_15_widget->setObjectName(QStringLiteral("N_15_widget"));
        N_15_widget->setGeometry(QRect(820, 549, 10, 80));
        N_15_widget->setAutoFillBackground(false);
        N_15_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_14_widget = new QWidget(tab_camac);
        N_14_widget->setObjectName(QStringLiteral("N_14_widget"));
        N_14_widget->setGeometry(QRect(790, 549, 10, 80));
        N_14_widget->setAutoFillBackground(false);
        N_14_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        label_14 = new QLabel(tab_camac);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(785, 630, 21, 16));
        N_13_widget = new QWidget(tab_camac);
        N_13_widget->setObjectName(QStringLiteral("N_13_widget"));
        N_13_widget->setGeometry(QRect(760, 549, 10, 80));
        N_13_widget->setAutoFillBackground(false);
        N_13_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_12_widget = new QWidget(tab_camac);
        N_12_widget->setObjectName(QStringLiteral("N_12_widget"));
        N_12_widget->setGeometry(QRect(670, 549, 10, 80));
        N_12_widget->setAutoFillBackground(false);
        N_12_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_11_widget = new QWidget(tab_camac);
        N_11_widget->setObjectName(QStringLiteral("N_11_widget"));
        N_11_widget->setGeometry(QRect(700, 549, 10, 80));
        N_11_widget->setAutoFillBackground(false);
        N_11_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_10_widget = new QWidget(tab_camac);
        N_10_widget->setObjectName(QStringLiteral("N_10_widget"));
        N_10_widget->setGeometry(QRect(730, 549, 10, 80));
        N_10_widget->setAutoFillBackground(false);
        N_10_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_09_widget = new QWidget(tab_camac);
        N_09_widget->setObjectName(QStringLiteral("N_09_widget"));
        N_09_widget->setGeometry(QRect(285, 549, 10, 80));
        N_09_widget->setAutoFillBackground(false);
        N_09_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_08_widget = new QWidget(tab_camac);
        N_08_widget->setObjectName(QStringLiteral("N_08_widget"));
        N_08_widget->setGeometry(QRect(255, 549, 10, 80));
        N_08_widget->setAutoFillBackground(false);
        N_08_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_07_widget = new QWidget(tab_camac);
        N_07_widget->setObjectName(QStringLiteral("N_07_widget"));
        N_07_widget->setGeometry(QRect(225, 549, 10, 80));
        N_07_widget->setAutoFillBackground(false);
        N_07_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_05_widget = new QWidget(tab_camac);
        N_05_widget->setObjectName(QStringLiteral("N_05_widget"));
        N_05_widget->setGeometry(QRect(165, 549, 10, 80));
        N_05_widget->setAutoFillBackground(false);
        N_05_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_06_widget = new QWidget(tab_camac);
        N_06_widget->setObjectName(QStringLiteral("N_06_widget"));
        N_06_widget->setGeometry(QRect(195, 549, 10, 80));
        N_06_widget->setAutoFillBackground(false);
        N_06_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_04_widget = new QWidget(tab_camac);
        N_04_widget->setObjectName(QStringLiteral("N_04_widget"));
        N_04_widget->setGeometry(QRect(135, 549, 10, 80));
        N_04_widget->setAutoFillBackground(false);
        N_04_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_03_widget = new QWidget(tab_camac);
        N_03_widget->setObjectName(QStringLiteral("N_03_widget"));
        N_03_widget->setGeometry(QRect(105, 549, 10, 80));
        N_03_widget->setAutoFillBackground(false);
        N_03_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        N_02_widget = new QWidget(tab_camac);
        N_02_widget->setObjectName(QStringLiteral("N_02_widget"));
        N_02_widget->setGeometry(QRect(75, 549, 10, 80));
        N_02_widget->setAutoFillBackground(false);
        N_02_widget->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
""));
        label_15 = new QLabel(tab_camac);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(725, 630, 21, 16));
        label_16 = new QLabel(tab_camac);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(665, 630, 21, 16));
        label_17 = new QLabel(tab_camac);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(250, 630, 21, 16));
        label_18 = new QLabel(tab_camac);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(190, 630, 21, 16));
        label_37 = new QLabel(tab_camac);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setGeometry(QRect(130, 630, 21, 16));
        label_38 = new QLabel(tab_camac);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setGeometry(QRect(70, 630, 21, 16));
        Start_Aquisition_pushButton = new QPushButton(tab_camac);
        Start_Aquisition_pushButton->setObjectName(QStringLiteral("Start_Aquisition_pushButton"));
        Start_Aquisition_pushButton->setGeometry(QRect(290, 450, 151, 41));
        Start_Aquisition_pushButton->setFont(font1);
        label_39 = new QLabel(tab_camac);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setGeometry(QRect(40, 630, 21, 16));
        Load_commands_pushButton = new QPushButton(tab_camac);
        Load_commands_pushButton->setObjectName(QStringLiteral("Load_commands_pushButton"));
        Load_commands_pushButton->setGeometry(QRect(680, 450, 341, 41));
        Load_commands_pushButton->setFont(font1);
        label_23 = new QLabel(tab_camac);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(430, 390, 161, 20));
        label_23->setFont(font6);
        tabWidget->addTab(tab_camac, icon, QString());
        Frequency_frame->raise();
        A_spinBox->raise();
        X_label->raise();
        Clear_All->raise();
        groupBox->raise();
        Sel_Bit_04->raise();
        Sel_Bit_09->raise();
        X_label_2->raise();
        Sel_Bit_07->raise();
        Status_label->raise();
        F_spinBox->raise();
        Sel_Bit_02->raise();
        Sel_Bit_01->raise();
        label_3->raise();
        label_5->raise();
        Sel_Bit_06->raise();
        Sel_Bit_11->raise();
        SEND_TO_USB_pushButton->raise();
        label_6->raise();
        Sel_Bit_12->raise();
        label->raise();
        Sel_Bit_08->raise();
        Sel_Bit_15->raise();
        Sel_Bit_03->raise();
        label_7->raise();
        Sel_Bit_10->raise();
        Sel_Bit_16->raise();
        Sel_Bit_13->raise();
        label_2->raise();
        Stop_ALL_pushButton->raise();
        Sel_Bit_14->raise();
        Sel_Bit_05->raise();
        N_spinBox->raise();
        Frequency_comboBox->raise();
        line->raise();
        N_23_widget->raise();
        N_22_widget->raise();
        label_8->raise();
        label_9->raise();
        N_21_widget->raise();
        label_10->raise();
        N_01_widget->raise();
        N_20_widget->raise();
        N_19_widget->raise();
        N_18_widget->raise();
        label_11->raise();
        label_12->raise();
        N_17_widget->raise();
        N_16_widget->raise();
        label_13->raise();
        N_15_widget->raise();
        N_14_widget->raise();
        label_14->raise();
        N_13_widget->raise();
        N_12_widget->raise();
        N_11_widget->raise();
        N_10_widget->raise();
        N_09_widget->raise();
        N_08_widget->raise();
        N_07_widget->raise();
        N_05_widget->raise();
        N_06_widget->raise();
        N_04_widget->raise();
        N_03_widget->raise();
        N_02_widget->raise();
        label_15->raise();
        label_16->raise();
        label_17->raise();
        label_18->raise();
        label_37->raise();
        label_38->raise();
        Start_Aquisition_pushButton->raise();
        label_39->raise();
        Load_commands_pushButton->raise();
        label_23->raise();
        tab_Single_NOT_ADP16 = new QWidget();
        tab_Single_NOT_ADP16->setObjectName(QStringLiteral("tab_Single_NOT_ADP16"));
        tab_Single_NOT_ADP16->setAutoFillBackground(false);
        tab_Single_NOT_ADP16->setStyleSheet(QStringLiteral(""));
        SINGLE_widget = new QWidget(tab_Single_NOT_ADP16);
        SINGLE_widget->setObjectName(QStringLiteral("SINGLE_widget"));
        SINGLE_widget->setGeometry(QRect(10, 60, 800, 400));
        SINGLE_widget->setAutoFillBackground(false);
        SINGLE_widget->setStyleSheet(QStringLiteral("color: rgb(255, 255, 0);"));
        label_21 = new QLabel(tab_Single_NOT_ADP16);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(1000, 50, 61, 31));
        QFont font10;
        font10.setPointSize(18);
        label_21->setFont(font10);
        N_Spectral_spinBox_Single = new QSpinBox(tab_Single_NOT_ADP16);
        N_Spectral_spinBox_Single->setObjectName(QStringLiteral("N_Spectral_spinBox_Single"));
        N_Spectral_spinBox_Single->setGeometry(QRect(830, 90, 121, 51));
        N_Spectral_spinBox_Single->setFont(font10);
        N_Spectral_spinBox_Single->setMinimum(1);
        N_Spectral_spinBox_Single->setMaximum(23);
        N_Spectral_spinBox_Single->setValue(17);
        Show_Selected_Spectral_Single_pushButton = new QPushButton(tab_Single_NOT_ADP16);
        Show_Selected_Spectral_Single_pushButton->setObjectName(QStringLiteral("Show_Selected_Spectral_Single_pushButton"));
        Show_Selected_Spectral_Single_pushButton->setGeometry(QRect(830, 190, 271, 51));
        Show_Spectral_Selected_From_File_Single_pushButton = new QPushButton(tab_Single_NOT_ADP16);
        Show_Spectral_Selected_From_File_Single_pushButton->setObjectName(QStringLiteral("Show_Spectral_Selected_From_File_Single_pushButton"));
        Show_Spectral_Selected_From_File_Single_pushButton->setEnabled(false);
        Show_Spectral_Selected_From_File_Single_pushButton->setGeometry(QRect(830, 280, 271, 51));
        label_22 = new QLabel(tab_Single_NOT_ADP16);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(870, 50, 21, 31));
        label_22->setFont(font10);
        N_Spectral_Channel_spinBox_Single = new QSpinBox(tab_Single_NOT_ADP16);
        N_Spectral_Channel_spinBox_Single->setObjectName(QStringLiteral("N_Spectral_Channel_spinBox_Single"));
        N_Spectral_Channel_spinBox_Single->setGeometry(QRect(980, 90, 121, 51));
        N_Spectral_Channel_spinBox_Single->setFont(font10);
        N_Spectral_Channel_spinBox_Single->setMinimum(1);
        N_Spectral_Channel_spinBox_Single->setMaximum(16);
        tabWidget->addTab(tab_Single_NOT_ADP16, QString());
        label_21->raise();
        N_Spectral_spinBox_Single->raise();
        Show_Selected_Spectral_Single_pushButton->raise();
        Show_Spectral_Selected_From_File_Single_pushButton->raise();
        label_22->raise();
        N_Spectral_Channel_spinBox_Single->raise();
        SINGLE_widget->raise();


        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1134, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);
        Frequency_comboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "KK_Control", 0));
        X_label->setText(QApplication::translate("MainWindow", "X", 0));
        Clear_All->setText(QApplication::translate("MainWindow", "\320\241\320\221\320\240\320\236\320\241\320\230\320\242\320\254", 0));
        NAF_Data_Label->setText(QApplication::translate("MainWindow", "00       00       00         00", 0));
        label_4->setText(QApplication::translate("MainWindow", " 7..0        15 - 8     23 - 16       31- 24", 0));
        Sel_Bit_04->setText(QApplication::translate("MainWindow", "04", 0));
        Sel_Bit_09->setText(QApplication::translate("MainWindow", "09", 0));
        X_label_2->setText(QApplication::translate("MainWindow", "Q", 0));
        Sel_Bit_07->setText(QApplication::translate("MainWindow", "07", 0));
        Status_label->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274 \"\320\240\320\243\320\247\320\235\320\236\320\231 \320\232\320\236\320\235\320\242\320\240\320\236\320\233\320\233\320\225\320\240\"", 0));
        Sel_Bit_02->setText(QApplication::translate("MainWindow", "02", 0));
        Sel_Bit_01->setText(QApplication::translate("MainWindow", "01", 0));
        label_3->setText(QApplication::translate("MainWindow", "F", 0));
        label_5->setText(QApplication::translate("MainWindow", "1 - 23", 0));
        Sel_Bit_06->setText(QApplication::translate("MainWindow", "06", 0));
        Sel_Bit_11->setText(QApplication::translate("MainWindow", "11", 0));
        SEND_TO_USB_pushButton->setText(QApplication::translate("MainWindow", "\320\227\320\220\320\224\320\220\320\242\320\254", 0));
        label_6->setText(QApplication::translate("MainWindow", "0 - 15", 0));
        Sel_Bit_12->setText(QApplication::translate("MainWindow", "12", 0));
        label->setText(QApplication::translate("MainWindow", "N", 0));
        Sel_Bit_08->setText(QApplication::translate("MainWindow", "08", 0));
        Sel_Bit_15->setText(QApplication::translate("MainWindow", "15", 0));
        Sel_Bit_03->setText(QApplication::translate("MainWindow", "03", 0));
        label_7->setText(QApplication::translate("MainWindow", "0 - 31", 0));
        Sel_Bit_10->setText(QApplication::translate("MainWindow", "10", 0));
        Sel_Bit_16->setText(QApplication::translate("MainWindow", "16", 0));
        Sel_Bit_13->setText(QApplication::translate("MainWindow", "13", 0));
        label_2->setText(QApplication::translate("MainWindow", " A", 0));
        Stop_ALL_pushButton->setText(QApplication::translate("MainWindow", "STOP ALL", 0));
        pushButton_stop_Sending->setText(QApplication::translate("MainWindow", "Stop Sending", 0));
        label_41->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\207\320\260\321\201\321\202\320\276\321\202\321\203 \320\276\320\261\320\274\320\265\320\275\320\260", 0));
        Sel_Bit_14->setText(QApplication::translate("MainWindow", "14", 0));
        Sel_Bit_05->setText(QApplication::translate("MainWindow", "05", 0));
#ifndef QT_NO_TOOLTIP
        N_spinBox->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>values 1-23</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_8->setText(QApplication::translate("MainWindow", "23", 0));
        label_9->setText(QApplication::translate("MainWindow", "22", 0));
        label_10->setText(QApplication::translate("MainWindow", "21", 0));
        label_11->setText(QApplication::translate("MainWindow", "18", 0));
        label_12->setText(QApplication::translate("MainWindow", "20", 0));
        label_13->setText(QApplication::translate("MainWindow", "16", 0));
        label_14->setText(QApplication::translate("MainWindow", "14", 0));
        label_15->setText(QApplication::translate("MainWindow", "12", 0));
        label_16->setText(QApplication::translate("MainWindow", "10", 0));
        label_17->setText(QApplication::translate("MainWindow", "08", 0));
        label_18->setText(QApplication::translate("MainWindow", "06", 0));
        label_37->setText(QApplication::translate("MainWindow", "04", 0));
        label_38->setText(QApplication::translate("MainWindow", "02", 0));
        Start_Aquisition_pushButton->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\235\320\260\320\261\320\276\321\200", 0));
        label_39->setText(QApplication::translate("MainWindow", "01", 0));
        Load_commands_pushButton->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \320\270\320\267 \321\204\320\260\320\271\320\273\320\260", 0));
        label_23->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\264 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \320\232\320\220\320\234\320\220\320\232", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_camac), QApplication::translate("MainWindow", "KK manual", 0));
        label_21->setText(QApplication::translate("MainWindow", "Chan.", 0));
        Show_Selected_Spectral_Single_pushButton->setText(QApplication::translate("MainWindow", "\320\237\320\236\320\232\320\220\320\227\320\220\320\242\320\254 Real Time", 0));
        Show_Spectral_Selected_From_File_Single_pushButton->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\270\320\267 \321\204\320\260\320\271\320\273\320\260", 0));
        label_22->setText(QApplication::translate("MainWindow", "N", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_Single_NOT_ADP16), QApplication::translate("MainWindow", "Single", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_ADP16_Alpha_Spectral), QApplication::translate("MainWindow", "ADP16 Alpha", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_FISSION), QApplication::translate("MainWindow", "ADP16 Fission", 0));
        tabWidget->setTabWhatsThis(tabWidget->indexOf(tab_FISSION), QApplication::translate("MainWindow", "LayOut of fission soectral", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
