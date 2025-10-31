//
// Created by 钭亦骏 on 25-10-27.
//

#include "Mainwindow.h"
#include <algorithm>
#include <random>
#include <QMessageBox>
#include <iostream>
#include <QApplication>

const int COLS = 20;
const int ROWS = 15;
const int BLOCK_SIZE = 30;
const int MARGIN = 10;
const int MinesNum = 30;
const QString path = "../../image/"; // 请自己修改

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口大小
    // setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setWindowTitle("Minesweeper");
    setFixedSize(MARGIN * 2 + COLS * BLOCK_SIZE,
                     MARGIN * 2 + ROWS * BLOCK_SIZE);

    // 设置背景为白色
    setStyleSheet("background-color: lightgray;");

    InitGame();
    std::cout << "开始运行";
    update();
}

Mainwindow::~Mainwindow() {
    delete this;
}

void Mainwindow::paintEvent(QPaintEvent *event) {
    // 绘制网格线
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawRect(MARGIN, MARGIN, BLOCK_SIZE * COLS, BLOCK_SIZE * ROWS);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            QRect rect(MARGIN + j * BLOCK_SIZE,
                       MARGIN + i * BLOCK_SIZE,
                       BLOCK_SIZE, BLOCK_SIZE);
            if (isOpened[i][j] == 0)
                painter.drawPixmap(rect, QPixmap(path + "initial.png"));

        // 插旗
            else if (isOpened[i][j] == 2)
                painter.drawPixmap(rect, QPixmap(path +"flag.png"));

            // 已打开
            else if (isOpened[i][j] == 1) {
                if (GameArea[i][j] == -1)
                    painter.drawPixmap(rect, QPixmap(path + "explode.png"));
                else
                    painter.drawPixmap(rect, QPixmap(QString(path + "%1.png").arg(GameArea[i][j])));
            }
        }

    }
}

void Mainwindow::mousePressEvent(QMouseEvent *event) {
    auto pos = event->position();
    int x = pos.x();
    int y = pos.y();

    // 点击点是否在游戏区域内
    if (x < MARGIN || y < MARGIN) return;

    int gridX = (x - MARGIN) / BLOCK_SIZE;  // 列索引
    int gridY = (y - MARGIN) / BLOCK_SIZE;  // 行索引

    if (gridX < 0 || gridX >= COLS || gridY < 0 || gridY >= ROWS) return;

    if (event->button() == Qt::LeftButton) {
        // 左键：打开格子
        Open(gridX, gridY);
        update();  // 重新绘制界面
    }
    else if (event->button() == Qt::RightButton) {
        // 右键：插旗逻辑（可选）
        if (isOpened[gridY][gridX] == 0 && FlagNum > 0)
            addFlag(gridX, gridY); // 插旗
        else if (isOpened[gridY][gridX] == 2)
            removeFlag(gridX, gridY); // 取消旗子
        update();
    }
}

void Mainwindow::InitGame() {
    random_mine(); // 在gamearea随机生成雷
    recognize(); // 生成数字
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            isOpened[i][j] = 0;
        }
    }
}

void Mainwindow::GameOver() {
    int ret = QMessageBox::question(
        this,
        "Game Over",
        "You clicked on a mine!\n\nDo you want to play again?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret == QMessageBox::Yes) {
        InitGame();  // 重新初始化地图（清空 + 随机布雷）
        update();    // 重新绘制
    } else {
        qApp->quit();  // ✅ 正确退出
    }
}
void Mainwindow::win() {
    int ret = QMessageBox::question(
        this,
        "You Win!",
        "WIN!\n\nDo you want to play again?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret == QMessageBox::Yes) {
        InitGame();  // 重新初始化地图（清空 + 随机布雷）
        update();    // 重新绘制
    } else {
         qApp->quit();  // ✅ 正确退出    // 退出游戏
    }
}



void Mainwindow::Open(int x, int y) {
    if (isOpened[y][x] == 0) {
        isOpened[y][x] = 1;
        if (GameArea[y][x] == -1) {
            GameOver();
        } else if (GameArea[y][x] == 0) {
            isOpened[y][x] = 0;
            surrounding(x, y);
        }
    }
}


void Mainwindow::addFlag(int x, int y) {
    if (GameArea[y][x] == -1) {
        MinesNum--;
    }
    if (MinesNum == 0) {
        win();
    }
    isOpened[y][x] = 2;
    FlagNum--;
}

void Mainwindow::removeFlag(int x, int y) {
    if (GameArea[y][x] == -1) {
        MinesNum++;
    }
    isOpened[y][x] = 0;
    FlagNum++;
}

int Mainwindow::remainMinesNum() {
    return MinesNum;
}

void Mainwindow::random_mine() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            GameArea[i][j] = 0;
        }
    }

    // 创建索引数组 0 ~ ROWS*COLS-1
    std::vector<int> indices(ROWS * COLS);
    std::iota(indices.begin(), indices.end(), 0);

    // 打乱顺序
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);

    // 前 30 个位置放 -1
    for (int k = 0; k < MinesNum; k++) {
        int pos = indices[k];
        int r = pos / COLS;
        int c = pos % COLS;
        GameArea[r][c] = -1;
    }


}

void Mainwindow::recognize() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (GameArea[i][j] == -1) {
                surround(j, i);
            }
        }
    }
}

void Mainwindow::surround(int x, int y) {
    for (int i = y-1; i <= y+1; i++) {
        for (int j = x-1; j <= x+1; j++) {
            if (i>=0 && i<ROWS && j>=0 && j<COLS) {
                if (GameArea[i][j] != -1) {
                    GameArea[i][j] += 1;
                }
            }
        }
    }
}

void Mainwindow::surrounding(int x, int y) {
    if (x < 0 || x >= COLS || y < 0 || y >= ROWS)
        return;
    if (isOpened[y][x] != 0)
        return;  // 已打开的不再递归

    // 打开当前格
    isOpened[y][x] = 1;

    // 若该格子不是0，只打开，不继续扩展
    if (GameArea[y][x] != 0)
        return;

    // 对周围八个格子递归
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS) {
                // 只有周围未打开的才继续递归
                if (isOpened[ny][nx] == 0) {
                    surrounding(nx, ny);
                }
            }
        }
    }
}

