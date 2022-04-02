// AoC 2021 - day 05
// =================

// date:  2021-12-05
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../flcTimer.h"

// ==========   PROGRAM FASING

enum eProgFase {
    EXAMPLE = 0,
    TEST,
    PUZZLE
};

eProgFase glbProgFase;    // what programming fase are you in - set at start of main()

// ========== DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

int glbMapX = -1;    // 10 for EXAMPLE, 1000 for TEST & PUZZLE
int glbMapY = -1;

short *glbMap;

struct sVent {
    int x1 = -1, y1 = -1;
    int x2 = -1, y2 = -1;
};

typedef std::vector<sVent> VentStream;

void InitGlbMap() {
    switch (glbProgFase) {
        case EXAMPLE: glbMapX =   10; glbMapY =   10; break;
        case TEST:
        case PUZZLE:  glbMapX = 1000; glbMapY = 1000; break;
    }
    glbMap = new short[glbMapX * glbMapY]{0};
}

void DisposeGlbMap() {
    delete glbMap;
    glbMap = nullptr;
    glbMapX = -1;
    glbMapY = -1;
}

// ========== data input functions

void AssembleVent( sVent &v, int x1, int y1, int x2, int y2 ) {
    v = { x1, y1, x2, y2 };
}

// populate the example data in this function for testing your algo
void MakeExampleData( VentStream &vData ) {

    vData.clear();
                                             // <<<<< ========== adapt to match columns of input file
    vData = {
        { 0, 9, 5, 9 },
        { 8, 0, 0, 8 },
        { 9, 4, 3, 4 },
        { 2, 2, 2, 1 },
        { 7, 0, 7, 4 },
        { 6, 4, 2, 0 },
        { 0, 9, 2, 9 },
        { 3, 4, 1, 4 },
        { 0, 0, 8, 8 },
        { 5, 5, 8, 2 },
    };
}

bool SplitString( const std::string &input, const std::string &token, std::string &out_before_token, std::string &out_after_token) {
    size_t tokenIndex = input.find( token );
    bool bTokenFound = (tokenIndex != std::string::npos);
    if (bTokenFound) {
        out_before_token = input.substr( 0, tokenIndex );
        out_after_token  = input.substr( tokenIndex + 1 );
    }
    return bTokenFound;
}

bool ReadOneVentLine( std::ifstream &dFileStream, sVent &v ) {
    std::string token[3];
    bool bAbort = false;

    if (dFileStream >> token[0] >> token[1] >> token[2]) {
        // parse x1, y1 out of first token
        size_t commaIndex = token[0].find( "," );
        std::string sX = token[0].substr( 0, commaIndex );
        std::string xY = token[0].substr( commaIndex + 1 );
        v.x1 = atoi( sX.c_str() );
        v.y1 = atoi( xY.c_str() );
        // parse x2, y2 out of third token (ignore second token)
        commaIndex = token[2].find( "," );
        sX = token[2].substr( 0, commaIndex );
        xY = token[2].substr( commaIndex + 1 );
        v.x2 = atoi( sX.c_str() );
        v.y2 = atoi( xY.c_str() );
    } else {
        bAbort = true;
    }

    return !bAbort;
}

