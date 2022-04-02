// AoC 2021 - day 04
// =================

// date:  2021-12-04
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <thread>

#include "../flcTimer.h"

// ========== data structure needed for this puzzle   <<<<< ========== adapt to match columns of input file

// models the numbers that are called during bingo
typedef int sCallNr;
typedef std::vector<sCallNr> NumberStream;

struct sBoardElt {
    int nNumber = -1;
    bool bMarked = false;
};

struct sBoard {
    sBoardElt board[5][5];    // addressing: [y][x]
};

typedef std::vector<sBoard> BoardStream;

// ========== data input functions

void AssembleBoard( sBoard &b, int e00, int e01, int e02, int e03, int e04,
                               int e10, int e11, int e12, int e13, int e14,
                               int e20, int e21, int e22, int e23, int e24,
                               int e30, int e31, int e32, int e33, int e34,
                               int e40, int e41, int e42, int e43, int e44 ) {

    b = { e00, false, e01, false, e02, false, e03, false, e04, false,
          e10, false, e11, false, e12, false, e13, false, e14, false,
          e20, false, e21, false, e22, false, e23, false, e24, false,
          e30, false, e31, false, e32, false, e33, false, e34, false,
          e40, false, e41, false, e42, false, e43, false, e44, false
    };
}

void MakeExampleBoards( sBoard &board1, sBoard &board2, sBoard &board3 ) {
    AssembleBoard( board1,
        22, 13, 17, 11,  0,
         8,  2, 23,  4, 24,
        21,  9, 14, 16,  7,
         6, 10,  3, 18,  5,
         1, 12, 20, 15, 19
    );
    AssembleBoard( board2,
         3, 15,  0,  2, 22,
         9, 18, 13, 17,  5,
        19,  8,  7, 25, 23,
        20, 11, 10, 24,  4,
        14, 21, 16, 12,  6
    );
    AssembleBoard( board3,
        14, 21, 17, 24,  4,
        10, 16, 15,  9, 19,
        18,  8, 23, 26, 20,
        22, 11, 13,  6,  5,
         2,  0, 12,  3,  7
    );
}

// populate the example data in this function for testing your algo
void MakeExampleData( NumberStream &vData, BoardStream &vBoards ) {
    vData.clear();
                                             // <<<<< ========== adapt to match columns of input file
    vData = { 7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1 };
    vBoards.clear();
    vBoards.resize( 3 );
    MakeExampleBoards( vBoards[0], vBoards[1], vBoards[2] );
}

// display for testing
void PrintBoard( sBoard &b ) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
           std::cout << (b.board[y][x].nNumber > 9 ? " "  : "  ");   // padd spaces
           std::cout <<  b.board[y][x].nNumber;
           std::cout << (b.board[y][x].bMarked     ? "* " : "  " );  // * if number is marked
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool WinningBoard( sBoard &b ) {
    auto row_fully_marked = [=]( int row ) {
        return b.board[row][0].bMarked &&
               b.board[row][1].bMarked &&
               b.board[row][2].bMarked &&
               b.board[row][3].bMarked &&
               b.board[row][4].bMarked;
    };
    auto col_fully_marked = [=]( int col ) {
        return b.board[0][col].bMarked &&
               b.board[1][col].bMarked &&
               b.board[2][col].bMarked &&
               b.board[3][col].bMarked &&
               b.board[4][col].bMarked;
    };
    // check if there is any row or column completely marked
    bool winflag = false;
    for (int i = 0; i < 5; i++) {
        if (row_fully_marked( i )) winflag = true;
        if (col_fully_marked( i )) winflag = true;
    }
    return winflag;
}

void PlayOneBoardTurn( sBoard &b, int callNumber ) {
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++) {
            if (b.board[y][x].nNumber == callNumber)
                b.board[y][x].bMarked = true;
        }
}

int SumOfUnmarkedNumbers( sBoard &b ) {
    int result = 0;
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            result += b.board[y][x].bMarked ? 0 : b.board[y][x].nNumber;
    return result;
}

void ReadCallNumbers( std::ifstream &dFileStream, NumberStream &vData ) {
    std::string sLine;
    if (dFileStream >> sLine) {
        std::string token;
        for (int i = 0; i < (int)sLine.length(); i++) {
            if (sLine[i] != ',') {
                token.append( 1, sLine[i] );
                if (i == (int)sLine.length() - 1) {
                    // there's a valid token still
                    vData.push_back( stoi( token ));
                    token.clear();
                }
            } else {  // it was a ','
                // wrap up this token and reset for next one
                vData.push_back( stoi( token ));
                token.clear();
            }
        }
    }
}

