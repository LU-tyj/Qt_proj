//
// Created by 钭亦骏 on 25-10-26.
//

#include "Widget.h"
#include <time.h>
#include <QMessageBox>
#include <QPainter>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <__filesystem/file_type.h>

int item1[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右L
int item2[4][4]=
{
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
//左L
int item3[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右S
int item4[4][4]=
{
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};
//左S
int item5[4][4]=
{
    {0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};
//山形
int item6[4][4]=
{
    {0,0,0,0},
    {0,0,1,0},
    {0,1,1,1},
    {0,0,0,0}
};
//长条
int item7[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
};

inline void block_cpy(int dblock[4][4],int sblock[4][4])
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            dblock[i][j]=sblock[i][j];
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(
        AREA_COL*BLOCK_SIZE+MARGIN*4+4*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE+MARGIN*2
    );

    // 初始化游戏逻辑
    InitGame();
}

Widget::~Widget()
{
    delete this;
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    //画游戏场景边框
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
    //画方块预告
    painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_block[i][j]==1)
                painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+j*BLOCK_SIZE,MARGIN+i*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    //绘制得分
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(QRect(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN*2+4*BLOCK_SIZE,BLOCK_SIZE*4,BLOCK_SIZE*4),Qt::AlignCenter,"score: "+QString::number(score));


    //绘制下落方块和稳定方块,注意方块边线的颜色是根据setPen来的，默认黑色
    // 思路是识别每个块的元素，涂上对应的颜色
    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
        {
            //绘制活动方块
            if(game_area[i][j]==1)
            {
                painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
            //绘制稳定方块
            else if(game_area[i][j]==2)
            {
                painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
        }
}

void Widget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == game_timer) {
        BlockMove(DOWN);
    }

    if (event->timerId() == paint_timer) {
        update();
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            BlockMove(UP);
            break;
        case Qt::Key_Down:
            BlockMove(DOWN);
            break;
        case Qt::Key_Left:
            BlockMove(LEFT);
            break;
        case Qt::Key_Right:
            BlockMove(RIGHT);
            break;
        case Qt::Key_Space:
            BlockMove(SPACE);
            break;
        default:
            break;
    }
}

void Widget::InitGame() {
    // 先把所有方块的值初始化为0
    for (int i = 0; i < AREA_ROW; i++) {
        for (int j = 0; j < AREA_COL; j++) {
            game_area[i][j]=0;
        }
    }

    // 定义两个刷新时间
    speed_ms = 800;
    refresh_ms = 30;

    // 初始化随机数种子，用来后续方块的随机生成
    srand(time(0));

    // 分数清零
    score = 0;

    // 开始游戏
    StartGame();
}

void Widget::StartGame() {
    // 开始计时
    game_timer = startTimer(speed_ms);
    paint_timer = startTimer(refresh_ms);

    // 产生初始的下一个方块
    int block_id = rand() % 7;
    CreatBlock(next_block, block_id);

    // 准备新方块
    ResetBlock();
}

void Widget::GameOver() {
    //游戏结束停止计时器
    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this,"failed","game over"); // 弹出提示框
}

/* 既要在game_area生成当前方块，又要准备下一个方块 */
void Widget::ResetBlock() {
    // 产生当前方块
    block_cpy(cur_block, next_block);
    GetBorder(cur_block, cur_border); // 计算此时的碰撞体积

    // 准备下一个方块
    int block_id = rand() % 7;
    CreatBlock(next_block, block_id);

    //设置初始方块坐标,以方块左上角为锚点
    block_point start_point;
    start_point.pos_x=AREA_COL/2-2;
    start_point.pos_y=0;
    block_pos = start_point;
}

void Widget::BlockRotate(int block[4][4]) {
    int temp_block[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp_block[3-j][i]=block[i][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            block[i][j]=temp_block[i][j];
}

// 朴实无华地把这些都copy下来
void Widget::CreatBlock(int block[4][4], int block_id) {
    switch (block_id)
    {
        case 0:
            block_cpy(block,item1);
            break;
        case 1:
            block_cpy(block,item2);
            break;
        case 2:
            block_cpy(block,item3);
            break;
        case 3:
            block_cpy(block,item4);
            break;
        case 4:
            block_cpy(block,item5);
            break;
        case 5:
            block_cpy(block,item6);
            break;
        case 6:
            block_cpy(block,item7);
            break;
        default:
            break;
    }
}

void Widget::GetBorder(int block[4][4],Border &border) {
    //计算上下左右边界
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.dbound=i;
                break; //直到计算到最后一行有1
            }
    for(int i=3;i>=0;i--)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.ubound=i;
                break;
            }
    for(int j=0;j<4;j++)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.rbound=j;
                break;
            }
    for(int j=3;j>=0;j--)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.lbound=j;
                break;
            }
}

/* 将运动的方块转换成静止的 */
void Widget::ConvertStable(int x, int y) {
    for (int i=cur_border.ubound;i<=cur_border.dbound;i++) {
        for (int j=cur_border.lbound;j<=cur_border.rbound;j++) {
            if (cur_block[i][j] == 1) {
                game_area[y + i][x + j] = 2;
            }
        }
    }
}