// this function can be used to input the data from the text file
void ReadData( const std::string &sFileName, VentStream &vData ) {

    // open filestream
    std::ifstream dataFileStream( sFileName );
    // read data stream
    vData.clear();
    bool bDone = dataFileStream.eof();
    while (!bDone) {
        sVent v;
        bool bSuccess = ReadOneVentLine( dataFileStream, v );
        if (bSuccess) {
            vData.push_back( v );
            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }

    dataFileStream.close();
}

// ========== output functions

void PrintMap( short *m ) {
    for (int y = 0; y < glbMapY; y++) {
        for (int x = 0; x < glbMapX; x++) {
            short n = m[y * glbMapX + x];
            std::cout << (n == 0 ? '.' : (char)(n + '0'));
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// output to console for testing
void PrintVentLines( VentStream &vData ) {
    for (auto elt : vData)
        std::cout << "(x1, y1): ("     << elt.x1                 // <<<<< ========== adapt to match columns of input file
                  << ", "              << elt.y1
                  << ") - (x2, y2): (" << elt.x2
                  << ", "              << elt.y2 << ")" << std::endl;
    std::cout << std::endl << "Nr of data elements: " << vData.size() << std::endl << std::endl;
}

// ========== data processing functions

int NrOfPointsAboveThreshold( short *m, short threshold, bool bPrint = false ) {
    int nCount = 0;
    for (int y = 0; y < glbMapY; y++) {
        for (int x = 0; x < glbMapX; x++) {
            short n = m[y * glbMapX + x];
            if (n >= threshold) {
                nCount += 1;
                if (bPrint) std::cout << "(x, y): (" << x << ", " << y << ") value: " << n << std::endl;
            }
        }
    }
    if (bPrint) std::cout << std::endl;
    return nCount;
}

void ProcessVentLines_HV( VentStream &vData ) {
    for (auto elt : vData) {
        if (elt.x1 == elt.x2) {                                  // horizontal lines
            int tmpy1 = elt.y1;
            int tmpy2 = elt.y2;
            if (tmpy1 > tmpy2) std::swap( tmpy1, tmpy2 );
            for (int i = tmpy1; i <= tmpy2; i++)
                glbMap[i * glbMapX + elt.x1] += 1;
        }
        if (elt.y1 == elt.y2) {                                  // vertical lines
            int tmpx1 = elt.x1;
            int tmpx2 = elt.x2;
            if (tmpx1 > tmpx2) std::swap( tmpx1, tmpx2 );
            for (int i = tmpx1; i <= tmpx2; i++)
                glbMap[elt.y1 * glbMapX + i] += 1;
        }
    }
}

void ProcessVentLines_D( VentStream &vData ) {
    for (auto elt : vData) {
        if (abs( elt.x2 - elt.x1 ) == abs( elt.y2 - elt.y1 )) {  // diagonal lines
            int nXstep = (elt.x1 > elt.x2) ? -1 : 1;
            int nYstep = (elt.y1 > elt.y2) ? -1 : 1;
            bool bDone = false;
            for (int x = elt.x1, y = elt.y1; !bDone ; x += nXstep, y += nYstep) {
                glbMap[y * glbMapX + x] += 1;
                bDone = (x == elt.x2);
            }
        }
    }
}

// ========== program phasing

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( VentStream &vData ) {
    switch (glbProgFase) {
        case EXAMPLE: MakeExampleData(           vData ); break;
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
    VentStream inputVents;
    GetInput( inputVents );

    tmr.TimeReport( "File reading: " );   // ====================^^^^^vvvvv

// ========== part 1

    InitGlbMap();
    ProcessVentLines_HV( inputVents );
    // test if the input reading works correctly
    if (glbProgFase == EXAMPLE || glbProgFase == TEST) {
        PrintVentLines( inputVents );
        if (glbProgFase == EXAMPLE)
            PrintMap( glbMap );
    }

    int answer1 = NrOfPointsAboveThreshold( glbMap, 2, glbProgFase != PUZZLE );
    std::cout << "answer 1 = " << answer1 << std::endl;
    DisposeGlbMap();

    tmr.TimeReport( "Part 1: " );   // =========================^^^^^vvvvv

// ========== part 2

    InitGlbMap();
    ProcessVentLines_HV( inputVents );
    ProcessVentLines_D(  inputVents );
    // test if the input reading works correctly
    if (glbProgFase == EXAMPLE || glbProgFase == TEST) {
        PrintVentLines( inputVents );
        if (glbProgFase == EXAMPLE)
            PrintMap( glbMap );
    }

    int answer2 = NrOfPointsAboveThreshold( glbMap, 2, glbProgFase != PUZZLE );
    std::cout << "answer 2 = " << answer2 << std::endl;
    DisposeGlbMap();

    tmr.TimeReport( "Part 2: " );   // ==============================^^^^^

    return 0;
}


