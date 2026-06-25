#include<iostream>
#include <conio.h>
#include <fstream>
#include<windows.h>
#include <cstdio>
#include "pf_stockfish.h"
using namespace std;
enum COLOR { BLACK, WHITE };
bool BM[8][8] = { false };
char replayHistory[100][8][8];
int replayCount = 0;
int currentMove = -1;
struct Piece
{
    char sym;
    COLOR color;
    int* position;


    bool (*isLegalMove)(Piece* board[8][8], int sr, int sc, int dr, int dc);
};
struct Player
{
    COLOR color;
    char sym;
};
//=============prototypes=================
// =======================================
void box(int br, int bc, Piece* board[][8]);
void drawPiece(char s, COLOR col, int cr, int cc);
void print_box(int sr, int sc, int row, int col, Piece* board[8][8]);
bool isCheck(Piece* board[8][8], COLOR kingColor);
bool isSelfCheck(Piece* board[8][8], int sr, int sc, int dr, int dc, COLOR C);
bool isCheckmate(Piece* board[8][8], COLOR playerColor);
void gotoRowCol(int rpos, int cpos);
void print_boxx(int sr, int sc, int re, int ce)
{
    char sym = -37;

    for (int r = 0; r < re; r++)
    {
        for (int c = 0; c < ce; c++)
        {
            if (r == 0 or r == re - 1 or c == 0 or c == ce - 1)
            {
                gotoRowCol(sr + r, sc + c);
                cout << sym;
            }
        }
    }

}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}
void hideConsoleCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOutput, scrn);
}
void printline(int pr, int pc, int p1r, int p1c, char sym = -37)
{
    for (float a = 0; a <= 1; a += 0.01)

    {

        int rows = a * pr + (1 - a) * p1r;

        int colums = a * pc + (1 - a) * p1c;
        if (p1r == pr)
            rows = pr;
        if (p1c == pc)
            colums = pc;
        gotoRowCol(rows, colums);
        cout << sym;
    }
}
void pawn(int cr, int cc)
{
    printline(cr - 4, cc, cr - 4, cc);
    printline(cr - 3, cc - 1, cr - 3, cc + 1);

    printline(cr - 2, cc - 2, cr + 3, cc - 2);
    printline(cr - 2, cc - 1, cr + 3, cc - 1);
    printline(cr - 2, cc, cr + 3, cc);

    printline(cr - 2, cc + 2, cr + 3, cc + 2);
    printline(cr - 2, cc + 1, cr + 3, cc + 1);

    printline(cr + 3, cc - 3, cr + 3, cc + 3);
    printline(cr + 4, cc - 3, cr + 4, cc + 3);


}
void R(int cr, int cc)
{


    printline(cr - 3, cc - 4, cr - 3, cc - 4);
    printline(cr - 3, cc + 4, cr - 3, cc + 4);


    printline(cr - 3, cc - 1, cr - 3, cc + 1);
    printline(cr - 4, cc, cr - 4, cc);
    printline(cr - 2, cc - 3, cr + 2, cc - 3);
    printline(cr - 2, cc - 2, cr + 2, cc - 2);
    printline(cr - 2, cc - 1, cr + 2, cc - 1);
    printline(cr - 2, cc, cr + 2, cc);
    printline(cr - 2, cc + 3, cr + 2, cc + 3);
    printline(cr - 2, cc + 2, cr + 2, cc + 2);
    printline(cr - 2, cc + 1, cr + 2, cc + 1);


    printline(cr + 3, cc - 4, cr + 3, cc + 4);
    printline(cr + 4, cc - 5, cr + 4, cc + 5);

}
void Bishop(int cr, int cc)
{

    printline(cr - 4, cc, cr - 4, cc);
    printline(cr - 3, cc - 1, cr - 3, cc + 1);
    printline(cr - 2, cc - 2, cr - 2, cc + 2);
    printline(cr - 1, cc - 3, cr - 1, cc + 3);
    printline(cr, cc - 2, cr, cc + 2);



    printline(cr + 1, cc - 3, cr + 1, cc + 3);
    printline(cr + 2, cc - 3, cr + 2, cc + 3);
    printline(cr + 3, cc - 3, cr + 3, cc + 3);

}
void queen(int cr, int cc)
{


    printline(cr - 3, cc - 3, cr + 1, cc - 3);
    printline(cr - 4, cc - 2, cr + 1, cc - 2);

    printline(cr - 5, cc, cr + 1, cc);
    printline(cr - 5, cc + 1, cr + 1, cc + 1);
    printline(cr - 4, cc + 3, cr + 1, cc + 3);
    printline(cr - 3, cc + 4, cr + 1, cc + 4);

    printline(cr + 2, cc - 2, cr + 2, cc + 3);
    printline(cr + 3, cc - 2, cr + 3, cc + 3);


}
void king(int cr, int cc)
{

    printline(cr - 4, cc - 3, cr + 1, cc - 3);
    printline(cr - 3, cc - 2, cr + 1, cc - 2);
    printline(cr - 2, cc - 1, cr + 1, cc - 1);
    printline(cr - 1, cc, cr + 1, cc);
    printline(cr - 2, cc + 1, cr + 1, cc + 1);
    printline(cr - 3, cc + 2, cr + 1, cc + 2);
    printline(cr - 4, cc + 3, cr + 1, cc + 3);

    printline(cr + 2, cc - 2, cr + 2, cc + 2);
    printline(cr + 3, cc - 2, cr + 3, cc + 2);

}
void Knight(int cr, int cc)
{

    printline(cr - 3, cc, cr - 3, cc);

    printline(cr - 2, cc - 1, cr - 2, cc - 1);
    printline(cr - 2, cc + 1, cr - 2, cc + 1);

    printline(cr - 1, cc - 2, cr + 3, cc - 2);

    printline(cr - 1, cc + 2, cr + 2, cc + 2);

    printline(cr + 3, cc + 1, cr + 3, cc + 1);



    printline(cr + 4, cc - 2, cr + 4, cc + 2);


}
bool isHorizontalMove(int sr, int sc, int dr, int dc)
{
    if (sr == dr and sc != dc)
    {
        return true;
    }
    return false;
}
bool isVerticalMove(int sr, int sc, int dr, int dc)
{
    if (sc == dc and sr != dr)
    {
        return true;
    }
    return false;
}
bool isDiagonalMove(int sr, int sc, int dr, int dc)
{
    if (abs(sr - dr) == abs(sc - dc))
    {
        return true;
    }
    return false;
}
bool isHorizontalPathClear(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    int step;
    if (sc < dc)
    {
        step = 1;
    }
    else
    {
        step = -1;
    }

    int c = sc + step;
    while (c != dc)
    {
        if (board[sr][c] != nullptr && board[sr][c]->sym != '-')
        {
            return false;
        }
        c = c + step;
    }
    return true;
}
bool isVerticalPathClear(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    int step;
    if (sr < dr)
    {
        step = 1;
    }
    else
    {
        step = -1;
    }

    int r = sr + step;
    while (r != dr)
    {
        if (r < 0 || r >= 8) return false;
        if (board[r][sc] == nullptr) return false;

        if (board[r][sc]->sym != '-')
        {
            return false;
        }
        r = r + step;
    }
    return true;
}
bool isDiagonalPathClear(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    int rstep;
    int cstep;

    if (dr > sr)
    {
        rstep = 1;
    }
    else
    {
        rstep = -1;
    }

    if (dc > sc)
    {
        cstep = 1;
    }
    else
    {
        cstep = -1;
    }

    int r = sr + rstep;
    int c = sc + cstep;

    while (r != dr and c != dc)
    {

        if (board[r][c] == nullptr)
            return false;

        if (board[r][c]->sym != '-')
        {
            return false;
        }

        r = r + rstep;
        c = c + cstep;
    }
    return true;
}
bool isLegalPawn(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    int direction;

    if (board[sr][sc]->color == WHITE)
        direction = -1;
    else
        direction = 1;

    if (isVerticalMove(sr, sc, dr, dc))
    {

        if (dr == sr + direction && board[dr][dc]->sym == '-')
            return true;


        if (board[sr][sc]->color == WHITE && sr == 6)
        {
            if (dr == 4 &&
                board[5][sc]->sym == '-' &&
                board[4][sc]->sym == '-')
                return true;
        }

        if (board[sr][sc]->color == BLACK && sr == 1)
        {
            if (dr == 3 &&
                board[2][sc]->sym == '-' &&
                board[3][sc]->sym == '-')
                return true;
        }
    }

    if (abs(dc - sc) == 1 && dr == sr + direction)
    {
        if (board[dr][dc]->sym != '-' &&
            board[dr][dc]->color != board[sr][sc]->color)
            return true;
    }

    return false;
}
bool isLegalRook(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    if (isHorizontalMove(sr, sc, dr, dc))
    {
        if (isHorizontalPathClear(board, sr, sc, dr, dc))
            return true;
    }

    if (isVerticalMove(sr, sc, dr, dc))
    {
        if (isVerticalPathClear(board, sr, sc, dr, dc))
            return true;
    }

    return false;
}
bool isLegalKnight(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    int r = abs(sr - dr);
    int c = abs(sc - dc);

    if (r == 2 && c == 1)
    {
        return true;
    }
    if (r == 1 && c == 2)
    {
        return true;
    }

    return false;
}
bool isLegalBishop(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    if (isDiagonalMove(sr, sc, dr, dc))
    {
        if (isDiagonalPathClear(board, sr, sc, dr, dc))
        {
            return true;
        }
    }
    return false;
}
bool isLegalQueen(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    if (isHorizontalMove(sr, sc, dr, dc))
    {
        if (isHorizontalPathClear(board, sr, sc, dr, dc))
        {
            return true;
        }
    }

    if (isVerticalMove(sr, sc, dr, dc))
    {
        if (isVerticalPathClear(board, sr, sc, dr, dc))
            return true;
    }

    if (isDiagonalMove(sr, sc, dr, dc))
    {
        if (isDiagonalPathClear(board, sr, sc, dr, dc))
            return true;
    }

    return false;
}
bool isLegalKing(Piece* board[8][8], int sr, int sc, int dr, int dc)
{
    if (abs(sr - dr) <= 1 && abs(sc - dc) <= 1)
    {
        return true;
    }

    return false;
}
void init(Piece* board[8][8], Player PS, COLOR cl)
{
    int r, c;

    for (r = 0; r < 8; r++)
    {
        for (c = 0; c < 8; c++)
        {
            board[r][c] = new Piece;
            board[r][c]->sym = '-';
            board[r][c]->color = cl;

            board[r][c]->position = new int[2];
            board[r][c]->position[0] = r;
            board[r][c]->position[1] = c;

            board[r][c]->isLegalMove = 0;
        }
    }


    for (c = 0; c < 8; c++)
    {
        board[6][c]->sym = 'P';
        board[6][c]->color = WHITE;
        board[6][c]->isLegalMove = isLegalPawn;

        board[1][c]->sym = 'p';
        board[1][c]->color = BLACK;
        board[1][c]->isLegalMove = isLegalPawn;
    }

    char blackRow[] = { 'r','n','b','q','k','b','n','r' };
    for (c = 0; c < 8; c++)
    {
        board[0][c]->sym = blackRow[c];
        board[0][c]->color = BLACK;

        if (blackRow[c] == 'r')
            board[0][c]->isLegalMove = isLegalRook;
        else if (blackRow[c] == 'n')
            board[0][c]->isLegalMove = isLegalKnight;
        else if (blackRow[c] == 'b')
            board[0][c]->isLegalMove = isLegalBishop;
        else if (blackRow[c] == 'q')
            board[0][c]->isLegalMove = isLegalQueen;
        else if (blackRow[c] == 'k')
            board[0][c]->isLegalMove = isLegalKing;
    }


    char whiteRow[] = { 'R','N','B','Q','K','B','N','R' };
    for (c = 0; c < 8; c++)
    {
        board[7][c]->sym = whiteRow[c];
        board[7][c]->color = WHITE;

        if (whiteRow[c] == 'R')
            board[7][c]->isLegalMove = isLegalRook;
        else if (whiteRow[c] == 'N')
            board[7][c]->isLegalMove = isLegalKnight;
        else if (whiteRow[c] == 'B')
            board[7][c]->isLegalMove = isLegalBishop;
        else if (whiteRow[c] == 'Q')
            board[7][c]->isLegalMove = isLegalQueen;
        else if (whiteRow[c] == 'K')
            board[7][c]->isLegalMove = isLegalKing;
    }
}
bool validateSource(Piece* board[8][8], int sr, int sc, COLOR current)
{
    if (sr < 0)
    {
        return false;
    }
    if (sr > 7)
    {
        return false;
    }
    if (sc < 0)
    {
        return false;
    }
    if (sc > 7)
    {
        return false;
    }

    if (board[sr][sc]->sym == '-')
    {
        return false;
    }

    if (board[sr][sc]->color == current)
    {
        return true;
    }

    return false;
}
bool validateDestination(Piece* board[8][8], int dr, int dc, COLOR current)
{
    if (dr < 0)
    {
        return false;
    }
    if (dr > 7)
    {
        return false;
    }
    if (dc < 0)
    {
        return false;
    }
    if (dc > 7)
    {
        return false;
    }

    if (board[dr][dc]->sym == '-')
    {
        return true;
    }

    if (board[dr][dc]->color != current)
    {
        return true;
    }

    return false;
}
void redrawSquare(Piece* board[8][8], int r, int c)
{
    int sr = r * 14;
    int sc = c * 14;

    print_box(sr, sc, r, c, board);
}
void updateBoard(Piece* board[8][8], int sr, int sc, int dr, int dc)
{

    delete[] board[dr][dc]->position;
    delete board[dr][dc];

    Piece* p = board[sr][sc];


    board[sr][sc] = new Piece;
    board[sr][sc]->sym = '-';
    board[sr][sc]->color = WHITE;
    board[sr][sc]->isLegalMove = 0;

    board[sr][sc]->position = new int[2];
    board[sr][sc]->position[0] = sr;
    board[sr][sc]->position[1] = sc;

    p->position[0] = dr;
    p->position[1] = dc;

    board[dr][dc] = p;
}
void changeTurn(COLOR& current)
{
    if (current == WHITE)
    {
        current = BLACK;
    }
    else
        current = WHITE;
}
void clearLine(int r, int c)
{
    gotoRowCol(r, c);
    cout << "                                          ";
}
void waitForEnter(int r, int c)
{
    gotoRowCol(r, c);
    color(12);
    cout << "Press ENTER to continue";

    while (true)
    {
        if (_getch() == 13)
            break;
    }

    clearLine(r, c);
    color(7);
}
bool isCheck(Piece* board[8][8], COLOR kingColor)
{
    int kr = -1, kc = -1;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if ((board[r][c]->sym == 'K' || board[r][c]->sym == 'k') &&
                board[r][c]->color == kingColor)
            {
                kr = r;
                kc = c;
                break;
            }
        }
        if (kr != -1)
            break;
    }


    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (board[r][c]->sym != '-' && board[r][c]->color != kingColor && board[r][c]->isLegalMove != nullptr)
            {
                if (board[r][c]->isLegalMove(board, r, c, kr, kc))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
bool isSelfCheck(Piece* board[8][8], int sr, int sc, int dr, int dc, COLOR C)
{
    Piece* src = board[sr][sc];
    Piece* dst = board[dr][dc];

    board[dr][dc] = src;

    board[sr][sc] = new Piece;
    board[sr][sc]->sym = '-';
    board[sr][sc]->color = WHITE;
    board[sr][sc]->isLegalMove = nullptr;

    bool check = isCheck(board, C);


    delete board[sr][sc];
    board[sr][sc] = src;
    board[dr][dc] = dst;

    return check;
}
bool isCheckmate(Piece* board[8][8], COLOR playerColor)
{
    if (!isCheck(board, playerColor))
        return false;

    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            if (board[sr][sc]->sym != '-' &&
                board[sr][sc]->color == playerColor &&
                board[sr][sc]->isLegalMove != 0)
            {
                for (int dr = 0; dr < 8; dr++)
                {
                    for (int dc = 0; dc < 8; dc++)
                        if ((board[dr][dc]->sym == '-' ||
                            board[dr][dc]->color != playerColor) &&
                            board[sr][sc]->isLegalMove(board, sr, sc, dr, dc))
                        {
                            if (!isSelfCheck(board, sr, sc, dr, dc, playerColor))
                                return false;
                        }
                }
            }
        }
    }
    return true;
}
bool isStalemate(Piece* board[8][8], COLOR playerColor)
{
    if (isCheck(board, playerColor))
        return false;
    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            if (board[sr][sc]->sym != '-' &&
                board[sr][sc]->color == playerColor &&
                board[sr][sc]->isLegalMove != nullptr)
            {
                for (int dr = 0; dr < 8; dr++)
                {
                    for (int dc = 0; dc < 8; dc++)
                    {
                        if (board[dr][dc]->sym == '-' ||
                            board[dr][dc]->color != playerColor)
                        {
                            if (board[sr][sc]->isLegalMove(board, sr, sc, dr, dc))
                            {
                                if (!isSelfCheck(board, sr, sc, dr, dc, playerColor))
                                    return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}
void highlightBox(int sr, int sc)
{
    char sym = -37;

    color(9);

    for (int c = 0; c < 14; c++)
    {
        gotoRowCol(sr, sc + c);
        cout << sym;

        gotoRowCol(sr + 13, sc + c);
        cout << sym;
    }

    for (int r = 0; r < 14; r++)
    {
        gotoRowCol(sr + r, sc);
        cout << sym;

        gotoRowCol(sr + r, sc + 13);
        cout << sym;
    }

    color(7);
}
void highlightPossibleMoves(Piece* board[8][8], int sr, int sc)
{
    if (board[sr][sc]->isLegalMove == nullptr)
        return;


    COLOR playerColor = board[sr][sc]->color;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (board[r][c]->sym == '-' || board[r][c]->color != board[sr][sc]->color)
            {

                if (board[sr][sc]->isLegalMove(board, sr, sc, r, c))
                {
                    if (!isSelfCheck(board, sr, sc, r, c, playerColor))
                    {
                        BM[r][c] = true;
                        highlightBox(r * 14, c * 14);
                    }
                }

            }

        }
    }
}
void saveGame(Piece* board[8][8], COLOR currentTurn)
{
    ofstream Rdr("savegam.txt");
    if (!Rdr)
        return;

    Rdr << (currentTurn == WHITE ? 'W' : 'B') << endl;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Rdr << board[r][c]->sym << " ";
        }
        Rdr << endl;
    }

    Rdr.close();
}
void loadGame(Piece* board[8][8], COLOR& currentTurn)
{
    ifstream Rdr("savegam.txt");
    if (!Rdr)
        return;

    char turn;
    Rdr >> turn;
    currentTurn = (turn == 'W') ? WHITE : BLACK;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            char ch;
            Rdr >> ch;

            board[r][c]->sym = ch;

            if (ch == '-')
                continue;

            board[r][c]->color = (ch >= 'A' && ch <= 'Z') ? WHITE : BLACK;

            if (ch == 'P' || ch == 'p')
                board[r][c]->isLegalMove = isLegalPawn;
            else if (ch == 'R' || ch == 'r')
                board[r][c]->isLegalMove = isLegalRook;
            else if (ch == 'N' || ch == 'n')
                board[r][c]->isLegalMove = isLegalKnight;
            else if (ch == 'B' || ch == 'b')
                board[r][c]->isLegalMove = isLegalBishop;
            else if (ch == 'Q' || ch == 'q')
                board[r][c]->isLegalMove = isLegalQueen;
            else if (ch == 'K' || ch == 'k')
                board[r][c]->isLegalMove = isLegalKing;
        }
    }

    Rdr.close();
}
void unHighlight(Piece* board[8][8])
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (BM[r][c])
            {
                redrawSquare(board, r, c);
                BM[r][c] = false;
            }
        }
    }
}
void playerMove(Piece* board[8][8], Player& current, COLOR C)
{

    int sr, sc, dr, dc;
    int rpos, cpos;


    /* for (int i = 0; i < 40; i++)
     {
         color(9);
         print_boxx(25, 125, 10, i);
     }
     color(7);*/

    while (true)
    {

        gotoRowCol(10, 130);
        if (C == WHITE)
            cout << "WHITE turn ";
        else
            cout << "BLACK turn ";


        while (true)
        {
            /*  for (int i = 0; i < 30; i++)
              {
                  color(9);
                  print_boxx(12, 125, 10, i);
              }
              color(7);*/
            gotoRowCol(15, 130);
            cout << "Select source   ";

            getRowColbyLeftClick(rpos, cpos);

            sr = rpos / 14;
            sc = cpos / 14;

            if (validateSource(board, sr, sc, C))
            {
                unHighlight(board);
                highlightPossibleMoves(board, sr, sc);
                break;
            }

            gotoRowCol(18, 130);
            cout << "Invalid source";
            waitForEnter(20, 130);
            clearLine(18, 130);
        }


        gotoRowCol(19, 130);
        cout << "Select destination   ";

        getRowColbyLeftClick(rpos, cpos);

        dr = rpos / 14;
        dc = cpos / 14;

        if (!validateDestination(board, dr, dc, C))
        {
            gotoRowCol(21, 130);
            cout << "Invalid destination";
            waitForEnter(23, 130);
            clearLine(21, 130);
            continue;
        }


        if (board[sr][sc]->isLegalMove &&
            board[sr][sc]->isLegalMove(board, sr, sc, dr, dc))
        {
            if (isSelfCheck(board, sr, sc, dr, dc, C))
            {
                gotoRowCol(24, 130);
                cout << "Illegal: King in check";
                waitForEnter(26, 130);
                clearLine(24, 130);

                continue;
            }

            unHighlight(board);
            updateBoard(board, sr, sc, dr, dc);


            redrawSquare(board, sr, sc);
            redrawSquare(board, dr, dc);

            break;
        }
        else
        {
            gotoRowCol(22, 130);
            cout << "Illegal move";
            waitForEnter(24, 130);
            clearLine(22, 130);
        }
    }
}
void drawPiece(char s, COLOR col, int cr, int cc)
{
    if (s == '-') return;


    if (col == WHITE)
        color(7);
    else
        color(0);

    if (s == 'P' || s == 'p')
        pawn(cr, cc);
    else if (s == 'R' || s == 'r')
        R(cr, cc);
    else if (s == 'N' || s == 'n')
        Knight(cr, cc);
    else if (s == 'B' || s == 'b')
        Bishop(cr, cc);
    else if (s == 'Q' || s == 'q')
        queen(cr, cc);
    else if (s == 'K' || s == 'k')
        king(cr, cc);

    color(7);
}
void print_box(int sr, int sc, int row, int col, Piece* board[8][8])
{
    char sym = -37;
    int r, c;

    for (r = 0; r < 14; r++)
    {
        for (c = 0; c < 14; c++)
        {

            gotoRowCol(sr + r, sc + c);
            if ((row + col) % 2 == 0)
            {
                color(14);
            }
            else
                color(2);

            if (!(r == 0 || r == 13 || c == 0 || c == 13))
            {

                cout << sym;
            }
            color(12);
            if (r == 0 || r == 13 || c == 0 || c == 13)
            {
                cout << sym;
            }

            color(7);


        }
    }
    for (r = 0; r < 14; r++)
    {
        for (c = 0; c < 14; c++)
        {
            if (r == 7 && c == 7)
            {
                int cr = sr + 7;
                int cc = sc + 7;
                drawPiece(
                    board[row][col]->sym, board[row][col]->color,
                    cr, cc);
            }


        }
    }
    color(7);
}
void box(int br, int bc, Piece* board[][8])
{
    int r, c;
    for (r = 0; r < 8; r++)
    {
        for (c = 0; c < 8; c++)
        {
            print_box(r * br, c * bc, r, c, board);
        }
    }
}
void saveReplay(Piece* board[8][8]) {



    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board[r][c] != nullptr)
                replayHistory[replayCount][r][c] = board[r][c]->sym;
            else
                replayHistory[replayCount][r][c] = ' ';
        }
    }

    replayCount++;
}
void printReplayBoard(int moveIndex) {
    Piece* tempBoard[8][8];


    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            tempBoard[r][c] = new Piece;
            tempBoard[r][c]->sym = replayHistory[moveIndex][r][c];

            if (tempBoard[r][c]->sym == '-')
                tempBoard[r][c]->color = WHITE;
            else if (tempBoard[r][c]->sym >= 'A' && tempBoard[r][c]->sym <= 'Z')
                tempBoard[r][c]->color = WHITE;
            else
                tempBoard[r][c]->color = BLACK;


            tempBoard[r][c]->isLegalMove = nullptr;
        }
    }

    system("cls");
    box(14, 14, tempBoard);

    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            delete tempBoard[r][c];
}
void saveState(Piece* board[8][8])
{
    currentMove++;
    replayCount = currentMove + 1;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            replayHistory[currentMove][r][c] = board[r][c]->sym;
}
void loadState(Piece* board[8][8], int index)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            char ch = replayHistory[index][r][c];
            board[r][c]->sym = ch;

            if (ch == '-')
                continue;

            board[r][c]->color =
                (ch >= 'A' && ch <= 'Z') ? WHITE : BLACK;

            if (ch == 'P' || ch == 'p')
                board[r][c]->isLegalMove = isLegalPawn;
            else if (ch == 'R' || ch == 'r')
                board[r][c]->isLegalMove = isLegalRook;
            else if (ch == 'N' || ch == 'n')
                board[r][c]->isLegalMove = isLegalKnight;
            else if (ch == 'B' || ch == 'b')
                board[r][c]->isLegalMove = isLegalBishop;
            else if (ch == 'Q' || ch == 'q')
                board[r][c]->isLegalMove = isLegalQueen;
            else if (ch == 'K' || ch == 'k')
                board[r][c]->isLegalMove = isLegalKing;
        }
    }
}
void replayGame() {
    for (int i = 0; i < replayCount; i++) {
        printReplayBoard(i);
        Sleep(1000);
    }
}
void undo(Piece* board[8][8], COLOR& C)
{
    if (currentMove <= 0)
        return;

    currentMove--;
    loadState(board, currentMove);
    changeTurn(C);
}
void redo(Piece* board[8][8], COLOR& C)
{
    if (currentMove >= replayCount - 1)
        return;

    currentMove++;
    loadState(board, currentMove);
    changeTurn(C);
}
int piecePoints(char s)
{
    if (s == 'p' || s == 'P') return 1;
    if (s == 'n' || s == 'N') return 3;
    if (s == 'b' || s == 'B') return 3;
    if (s == 'r' || s == 'R') return 5;
    if (s == 'q' || s == 'Q') return 9;
    if (s == 'k' || s == 'K') return 100;
    return 0;
}
void computerMoveSmart(Piece* board[8][8])
{
    int bestGain = -999;
    int bestSR = -1, bestSC = -1, bestDR = -1, bestDC = -1;

    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            if (board[sr][sc]->sym == '-' ||
                board[sr][sc]->color != BLACK ||
                board[sr][sc]->isLegalMove == nullptr)
                continue;

            for (int dr = 0; dr < 8; dr++)
            {
                for (int dc = 0; dc < 8; dc++)
                {
                    if ((board[dr][dc]->sym == '-' ||
                        board[dr][dc]->color == WHITE) &&
                        board[sr][sc]->isLegalMove(board, sr, sc, dr, dc) &&
                        !isSelfCheck(board, sr, sc, dr, dc, BLACK))
                    {

                        Piece* src = board[sr][sc];
                        Piece* dst = board[dr][dc];

                        board[dr][dc] = src;
                        board[sr][sc] = new Piece;
                        board[sr][sc]->sym = '-';
                        board[sr][sc]->isLegalMove = nullptr;

                        int oldR = src->position[0];
                        int oldC = src->position[1];
                        src->position[0] = dr;
                        src->position[1] = dc;


                        if (isCheckmate(board, WHITE))
                        {
                            updateBoard(board, sr, sc, dr, dc);
                            redrawSquare(board, sr, sc);
                            redrawSquare(board, dr, dc);
                            return;
                        }


                        src->position[0] = oldR;
                        src->position[1] = oldC;
                        delete board[sr][sc];
                        board[sr][sc] = src;
                        board[dr][dc] = dst;


                        int gain = piecePoints(board[dr][dc]->sym) - piecePoints(board[sr][sc]->sym);
                        if (gain > bestGain)
                        {
                            bestGain = gain;
                            bestSR = sr; bestSC = sc;
                            bestDR = dr; bestDC = dc;
                        }
                    }
                }
            }
        }
    }

    if (bestSR != -1)
    {
        updateBoard(board, bestSR, bestSC, bestDR, bestDC);
        redrawSquare(board, bestSR, bestSC);
        redrawSquare(board, bestDR, bestDC);
    }
}





