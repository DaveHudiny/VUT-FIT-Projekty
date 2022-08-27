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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *Display;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButtonNum_3;
    QPushButton *pushButtonNum_8;
    QPushButton *pushButtonMinus;
    QPushButton *pushButtonNum_6;
    QPushButton *pushButtonNum_0;
    QPushButton *pushButtonNum_5;
    QPushButton *pushButtonNum_2;
    QPushButton *pushButtonPlus;
    QPushButton *pushButtonNum_7;
    QPushButton *pushButtonDot;
    QPushButton *pushButtonSIN;
    QPushButton *pushButtonNum_4;
    QPushButton *pushButtonCOTG;
    QPushButton *pushButtonCOS;
    QPushButton *pushButtonDiv;
    QPushButton *pushButtonNum_9;
    QPushButton *pushButtonNum_1;
    QPushButton *pushButtonMul;
    QPushButton *pushButtonComma;
    QPushButton *pushButtonTG;
    QPushButton *pushButtonDel;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_4;
    QPushButton *pushButtonRoot;
    QPushButton *pushButtonFactorial;
    QPushButton *pushButtonPower;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonBracketR;
    QPushButton *pushButtonBracketL;
    QPushButton *pushButtonAns;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_3;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonRadiants;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(485, 342);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(485, 342));
        MainWindow->setMaximumSize(QSize(485, 342));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        Display = new QLineEdit(centralwidget);
        Display->setObjectName(QString::fromUtf8("Display"));
        Display->setGeometry(QRect(10, 10, 461, 61));
        QFont font;
        font.setPointSize(12);
        Display->setFont(font);
        Display->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        Display->setReadOnly(true);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(100, 80, 281, 231));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonNum_3 = new QPushButton(layoutWidget);
        pushButtonNum_3->setObjectName(QString::fromUtf8("pushButtonNum_3"));
        sizePolicy.setHeightForWidth(pushButtonNum_3->sizePolicy().hasHeightForWidth());
        pushButtonNum_3->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font1.setPointSize(12);
        pushButtonNum_3->setFont(font1);
        pushButtonNum_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_3, 4, 2, 1, 1);

        pushButtonNum_8 = new QPushButton(layoutWidget);
        pushButtonNum_8->setObjectName(QString::fromUtf8("pushButtonNum_8"));
        sizePolicy.setHeightForWidth(pushButtonNum_8->sizePolicy().hasHeightForWidth());
        pushButtonNum_8->setSizePolicy(sizePolicy);
        pushButtonNum_8->setFont(font1);
        pushButtonNum_8->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_8, 2, 1, 1, 1);

        pushButtonMinus = new QPushButton(layoutWidget);
        pushButtonMinus->setObjectName(QString::fromUtf8("pushButtonMinus"));
        sizePolicy.setHeightForWidth(pushButtonMinus->sizePolicy().hasHeightForWidth());
        pushButtonMinus->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setPointSize(13);
        pushButtonMinus->setFont(font2);
        pushButtonMinus->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonMinus, 5, 3, 1, 1);

        pushButtonNum_6 = new QPushButton(layoutWidget);
        pushButtonNum_6->setObjectName(QString::fromUtf8("pushButtonNum_6"));
        sizePolicy.setHeightForWidth(pushButtonNum_6->sizePolicy().hasHeightForWidth());
        pushButtonNum_6->setSizePolicy(sizePolicy);
        pushButtonNum_6->setFont(font1);
        pushButtonNum_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_6, 3, 2, 1, 1);

        pushButtonNum_0 = new QPushButton(layoutWidget);
        pushButtonNum_0->setObjectName(QString::fromUtf8("pushButtonNum_0"));
        sizePolicy.setHeightForWidth(pushButtonNum_0->sizePolicy().hasHeightForWidth());
        pushButtonNum_0->setSizePolicy(sizePolicy);
        pushButtonNum_0->setFont(font1);
        pushButtonNum_0->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_0, 5, 1, 1, 1);

        pushButtonNum_5 = new QPushButton(layoutWidget);
        pushButtonNum_5->setObjectName(QString::fromUtf8("pushButtonNum_5"));
        sizePolicy.setHeightForWidth(pushButtonNum_5->sizePolicy().hasHeightForWidth());
        pushButtonNum_5->setSizePolicy(sizePolicy);
        pushButtonNum_5->setFont(font1);
        pushButtonNum_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_5, 3, 1, 1, 1);

        pushButtonNum_2 = new QPushButton(layoutWidget);
        pushButtonNum_2->setObjectName(QString::fromUtf8("pushButtonNum_2"));
        sizePolicy.setHeightForWidth(pushButtonNum_2->sizePolicy().hasHeightForWidth());
        pushButtonNum_2->setSizePolicy(sizePolicy);
        pushButtonNum_2->setFont(font1);
        pushButtonNum_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_2, 4, 1, 1, 1);

        pushButtonPlus = new QPushButton(layoutWidget);
        pushButtonPlus->setObjectName(QString::fromUtf8("pushButtonPlus"));
        sizePolicy.setHeightForWidth(pushButtonPlus->sizePolicy().hasHeightForWidth());
        pushButtonPlus->setSizePolicy(sizePolicy);
        pushButtonPlus->setFont(font2);
        pushButtonPlus->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonPlus, 4, 3, 1, 1);

        pushButtonNum_7 = new QPushButton(layoutWidget);
        pushButtonNum_7->setObjectName(QString::fromUtf8("pushButtonNum_7"));
        sizePolicy.setHeightForWidth(pushButtonNum_7->sizePolicy().hasHeightForWidth());
        pushButtonNum_7->setSizePolicy(sizePolicy);
        pushButtonNum_7->setFont(font1);
        pushButtonNum_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_7, 2, 0, 1, 1);

        pushButtonDot = new QPushButton(layoutWidget);
        pushButtonDot->setObjectName(QString::fromUtf8("pushButtonDot"));
        sizePolicy.setHeightForWidth(pushButtonDot->sizePolicy().hasHeightForWidth());
        pushButtonDot->setSizePolicy(sizePolicy);
        pushButtonDot->setFont(font);
        pushButtonDot->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonDot, 5, 0, 1, 1);

        pushButtonSIN = new QPushButton(layoutWidget);
        pushButtonSIN->setObjectName(QString::fromUtf8("pushButtonSIN"));
        sizePolicy.setHeightForWidth(pushButtonSIN->sizePolicy().hasHeightForWidth());
        pushButtonSIN->setSizePolicy(sizePolicy);
        QFont font3;
        font3.setPointSize(12);
        font3.setItalic(true);
        pushButtonSIN->setFont(font3);
        pushButtonSIN->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonSIN, 0, 0, 1, 1);

        pushButtonNum_4 = new QPushButton(layoutWidget);
        pushButtonNum_4->setObjectName(QString::fromUtf8("pushButtonNum_4"));
        sizePolicy.setHeightForWidth(pushButtonNum_4->sizePolicy().hasHeightForWidth());
        pushButtonNum_4->setSizePolicy(sizePolicy);
        pushButtonNum_4->setFont(font1);
        pushButtonNum_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_4, 3, 0, 1, 1);

        pushButtonCOTG = new QPushButton(layoutWidget);
        pushButtonCOTG->setObjectName(QString::fromUtf8("pushButtonCOTG"));
        sizePolicy.setHeightForWidth(pushButtonCOTG->sizePolicy().hasHeightForWidth());
        pushButtonCOTG->setSizePolicy(sizePolicy);
        pushButtonCOTG->setFont(font3);
        pushButtonCOTG->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonCOTG, 0, 3, 1, 1);

        pushButtonCOS = new QPushButton(layoutWidget);
        pushButtonCOS->setObjectName(QString::fromUtf8("pushButtonCOS"));
        sizePolicy.setHeightForWidth(pushButtonCOS->sizePolicy().hasHeightForWidth());
        pushButtonCOS->setSizePolicy(sizePolicy);
        pushButtonCOS->setFont(font3);
        pushButtonCOS->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonCOS, 0, 1, 1, 1);

        pushButtonDiv = new QPushButton(layoutWidget);
        pushButtonDiv->setObjectName(QString::fromUtf8("pushButtonDiv"));
        sizePolicy.setHeightForWidth(pushButtonDiv->sizePolicy().hasHeightForWidth());
        pushButtonDiv->setSizePolicy(sizePolicy);
        pushButtonDiv->setFont(font2);
        pushButtonDiv->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonDiv, 2, 3, 1, 1);

        pushButtonNum_9 = new QPushButton(layoutWidget);
        pushButtonNum_9->setObjectName(QString::fromUtf8("pushButtonNum_9"));
        sizePolicy.setHeightForWidth(pushButtonNum_9->sizePolicy().hasHeightForWidth());
        pushButtonNum_9->setSizePolicy(sizePolicy);
        pushButtonNum_9->setFont(font1);
        pushButtonNum_9->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_9, 2, 2, 1, 1);

        pushButtonNum_1 = new QPushButton(layoutWidget);
        pushButtonNum_1->setObjectName(QString::fromUtf8("pushButtonNum_1"));
        sizePolicy.setHeightForWidth(pushButtonNum_1->sizePolicy().hasHeightForWidth());
        pushButtonNum_1->setSizePolicy(sizePolicy);
        pushButtonNum_1->setFont(font1);
        pushButtonNum_1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #a1a1a1;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonNum_1, 4, 0, 1, 1);

        pushButtonMul = new QPushButton(layoutWidget);
        pushButtonMul->setObjectName(QString::fromUtf8("pushButtonMul"));
        sizePolicy.setHeightForWidth(pushButtonMul->sizePolicy().hasHeightForWidth());
        pushButtonMul->setSizePolicy(sizePolicy);
        pushButtonMul->setFont(font2);
        pushButtonMul->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonMul, 3, 3, 1, 1);

        pushButtonComma = new QPushButton(layoutWidget);
        pushButtonComma->setObjectName(QString::fromUtf8("pushButtonComma"));
        sizePolicy.setHeightForWidth(pushButtonComma->sizePolicy().hasHeightForWidth());
        pushButtonComma->setSizePolicy(sizePolicy);
        pushButtonComma->setFont(font);
        pushButtonComma->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonComma, 5, 2, 1, 1);

        pushButtonTG = new QPushButton(layoutWidget);
        pushButtonTG->setObjectName(QString::fromUtf8("pushButtonTG"));
        sizePolicy.setHeightForWidth(pushButtonTG->sizePolicy().hasHeightForWidth());
        pushButtonTG->setSizePolicy(sizePolicy);
        pushButtonTG->setFont(font3);
        pushButtonTG->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout->addWidget(pushButtonTG, 0, 2, 1, 1);

        pushButtonDel = new QPushButton(centralwidget);
        pushButtonDel->setObjectName(QString::fromUtf8("pushButtonDel"));
        pushButtonDel->setGeometry(QRect(10, 80, 81, 61));
        sizePolicy.setHeightForWidth(pushButtonDel->sizePolicy().hasHeightForWidth());
        pushButtonDel->setSizePolicy(sizePolicy);
        QFont font4;
        font4.setPointSize(15);
        pushButtonDel->setFont(font4);
        pushButtonDel->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));
        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 150, 81, 161));
        gridLayout_4 = new QGridLayout(layoutWidget1);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        pushButtonRoot = new QPushButton(layoutWidget1);
        pushButtonRoot->setObjectName(QString::fromUtf8("pushButtonRoot"));
        sizePolicy.setHeightForWidth(pushButtonRoot->sizePolicy().hasHeightForWidth());
        pushButtonRoot->setSizePolicy(sizePolicy);
        pushButtonRoot->setFont(font);
        pushButtonRoot->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_4->addWidget(pushButtonRoot, 2, 0, 1, 1);

        pushButtonFactorial = new QPushButton(layoutWidget1);
        pushButtonFactorial->setObjectName(QString::fromUtf8("pushButtonFactorial"));
        sizePolicy.setHeightForWidth(pushButtonFactorial->sizePolicy().hasHeightForWidth());
        pushButtonFactorial->setSizePolicy(sizePolicy);
        QFont font5;
        font5.setPointSize(12);
        font5.setBold(false);
        font5.setWeight(50);
        pushButtonFactorial->setFont(font5);
        pushButtonFactorial->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_4->addWidget(pushButtonFactorial, 3, 0, 1, 1);

        pushButtonPower = new QPushButton(layoutWidget1);
        pushButtonPower->setObjectName(QString::fromUtf8("pushButtonPower"));
        sizePolicy.setHeightForWidth(pushButtonPower->sizePolicy().hasHeightForWidth());
        pushButtonPower->setSizePolicy(sizePolicy);
        pushButtonPower->setFont(font);
        pushButtonPower->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_4->addWidget(pushButtonPower, 1, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButtonBracketR = new QPushButton(layoutWidget1);
        pushButtonBracketR->setObjectName(QString::fromUtf8("pushButtonBracketR"));
        sizePolicy.setHeightForWidth(pushButtonBracketR->sizePolicy().hasHeightForWidth());
        pushButtonBracketR->setSizePolicy(sizePolicy);
        pushButtonBracketR->setFont(font);
        pushButtonBracketR->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_2->addWidget(pushButtonBracketR, 1, 1, 1, 1);

        pushButtonBracketL = new QPushButton(layoutWidget1);
        pushButtonBracketL->setObjectName(QString::fromUtf8("pushButtonBracketL"));
        sizePolicy.setHeightForWidth(pushButtonBracketL->sizePolicy().hasHeightForWidth());
        pushButtonBracketL->setSizePolicy(sizePolicy);
        pushButtonBracketL->setFont(font);
        pushButtonBracketL->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_2->addWidget(pushButtonBracketL, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 0, 1, 1);

        pushButtonAns = new QPushButton(centralwidget);
        pushButtonAns->setObjectName(QString::fromUtf8("pushButtonAns"));
        pushButtonAns->setGeometry(QRect(390, 170, 81, 141));
        sizePolicy.setHeightForWidth(pushButtonAns->sizePolicy().hasHeightForWidth());
        pushButtonAns->setSizePolicy(sizePolicy);
        QFont font6;
        font6.setPointSize(16);
        pushButtonAns->setFont(font6);
        pushButtonAns->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #ffc57d;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffa041;\n"
"	border: 1px solid gray;\n"
"	color: #6b6b6b;\n"
"}\n"
""));
        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(390, 80, 83, 81));
        gridLayout_3 = new QGridLayout(layoutWidget2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        pushButtonClear = new QPushButton(layoutWidget2);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        sizePolicy.setHeightForWidth(pushButtonClear->sizePolicy().hasHeightForWidth());
        pushButtonClear->setSizePolicy(sizePolicy);
        QFont font7;
        font7.setPointSize(24);
        font7.setBold(false);
        font7.setWeight(50);
        pushButtonClear->setFont(font7);
        pushButtonClear->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_3->addWidget(pushButtonClear, 0, 0, 1, 1);

        pushButtonRadiants = new QPushButton(layoutWidget2);
        pushButtonRadiants->setObjectName(QString::fromUtf8("pushButtonRadiants"));
        sizePolicy.setHeightForWidth(pushButtonRadiants->sizePolicy().hasHeightForWidth());
        pushButtonRadiants->setSizePolicy(sizePolicy);
        pushButtonRadiants->setFont(font);
        pushButtonRadiants->setStyleSheet(QString::fromUtf8("QPushButton:pressed {\n"
"	background-color: #a1a1a1;\n"
"	border: 1px solid gray;\n"
"	color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid gray;\n"
"	color: #4c4c4c;\n"
"}\n"
""));

        gridLayout_3->addWidget(pushButtonRadiants, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 485, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Kalkulando", nullptr));
        pushButtonNum_3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        pushButtonNum_8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        pushButtonMinus->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        pushButtonNum_6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        pushButtonNum_0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButtonNum_5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        pushButtonNum_2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        pushButtonPlus->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        pushButtonNum_7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        pushButtonDot->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        pushButtonSIN->setText(QCoreApplication::translate("MainWindow", "sin", nullptr));
        pushButtonNum_4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        pushButtonCOTG->setText(QCoreApplication::translate("MainWindow", "cotg", nullptr));
        pushButtonCOS->setText(QCoreApplication::translate("MainWindow", "cos", nullptr));
        pushButtonDiv->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        pushButtonNum_9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        pushButtonNum_1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        pushButtonMul->setText(QCoreApplication::translate("MainWindow", "*", nullptr));
        pushButtonComma->setText(QCoreApplication::translate("MainWindow", ",", nullptr));
        pushButtonTG->setText(QCoreApplication::translate("MainWindow", "tg", nullptr));
        pushButtonDel->setText(QCoreApplication::translate("MainWindow", "DEL", nullptr));
        pushButtonRoot->setText(QCoreApplication::translate("MainWindow", "\342\210\232", nullptr));
        pushButtonFactorial->setText(QCoreApplication::translate("MainWindow", "!", nullptr));
        pushButtonPower->setText(QCoreApplication::translate("MainWindow", "^", nullptr));
        pushButtonBracketR->setText(QCoreApplication::translate("MainWindow", ")", nullptr));
        pushButtonBracketL->setText(QCoreApplication::translate("MainWindow", "(", nullptr));
        pushButtonAns->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
        pushButtonClear->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        pushButtonRadiants->setText(QCoreApplication::translate("MainWindow", "RADIANTS", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
