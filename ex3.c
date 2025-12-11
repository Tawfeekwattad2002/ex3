/* Name: Tawfek watted
   ID: 211438973
   Assignment: ex3*/

#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char board[][COLS], int col, int rows, int cols);

int isBoardFull(char board[][COLS], int rows, int cols);

int isInBounds(int r, int c, int rows, int cols);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char board[][COLS], int col, int rows, int cols);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char board[][COLS], int col, int rows, int cols, char token);

int checkVictory(char board[][COLS], int rows, int cols, int lastRow, int lastCol, char token);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char board[][COLS], int cols, int rows);

/* Computer*/
int computerChoose(char board[][COLS], int cols, int rows, char myToken, char oppToken);

void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type);

void initBoard(char board[][COLS], int rows, int cols);


void printBoard(char board[][COLS], int rows, int cols);

int getPlayerType(int playerNumber);

int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

int isColumnFull(char board[][COLS], int col, int rows, int cols){
    if (rows<=0 || cols<=0) {
        return 1;
    }
    if (col<0 || col>=cols) {
        return 1;
    }
    return board[0][col]!= EMPTY;
}

int isBoardFull(char board[][COLS], int rows, int cols){
    for (int c = 0; c < cols; c++){
        if(!isColumnFull(board, c, rows, cols)){
        return 0;
        }
     }
  return 1;
}

int isInBounds(int r, int c, int rows, int cols){
     return r>=0 && r<rows && c>=0 && c<cols;
}

int getFreeRow(char board[][COLS], int col, int rows, int cols){
    if (col<0 || col>=cols) {
        return -1;
    }
    for (int r= rows-1 ; r >= 0 ; r--){
         if(board[r][col] == EMPTY){
               return r;
           }
      }
   return -1;
}

int makeMove(char board[][COLS], int col, int rows, int cols, char token){
     if(col<0 || col>=cols || isColumnFull(board, col, rows, cols)){
     return -1;}

     int row= getFreeRow(board, col, rows, cols);
     board[row][col]=token;
     return row;
}

int checkVictory(char board[][COLS], int rows, int cols, int lastRow, int lastCol, char token){
     int directions[4][2]={{0,1},{1,0},{1,1},{1,-1}};
     for(int d=0;d<4;d++){
      int dr=directions[d][0];
      int dc=directions[d][1];
      int count=1;
      for(int i=1;i<CONNECT_N;i++){
            int r=lastRow-dr*i;
            int c=lastCol-dc*i;
              if(isInBounds(r, c, rows, cols) && board[r][c]==token){
       count++;
       }
       else break;
}
         for(int i=1;i<CONNECT_N;i++){
             int r=lastRow+dr*i;
             int c=lastCol+dc*i;
             if(isInBounds(r, c, rows, cols) && board[r][c]==token){
                 count++;
             }
             else break;
         }
       if(count>=CONNECT_N){
        return 1;
         }
   }
     return 0;
}