const char* boardToconvert(Piece* board[8][8], COLOR turn)
{
    char* convert = new char[128]; 
    int idx = 0;

    for (int r = 0; r < 8; r++)
    {
        int empty = 0;
        for (int c = 0; c < 8; c++)
        {
            char s = board[r][c]->sym;
            if (s == '-')
            {
                empty++;
            }
            else
            {
                if (empty > 0)
                {
                    convert[idx++] = '0' + empty;
                    empty = 0;
                }
                convert[idx++] = s;
            }
        }
        if (empty > 0)
            convert[idx++] = '0' + empty;

        if (r != 7)
            convert[idx++] = '/';
    }

    convert[idx++] = ' ';
    if (turn == WHITE)
    {
        convert[idx++] = 'w';
    }
    else
    {
        convert[idx++] = 'b';
    }


    convert[idx++] = ' ';
    convert[idx++] = 'K';
    convert[idx++] = 'Q';
    convert[idx++] = 'k';
    convert[idx++] = 'q';

    convert[idx++] = ' ';
    convert[idx++] = '-';

    convert[idx++] = ' ';
    convert[idx++] = '0';

    convert[idx++] = ' ';
    convert[idx++] = '1';

    convert[idx] = '\0';
    return  convert;
}




int main1()
{
    hideConsoleCursor();
    int i = 0;
    Piece* board[8][8];

    Player white, black, current;
    COLOR C = WHITE;
    white.color = WHITE;
    white.sym = 'W';

    black.color = BLACK;
    black.sym = 'B';

    current = white;

    system("cls");
    gotoRowCol(20, 50);
    cout << "1. New Game";
    gotoRowCol(22, 50);
    cout << "2. Previous Game";
    gotoRowCol(24, 50);
    cout << "Press 1 or 2: ";

    char choice = _getch();

    if (choice == '2')
    {
        init(board, white, WHITE);
        COLOR savedTurn = WHITE;
        loadGame(board, savedTurn);
        current = (savedTurn == WHITE) ? white : black;
        C = savedTurn;
    }
    else
    {
        init(board, white, WHITE);
        current = white;
        C = WHITE;
    }

    system("cls");
    box(14, 14, board);

    do
    {
        hideConsoleCursor();
        COLOR opponent = (C == WHITE) ? BLACK : WHITE;

        if (C == WHITE)
        {
            playerMove(board, current, C);
        }
        else
        {
            Sleep(500);
            computerMoveSmart(board);
        }

        saveState(board);
        saveReplay(board);
        saveGame(board, C);

        if (isCheckmate(board, opponent))
        {
            system("cls");
            box(14, 14, board);
            gotoRowCol(40, 120);
            cout << "CHECKMATE! ";
            if (C == WHITE)
                cout << "WHITE WINS!";
            else
                cout << "BLACK WINS!";
            _getch();
            break;
        }

        if (isStalemate(board, opponent))
        {
            system("cls");
            box(14, 14, board);
            gotoRowCol(50, 120);
            cout << "STALEMATE! DRAW GAME";
            break;
        }


        if (isCheck(board, opponent))
        {
            gotoRowCol(60, 120);
            cout << "CHECK!";
        }
        else
        {
            clearLine(60, 120);
        }

        i++;


        if (i % 10 == 0)
        {
            gotoRowCol(70, 140);
            cout << "Do you want to replay the game ? (y/n): ";
            char ch = _getch();
            if (ch == 'y' || ch == 'Y')
            {
                replayGame();
            }
            clearLine(70, 140);
        }

        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'u' || ch == 'U')
            {
                undo(board, C);
                system("cls");
                box(14, 14, board);
            }
            else if (ch == 'r' || ch == 'R')
            {
                redo(board, C);
                system("cls");
                box(14, 14, board);
            }
        }

        changeTurn(C);
    } while (true);

    return 0;
}

