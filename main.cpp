#include <iostream>
#include <windows.h>
#include "tetrominos.h"
#include <string>


const int Scores[4]{
        40,
        100,
        300,
        1200
};

bool playfield[20][10];
int score = 0;
int lines = 0;
bool currentPiece[4][4];
int currentPieceX = 5;
int currentPieceY = -3;
int speed = 20;
//change pos to struct later
//COORD currentPiecePos = {5, -3};
auto nextPiece = I;

void setNextPiece() {
    srand(time(NULL));

    // Copy the values of nextPiece to currentPiece
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentPiece[i][j] = nextPiece[i][j];
        }
    }

    // Select the next piece
    switch (rand() % 7) {
    case 0:
        nextPiece = I;
        break;
    case 1:
        nextPiece = O;
        break;
    case 2:
        nextPiece = T;
        break;
    case 3:
        nextPiece = S;
        break;
    case 4:
        nextPiece = Z;
        break;
    case 5:
        nextPiece = J;
        break;
    case 6:
        nextPiece = L;
        break;
    }
}

bool checkCollision() {
    for (int i = 3; i >= 0; i--) {
        for (int j = 3; j >= 0; j--) {
            if (currentPiece[i][j] &&
                (playfield[currentPieceY + i + 1][currentPieceX + j] || currentPieceY + i + 1 > 19) &&
                !currentPiece[i+1][j]) {
                return true;
            }
        }
    }
    return false;
}

void setPieceFields(bool b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentPiece[i][j] == 1) {
                playfield[currentPieceY + i][currentPieceX + j] = b;
            }
        }
    }
}

// TODO change speed function

void checkLineClear() {
    int cleared = 0;
    int clearedLines[20];
    int clearedIndex = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (playfield[i][j] == 0) break;
            if (j == 9) {
                cleared++;
                clearedLines[clearedIndex++] = i;
            }
        }
    }
    if (cleared < 1) {
        return;
    }
    int tempScore = score;
    int tempLines = lines;
    score += Scores[cleared - 1];
    if (score - tempScore > 1200) score = tempScore;
    lines += cleared;
    if (lines - tempLines > 4) lines = tempLines;
    for (int c = 0; c < cleared; c++) {
        for (int j = clearedLines[c]; j > 0; j--) {
            for (int k = 0; k < 10; k++) {
                playfield[j][k] = playfield[j - 1][k];
            }
        }
    }
}




void drawLine(int line) {

    printf("<!");
    for (int i = 0; i < 10; i++) {
        printf("%s", playfield[line][i] ? "[]" : " .");
    }
    printf("!>\n");
}


void drawScreen() {
    system("cls");
    printf("Lines cleared: %d", lines);
    for (int i = 0; i < 5 - std::to_string(lines).length(); i++) {
        printf(" ");
    }
    drawLine(0);

    printf("Score: %d", score);
    for (int i = 0; i < 13 - std::to_string(score).length(); i++) {
        printf(" ");
    }

    drawLine(1);

    printf("Next:               ");
    drawLine(2);
    printf("__________          ");
    drawLine(3);
    for (int i = 0; i < 4; i++) {
        printf("|");
        for (int j = 0; j < 4; ++j) {
            nextPiece[i][j] == 1 ? printf("[]") : printf("  ");
        }
        printf("|          ");
        drawLine(i + 4);
    }
    printf("|________|          ");
    drawLine(8);
    for (int i = 9; i < 20; i++) {
        printf("                    ");
        drawLine(i);
    }
    printf("                    <!********************!>\n");
    printf("                      \\/\\/\\/\\/\\/\\/\\/\\/\\/\\/  \n");
}


bool checkPieceInBounds() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentPiece[i][j] == 1 && (currentPieceX + j < 0 || currentPieceX + j > 9 || currentPieceY + i > 19)) {
                return false;
            }
        }
    }
    return true;
}

void rotate() {
    bool temp[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = currentPiece[i][j];
        }
    }

    for (int x = 0; x < 2; x++) {
        for (int y = x; y < 3 - x; y++) {
            if (!checkPieceInBounds() || checkCollision()) {
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        currentPiece[i][j] = temp[i][j];
                    }
                }
                return;
            }
            bool tempB = currentPiece[x][y];
            currentPiece[x][y] = currentPiece[y][3 - x];
            currentPiece[y][3 - x] = currentPiece[3 - x][3 - y];
            currentPiece[3 - x][3 - y] = currentPiece[3 - y][x];
            currentPiece[3 - y][x] = tempB;
        }
    }
}


void move() {
    if (GetAsyncKeyState(VK_LEFT)) {
        for (int i = 3; i >= 0; i--) {
            for (int j = 3; j >= 0; j--) {
                if (currentPiece[i][j] &&
                    (playfield[currentPieceY + i][currentPieceX + j - 1] && currentPieceX + j - 1 > 0) &&
                    !currentPiece[i][j - 1]) {
                    return;
                }
            }
        }
        currentPieceX--;
        if (!checkPieceInBounds() || checkCollision()) {
            currentPieceX++;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT)) {
        for (int i = 3; i >= 0; i--) {
            for (int j = 3; j >= 0; j--) {
                if (currentPiece[i][j] &&
                    (playfield[currentPieceY + i][currentPieceX + j + 1] && currentPieceX + j + 1 < 9) &&
                    !currentPiece[i][j + 1]) {
                    return;
                }
            }
        }
        currentPieceX++;
        if (!checkPieceInBounds() || checkCollision()) currentPieceX--;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        if (currentPieceY != 19)
            currentPieceY++;
    }
    if (GetAsyncKeyState(VK_UP)) {
        rotate();
    }

}

void drawPiece(HANDLE consoleHandle) {
    for (int i = 0; i < 4; i++) {
        if (currentPieceY + i < 0)continue;
        SetConsoleCursorPosition(consoleHandle, COORD{ static_cast<SHORT>(20),static_cast<SHORT>(i + currentPieceY) });
        drawLine(i + currentPieceY);
    }
}

void gameLoop(HANDLE consoleHandle) {
    drawScreen();
    Sleep(20);
    while (true) {
        for (int i = 0; i < speed - 10; ++i) {

            if (i == 0 && !checkCollision())currentPieceY++;
            move();
            setPieceFields(true);
            if (checkCollision()) {
                checkLineClear();
                setNextPiece();
                currentPieceX = 5;
                // MINUS 2 IS MAX OTHERWISE IT CRASHES
                currentPieceY = -2;
                drawScreen();
                if (checkCollision()) {
                    // Game over condition
                    printf("Game Over! Your score is: %d\n", score);
                    return;
                }
            }
            drawPiece(consoleHandle);
            Sleep(60);
            setPieceFields(false);
            drawPiece(consoleHandle);
        }
    }

}

int main() {
    // get window hanle and buffer info
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFOEX csbiex;

    // change window size
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &csbiex);
    csbiex.dwSize = COORD{ 48, 22 };
    csbiex.srWindow = SMALL_RECT{ 0, 0, 48, 22 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &csbiex.srWindow);

    COORD new_size;
    new_size.X = csbiex.srWindow.Right - csbiex.srWindow.Left + 1;
    new_size.Y = csbiex.srWindow.Bottom - csbiex.srWindow.Top + 1;
    SetConsoleScreenBufferSize(consoleHandle, new_size);

    // hide cursor
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);


    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            playfield[i][j] = false;
        }
    }
    setNextPiece();
    gameLoop(consoleHandle);


    std::cin.get();
    return 0;
}
