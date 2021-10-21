#include "game.h"

Game::Game(QMainWindow *parent) : QMainWindow(parent)
{
    setStyleSheet("background-color:black;");
    setFixedSize(WIDTH,HEIGHT);
    loadImages();
    initGame();
}


void Game::timerEvent(QTimerEvent *event){
    Q_UNUSED(event);

    if(game_state==RUN){
        checkFood();
        collision();
        move();
    }
    else if(game_state==PAUSE){
        killTimer(timer_id);
    }
    repaint();
}

void Game::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    if(key == Qt::Key_Escape){
        close();
    }
    if(game_state == RUN){
        if(key == Qt::Key_Up && snake_dir!=DOWN){
            snake_dir = UP;
        }
        if(key == Qt::Key_Down && snake_dir!=UP){
            snake_dir = DOWN;
        }
        if(key == Qt::Key_Left && snake_dir!=RIGHT){
            snake_dir = LEFT;
        }
        if(key == Qt::Key_Right && snake_dir!=LEFT){
            snake_dir = RIGHT;
        }
        if(key == Qt::Key_Space){
            game_state = PAUSE;
        }
    }
    else if(game_state == PAUSE){
        if(key == Qt::Key_Space){
            game_state = RUN;
            timer_id = startTimer(DELAY);
        }
    }
    else if(game_state == END){
        if(key == Qt::Key_Space){
            game_state = RUN;
            initGame();
        }
    }
}

void Game::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    draw();
}

void Game::initGame(){
    game_state = RUN;
    score = 0;
    loadRecord();
    snake_length = 3;
    snake_dir = UP;
    for(int i = 0; i< snake_length; ++i){
        snake_coords[i].x = uint(WIDTH/(2*BLOCK_SIZE))*BLOCK_SIZE;
        snake_coords[i].y = (uint(HEIGHT/(2*BLOCK_SIZE)) + i)*BLOCK_SIZE;
    }
    updateFood();
    timer_id = startTimer(DELAY);
}

void Game::loadImages(){
    snake_head.load("assets/head.png");
    snake_body.load("assets/body.png");
    food.load("assets/food.png");
}

void Game::move(){
    for(int i=snake_length; i>0; --i){
        snake_coords[i].x = snake_coords[i-1].x;
        snake_coords[i].y = snake_coords[i-1].y;
    }
    if(snake_dir == UP){
        snake_coords[0].y -= BLOCK_SIZE;
    }
    if(snake_dir == DOWN){
        snake_coords[0].y += BLOCK_SIZE;
    }
    if(snake_dir == LEFT){
        snake_coords[0].x -= BLOCK_SIZE;
    }
    if(snake_dir == RIGHT){
        snake_coords[0].x += BLOCK_SIZE;
    }
}

void Game::collision(){
    if(snake_coords[0].x < 0 || snake_coords[0].y < 0 || snake_coords[0].x >= WIDTH || snake_coords[0].y >= HEIGHT){
        game_state = END;
    }
    if(snake_length>4){
        for(int i=4; i<snake_length; ++i){
            if(snake_coords[i].x == snake_coords[0].x && snake_coords[i].y == snake_coords[0].y){
                game_state = END;
            }

        }
    }
    if(game_state == END){
        killTimer(timer_id);
    }
}

void Game::checkFood(){
    if(snake_coords[0].x == food_coords.x && snake_coords[0].y == food_coords.y){
        ++snake_length;
        ++score;
        if(score > record){
            saveRecord();
        }
        updateFood();
    }
}

void Game::updateFood(){
    Coords old_coords;
    old_coords.x = food_coords.x;
    old_coords.y = food_coords.y;
    using std::srand;
    QTime time = QTime::currentTime();
    srand(static_cast<uint>(time.msec()));
    food_coords.x = (rand() % uint(WIDTH/BLOCK_SIZE)) * BLOCK_SIZE;
    food_coords.y = (rand() % uint(HEIGHT/BLOCK_SIZE)) * BLOCK_SIZE;
    if(old_coords.x == food_coords.x && old_coords.y == food_coords.y ){
        updateFood();
    }
}

void Game::draw(){
    QPainter painter(this);
    if(game_state == RUN){
        painter.drawImage(food_coords.x, food_coords.y, food);
        for(int i=0; i < snake_length; ++i){
            if(i==0){
                painter.drawImage(snake_coords[i].x, snake_coords[i].y, snake_head);
            }else{
                painter.drawImage(snake_coords[i].x, snake_coords[i].y, snake_body);
            }
        }
    }else if(game_state == PAUSE){
        text(painter, "Pause");
    }else{
        text(painter, "Game over!");
    }
    QString str = "Score: %1 Record: %2";
    statusBar()->showMessage(str.arg(score).arg(record));
}

void Game::text(QPainter &painter, QString text){
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics f_string(font);
    int w = f_string.horizontalAdvance(text);
    painter.translate(QPoint(width()/2, height()/2));
    painter.drawText(-w/4, 0, text);
}

void Game::loadRecord(){
    QFile file("record");
    try {
        if (!file.open(QIODevice::ReadOnly)){
            throw -1;
        }
        QTextStream in(&file);
        QString rec;
        rec = in.readLine();
        record = rec.toInt();

    }  catch (...) {
        qWarning("Cannot open file for reading record");
    }
}

void Game::saveRecord(){
    QFile file("record");
    try {
        if (!file.open(QIODevice::WriteOnly)){
            throw -1;
        }
        QTextStream out(&file);
        out << score;

    }  catch (...) {
        qWarning("Cannot open file for write record");
    }
}
