#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H
#include"gamewindow.h"
#include <QMainWindow>

/*
选项窗口
用于用户选择难度
*/
namespace Ui {
class ChoiceWindow;
}

class ChoiceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChoiceWindow(GameWindow *parent = nullptr)         ;//构造函数
    ~ChoiceWindow()                                             ;//析构函数
    void saveDefault(GameWindow * parent)                       ;//储存玩家选择的默认的难度
    bool saveCustom(GameWindow * parent)                        ;//储存玩家自定义的参数并初始化，设置参数成功返回true

private:
    Ui::ChoiceWindow *ui;
};

#endif // CHOICEWINDOW_H
