#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include<QPushButton>
#include <QWidget>
#include <QMouseEvent>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr)                ;//構造函數
    void mousePressEvent(QMouseEvent *e)                            ;//重载按下事件
    void mouseReleaseEvent(QMouseEvent *e)                          ;//重载抬起事件
signals:
    void rightChecked()                                             ;//左击
    void leftChecked()                                              ;//右击
    void leftRightPressed()                                         ;//左右同时按下
    void leftRightReleased()                                        ;//左右同时松开
    void leftReleased()                                             ;//左松开

private:
    bool leftMousePressed = false                                   ;//左键按下标志
    bool rightMousePressed = false                                  ;//右键按下标志

};

#endif // MYPUSHBUTTON_H
