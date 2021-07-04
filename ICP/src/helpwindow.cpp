/**
  @file helpwindow.cpp
  @author David Hudak
  @brief file contains implementation of dialog window for help.
  */

#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true); // Dialog window should be readonly.
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
