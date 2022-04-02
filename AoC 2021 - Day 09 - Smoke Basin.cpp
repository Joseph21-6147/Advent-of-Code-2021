#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../flcTimer.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgFase {     // what programming fase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgFase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef string sData;               // data consists of a list of strings, each containing digits 0-9
typedef vector<sData> DataStream;

struct sCoord {                     // to store lists of points (low points resp. bassins)
    int x, y;
};
typedef vector<sCoord> MapType;

#define DIR_UP 0                    // constants for the basin algo
#define DIR_LT 1
#define DIR_DN 2
#define DIR_RT 3

// ==========   DATA INPUT FUNCTIONS

// populate the example data in this function
void MakeExampleData( DataStream &vData ) {     // <<<<< ========== adapt to match columns of input file
    vData.clear();
    vData = { "2199943210",
              "3987894921",
              "9856789892",
              "8767896789",
              "9899965678" };
}

// this function is to input the data from the text file
void ReadData( string sFileName, DataStream &vData ) {
    ifstream dataFileStream( sFileName );
    vData.clear();
    sData sDatum;
    while (dataFileStream >> sDatum )      // <<<<< ========== adapt to match columns of input file
        vData.push_back( sDatum );
    dataFileStream.close();
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintDataStream( DataStream &vData ) {
    for (int i = 0; i < (int)vData.size(); i++)
        cout << "Data: " << vData[i] << endl;
}

void PrintMapType( MapType &m ) {
    for (auto elt : m)
        cout << "(" << elt.x << ", " << elt.y << ") ";
    cout << endl;
}

// ==========   DATA PROCESSING

// returns true if an element with coordinate pair (x, y) is already in basin
bool Exists( MapType &basin, int x, int y ) {
    for (auto e : basin )
        if (e.x == x && e.y == y) return true;
    return false;
}

// function to expand an already existing basin. It checks on all criteria for a cell/coord
// to be part of the basin, if so it adds the current coord, and attemtpts to expand with the
// neighbouring cells (using recursive calls)
void ExpandBasin( DataStream &vData, MapType &basin, int toDir, int x, int y, int val ) {
    if (y >= 0 && y < (int)vData.size() &&      // don't expand if coord is out of bounds
        x >= 0 && x < (int)vData[y].length()) {

        if (!Exists( basin, x, y )) {           // don't expand if coord is already in the basin

            int nValue = vData[y][x] - '0';
            if (nValue != 9 && nValue > val) {  // check criteria for adding coord to the basin
                // add current location
                basin.push_back( { x, y } );
                // recurse over all neighbors, except the one this function was called from
                int nSkip = (toDir + 2) % 4;
                if (nSkip != DIR_UP) ExpandBasin( vData, basin, DIR_UP, x    , y - 1, nValue );
                if (nSkip != DIR_DN) ExpandBasin( vData, basin, DIR_DN, x    , y + 1, nValue );
                if (nSkip != DIR_LT) ExpandBasin( vData, basin, DIR_LT, x - 1, y    , nValue );
                if (nSkip != DIR_RT) ExpandBasin( vData, basin, DIR_RT, x + 1, y    , nValue );
            }
        }
    }
}

// assumption - this function is only called from a lowpoint (x, y)
// and (x, y) is a valid coordinate pair - i.e. in bounds
// It uses ExpandBasin() to recursively check the boundaries of the basin
void MakeBasin( DataStream &vData, MapType &basin, int x, int y ) {
    basin.push_back( { x, y } );        // add current low point to basin

    int nValue = vData[y][x] - '0';     // expand basin in all possible directions
    ExpandBasin( vData, basin, DIR_UP, x    , y - 1, nValue );
    ExpandBasin( vData, basin, DIR_DN, x    , y + 1, nValue );
    ExpandBasin( vData, basin, DIR_LT, x - 1, y    , nValue );
    ExpandBasin( vData, basin, DIR_RT, x + 1, y    , nValue );
}

// Creates a list of low points
void FindLowPoints( DataStream &vData, MapType &pointList ) {
    for (int i = 0; i < (int)vData.size(); i++) {
        for (int j = 0; j < (int)vData[i].length(); j++) {
            // make sure that border values are irrelevant if the neighbour is out of bounds
            char up, dn, lt, rt;
            char maxVal = '9' + 1;
            if (i == 0)                        up = maxVal; else up = vData[i - 1][j    ];
            if (i == (int)vData.size() - 1)    dn = maxVal; else dn = vData[i + 1][j    ];
            if (j == 0)                        lt = maxVal; else lt = vData[i    ][j - 1];
            if (j == (int)vData[i].size() - 1) rt = maxVal; else rt = vData[i    ][j + 1];

            char c = vData[i][j];
            if (c < up && c < dn && c < lt && c < rt) {
//                cout << "found low point at ( " << j << ", " << i << "): " << c << endl;
                pointList.push_back( { j, i } );
            }
        }
    }
}

void FindLargestBasins( DataStream &vData, MapType &lowPointsList ) {

    vector<int> vBasinSizes;
    for (int i = 0; i < (int)lowPointsList.size(); i++) {
        MapType myBasin;
        MakeBasin( vData, myBasin, lowPointsList[i].x, lowPointsList[i].y );
//        PrintMapType( myBasin );
        vBasinSizes.push_back( myBasin.size() );
//        cout << "Basin found - size: " << myBasin.size() << " content: ";
    }
    // sort to get largest basins at start of vector
    sort( vBasinSizes.begin(), vBasinSizes.end(), []( int n1, int n2 ) { return n1 > n2; } );
//    cout << endl << endl;
//    for (int i = 0; i < (int)vBasinSizes.size(); i++)
//        cout << "Basin " << i << " has size " << vBasinSizes[i] << endl;

    cout << "product of largest three basins = " << vBasinSizes[0] * vBasinSizes[1] * vBasinSizes[2] << endl;
}

// ========== program phasing

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( DataStream &vData ) {
    switch (glbProgFase) {
        case EXAMPLE: MakeExampleData(            vData ); break;
        case TEST:    ReadData( "input.test.txt", vData ); break;
        case PUZZLE:  ReadData( "input.txt",      vData ); break;
    }
}

// ========== puzzle solutions

int main()
{
    flcTimer tmr;
    glbProgFase = PUZZLE;

    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgFase (example, test, puzzle)
    DataStream inputData;
    GetInput( inputData );
//    PrintDataStream( inputData );

    int nSize   = inputData.size();
    cout << "data stats - #elts " << nSize << endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    MapType lowPointsList;
    FindLowPoints( inputData, lowPointsList );

//    PrintMapType( lowPointsList );
    int nCumLowPoints  = 0;
    int nCumRiskLevels = 0;
    for (auto &e : lowPointsList) {
        nCumLowPoints  +=  inputData[e.y][e.x] - '0';
        nCumRiskLevels += (inputData[e.y][e.x] - '0') + 1;
    }

    cout << "Cum. low points: " << nCumLowPoints << " Cum. risk levels: " << nCumRiskLevels << endl;

    tmr.TimeReport( "Timing 1 : " );   // ==============================^^^^^

// ========== part 2

//    PrintDataStream( inputData );
    FindLargestBasins( inputData, lowPointsList );

    tmr.TimeReport( "Timing 2 : " );   // ==============================^^^^^

    return 0;
}


