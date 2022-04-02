#include <iostream>
#include <vector>
#include <map>
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

typedef map<short, long long> FishMap;    // regular long int is 32 bit, that's *not enough* to solve this puzzle

// ========== data input functions

// populate the example data in this function for testing your algo
void MakeExampleData( FishMap &vData ) {

    vData.clear();
                                             // <<<<< ========== adapt to match columns of input file
    vector tmpInput = { 3, 4, 3, 1, 2 };
    for (int i = 0; i < (int)tmpInput.size(); i++)
        vData[ tmpInput[i] ] += 1;
}

// this function can be used to input the data from the text file
void ReadTestData( FishMap &vData ) {

    // read data stream
    vData.clear();
    vector tmpInput = {
        3,5,3,1,4,4,5,5,2,1,4,3,5,1,3,5,3,2,4,3,5,3,1,1,2,1,4,5,3,1,4,5,4,3,3,4,3,1
    };
    for (int i = 0; i < (int)tmpInput.size(); i++)
        vData[ tmpInput[i] ] += 1;
}

// this function can be used to input the data from the text file
void ReadData( FishMap &vData ) {

    // read data stream
    vData.clear();
    vector tmpInput = {
        3,5,3,1,4,4,5,5,2,1,4,3,5,1,3,5,3,2,4,3,5,3,1,1,2,1,4,5,3,1,4,5,4,3,3,4,3,1,
        1,2,2,4,1,1,4,3,4,4,2,4,3,1,5,1,2,3,2,4,4,1,1,1,3,3,5,1,4,5,5,2,5,3,3,1,1,2,
        3,3,3,1,4,1,5,1,5,3,3,1,5,3,4,3,1,4,1,1,1,2,1,2,3,2,2,4,3,5,5,4,5,3,1,4,4,2,
        4,4,5,1,5,3,3,5,5,4,4,1,3,2,3,1,2,4,5,3,3,5,4,1,1,5,2,5,1,5,5,4,1,1,1,1,5,3,
        3,4,4,2,2,1,5,1,1,1,4,4,2,2,2,2,2,5,5,2,4,4,4,1,2,5,4,5,2,5,4,3,1,1,5,4,5,3,
        2,3,4,1,4,1,1,3,5,1,2,5,1,1,1,5,1,1,4,2,3,4,1,3,3,2,3,1,1,4,4,3,2,1,2,1,4,2,
        5,4,2,5,3,2,3,3,4,1,3,5,5,1,3,4,5,1,1,3,1,2,1,1,1,1,5,1,1,2,1,4,5,2,1,5,4,2,
        2,5,5,1,5,1,2,1,5,2,4,3,2,3,1,1,1,2,3,1,4,3,1,2,3,2,1,3,3,2,1,2,5,2
    };
    for (int i = 0; i < (int)tmpInput.size(); i++)
        vData[ tmpInput[i] ] += 1;
}

// ========== output functions

// output to console for testing
void PrintFishStream( FishMap &vData ) {
    FishMap::iterator it;
    for ( it = vData.begin(); it != vData.end(); it++ ) {
        cout << " phs: " << (*it).first << " #: " << (*it).second ;
    }
}

// ========== data processing functions

void ProcessFishStream( FishMap &vData ) {
    long long cacheZeroPhased = vData[ 0 ];  // temporary store fish that will spawn

    for (int phase = 1; phase <= 8; phase++) {   // progress phases
        vData[ phase - 1 ] = vData[ phase ];
    }

    vData[8] = cacheZeroPhased;
    vData[6] += cacheZeroPhased;
}

void Simulate( int nrOfDays, FishMap &vData, bool bOutput ) {

    for (int i = 0; i < nrOfDays; i++) {

        ProcessFishStream( vData );
        if (bOutput) {
            cout << "Day " << i + 1 << ": ";
            PrintFishStream( vData );
            cout << endl;
        } else {
//            long long lTotal = 0;
//            for (auto e : vData )
//                lTotal += e.second;
//            cout << "Day " << i + 1 << ": " << lTotal << " fish" << endl;
        }
    }
}

// ========== program phasing

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( FishMap &vData ) {
    switch (glbProgFase) {
        case EXAMPLE: MakeExampleData( vData ); break;
        case TEST:    ReadTestData(    vData ); break;
        case PUZZLE:  ReadData(        vData ); break;
    }
}

// ========== puzzle solutions

int main()
{
    flcTimer tmr;
    glbProgFase = PUZZLE;

    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgFase (example, test, puzzle)
    FishMap inputFish;
    GetInput( inputFish );

    tmr.TimeReport( "File reading: " );   // ====================^^^^^vvvvv

// ========== part 1 & 2

    int nDays = 256;
    bool bOutput = (nDays <= 20);
    Simulate( nDays, inputFish, bOutput  );
    long long lTotal = 0;
    for (auto e : inputFish )
        lTotal += e.second;
    cout << endl << "answer 2: After " << nDays << " days there will be " << lTotal << " lantern fish" << endl;

    tmr.TimeReport( "Part 2: " );   // ==============================^^^^^

    return 0;
}


