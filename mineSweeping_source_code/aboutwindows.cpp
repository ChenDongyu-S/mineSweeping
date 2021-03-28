#include "aboutwindows.h"
#include "ui_aboutwindows.h"

AboutWindows::AboutWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutWindows)
{
    ui->setupUi(this);
    ui->aboutInfo->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    setWindowTitle("关于扫雷");
}

AboutWindows::~AboutWindows()
{
    delete ui;
}
