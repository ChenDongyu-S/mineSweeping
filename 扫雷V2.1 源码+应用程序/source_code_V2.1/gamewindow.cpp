#include "gamewindow.h"
#include "ui_gamewindow.h"
#include<QPushButton>
#include"choicewindow.h"
#include<QTime>
#include<QTimer>
#include<QDebug>
#include<QMessageBox>
#include<aboutwindows.h>
#include"rankwindow.h"
#include"helpwindow.h"
//构造函数，定义了窗口各个控件的作用和连接关系
GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);


    //修改窗口标题
    setWindowTitle("扫雷");

    //游戏生成时直接初始化
    init();

    //建立对应案件的联系

    //帮助菜单内：
    //游戏说明
    connect(ui->actiondescription,&QAction::triggered,[=](){
        HelpWindow * c = new HelpWindow(this);
        c->setWindowModality(Qt::WindowModal);
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();
    });
    //关于
    connect(ui->actionabout,&QAction::triggered,[=](){
        AboutWindows * c = new AboutWindows(this);
        c->setWindowModality(Qt::WindowModal);
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();
    });

    //选项菜单
    connect(ui->choice,&QAction::triggered,[=](){
        ChoiceWindow * c = new ChoiceWindow(this);
        c->setWindowModality(Qt::WindowModal);
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();

    });
    //选项菜单内：
    //排行榜
    connect(ui->rank,&QAction::triggered,[=](){
        RankWindow * c = new RankWindow(this);
        c->setWindowModality(Qt::WindowModal);
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();
    });

    //初始化
    connect(ui->init,&QAction::triggered,[=](){
        init();
    });

    //中间笑脸按钮
    //点击就初始化
    connect(ui->smileButton,&QPushButton::clicked,[=](){
        init();
    });
    //按下的时候出现惊讶表情
    connect(ui->smileButton,&QPushButton::pressed,[=](){
        ui->smileButton->setIcon(QIcon(":/image/jinya.png"));
    });
    //松开的时候恢复嘻嘻表情
    connect(ui->smileButton,&QPushButton::released,[=](){
        ui->smileButton->setIcon(QIcon(":/image/xixi.png"));
    });


    //设置LCD显示器的参数
    //左边时间显示
    ui->lcdTimeCount->setDigitCount(3)                                                          ;//设置可显示的数字个数
    ui->lcdTimeCount->setStyleSheet("border: 1px solid green; color: red; background: black;")  ;//设置样式
    //右边雷数显示
    ui->lcdMineCount->setDigitCount(3)                                                          ;//设置可显示的数字个数
    ui->lcdMineCount->setStyleSheet("border: 1px solid green; color: red; background: black;")  ;//设置样式
    ui->lcdMineCount->display(mineCount)                                                        ;//设置显示的具体数字

    //创建计时器对象，计时
    timer = new QTimer(this)                                                                    ;//定义计时器
    timer->start(1000)                                                                          ;//开始计时



}

//析构函数
GameWindow::~GameWindow()
{
    delete ui;
}