bool ReadOneBoard( std::ifstream &dFileStream, sBoard &b ) {
    int tmparr[5][5] = { -1 };
    bool bAbort = false;
    // fill tmparr with the next 5 lines, having 5 int values per line
    for (int j = 0; j < 5; j++) {
        std::string sElt[5];
        if (dFileStream >> sElt[0] >> sElt[1] >> sElt[2] >> sElt[3] >> sElt[4] ) {   // read next line
            for (int i = 0; i < 5; i++) {
                tmparr[j][i] = stoi( sElt[i] );
            }
        } else    // if ifstream operations failed, must be eof
            bAbort = true;
    }

    AssembleBoard( b,
        tmparr[0][0], tmparr[0][1], tmparr[0][2], tmparr[0][3], tmparr[0][4],
        tmparr[1][0], tmparr[1][1], tmparr[1][2], tmparr[1][3], tmparr[1][4],
        tmparr[2][0], tmparr[2][1], tmparr[2][2], tmparr[2][3], tmparr[2][4],
        tmparr[3][0], tmparr[3][1], tmparr[3][2], tmparr[3][3], tmparr[3][4],
        tmparr[4][0], tmparr[4][1], tmparr[4][2], tmparr[4][3], tmparr[4][4]
    );

    return !bAbort;
}

// this function can be used to input the data from the text file
void ReadData( const std::string &sFileName, NumberStream &vData, BoardStream &vBoards) {
    // open filestream
    std::ifstream dataFileStream( sFileName );
    // read initial stream of call numbers
    vData.clear();
    ReadCallNumbers( dataFileStream, vData );
    // read boards
    bool bDone = dataFileStream.eof();
    vBoards.clear();
    while (!bDone) {
        sBoard b;
        bool bSuccess = ReadOneBoard( dataFileStream, b );
        if (bSuccess) {
            vBoards.push_back( b );
            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }
}

// display call numbers for testing
void PrintCallNumbers( NumberStream &vData ) {
    for (auto elt : vData)
        std::cout << elt << std::endl;              // <<<<< ========== adapt to match columns of input file
    std::cout << std::endl << "Nr of data elements: " << vData.size() << std::endl << std::endl;
}

// display boards for testing
void PrintBoards( BoardStream &vBoards ) {
    for (auto elt : vBoards)
        PrintBoard( elt );
}

// ========== programming fase

enum eProgFase {
    EXAMPLE = 0,
    TEST,
    PUZZLE
};

eProgFase glbProgFase;    // what programming fase are you in - set at start of main()

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( NumberStream &vData, BoardStream &vBoards ) {
    switch (glbProgFase) {
        case EXAMPLE: MakeExampleData(            vData, vBoards ); break;
        case TEST:    ReadData( "input.test.txt", vData, vBoards ); break;
        case PUZZLE:  ReadData( "input.txt",      vData, vBoards ); break;
    }
}

// ========== puzzle solutions

int main()
{
    flcTimer tmr;
    glbProgFase = PUZZLE;

    tmr.StartTiming();

    // get input data, depending on the glbProgFase (example, test, puzzle)
    NumberStream inputNumbers;
    BoardStream inputBoards;
    GetInput( inputNumbers, inputBoards );
    // test if the input reading works correctly
    if (glbProgFase == EXAMPLE || glbProgFase == TEST) {
        PrintCallNumbers( inputNumbers );
        PrintBoards( inputBoards );
    }

    tmr.TimeReport( "File reading: " );

// ========== part 1

    bool game_over = false;
    for (int i = 0; i < (int)inputNumbers.size() && !game_over; i++) {
        for (int j = 0; j < (int)inputBoards.size(); j++) {
            PlayOneBoardTurn( inputBoards[j], inputNumbers[i] );
            if (WinningBoard( inputBoards[j] )) {
                game_over = true;

                int nSumUnmarked = SumOfUnmarkedNumbers( inputBoards[j] );
                int nLastCalled  = inputNumbers[i];

                std::cout << "board " << j << " won in turn " << i << std::endl;

                std::cout << "sum of unmarked nrs: "   << nSumUnmarked
                          << " winning number: "       <<                nLastCalled
                          << " --> product of these: " << nSumUnmarked * nLastCalled << std::endl << std::endl;
            }
        }
    }

    tmr.TimeReport( "Part 1: " );

// ========== part 2

    // create a vector to keep track of boards that already won. Same size as vector inputBoards, init to false
    std::vector<bool> winners( inputBoards.size(), false );

    game_over = false;
    for (int i = 0; i < (int)inputNumbers.size() && !game_over; i++) {
        for (int j = 0; j < (int)inputBoards.size(); j++) {
            PlayOneBoardTurn( inputBoards[j], inputNumbers[i] );
            if (!winners[j] && WinningBoard( inputBoards[j] )) {
                winners[j] = true;
                int nBoardsLeft = count_if( winners.begin(), winners.end(), []( bool b ) { return !b; } );
                game_over = (nBoardsLeft == 0);

                if (game_over) {
                    int nSumUnmarked = SumOfUnmarkedNumbers( inputBoards[j] );
                    int nLastCalled  = inputNumbers[i];

                    std::cout << "board " << j << " is the last one to win in turn " << i << std::endl;

                    std::cout << "sum of unmarked nrs: "   << nSumUnmarked
                              << " winning number: "       << nLastCalled
                              << " --> product of these: " << nSumUnmarked * nLastCalled << std::endl << std::endl;
                }
            }
        }
    }

    tmr.TimeReport( "Part 2: " );

    return 0;
}


