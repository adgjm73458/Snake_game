#ifndef GAME_H
#define GAME_H

#include <QPainter>
#include <QTime>
#include <QKeyEvent>
#include <QMainWindow>
#include <QStatusBar>
#include <QTextStream>
#include <QFile>

enum GameState{
    RUN,
    PAUSE,
    END
};
enum SnakeDirection{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
struct Coords{
    int x,y;
};

class Game : public QMainWindow
{
    Q_OBJECT
public:
    explicit Game(QMainWindow *parent = nullptr);
protected:
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);
private:
    static const int WIDTH = 500;
    static const int HEIGHT = 500;
    static const int BLOCK_SIZE = 10;
    static const int GRID_SIZE = WIDTH*HEIGHT/(BLOCK_SIZE*BLOCK_SIZE);
    static const int DELAY = 150;

    GameState game_state;
    int timer_id;
    int score;
    int record;

    QImage snake_head;
    QImage snake_body;
    SnakeDirection snake_dir;
    Coords snake_coords[GRID_SIZE];
    int snake_length;

    QImage food;
    Coords food_coords;

    void initGame();
    void loadImages();
    void move();
    void collision();
    void checkFood();
    void updateFood();
    void draw();
    void text(QPainter &painter, QString text);
    void loadRecord();
    void saveRecord();
};

#endif // GAME_H