/* 判断输入dir命令后是否会碰撞 */
bool Widget::IsCollide(int x, int y, Direction dir) {
    int temp_block[4][4];
    block_cpy(temp_block, cur_block);
    Border temp_border;
    GetBorder(temp_block, temp_border);

    switch(dir) {
        case UP:
            BlockRotate(temp_block);
            GetBorder(temp_block, temp_border);
            break;
        case DOWN:
            y += 1;
            break;
        case LEFT:
            x -= 1;
            break;
        case RIGHT:
            x += 1;
            break;
        default:
            break;
    }

    // 更严格的边界检查
    for(int i = temp_border.ubound; i <= temp_border.dbound; i++) {
        for(int j = temp_border.lbound; j <= temp_border.rbound; j++) {
            if(temp_block[i][j] == 1) {
                // 检查是否超出左右边界
                if(x + j < 0 || x + j >= AREA_COL)
                    return true;
                // 检查是否超出下边界
                if(y + i >= AREA_ROW)
                    return true;
                // 检查是否与稳定方块碰撞
                if(y + i >= 0 && game_area[y + i][x + j] == 2)
                    return true;
            }
        }
    }
    return false;
}

void Widget::BlockMove(Direction dir) {
    bool block_stabilized = false; // 标记方块是否刚刚稳定

    switch (dir) {
        case UP:
            // 先检查旋转后是否会碰撞
            if(IsCollide(block_pos.pos_x,block_pos.pos_y,UP))
                break;

            // 清除当前方块
            for(int i=0;i<4;i++) {
                for(int j=0;j<4;j++) {
                    if(cur_block[i][j] == 1 &&
                       block_pos.pos_y+i < AREA_ROW &&
                       block_pos.pos_x+j < AREA_COL) {
                        game_area[block_pos.pos_y+i][block_pos.pos_x+j] = 0;
                       }
                }
            }

            // 执行旋转
            BlockRotate(cur_block);
            GetBorder(cur_block, cur_border); // 重新计算边界

            // 绘制旋转后的方块，但不覆盖稳定方块
            for(int i=0;i<4;i++) {
                for(int j=0;j<4;j++) {
                    if(cur_block[i][j] == 1 &&
                       block_pos.pos_y+i < AREA_ROW &&
                       block_pos.pos_x+j < AREA_COL &&
                       game_area[block_pos.pos_y+i][block_pos.pos_x+j] != 2) { // 不覆盖稳定方块
                        game_area[block_pos.pos_y+i][block_pos.pos_x+j] = 1;
                       }
                }
            }
            break;
    case DOWN:
        if(block_pos.pos_y+cur_border.dbound==AREA_ROW-1)
        {
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            block_stabilized = true; // 标记方块已稳定
            break;
        }
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            block_stabilized = true; // 标记方块已稳定
            break;
        }
        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            game_area[block_pos.pos_y][block_pos.pos_x+j]=0;
        block_pos.pos_y+=1;
        for(int i=0;i<4;i++)
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case LEFT:
        if(block_pos.pos_x+cur_border.lbound==0||IsCollide(block_pos.pos_x,block_pos.pos_y,LEFT))
            break;
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            game_area[block_pos.pos_y+i][block_pos.pos_x+3]=0;
        block_pos.pos_x-=1;
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.pos_x+j>=0&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case RIGHT:
        if(block_pos.pos_x+cur_border.rbound==AREA_COL-1||IsCollide(block_pos.pos_x,block_pos.pos_y,RIGHT))
            break;
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            game_area[block_pos.pos_y+i][block_pos.pos_x]=0;
        block_pos.pos_x+=1;
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.pos_x+j<=AREA_COL-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case SPACE:
        while(block_pos.pos_y+cur_border.dbound<AREA_ROW-1&&!IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                game_area[block_pos.pos_y][block_pos.pos_x+j]=0;
            block_pos.pos_y+=1;
            for(int i=0;i<4;i++)
                for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                    if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                        game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        }
        ConvertStable(block_pos.pos_x,block_pos.pos_y);
        ResetBlock();
        block_stabilized = true; // 标记方块已稳定
        break;
    default:
        break;
    }

    // 只在方块稳定后进行消行检查
    if (block_stabilized) {
        //处理消行，整个场景上面的行依次下移
        int i=AREA_ROW-1;
        int line_count=0; //记消行数

        while(i>=1)
        {
            bool is_line_full=true;
            for(int j=0;j<AREA_COL;j++) {
                // 只检查稳定方块(值为2)，忽略活动方块(值为1)
                if(game_area[i][j] != 2) {
                    is_line_full=false;
                    i--;
                    break;
                }
            }
            if(is_line_full)
            {
                // 将上面的所有行下移
                for(int k=i;k>=1;k--) {
                    for(int j=0;j<AREA_COL;j++) {
                        game_area[k][j] = game_area[k-1][j];
                    }
                }
                // 最顶行清零
                for(int j=0;j<AREA_COL;j++) {
                    game_area[0][j] = 0;
                }
                line_count++;
                // 注意：这里不减少i，因为我们需要再次检查当前行（新移下来的行）
            } else {
                i--; // 只有没消行时才移动到上一行
            }
        }

        score += line_count * 10; //得分

        //判断游戏是否结束
        for(int j=0;j<AREA_COL;j++) {
            if(game_area[0][j]==2) {
                GameOver();
                break;
            }
        }
    }
}