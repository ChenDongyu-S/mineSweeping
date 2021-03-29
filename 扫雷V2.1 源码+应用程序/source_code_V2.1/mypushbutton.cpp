#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}
void MyPushButton::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::RightButton){
        emit rightChecked();
    }
    if(e->button() == Qt::LeftButton && e->button()){
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
    else if(rightMousePressed == true) emit leftReleased();
    leftMousePressed = false;
    rightMousePressed = false;
}