//初始化
void GameWindow::init(){


    ui->smileButton->setIcon(QIcon(":/image/xixi.png"))     ;//初始化表情
    initMineTable()                                         ;//初始化雷表
    sec=0                                                   ;//初始化时间
    isTimeCount = true                                      ;//开始计时
    mineCount = mineNum                                     ;//初始化雷数
    setFixedSize(40+20*height,100+20*wid)                   ;//根据参数设置窗口大小


    //因为之前把每一个按钮都在堆上开辟了空间，所以在重新初始化的时候，必须清理掉这些空间
    if(map.size() != 0){
        for(int i = 0; i<map.size(); ++i){
            for(int j=0; j<map[0].size(); j++){
                if(map[i][j] != nullptr) delete map[i][j];
            }
        }
    }
    map.clear();//将map里的指针清空

    //根据参数，设置好雷区的尺寸，并在每个格子里设置好按钮
    for(int i = 0; i<height; ++i){
        QVector<MyPushButton *> t;
        for(int j=0; j<wid; j++){
            MyPushButton * temp = new MyPushButton(this);
            temp->resize(20,20);
            temp->move(20+20*i,80+20*j);
            t.push_back(temp);

        }
        map.push_back(t);
    }

    //显示这个按钮，并建立每个按钮的连接，部分使用了MyPushButton作用域的信号为重载过的
    //这里我就直接写了，不做过多的封装了
    for(int i = 0; i<map.size(); ++i){
        qDebug()<<"建立行数i:"<<i;
        for(int j=0; j<map[0].size(); j++){

            map[i][j]->show();//显示按钮

            //建立按钮与双击按下时的联系
            connect(map[i][j],&MyPushButton::leftRightPressed,[=](){

                qDebug()<<"同时按下";

                if(mineTable[i+1][j+1] == -2){
                    //如果被打开，则遍历周边8个格子
                    for(int x= i-1; x<i+2; ++x){
                        for(int y = j-1; y<j+2; ++y){
                            if(y>=0 && x>=0 && x< height && y<wid){
                                if(mineTable[x+1][y+1] > -2 && mineTable[x+1][y+1] < 9 ) {
                                    map[x][y]->setIcon(QIcon(":/image/blank.png"));
                                }
                            }
                        }
                    }
                }
                gameOver(true);//判断游戏是否结束
            });

            //建立双击抬起的与按钮的联系
            connect(map[i][j],&MyPushButton::leftRightReleased,[=](){
                ui->smileButton->setIcon(QIcon(":/image/xixi.png"));
                qDebug()<<"同时抬起";

                /*
                第一次遍历记录下周边的错误旗子数、雷数、问号数
                做判定：
                1：同时存在雷数和错误旗子，游戏结束，雷曝光
                2：存在雷或者存在问号或者存在错误旗子数，不操作
                3：如果都不存在，全部打开，设置为已读-2状态
                */

                int mineCount = 0               ;//旗子数
                int flagCount = 0               ;//雷数
                int quesCount = 0               ;//问号数

                //遍历周边8个格子
                for(int x= i-1; x<i+2; ++x){
                    for(int y = j-1; y<j+2; ++y){
                        if(y>=0 && x>=0 && x< height && y<wid){
                            //第一次遍历记录下周边的错误旗子数、雷数、问号数
                            //做判定：

                            if(mineTable[x+1][y+1] == -1 ){
                                ++mineCount;
                            }
                            if(mineTable[x+1][y+1] >= 9 && mineTable[x+1][y+1] <= 80 && mineTable[x+1][y+1] == -10){
                                ++quesCount;
                            }
                            if( mineTable[x+1][y+1] >= 99){
                                ++flagCount;
                            }
                            if(mineTable[x+1][y+1] > -2 && mineTable[x+1][y+1] < 9) map[x][y]->setIcon(QIcon(""));
                        }
                    }
                }

                //做判定：
                //1：同时存在雷数和错误旗子，游戏结束，雷曝光
                //2：存在雷或者存在问号或者存在错误旗子数，不操作
                //3：如果都不存在，全部打开，设置为已读-2状态
                if(flagCount != 0 && mineCount != 0){
                    gameOver(false);
                }
                else if(flagCount == 0 && mineCount == 0 && quesCount ==0){
                    for(int x= i-1; x<i+2; ++x){
                        for(int y = j-1; y<j+2; ++y){
                            if(y>=0 && x>=0 && x< height && y<wid){
                                if(mineTable[x+1][y+1] > -2 && mineTable[x+1][y+1] < 9){
                                    int count = 10          ;//count为搜索的次数
                                    mineDfs(x,y,count)      ;//点到0会扩散的深度优先搜索函数
                                }
                            }
                        }
                    }
                }
                gameOver(true);
            });

            //建立鼠标左键与按钮的联系

            //按下左键，大表情切换为吓吓！
            connect(map[i][j],&MyPushButton::leftChecked,[=](){
                ui->smileButton->setIcon(QIcon(":/image/xiaxia.png"));
            });

            //松开左键，大表情切换回来，并且翻开格子，并且第一次按下还会触发计时
            connect(map[i][j],&MyPushButton::leftReleased,[=](){

                ui->smileButton->setIcon(QIcon(":/image/xixi.png"));

                //初始化中将isTimeCount设置为true，则表情还未计时，此时计时，并将isTimeCount设置为false;
                if(isTimeCount){
                    connect(timer,&QTimer::timeout,[=](){
                        sec++;
                        qDebug()<<sec;
                        ui->lcdTimeCount->display(sec);
                    });
                    isTimeCount = false;
                }

                //踩到雷
                if(mineTable[i+1][j+1] == -1){
                    map[i][j]->setIcon(QIcon(":/image/mine.png"));
                    gameOver(false);
                }

                //踩到空地，既没有雷也没有旗子问号
                else if(mineTable[i+1][j+1] >= 0 && mineTable[i+1][j+1] < 9){
                    int count = 10;
                    mineDfs(i,j,count);
                }

                //判断游戏结束
                gameOver(true);

            });

            //建立鼠标右键与按钮的联系
            //这里需要知道我们使用右键需要标记两个状态，棋子和问号，因此我们在插入棋子的时候将雷表中的值*100，问号*10
            //这样我们就可以在遍历的时候判定，如果有值小于等于-100的话，游戏失败
            connect(map[i][j],&MyPushButton::rightChecked,[=](){

                //第一次点击右键也会计时
                if(isTimeCount){
                    connect(timer,&QTimer::timeout,[=](){
                        sec++;
                        qDebug()<<sec;
                        ui->lcdTimeCount->display(sec);
                    });
                    isTimeCount = false;
                }


                //无状态
                if(mineTable[i+1][j+1] > -2 && mineTable[i+1][j+1] < 9){
                    --mineCount;
                    ui->lcdMineCount->display(mineCount);
                    map[i][j]->setIcon(QIcon(":/image/flag.png"));
                    if(mineTable[i+1][j+1] == 0) mineTable[i+1][j+1] = 99;
                    else mineTable[i+1][j+1] *= 100;
                }
                //旗子态
                else if(mineTable[i+1][j+1] == -100 || mineTable[i+1][j+1] > 90){
                    ++mineCount;
                    ui->lcdMineCount->display(mineCount);
                    map[i][j]->setIcon(QIcon(":/image/question.png"));
                    if(mineTable[i+1][j+1] == 99) mineTable[i+1][j+1] = 9;
                    else mineTable[i+1][j+1] /= 10;
                }
                //问号态
                else if(mineTable[i+1][j+1] == -10 || mineTable[i+1][j+1] >= 9){
                    map[i][j]->setIcon(QIcon(""));
                    if(mineTable[i+1][j+1] == 9) mineTable[i+1][j+1] = 0;
                    else mineTable[i+1][j+1] /= 10;
                }
            });


        }
    }

}



