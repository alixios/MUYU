#ifndef __GAME_H
#define __GAME_H

#define CELL_SIZE 40  /* 棋子像素 */

#define ROWS 12       /* 棋盘行列 */
#define COLS 20      

/* 棋 */
void play_gobang_game();

/* 画板 */
void draw_board();

/* 检查是否有玩家获胜 */
int check_win(int chess[ROWS][COLS], int row, int col, int currentPlayer);

#endif