// AoC 2021 - day 11
// =================

// date:  2021-12-11
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <vector>
#include <fstream>

#include "../flcTimer.h"

// ==========   PROGRAM PHASING

enum eProgFase {     // what programming fase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgFase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef std::string sData;                    // data consists of a list of strings, each containing opening and closing braces (){}<>[]
typedef std::vector<sData> DataStream;

#define MAP_X 10                         // dimensions of octopus field
#define MAP_Y 10

bool bFlash[MAP_X][MAP_Y] = { false };   // flag map to signal if octopussy flashed at current step

// ==========   DATA INPUT FUNCTIONS

// populate the example data in this function
void MakeData( DataStream &vData ) {     // <<<<< ========== adapt to match columns of input file
    vData.clear();
    vData = {
//        "11111",   // there were two examples in this puzzle
//        "19991",   // 5x5 example
//        "19191",
//        "19991",
//        "11111",

        "5483143223",    // 10x10 example
        "2745854711",
        "5264556173",
        "6141336146",
        "6357385478",
        "4167524645",
        "2176841721",
        "6882881134",
        "4846848554",
        "5283751526"
    };
}

// this function is to input the data from the text file
void ReadData( const std::string &sFileName, DataStream &vData ) {
    std::ifstream dataFileStream( sFileName );
    vData.clear();
    sData sDatum;
    while (dataFileStream >> sDatum )      // <<<<< ========== adapt to match columns of input file
        vData.push_back( sDatum );
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintDataStream( DataStream &vData ) {
    for (int i = 0; i < (int)vData.size(); i++)
        std::cout << "Data: " << vData[i] << std::endl;
    std::cout << std::endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( DataStream &vData, bool bDisplay = false ) {
    switch (glbProgFase) {
        case EXAMPLE: MakeData(                   vData ); break;
        case TEST:    ReadData( "input.test.txt", vData ); break;
        case PUZZLE:  ReadData( "input.txt",      vData ); break;
    }
    if (bDisplay)
        PrintDataStream( vData );
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

// sets flash flag for octo in (x, y) and then
// updates Moore neighborhood (i.e. including diagonals)
void Flash( DataStream &vData, int x, int y ) {
    // set flash flag of current cell
    bFlash[x][y] = true;
    // clip against neighborhood boundaries
    int x_start = std::max(         0, x - 1 );
    int y_start = std::max(         0, y - 1 );
    int x_stop  = std::min( MAP_X - 1, x + 1 );
    int y_stop  = std::min( MAP_Y - 1, y + 1 );
    // increase energy level of neighborhood
    for (int ty = y_start; ty <= y_stop; ty++) {
        for (int tx = x_start; tx <= x_stop; tx++) {
            if (!(tx == x && ty == y)) {   // skip current cell, we already processed it
                vData[ty][tx] += 1;
            }
        }
    }
}

// There are three passes:
// 1. increase energy level for all octo's
// 2. flash where appropriate, and repeat doing this until no new
//    flashes are triggered
// 3. count the number of flashes, and reset the energy level and flags
//    of the octo's that flashed
// Function returns number of flashes in this step.
int ProcessOneStep( DataStream &vData ) {
    int nFlashes = 0;
    // increase energy level
    for (int y = 0; y < MAP_Y; y++) {
        for (int x = 0; x < MAP_X; x++) {
            vData[y][x] += 1;
        }
    }
    // flash where appropriate - a flash may trigger another flash, so
    // iterate until no progressive flashes died out
    bool bStillFlashing;
    do {
        bStillFlashing = false;
        for (int y = 0; y < MAP_Y; y++) {
            for (int x = 0; x < MAP_X; x++) {
                if (vData[y][x] > '9' && !bFlash[x][y]) {
                    Flash( vData, x, y );
                    bStillFlashing = true;
                }
            }
        }
    } while (bStillFlashing);
    // count nr of flashes for this step and reset flash flag map
    for (int y = 0; y < MAP_Y; y++) {
        for (int x = 0; x < MAP_X; x++) {
            if (bFlash[x][y]) {
                nFlashes += 1;
                bFlash[x][y] = false;
                vData[y][x] = '0';
            }
        }
    }
    return nFlashes;
}

int main()
{
    glbProgFase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgFase (example, test, puzzle)
    DataStream inputData;
    GetInput( inputData, false );
    std::cout << "Data stats - #elts " << inputData.size() << std::endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    int nCountFlashes = 0;
    for (int i = 1; i <= 100; i++) {
        nCountFlashes += ProcessOneStep( inputData );
    }
    std::cout << "Total nr of flashes after 100 steps: " << nCountFlashes << std::endl;

    tmr.TimeReport( "Timing 1 : " );   // ==============================^^^^^

// ========== part 2

    // NOTE: part 1 code has altered the input data, so re-read it!
    GetInput( inputData, false );
    tmr.StartTiming();

    bool bSolved = false;
    for (int i = 1; !bSolved; i++) {
        int nStepFlashes = ProcessOneStep( inputData );
        if (nStepFlashes == MAP_X * MAP_Y) {
            std::cout << "All octopuses flashed at step: " << i << std::endl;
            bSolved = true;
        }
    }

    tmr.TimeReport( "Timing 2 : " );   // ==============================^^^^^

    return 0;
}
