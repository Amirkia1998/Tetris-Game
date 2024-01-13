#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 16  // one extra row for the last row to act as an obstacle
#define SIZE_SHAPES 3


//##################################################################################### PROTOTYPES
int getRowSize();
int getColSize();
void initializeBoard(int board[MAX][MAX], int m, int n);
void displayBoard(int board[MAX][MAX], int m, int n);
void displayShape(int currentShape[SIZE_SHAPES][SIZE_SHAPES]);
void createShape(int currentShape[SIZE_SHAPES][SIZE_SHAPES]);
void rotateShape(int currentShape[SIZE_SHAPES][SIZE_SHAPES]);
int findFirstCollision(const int board[MAX][MAX], const int currentShape[SIZE_SHAPES][SIZE_SHAPES], int m, int inputCol);
int moveShape(int board[MAX][MAX], const int currentShape[SIZE_SHAPES][SIZE_SHAPES], int startingRow, int inputCol, int n);
int updateScore(int board[MAX][MAX], int m, int n);
void clearFilledRow(int board[MAX][MAX], int filledRow, int n);
int gameOver(const int board[MAX][MAX], int n);

//######################################################################################## MAIN
int main(void) {

//--------------------------------------------------- VARIABLES
    int selection=0, startOver = 1, score = 0, highestScore = 0;
    int board[MAX][MAX], m, n, needNewShape = 1;
    int currentShape[SIZE_SHAPES][SIZE_SHAPES] = {0};

//--------------------------------------------------------- INITIALIZE BOARD
    m = getRowSize();
    n = getColSize();
    initializeBoard(board, m, n);

//--------------------------------------------------------------- MENU
    do {
        if (needNewShape) {
            createShape(currentShape);
        }
        displayBoard(board, m, n);
        printf("Incoming Shape:\n");
        displayShape(currentShape);
        printf("0.Exit\t1.Play\t2.Rotate Shape\t(Your Score: %d)", score);
        printf("\n");

        scanf("%d", &selection);

//-------------------------------------------------------------- PLAY
        if (selection == 1) {
            int inputCol, startingRow, moveStatus;

            printf("Enter the column number: ");
            scanf("%d", &inputCol);

            // find the suitable position for shape
            startingRow = findFirstCollision(board, currentShape, m, inputCol);

            // check if the game is finished or not
            if (startingRow < 0) {
                moveStatus = -1;    // -1 means game is finished
            }
            else {                  // else move the incoming shape
                moveStatus = moveShape(board, currentShape, startingRow, inputCol, n);
            }

//---------------------------------------------------------------------------------- CHECK MOVE STATUS
            // game over
            if (moveStatus == -1 || gameOver(board, n)) {
                // update the new highest record
                if (score > highestScore) {
                    highestScore = score;
                }
                printf("\n----------------------------------------------------------\n");
                printf("Game Over! Your score: %d (Highest Record: %d)", score, highestScore);
                printf("\n----------------------------------------------------------\n");
                displayBoard(board, m, n);

                printf("\n0.Exit\t 1.New Game\n");
                scanf("%d", &startOver);

                // if players wants a new game
                if(startOver) {
                    printf("\n----------------------------------------------------------\n");
                    printf("New Game");
                    printf("\n----------------------------------------------------------\n");
                    m = getRowSize();
                    n = getColSize();
                    initializeBoard(board, m, n);
                    needNewShape = 1;
                    score = 0;
                }

            }
            else if (moveStatus == 0) {
                printf("\n----------------------------------------------------------\n");
                printf("!!! ERROR: shape does not fit in the board !!!");
                printf("\n----------------------------------------------------------\n");
                needNewShape = 0;
            }
            else if (moveStatus == 1) {
                score += updateScore(board, m, n);
                printf("\n----------------------------------------------------------\n");
                printf("!!! Successful move !!!");
                printf("\n----------------------------------------------------------\n");
                needNewShape = 1;
            }

        }
//-------------------------------------------------------------------- ROTATE
        else if (selection == 2) {
            rotateShape(currentShape);
            needNewShape = 0;
        }

    } while (selection != 0 && startOver == 1);


    return 0;
}


//######################################################################################## FUNCTIONS
int getRowSize() {
    int m;

    do {
        printf("Enter the number of rows (should not exceed %d): \n", MAX-1);
        scanf("%d", &m);
    } while (m > MAX);

    return m;
}

int getColSize() {
    int n;
    do {
        printf("Enter the number of columns (should not exceed %d): \n", MAX-1);
        scanf("%d", &n);
    } while ( n > MAX);

    return n;
}

void initializeBoard(int board[MAX][MAX], int m, int n) {

//--------------------------------------- ASSIGN 0
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = 0;
        }
    }

//  Assign 1 to the bottom row to act as an obstacle
//  bottom index is (last playable row + 1)
    for (int j = 0; j < n; ++j) {
        board[m][j] = 1;
    }

}


void displayBoard(int board[MAX][MAX], int m, int n) {

    for (int i = 0; i < m; ++i) {
        printf("%d\t| ", i);
        for (int j = 0; j < n; ++j) {
            printf("%d ", board[i][j]);
        }
        printf("|\n");
    }

    printf("\t");
    printf("  ");
    for (int i = 0; i < n; ++i) {
        printf("| ");
    }
    printf("\n");
    printf("\t");
    printf("  ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", i%10);
    }
    printf("\n");
}