int humanChoose(char board[][COLS], int cols, int rows){
    int col;
    while(1){
            printf("Enter column (1-%d): ",cols);
            int n = scanf("%d",&col);
       if(n!=1){
            printf("Invalid input. Enter a number.\n");
            while(getchar()!='\n');
            continue;
          }
     col--;
       if(col<0 || col>=cols){
        printf ("Invalid column. Choose between 1 and %d.\n",cols);
        continue;
           }
       if(isColumnFull(board, col, rows, cols)){
        printf("Column %d is full. Choose another column.\n",col+1);
        continue;
           }
       return col;
   }
}
int computerChoose(char board[][COLS],int cols,int rows,char myToken,char oppToken){
    int directions[4][2]={{0,1},{1,0},{1,1}, {1,-1}};
    // building order
    int order[COLS];
    for(int c=0;c<cols;c++){
        order[c]=c;
    }
int center2=cols+1;  // we use doubled distances: dist2 = |2*(col+1) - (cols+1)|
    for(int i=0;i<cols-1;i++){
        int best=i;
        for (int j=i+1;j<cols;j++){
            int c1=order[best];
            int c2=order[j];

            int d1=2*(c1+1)-center2;
            if(d1<0)d1=-d1;
            int d2=2*(c2+1)-center2;
            if(d2<0)d2=-d2;

            // prefering smaller distance; if tie, smaller index (left)
            if(d2<d1 || (d2==d1 && c2<c1)){
                best=j;
            }
        }
        if(best!=i){
            int tmp=order[i];
            order[i]=order[best];
            order[best]=tmp;
        }
    }
    //1) Winning move for myToken
    for(int idx=0;idx<cols;idx++) {
        int col=order[idx];
        if(isColumnFull(board,col,rows,cols)) {
            continue;
        }

        char tempBoard[ROWS][COLS];
        for(int r=0;r<rows;r++) {
            for(int cc=0;cc<cols;cc++) {
                tempBoard[r][cc]=board[r][cc];
            }
        }

        int row=makeMove(tempBoard, col, rows, cols, myToken);
        if(row!=-1 && checkVictory(tempBoard,rows,cols,row,col,myToken)){
            return col;
        }
    }

    //2) Block opponent's winning move
    for(int idx=0;idx<cols;idx++) {
        int col=order[idx];
        if(isColumnFull(board,col,rows,cols)) {
            continue;
        }

        char tempBoard[ROWS][COLS];
        for(int r=0; r<rows;r++){
            for(int cc=0;cc<cols;cc++){
                tempBoard[r][cc]=board[r][cc];
            }
        }

        int row=makeMove(tempBoard,col,rows,cols,oppToken);
        if(row!=-1 && checkVictory(tempBoard,rows,cols,row,col,oppToken)){
            return col;
        }
    }

    //3) Create a sequence of three for myToken
    for(int idx=0;idx<cols;idx++){
        int col=order[idx];
        if (isColumnFull(board,col,rows,cols)){
            continue;
        }

        char tempBoard[ROWS][COLS];
        for(int r=0;r<rows;r++){
            for(int cc=0;cc<cols;cc++){
                tempBoard[r][cc]=board[r][cc];
            }
        }

        int row=makeMove(tempBoard,col,rows,cols,myToken);
        if(row==-1){
            continue;
        }

        int makesThree = 0;
        for(int d=0;d<4 && !makesThree;d++){
            int dr=directions[d][0];
            int dc=directions[d][1];
            int count=1;

            // negative direction
            for (int i=1;i<CONNECT_N;i++){
                int rr=row-dr*i;
                int cc2=col-dc*i;
                if (isInBounds(rr,cc2,rows,cols) && tempBoard[rr][cc2]==myToken){
                    count++;
                } else{
                    break;
                }
            }
            // positive direction
            for(int i=1;i<CONNECT_N;i++){
                int rr=row+dr*i;
                int cc2=col+dc*i;
                if (isInBounds(rr,cc2,rows,cols) && tempBoard[rr][cc2]==myToken){
                    count++;
                } else{
                    break;
                }
            }

            if(count==3){   // exactly 3 in a row
                makesThree=1;
            }
        }

        if(makesThree){
            return col;   // first column in teacher order that creates 3
        }
    }

    //4) Block opponent's sequence of three
    for(int idx=0;idx<cols;idx++){
        int col=order[idx];
        if(isColumnFull(board,col,rows,cols)){
            continue;
        }

        char tempBoard[ROWS][COLS];
        for(int r=0;r<rows;r++){
            for(int cc=0;cc<cols;cc++){
                tempBoard[r][cc]=board[r][cc];
            }
        }

        int row=makeMove(tempBoard,col,rows,cols,oppToken);
        if (row==-1) {
            continue;
        }

        int blocksThree=0;
        for(int d=0;d<4 && !blocksThree;d++){
            int dr=directions[d][0];
            int dc=directions[d][1];
            int count=1;

            // negative direction
            for(int i=1;i<CONNECT_N;i++){
                int rr=row-dr*i;
                int cc2=col-dc*i;
                if(isInBounds(rr,cc2,rows,cols) && tempBoard[rr][cc2]==oppToken){
                    count++;
                } else{
                    break;
                }
            }
            // positive direction
            for(int i=1; i<CONNECT_N;i++){
                int rr=row+dr*i;
                int cc2=col+dc*i;
                if(isInBounds(rr,cc2,rows,cols) && tempBoard[rr][cc2]==oppToken){
                    count++;
                } else{
                    break;
                }
            }

            if(count==3){
                blocksThree=1;
            }
        }

        if(blocksThree){
            return col;
        }
    }

    //5) Arbitrary ordering rule: first legal column in order[]
    for(int idx=0;idx<cols;idx++){
        int col = order[idx];
        if(!isColumnFull(board,col,rows,cols)) {
            return col;
        }
    }

    // Board should be full if we reach here
    return 0;
}

void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type){
    char tokens[2]={TOKEN_P1,TOKEN_P2};
    int types[2]={p1Type,p2Type};
    int currentPlayer=0;

    while(1){
        int playerNum=currentPlayer+1;
        char token=tokens[currentPlayer];

        printf("Player %d (%c) turn.\n",playerNum,token);

        int col;
        if (types[currentPlayer]==HUMAN) {
            col=humanChoose(board,cols,rows);
        } else{
            col=computerChoose(board,cols,rows,tokens[currentPlayer],tokens[1-currentPlayer]);
            printf("Computer chose column %d\n",col+1);
        }

        int row=makeMove(board,col,rows,cols,token);
        printBoard(board,rows,cols);

        if (checkVictory(board,rows,cols,row,col,token)) {
            printf("Player %d (%c) wins!\n",playerNum,token);
            break;
        }

        if (isBoardFull(board,rows,cols)) {
            printf("Board full and no winner. It's a tie!\n");
            break;
        }

        currentPlayer=1-currentPlayer;
    }
}

void initBoard(char board[][COLS],int rows,int cols){
    for (int r=0;r<rows;r++)
        for (int c=0;c<cols;c++)
            board[r][c]=EMPTY;
}


void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}
