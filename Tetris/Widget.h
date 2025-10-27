//
// Created by 钭亦骏 on 25-10-26.
//

#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>

const int BLOCK_SIZE=25; //单个方块单元的边长
const int MARGIN=5; //场景边距
const int AREA_ROW=20; //场景行数
const int AREA_COL=12; //场景列数

//方向
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};

//定义边界信息
struct Border
{
    int ubound;
    int dbound;
    int lbound;
    int rbound;
};

//坐标
struct block_point
{
    int pos_x;
    int pos_y;
    //    block_point(int x,int y):pos_x(x),pos_y(y){}
};

namespace Ui {
    class Widget;
}

//拷贝方块图案


class Widget : public QWidget {
    Q_OBJECT // 宏定义
public:
    void InitGame();
    void StartGame();
    void GameOver();

    void ResetBlock(); // 重制方块
    void BlockMove(Direction dir); // 移动方块
    void BlockRotate(int block[4][4]); // 旋转方块
    void CreatBlock(int block[4][4], int block_id); // 创造方块
    void GetBorder(int block[4][4],Border &border); //计算边界
    void ConvertStable(int x, int y); // 稳定方块
    bool IsCollide(int x, int y, Direction dir); // 是否碰撞

    // explicit 防止发生编译器隐式转换
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    // virtual
    virtual void paintEvent(QPaintEvent *event); //场景刷新
    virtual void timerEvent(QTimerEvent *event); //定时器事件
    virtual void keyPressEvent(QKeyEvent *event); //键盘响应
private:
    Ui::Widget *ui = nullptr;

    int game_area[AREA_ROW][AREA_COL]; //场景区域，1表示活动的方块，2表示稳定的方块，0表示空
    block_point block_pos; // 方块位置
    int cur_block[4][4]; //当前方块形状
    Border cur_border; //当前方块边界
    int next_block[4][4]; //下一个方块形状
    bool isStable; //当前方块是否稳定了
    int score;  //游戏分数
    int game_timer; //方块下落计时器
    int paint_timer; //渲染刷新计时器
    int speed_ms; //下落时间间隔
    int refresh_ms; //刷新时间间隔
};



#endif //WIDGET_H
