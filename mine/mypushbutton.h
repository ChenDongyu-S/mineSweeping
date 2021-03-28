#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include<QPushButton>
#include <QWidget>
#include <QMouseEvent>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void rightChecked();
    void leftChecked();
    void leftRightPressed();
    void leftRightReleased();
    void leftReleased();
    //void emitLRPressed();
private:
    bool leftMousePressed = false;
    bool rightMousePressed = false;

};

#endif // MYPUSHBUTTON_H
