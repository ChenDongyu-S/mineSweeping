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
    explicit GameWindow(QWidget *parent = nullptr)      ;//构造函数
    ~GameWindow()                                       ;//析构函数
    void init()                                         ;//初始化界面，每次调用时，会根据玩家的设置初始化地图
    void setHeight(int)                                 ;//提供给外部的接口，用于改变雷区的高度
    void setWid(int)                                    ;//提供给外部的接口，用于改变雷区的宽度
    void setMineNum(int)                                ;//提供给外部的接口，用于改变雷的数量
    void initMineTable()                                ;//初始化雷表
    int initMyRand(int,int)                             ;//产生一个从low到high的随机数
    int aroundCount(int,int)                            ;//计算雷表上的数字
    void mineDfs(int,int,int)                           ;//递归翻开0值格子
    void gameOver(bool)                                 ;//游戏结束判断胜利函数
    void mapLight()                                     ;//用于点亮地图




private:
    Ui::GameWindow *ui;

    int winCount                                        ;//设置获胜计数，一般记录了无雷的格子的数量，若计数用完，则游戏结束，判断是否胜利
    int height = 10                                     ;//雷区高度
    int wid = 10                                        ;//雷区宽度
    int mineNum = 10                                    ;//雷的数量
    QVector<QVector<MyPushButton*>> map                 ;//用于显示雷区布局的数组，每一个格子使用一个Button表示
    QVector<QVector<int>> mineTable                     ;//雷表，用于存储雷的实际情况,其值在[0,8]内为周边的雷数，-1：有雷，-2：已被打开，[9,80]内：问号。[99，800]：旗子。
    int mineCount                                       ;//用于雷的计数：每次插旗子减少一个雷，每次取消旗子增加一个雷
    QTimer * timer                                      ;//定义一个计时器，用于计时
    int sec = 0                                         ;//定义秒数，接收定时，并输出
    bool isTimeCount =false                             ;//开始计时标记，设置为true才会触发计时



};

#endif // GAMEWINDOW_H
