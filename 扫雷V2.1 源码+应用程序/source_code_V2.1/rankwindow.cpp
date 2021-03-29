#include "rankwindow.h"
#include "ui_rankwindow.h"

RankWindow::RankWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RankWindow)
{
    ui->setupUi(this);
    ui->rankText->setStyleSheet("background:transparent;border-width:0;border-style:outset");
}

RankWindow::~RankWindow()
{
    delete ui;
}
