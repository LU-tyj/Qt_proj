# 概要
做一个基础的 20 * 15 50雷的扫雷游戏，

本来以为会需要做一个别的专门的类，比如说雷等。
但后面发现使用isOpen和GameArea即可。

- `int isOpened[15][20];`  0 没打开 1 打开 2 插旗子
- `int GameArea[15][20];`  存储周围雷的数量 -1 为雷

程序最难点应该就是一个普通的递归，但你点击blank处时要自动打开别的地方的blank。

还有就是ui方面，使用mousePressEvent，并识别position

最后就是搞清楚ROW COL 对应 y轴 x轴

其中 `Mainwindow.cpp` 中的 `path` 需要自己修改。 

扫雷，现在就做些小游戏。

## 参考资料

[github src](https://github.com/xiaoxi666/mines_sweeper)

## Qt 知识

### .ui 以及 ui\_mainwindow.h

参考：[stack overflow](https://stackoverflow.com/questions/24855666/qt-ui-namespace)

为什么用Qt creator 创建项目时会在mainwindow.h中自动生成：

```
namespace Ui {
class MainWindow;
}
```

以及为什么会出现 `#include "ui_mainwindow.h`

这两个都是因为Qt在编译时会把.ui文件编译成`ui_mainwindow.h`，其中里面大致的内容为：

```
namespace Ui {
class MainWindow {
public:
    QWidget *centralWidget;
    QPushButton *pushButton;

    void setupUi(QMainWindow *MainWindow) {
        // 初始化布局、控件等
    }
};
}
```

然后就可以在`mainwindow.cpp`中直接用 `ui->setupUi(this);`

所以这就可以把.ui文件和 `mainwindow` 联系起来。