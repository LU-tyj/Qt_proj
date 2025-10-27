# Proj1: Tetris
## 参考资源

*   [csdn](https://blog.csdn.net/u012234115/article/details/45966479)
*   [github src](https://github.com/tashaxing/QtTetris)
*   stack overflow等

## 主要内容

用qt写一个Tetris

其中涉及绘图、键鼠捕捉，以及时间帧数相关的内容

## CPP 知识补漏

### explicit

[stack overflow](https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean)

禁止编译器发生显性表达

```
explicit Widget(QWidget *parent = 0);
```

### virtual

使后面的继承函数可以进行覆盖和修改这些方法（多态）

虚函数

```
virtual void paintEvent(QPaintEvent *event); //场景刷新
```

### inline

[内联函数](https://www.geeksforgeeks.org/cpp/inline-functions-cpp/)，常写在头文件中

允许编译器在调用点直接展开函数体，而不是生成真正的函数调用指令

## Qt 相关知识

### Q\_OBJECT宏定义

参考[stack overflow](https://stackoverflow.com/questions/1368584/what-does-the-q-object-macro-do-why-do-all-qt-objects-need-this-macro)

这样就可以直接 `connect` 而不是 `QObject::connect`

所以基本每个Qt的Widget或window class 中会自带一个`Q_OBJECT`

```
class Widget : public QWidget{
	Q_OBJECT
public:
	...
} 
```

### Time

本项目要控制方块随时间自动下降，也就是每隔一段时间触发 `BlockMove(DOWN)` 

参考资料：[csdn](https://blog.csdn.net/lusanshui/article/details/97638520)，[cnblogs](https://www.cnblogs.com/zhoug2020/p/6423584.html)

Qt中管理时间的模块如下：

*   timerEvent
    
    这个是属于QObject类中的，与定时器相关的成员函数有：startTimer()、timeEvent()、killTimer()。

> [!NOTE]
> ```
> int QObject::startTimer(int interval);
> ```
> 
> 开始一个定时器并返回定时器ID，如果不能开始一个定时器，将返回0。定时器开始后，每隔interval毫秒间隔将触发一次超时事件，直到killTimer()被调用来删除定时器。如果interval为0，那么定时器事件每次发生时没有窗口系统事件处理。
> 
> 这个时候就可以使用 `event→timerId()` 来获取计时器ID，和上面定义的计时器ID相比较，这样就可以在对应间隔时间干相应的事。

> [!NOTE]
> ```
> virtual void QObject::timerEvent(QTimerEvent *event);
> ```
> 
> 虚函数 `timerEvent()` 被重载来实现用户的超时事件处理函数。如果有多个定时器在运行，`QTimerEvent::timerId()` 被用来查找指定定时器，对其进行操作。
> 
> 当定时器事件发生时，虚函数`timerEvent()`随着QTimerEvent事件参数类一起被调用，重载这个函数可以获得定时器事件。

```
// 在.h文件中定义过两个timer
void Widget::timerEvent(QTimerEvent *event)
{
    //方块下落
    if(event->timerId()==game_timer) // 如果Id与方块下落计时器相同时
        BlockMove(DOWN);
    //刷新画面
    if(event->timerId()==paint_timer) // 如果Id与画面刷新计时器相同时
    
        update();
}
```

*   QTimer
    
    直接上代码：
    
    ```
    // 每隔10s进行一次文件压缩的函数
    // .h中声明QTimer对象和槽函数
    public slots:
    	void onZipTodayFile();
    private:
    	QTimer* filezipTimer;
    	
    // .cpp中实例化
    filezipTimer = new QTimer();
    filezipTimer->setInterval(10000); // 10s
    connect(fileziptimer, timeout(), this, onZipTodayFile());
    filezipTimer->start();
    ```
    
    如果只出发一次可以使用 `singleShot()` :
    
    ```
    QTimer::singleShot(10000, this, SLOT(onZipTodayFile());
    ```
    
    ### QPainter

`update()` : 可以自动重新绘图，也就是重新触发paintEvent的内容（所以用virtual来方便重写）

首先要声明`QPainter painter(this);`

Qt里的大部分都这样，声明一个修改Widget的画笔

看一段代码：

```
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //画游戏场景边框
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
    //画方块预告
    painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_block[i][j]==1)
                painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+j*BLOCK_SIZE,
                	MARGIN+i*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
}
```

> [!TIP]
> 1.  声明对Widget的画笔
> 2.  设置画笔颜色和样式
> 3.  drewRect画矩形
>     
>     ```
>     painter.drawRect(x, y, size_x, size_y);
>     ```

### Keypress event

顾名思义，直接看代码

```
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
```

## BUGs

在写的时候遇到很多bug，很多交给deepseek解决了

### 只有白色，没有画面

因为创建了一个新的widget覆盖在原widget上。在Qt creater中可以直接调整ui，而Clion时只能用cmake，所以就：

```
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
```

ui时直接：

```
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //调整窗口尺寸布局
    resize(AREA_COL*BLOCK_SIZE+MARGIN*4+4*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE+MARGIN*2);
    //初始化游戏
    InitGame();
}
```

### 初始化时第一个方块生成错误

ResetBlock函数出错，把映射到next\_block上的映射到cur\_block上了

```
void Widget::ResetBlock() {
    // 产生当前方块
    block_cpy(cur_block, next_block);
    GetBorder(cur_block, cur_border); // 计算此时的碰撞体积

    // 准备下一个方块
    int block_id = rand() % 7;
    CreatBlock(next_block, block_id); // 这里写成了cur_block

    //设置初始方块坐标,以方块左上角为锚点
    block_point start_point;
    start_point.pos_x=AREA_COL/2-2;
    start_point.pos_y=0;
    block_pos = start_point;
}
```

### moveblock

在moveblock中出了很多错，包括：

*   cur\_block会把存在的方块覆盖
*   cur\_block还没固定就直接消除了
*   cur\_block消除后自身不会消失
*   cur\_block旋转会把周围的方块覆盖

解决方法：

*   添加bool变量 `block_stablized`，只有为true之后再统一进行消行。解决了前三个问题
*   将 `moveblock(UP)` 旋转时的直接映射投影过程变成先消除原本的 `cur_block`，然后再把改变后的复制到 `game_area` 中，这样避免了对其他方块的改变。