void createShape(int currentShape[SIZE_SHAPES][SIZE_SHAPES]) {

//-------------------------------------------------------- SAMPLE SHAPES
    int shape0[SIZE_SHAPES][SIZE_SHAPES] = {
        {1,1,1},
        {0,0,0},
        {0,0,0}
    };

    int shape1[SIZE_SHAPES][SIZE_SHAPES] = {
        {1,1,0},
        {1,1,0},
        {0,0,0}
    };

    int shape2[SIZE_SHAPES][SIZE_SHAPES] = {
        {1,0,0},
        {1,1,0},
        {0,0,0}
    };

    int shape3[SIZE_SHAPES][SIZE_SHAPES] = {
        {1,0,0},
        {0,0,0},
        {0,0,0}
    };
    int shape4[SIZE_SHAPES][SIZE_SHAPES] = {
        {1,1,0},
        {0,1,1},
        {0,0,0}
    };
    int shape5[SIZE_SHAPES][SIZE_SHAPES] = {
        {1,1,0},
        {0,0,0},
        {0,0,0}
    };


//----------------------------------------------------- SELECT RANDOM SHAPE
    srand(time(NULL));
    int randomNum = rand() % 6;

    if (randomNum == 0) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                currentShape[i][j] = shape0[i][j];
            }
        }
    }
    else if (randomNum == 1) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                currentShape[i][j] = shape1[i][j];
            }
        }
    }
    else if (randomNum == 2) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                currentShape[i][j] = shape2[i][j];
            }
        }
    }
    else if (randomNum == 3) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                currentShape[i][j] = shape3[i][j];
            }
        }
    }
    else if (randomNum == 4) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                currentShape[i][j] = shape4[i][j];
            }
        }
    }
    else if (randomNum == 5) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                currentShape[i][j] = shape5[i][j];
            }
        }
    }

}


void displayShape(int currentShape[SIZE_SHAPES][SIZE_SHAPES]) {

    for (int i = 0; i < SIZE_SHAPES; ++i) {
        for (int j = 0; j < SIZE_SHAPES; ++j) {
            printf("%d ", currentShape[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void rotateShape(int currentShape[SIZE_SHAPES][SIZE_SHAPES]) {
    int temp[SIZE_SHAPES][SIZE_SHAPES];

    // Copy the original shape to a temporary matrix
    for (int i = 0; i < SIZE_SHAPES; ++i) {
        for (int j = 0; j < SIZE_SHAPES; ++j) {
            temp[i][j] = currentShape[i][j];
        }
    }

    // Rotate the shape by copying values from the temporary matrix to the rotated position
    for (int i = 0; i < SIZE_SHAPES; ++i) {
        for (int j = 0; j < SIZE_SHAPES; ++j) {
            currentShape[i][j] = temp[SIZE_SHAPES - 1 - j][i];
        }
    }
}


int findFirstCollision(const int board[MAX][MAX], const int currentShape[SIZE_SHAPES][SIZE_SHAPES], int m, int inputCol) {

    int obstacleFound = 0, startingRow = 0;

    // decrease the shape until it hits an obstacle
    while (!obstacleFound) {
        for (int i = 0; i < SIZE_SHAPES; ++i) {
            for (int j = 0; j < SIZE_SHAPES; ++j) {
                if (currentShape[i][j] == 1 && board[startingRow + i][inputCol + j] == 1) {
                    obstacleFound = 1;
                }
            }
        }

        if(!obstacleFound)
            startingRow++;
    }

    return --startingRow;
}


int moveShape(int board[MAX][MAX], const int currentShape[SIZE_SHAPES][SIZE_SHAPES], int startingRow, int inputCol, int n) {

    // check if the 1's (the shape) fits in the board
    for (int i = 0; i <SIZE_SHAPES; ++i) {
        for (int j = 0; j < SIZE_SHAPES; ++j) {
            if (currentShape[i][j] == 1 && inputCol + j >= n) {
                    return 0;
            }
        }
    }

    // move and place the shape in the board
    for (int i = 0; i <SIZE_SHAPES; ++i) {
        for (int j = 0; j < SIZE_SHAPES; ++j) {
            if (currentShape[i][j] == 1) {
                board[startingRow + i][inputCol + j] = 1;
            }
        }
    }

    return 1;
}


int updateScore(int board[MAX][MAX], int m, int n) {
    int existsFilledRow = 1;
    int score = 0;

    // finds the filled rows and adds to score
    while (existsFilledRow) {
        existsFilledRow = 0;

        for (int i = m-1; i >= 0; --i) {
            int counter = 0;

            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 1)
                    counter++;
            }

            if (counter == n) {
                existsFilledRow = 1;
                clearFilledRow(board, i, n);
                score = score + n;
            }

        }
    }

    return score;
}


void clearFilledRow(int board[MAX][MAX], int filledRow, int n) {
    // Move all rows above the filledRow down by one position
    for (int i = filledRow; i > 0; --i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = board[i - 1][j];
        }
    }

    // Set the top row to empty (0)
    for (int j = 0; j < n; ++j) {
        board[0][j] = 0;
    }

    printf("\n----------------------------------------------------------\n");
    printf("Row %d cleared. You earned +%d points", filledRow, n);
    printf("\n----------------------------------------------------------"
           "");
}


int gameOver(const int board[MAX][MAX], int n) {
    for (int j = 0; j < n; ++j) {
        // if the topmost row is reached, the game is over
        if (board[0][j] == 1)
            return 1;
    }

    return 0;
}


