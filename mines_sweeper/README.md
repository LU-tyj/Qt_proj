# 概要
做一个基础的 20 * 15 50雷的扫雷游戏，

本来以为会需要做一个别的专门的类，比如说雷等。
但后面发现使用isOpen和GameArea即可。

- `int isOpened[15][20];`  0 没打开 1 打开 2 插旗子
- `int GameArea[15][20];`  存储周围雷的数量 -1 为雷

程序最难点应该就是一个普通的递归，但你点击blank处时要自动打开别的地方的blank。

还有就是ui方面，使用mousePressEvent，并识别position

最后就是搞清楚ROW COL 对应 y轴 x轴
