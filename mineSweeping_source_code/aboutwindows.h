#ifndef ABOUTWINDOWS_H
#define ABOUTWINDOWS_H

#include <QMainWindow>

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
