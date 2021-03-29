#ifndef ABOUTWINDOWS_H
#define ABOUTWINDOWS_H

#include <QMainWindow>

/*
这个是菜单下->帮助->关于 窗口
主要记录了一些关于这个软件的信息
基本都是使用ui直接拖拽空间实现的
*/

namespace Ui {
class AboutWindows;
}

class AboutWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutWindows(QWidget *parent = nullptr);
    ~AboutWindows();

private:
    Ui::AboutWindows *ui;
};

#endif // ABOUTWINDOWS_H
