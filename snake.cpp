#include "snake.h"
#include <QPainter>
#include <QTime>
#include <QDebug>

Snake::Snake(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color:black;");
    start = false;
    resize(B_WIDTH, B_HEIGHT);
}

Snake::~Snake()
{
}

void Snake::loadImages()
{
    body.load("_Images/body.png");
    head.load("_Images/head.png");
    apple.load("_Images/apple.png");
}

void Snake::initGame()
{
    this->window()->setWindowTitle("Snake. Your score: " + QString::number(0));

    leftDirection = false;
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    inGame = true;
    score = 0;

    loadImages();

    dots = 3;

    for(int z = 0; z < dots; z++){
        x[z] = 50 - z * 10;
        y[z] = 50;
    }

    locateApple();

    timerId = startTimer(DELAY);
}

void Snake::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    if(start){
        doDrawing();
    }else {
        QPainter qp(this);
        menu(qp);
    }
}

void Snake::doDrawing()
{
    QPainter qp(this);

    if(inGame){
        qp.drawImage(apple_x, apple_y, apple);

        for(int z = 0; z < dots; z++){
            if(z == 0){
                qp.drawImage(x[z], y[z], head);
            }else{
                qp.drawImage(x[z], y[z], body);
            }
        }
    }else{
        gameOver(qp);
    }
}

void Snake::gameOver(QPainter &qp)
{
    this->window()->setWindowTitle("Snake. Result: " + QString::number(score));

    QString messageNameGame = "Game over";
    QString messageForOpenMenu = "Press \"S\" to start";

    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);

    QFont fontSecond("Courier", 12, QFont::DemiBold);
    QFontMetrics fms(fontSecond);

    int textWidthMessageNameGame = fm.width(messageNameGame);
    int textWidthMessageForOpenMenu = fms.width(messageForOpenMenu);

    qp.setPen(QColor(Qt::white));
    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2)); // centre

    qp.drawText(-textWidthMessageNameGame/2, 0, messageNameGame);

    qp.setFont(fontSecond);
    qp.drawText(-textWidthMessageForOpenMenu/2, 25, messageForOpenMenu);
}

void Snake::menu(QPainter &qp)
{
    QString messageNameGame = "\"Snake\"";
    QString messageForOpenMenu = "Press \"S\" to start";

    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);

    QFont fontSecond("Courier", 12, QFont::DemiBold);
    QFontMetrics fms(fontSecond);

    int textWidthMessageNameGame = fm.width(messageNameGame);
    int textWidthMessageForOpenMenu = fms.width(messageForOpenMenu);

    qp.setPen(QColor(Qt::white));
    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2)); // centre

    qp.drawText(-textWidthMessageNameGame/2, 0, messageNameGame);

    qp.setFont(fontSecond);
    qp.drawText(-textWidthMessageForOpenMenu/2, 25, messageForOpenMenu);
}

void Snake::checkApple()
{
    if((x[0] == apple_x) && (y[0] == apple_y)){
        dots++;
        score += 10;
        this->window()->setWindowTitle("Snake. Your score: " + QString::number(score));
        locateApple();
    }
}

void Snake::move()
{
    for(int z = dots; z > 0; z--)
    {
        x[z] = x[z - 1];
        y[z] = y[z - 1];
    }

    if(leftDirection){
        x[0] -= DOT_SIZE;
    }

    if(rightDirection){
        x[0] += DOT_SIZE;
    }

    if(upDirection){
        y[0] -= DOT_SIZE;
    }

    if(downDirection){
        y[0] += DOT_SIZE;
    }
}

void Snake::checkCollision()
{
    for(int z = dots; z > 0; z--){
        if((z > 4) && (x[0] == x[z]) && (y[0] == y[z])){
            inGame = false;
        }
    }

    if(y[0] >= B_HEIGHT){
        inGame = false;
    }

    if(y[0] < 0){
        inGame = false;
    }

    if(x[0] >= B_WIDTH){
        inGame = false;
    }

    if(x[0] < 0){
        inGame = false;
    }

    if(!inGame){
        killTimer(timerId);
    }
}

void Snake::locateApple()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int r = qrand() % RAND_POS;
    apple_x = (r * DOT_SIZE);

    r = qrand() % RAND_POS;
    apple_y = (r * DOT_SIZE);
}

void Snake::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    if(inGame){
        checkApple();
        checkCollision();
        move();
    }

    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    if((key == Qt::Key_Left) && (!rightDirection)){
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if((key == Qt::Key_Right) && (!leftDirection)){
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if((key == Qt::Key_Up) && (!downDirection)){
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if((key == Qt::Key_Down) && (!upDirection)){
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if(key == Qt::Key_S){
        start = true;
        qDebug() << "start: " + QString::number((int)start);
        initGame();

    }

    QWidget:: keyPressEvent(e);
}
