#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <QMainWindow>

namespace Ui {
class RankWindow;
}

class RankWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RankWindow(QWidget *parent = nullptr);
    ~RankWindow();

private:
    Ui::RankWindow *ui;
};

#endif // RANKWINDOW_H
