#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}
void MyPushButton::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::RightButton && e->button() != Qt::LeftButton){
        emit rightChecked();
    }
    if(e->button() == Qt::LeftButton && e->button() != Qt::RightButton){
        emit leftChecked();
    }
    if(e->button() == Qt::RightButton){
        //emit leftRightPressed();
        leftMousePressed = true;
    }
    if(e->button() == Qt::LeftButton){
        //emit leftRightPressed();
        rightMousePressed = true;
    }
    if(rightMousePressed == true && leftMousePressed == true){
        emit leftRightPressed();
    }
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e){

    if(leftMousePressed == true && rightMousePressed == true) emit leftRightReleased();
    leftMousePressed = false;
    rightMousePressed = false;
}




