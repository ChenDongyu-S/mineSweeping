#include "choicewindow.h"
#include "ui_choicewindow.h"
#include<QRadioButton>
#include<gamewindow.h>
#include<QDebug>
#include<QMessageBox>
ChoiceWindow::ChoiceWindow(GameWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::ChoiceWindow)
{
    ui->setupUi(this);
    setWindowTitle("选项");


    //连接应用
    //如果应用被按下，则会保存当前信息
    connect(ui->b_choice_apply,&QPushButton::clicked,[=](){

        //保存默认的信息
        if(ui->radioButton_default->isChecked()){
            saveDefault(parent);
        }

        //保存用户自定义的信息
        else if(ui->radioButton_custom->isChecked()){
            saveCustom(parent);
        }

    });

    connect(ui->b_choice_enter,&QPushButton::clicked,[=](){

        //保存默认的信息
        if(ui->radioButton_default->isChecked()){
            saveDefault(parent);
            this->close();
        }

        //保存用户自定义的信息,如果信息没出错，则关闭窗口
        else if(ui->radioButton_custom->isChecked()){
            if(saveCustom(parent)) this->close();
        }

    });
    connect(ui->b_choice_cancel,&QPushButton::clicked,this,&QMainWindow::close);
}

//析构函数
ChoiceWindow::~ChoiceWindow()
{

    delete ui;
}

//通过窗口交互设置游戏默认参数
void ChoiceWindow::saveDefault(GameWindow *parent){
    if(ui->radioButton_defulat_midLevel->isChecked()){
        parent->setHeight(25);
        parent->setWid(16);
        parent->setMineNum(100);
    }
    else if(ui->radioButton_defulat_highLevel->isChecked()){
        parent->setHeight(35);
        parent->setWid(16);
        parent->setMineNum(200);
    }
    else {
        parent->setHeight(10);
        parent->setWid(10);
        parent->setMineNum(10);
    }
    parent->init();
}

//通过窗口交互设置游戏自定义参数
bool ChoiceWindow::saveCustom(GameWindow *parent){
    int h = ui->text_choice_height->text().toInt();
    int w = ui->text_choice_wid->text().toInt();
    int m = ui->text_choice_mineNum->text().toInt();
    if((h>0 && h<=40) && (w>0 && w<= 40)){
        if(m<0 || m>(h*m>>1)){
             QMessageBox::critical(this,"err","您输入的雷数（0-面积/2）有误，应用失败",QMessageBox::Ok);
             return false;
        }
        else{
            parent->setHeight(h);
            parent->setWid(w);
            parent->setMineNum(m);
            parent->init();
            return true;
        }

    }
    else{
        QMessageBox::critical(this,"err","您输入的尺寸有误（0-40）（0-40），应用失败",QMessageBox::Ok);
        return false;
    }
}