int main2()
{
    hideConsoleCursor();
    if (!stockfish_init())
    {
        cout << "Stockfish failed to start!";
        return 0;
    }

    int i = 0;
    Piece* board[8][8];

    Player white, black, current;
    COLOR C = WHITE;
    white.color = WHITE;
    white.sym = 'W';

    black.color = BLACK;
    black.sym = 'B';

    current = white;

    system("cls");
    gotoRowCol(20, 50);
    cout << "1. New Game";
    gotoRowCol(22, 50);
    cout << "2. Previous Game";
    gotoRowCol(24, 50);
    cout << "Press 1 or 2: ";

    char choice = _getch();

    if (choice == '2')
    {
        init(board, white, WHITE);
        COLOR savedTurn = WHITE;
        loadGame(board, savedTurn);
        current = (savedTurn == WHITE) ? white : black;
        C = savedTurn;
    }
    else
    {
        init(board, white, WHITE);
        current = white;
        C = WHITE;
    }

    system("cls");
    box(14, 14, board);

    do
    {
        hideConsoleCursor();
        COLOR opponent = (C == WHITE) ? BLACK : WHITE;

        if (C == WHITE)
        {
            playerMove(board, current, C);
        }
        else
        {
            Sleep(500);
            const char* convert = boardToFEN(board, C);
            int sr, sc, dr, dc;
            char promo;

            char move[32];
            if (stockfish_bestmove(convert, 200, move))
            {
                bestmove_parser(move, sr, sc, dr, dc, promo);
            }
            updateBoard(board, sr, sc, dr, dc);
            redrawSquare(board, sr, sc);
            redrawSquare(board, dr, dc);
        }
        changeTurn(C);
        saveState(board);
        saveReplay(board);
        saveGame(board, C);

        if (isCheckmate(board, opponent))
        {
            system("cls");
            box(14, 14, board);
            gotoRowCol(40, 120);
            cout << "CHECKMATE! ";
            if (C == WHITE)
                cout << "BLACK WINS!";
            else
                cout << "WHITE WINS!";
            _getch();
            break;
        }

        if (isStalemate(board, opponent))
        {
            system("cls");
            box(14, 14, board);
            gotoRowCol(50, 120);
            cout << "STALEMATE! DRAW GAME";
            break;
        }


        if (isCheck(board, opponent))
        {
            gotoRowCol(60, 120);
            cout << "CHECK!";
        }
        else
        {
            clearLine(60, 120);
        }

        i++;


        if (i % 10 == 0)
        {
            gotoRowCol(70, 140);
            cout << "Do you want to replay the game ? (y/n): ";
            char ch = _getch();
            if (ch == 'y' || ch == 'Y')
            {
                replayGame();
            }
            clearLine(70, 140);
        }

        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'u' || ch == 'U')
            {
                undo(board, C);
                system("cls");
                box(14, 14, board);
            }
            else if (ch == 'r' || ch == 'R')
            {
                redo(board, C);
                system("cls");
                box(14, 14, board);
            }
        }
    } while (true);

    return 0;
}



void menu()
{
    cout << "----------Menu----------";
    cout << "\n\n1. human vs  my computer ";
    cout << "\n2. human vs smart computer\n";
    cout << "0. Quit\n";
}



int main()
{
    int choice;
    while (true)
    {
        menu();
        cout << "Which Problem :";
        cin >> choice;
        cout << "\n";
        switch (choice)
        {
        case 1:
            main1();
            break;
        case 2:
            main2();
            break;
        case 0:
            cout << "Thank You\n";
            return 0;
        default:
            cout << "Wrong Input\n"; break;
        }
        cout << endl;
        system("pause");
        system("cls");
    }
}