//在设定雷表的时候，建议把雷表的尺寸在map的尺寸上加2，这样便于统计每个格子上周围的雷数，而不用再加很多条件判断
void GameWindow::initMineTable(){
    winCount = height*wid - mineNum;
    mineTable.clear();
    int h = height+2;
    int w = wid +2;
    int m = mineNum;
    //初始化雷表
    for(int i = 0; i<h; ++i){
        QVector<int> temp;
        for(int j = 0; j<w; ++j){
            temp.push_back(0);
        }
        mineTable.push_back(temp);
    }
    //设置雷的位置
    QTime time;
    srand(time.msec()+time.second());
    while(m!=0){
        int rand_h = 1+rand()%height;
        int rand_w = 1+rand()%wid;
        if(mineTable[rand_h][rand_w] == 0) {
            mineTable[rand_h][rand_w] = -1;
            --m;
        }
    }
    //设定没有的格子上的数字
    for(int i = 1; i<h-1; ++i){
        for(int j = 1; j<w-1; ++j){
            if(mineTable[i][j] == 0){
                mineTable[i][j] = aroundCount(i,j);

            }
        }
    }

}

//计算中心格子周围的雷数
int GameWindow::aroundCount(int i, int j){
    int count = 0;
    for (int x = i - 1; x <= i + 1; ++x) {
        for (int y = j - 1; y <= j + 1; ++y) {
            if (mineTable[x][y] == -1) {
                ++count;
            }
        }
    }
    return count;
}

//实现0号区域的递归翻开
void GameWindow::mineDfs(int i, int j, int count){
    if(i<0 || j<0 || i>=height || j>=wid || mineTable[i+1][j+1] == -2 || count == 0 || (mineTable[i+1][j+1] >= 99) || (mineTable[i+1][j+1] >= 9)) return;
    if(mineTable[i+1][j+1] != 0) {
        QString temp = QString::number(mineTable[i+1][j+1]);
        map[i][j]->setText(temp);
        --winCount;
        map[i][j]->setStyleSheet("background-image: url(:/image/blank.png);");
        mineTable[i+1][j+1] = -2;
    }
    else{
        --count;
        map[i][j]->setText("0");
        --winCount;
        map[i][j]->setStyleSheet("background-image: url(:/image/blank.png);");
        mineTable[i+1][j+1] = -2;
        for (int x = i - 1; x <= i + 1; ++x) {
            for (int y = j - 1; y <= j + 1; ++y) {
                if (!(i == x && j == y)) {

                    mineDfs(x,y,count);
                }
            }
        }

    }
}

//游戏结束判定
void GameWindow::gameOver(bool isWin){

    if(isWin){
        if(this->winCount == 0){
            timer->disconnect()     ;//断开所有联系
            this->mapLight()        ;//照亮雷区所有的雷
            ui->smileButton->setIcon(QIcon(":/image/qinqin.png"));
            QMessageBox::information(this,"提示","游戏胜利，恭喜你！\n点击Ok重新开始游戏！",QMessageBox::Ok);
        }
    }
    else{
        this->mapLight()            ;//照亮雷区所有的雷
        timer->disconnect()         ;//断开所有联系
        ui->smileButton->setIcon(QIcon(":/image/kuku.png"));
        QMessageBox::critical(this,"提示","游戏失败，你踩到雷啦！笨蛋~\n点击Ok重新开始游戏！",QMessageBox::Ok);
    }

}


//照亮雷区所有的雷
void GameWindow::mapLight(){
    for(int i= 0; i<height; ++i){
        for(int j =0; j<wid; ++j){
            map[i][j]->disconnect();
            //qDebug()<<"1111111111";
            if(mineTable[i+1][j+1] == -1){
                map[i][j]->setIcon(QIcon(":/image/mine.png"));

            }
            else if(mineTable[i+1][j+1] > 90){
                map[i][j]->setIcon(QIcon(":/image/err.png"));
            }
        }
    }
}



//以下是提供给外部函数用于修改成员的函数
void GameWindow::setHeight(int h){
    height = h;
}

void GameWindow::setWid(int w){
    wid = w;
}

void GameWindow::setMineNum(int m){
    mineNum = m;
}
