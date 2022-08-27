#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"

bool radiants = false;
QString buffer = "";
std::string buffer_str = "";
controller* Controller = new controller;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->Display->setText(buffer);
    QPushButton *numBtn[10];
        for (int i = 0; i < 10; ++i) {
            QString btnName = "pushButtonNum_" + QString::number(i);
            numBtn[i] = MainWindow::findChild<QPushButton *>(btnName);
            connect(numBtn[i], SIGNAL(released()), this, SLOT(NumPressed()));
        }

        connect(ui->pushButtonRadiants, SIGNAL(released()), this, SLOT(RadiantsPressed()));
        connect(ui->pushButtonAns, SIGNAL(released()), this, SLOT(SolvePressed()));
        connect(ui->pushButtonDel, SIGNAL(released()), this, SLOT(DelPressed()));
        connect(ui->pushButtonClear, SIGNAL(released()), this, SLOT(ClearPressed()));

        connect(ui->pushButtonCOS, SIGNAL(released()), this, SLOT(FuncPressed()));
        connect(ui->pushButtonSIN, SIGNAL(released()), this, SLOT(FuncPressed()));
        connect(ui->pushButtonCOTG, SIGNAL(released()), this, SLOT(FuncPressed()));
        connect(ui->pushButtonTG, SIGNAL(released()), this, SLOT(FuncPressed()));
        
        connect(ui->pushButtonPlus, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonMinus, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonMul, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonDiv, SIGNAL(released()), this, SLOT(MathPressed()));

        connect(ui->pushButtonBracketL, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonBracketR, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonComma, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonFactorial, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonDot, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonPower, SIGNAL(released()), this, SLOT(MathPressed()));
        connect(ui->pushButtonRoot, SIGNAL(released()), this, SLOT(MathPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MathPressed()
{

    QPushButton* btn = (QPushButton*)sender();
    buffer = btn->objectName();
    buffer_str = buffer.toStdString();

    if  (buffer_str == "pushButtonRoot")
    {
        buffer = "q";
    }
    else
    {
        buffer = btn->text();
    }
    buffer_str = buffer.toStdString();
    try {
        Controller->AddOperatorToEquation(buffer_str);
    }
    catch (...) {
        ui->Display->setText("error: cannot add another operator!");
        return;
    }
    buffer = QString::fromStdString(Controller->GetImprovedBuffer());
    ui->Display->setText(buffer);
}

void MainWindow::SolvePressed()
{
    double solved;
    try {
        solved = Controller->Solve(Controller->GetBuffer());
    }
    catch (...) {
        ui->Display->setText("error: cannot solve equation!");
        return;
    }
    buffer = QString::number(solved);
    Controller->Clear();
    buffer_str = std::to_string(solved);
    Controller->AddNumberToEquation(buffer_str);
    ui->Display->setText(buffer);
}

void MainWindow::FuncPressed()
{
    QPushButton *btn = (QPushButton *)sender();
    buffer = btn->text();
    if (QString::compare(buffer, "sin", Qt::CaseInsensitive) == 0)
    {
        buffer = "s";
    } else if (QString::compare(buffer, "cos", Qt::CaseInsensitive) == 0)
    {
        buffer = "c";
    }
    else if (QString::compare(buffer, "tg", Qt::CaseInsensitive) == 0)
    {
        buffer = "t";
    }
    else if (QString::compare(buffer, "cotg", Qt::CaseInsensitive) == 0)
    {
        buffer = "g";
    }
    else
    {
        buffer = "3.14159265359";
    }
    buffer_str = buffer.toStdString();
    Controller->AddOperatorToEquation(buffer_str);
    buffer = QString::fromStdString(Controller->GetImprovedBuffer());
    ui->Display->setText(buffer);
}

void MainWindow::NumPressed()
{
    QPushButton* btn = (QPushButton*)sender();
    buffer = btn->text();
    buffer_str = buffer.toStdString();
    Controller->AddNumberToEquation(buffer_str);
    buffer = QString::fromStdString(Controller->GetImprovedBuffer());
    ui->Display->setText(buffer);
}


void MainWindow::DelPressed()
{
    buffer = QString::fromStdString(Controller->GetImprovedBuffer());
    if(buffer == "")
    { }
    else {
        Controller->DeleteChar();
    }
    buffer = QString::fromStdString(Controller->GetImprovedBuffer());
    ui->Display->setText(buffer);
}

void MainWindow::ClearPressed()
{
    Controller->Clear();
    buffer = QString::fromStdString(Controller->GetImprovedBuffer());
    ui->Display->setText(buffer);
}

void MainWindow::RadiantsPressed()
{
    if (radiants == true)
    {
        ui->pushButtonRadiants->setText("RADIANS");
        Controller->SetInRadian(false);
        radiants = false;
    } else
    {
        ui->pushButtonRadiants->setText("DEGREES");
        Controller->SetInRadian(true);
        radiants = true;
    }
}
