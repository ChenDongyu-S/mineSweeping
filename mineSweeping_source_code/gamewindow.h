#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include"mypushbutton.h"
#include <QMainWindow>
#include <QVector>
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);//构造函数
    ~GameWindow();//析构函数
    void init();//初始化界面，每次调用时，会根据玩家的设置初始化地图
    void setHeight(int);//提供给外部的接口，用于改变雷区的高度
    void setWid(int);//提供给外部的接口，用于改变雷区的宽度
    void setMineNum(int);//提供给外部的接口，用于改变雷的数量
    void initMineTable();//初始化雷表
    int initMyRand(int low,int high);//产生一个从low到high的随机数
    int aroundCount(int,int);//计算雷表上的数字
    void mineDfs(int,int,int);
    void gameOver(bool);
    void mapLight();//用于点亮地图




private:
    Ui::GameWindow *ui;
    int winCount;
    int height = 10;
    int wid = 10;
    int mineNum = 10;
    QVector<QVector<MyPushButton*>> map;//用于显示雷区布局的数组
    QVector<QVector<int>> mineTable;//用于存储雷的实际情况,值大于0时为周边的雷数，-1：有雷，-2：已被打开
    QMessageBox::StandardButton *Ok = nullptr;
    int mineCount;//用于雷的计数：每次插旗子减少一个雷，每次取消旗子增加一个雷

    QTimer * timer;
    int sec = 0;

    bool isTimeCount =false;
signals:
    void setInit();

};

#endif // GAMEWINDOW_H
