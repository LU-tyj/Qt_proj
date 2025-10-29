//
// Created by 钭亦骏 on 25-10-27.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

class Mainwindow : public QMainWindow {
    Q_OBJECT
public:
    explicit Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();
    virtual void paintEvent(QPaintEvent *event); //场景刷新
    virtual void mousePressEvent(QMouseEvent *event); //键盘响应

    // gameset
    void InitGame(); // 初始化游戏，随机产生雷
    void GameOver(); // 结束游戏，询问要不要再来一局或退出
    void win(); // 游戏胜利
    int remainMinesNum(); // 返回还没被排的雷的数量
    void addFlag(int x, int y); // 添加旗帜并改变FlagNum
    void removeFlag(int x, int y); // 移除旗帜并改变FlagNum
    void Open(int x, int y); // 打开 (x, y) 处，以及周围所有0的

private:
    int FlagNum = 30;
    int MinesNum = 30;
    int GameArea[15][20]; // 存储周围雷的数量 -1 为雷
    int isOpened[15][20]; // 0 没打开 1 打开 2 插旗子
    void random_mine(); // 辅助函数，在开始时初始化雷
    void recognize(); // 辅助函数，在初始化时计算每个块周围雷的数量
    void surround(int x, int y); // 辅助函数，使雷旁边所有的非雷区块加一
    void surrounding(int x, int y); // 辅助函数，当一个open时把所有能open的都打开
};



#endif //MAINWINDOW_H
