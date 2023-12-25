#include "jpg2rgb.h"
#include "game.h"
#include "screen.h"
#include "novel.h"

extern int fd_lcd;
extern int * map;

void play_gobang_game(){
    /* 二维数组映射棋盘 */
    int chess[ROWS][COLS] = {0};

    int flag = 1;

    while (flag){
        /* 写入棋盘 */
        draw_board();
        /* 清空记录 */
        memset(chess, 0, sizeof(chess));

        int currentPlayer = 1; // 玩家1或2

        while(1){
            char *black = "/alan/assets/image/black.jpg";
            char *white = "/alan/assets/image/white.jpg";
            /* 轮到黑子还是白子说话 */
            char * chessColor = (currentPlayer == 1) ? black : white;

            /* 点击屏幕返回坐标 */
            int position_x = 0, position_y = 0;
            int chess_event = getChessInput(&position_x, &position_y);

            if(chess_event == UP_SLIDE){
                flag = 0;
                break;
            }if(chess_event == LEFT_SLIDE || chess_event == RIGHT_SLIDE){
                /* 悔棋 */
                
            } 

            printf("坐标：%d , %d\n", position_x, position_y);

            /* 根据坐标判断棋子位置 */
            int row = position_y / CELL_SIZE; // 计算所落子的行
            int col = position_x / CELL_SIZE; // 计算所落子的列

            /* 落子 */
            if (chess[row][col] == 0) {
                chess[row][col] = currentPlayer; // 在棋盘上落子

                // 在屏幕上显示落子
                int x = col * CELL_SIZE;
                int y = row * CELL_SIZE;
                showjpg(x, y, chessColor);

                // 判断胜利条件
                if (check_win(chess, row, col, currentPlayer)) {
                    // 处理胜利情况
                    if(currentPlayer == 1){
                        show_font_str("黑子胜！", 150, 70);
                        sleep(3);
                    }else{
                        show_font_str("白子胜！", 150, 70);
                        sleep(3);
                    }

                    break; 
                }

                // 切换下一个玩家
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        }
        
    }  
}

// 绘制棋盘
void draw_board() {

    // 循环绘制十字图片，填充整个棋盘
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            // 每个单元格位置
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            showjpg(x, y, "/alan/assets/image/chess.jpg");
        }
    }
}


/* 定义水平、垂直、对角线的方向 */
int dx[4] = {0, 1, 1,  1}; 
int dy[4] = {1, 1, 0, -1};

/*  检查是否有玩家获胜 */
int check_win(int chess[ROWS][COLS], int row, int col, int currentPlayer) {
    for (int i = 0; i < 4; ++i) {
        int count = 1; // 计数器，记录连续棋子数量

        // 沿着当前方向前进
        for (int k = 1; k < 5; ++k) {
            int r = row + k * dx[i];
            int c = col + k * dy[i];

            if (r >= 0 && r < ROWS && c >= 0 && c < COLS && chess[r][c] == currentPlayer) {
                count++;
            } else {
                break;
            }
        }

        // 再往反方向前进
        for (int k = 1; k < 5; ++k) {
            int r = row - k * dx[i];
            int c = col - k * dy[i];

            if (r >= 0 && r < ROWS && c >= 0 && c < COLS && chess[r][c] == currentPlayer) {
                count++;
            } else {
                break;
            }
        }

        // 如果存在连续五子，玩家获胜
        if (count >= 5) {
            return 1;
        }
    }

    return 0; // 无玩家获胜
}