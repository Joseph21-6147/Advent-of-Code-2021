// AoC 2021 - day 14
// =================

// date:  2021-12-14
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#include "../flcTimer.h"

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef std::string PolTemplate;   // in the form "NNCB"

// when pair like "XX" is encountered ...
// ... insert "Y" into it
typedef std::map<std::string, char> InsertStream;
typedef std::map<std::string, char>::iterator InsertIterator;

// ==========   DATA INPUT FUNCTIONS

// this function is used to input the data from the text file
void ReadData( const std::string &sFileName, PolTemplate &tData, InsertStream &iData ) {
    // open filestream
    std::ifstream dataFileStream( sFileName );
    // read data stream
    tData.clear();
    iData.clear();
    if (!(dataFileStream >> tData))
        std::cout << "ERROR - ReadData() --> can't find polymer template..." << std::endl;
    else {
        bool bDone = dataFileStream.eof();
        std::string token[3];
        while (!bDone) {
            // get next token and determine what to do
            if (dataFileStream >> token[0] >> token[1] >> token[2]) {
                iData.insert( make_pair( token[0], token[2][0] ));
                bDone = dataFileStream.eof();
            } else {
                bDone = true;
            }
        }
    }
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintPolTemplate( const PolTemplate &tData ) {
    std::cout << "Polymer template: " << tData << " " << std::endl;
}

// output to console for testing
void PrintInsertStream( InsertStream &iData ) {
    InsertIterator it;
    for (it = iData.begin(); it != iData.end(); it++)
        std::cout << "Insert instruction - when encountering: " << (*it).first << " insert " << (*it).second << std::endl;
    std::cout << std::endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program phase (glbProgPhase)
void GetInput( PolTemplate &tData, InsertStream &iData, bool bDisplay = false ) {
    switch (glbProgPhase) {
        case EXAMPLE: ReadData( "input.example.txt", tData, iData ); break;
        case TEST:    ReadData( "input.test.txt"   , tData, iData ); break;
        case PUZZLE:  ReadData( "input.txt"        , tData, iData ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintPolTemplate( tData );
        PrintInsertStream( iData );
    }
}

// ==========  Part 1 - PUZZLE SPECIFIC SOLUTIONS

// apply replace instructions (instr list) onto polymer (pol) one time
void OneInsertionStep( const PolTemplate &in, InsertStream &instr, PolTemplate &out ) {
    for (int i = 0; i < (int)in.length() - 1; i++) {
        std::string match = in.substr( i, 2 );
        InsertIterator it = instr.find( match );
        if (it == instr.end())
            std::cout << "ERROR - OneInsertionStep() --> can't find polymer replace instruction..." << std::endl;
        else {
            out.append( std::string( 1, match[0] ));
            out.append( std::string( 1, (*it).second ));
        }
    }
    if (in.length() > 0)
        out.append( std::string( 1, in.back() ));
}

int NrOfChars( const PolTemplate &pol, char C ) {
//    return (int)count(pol.begin(), pol.end(), C );
    int result = 0;
    for (int i = 0; i < (int)pol.length(); i++)
        if (pol[i] == C)
            result += 1;
    return result;
}

int MaxOfArray( int val[], int nrElts ) {
    int result = INT_MIN;
    for (int i = 0; i < nrElts; i++)
        if (val[i] > result)
            result = val[i];
    return result;
}

int MinOfArray( int val[], int nrElts ) {
    int result = INT_MAX;
    for (int i = 0; i < nrElts; i++)
        if (val[i] > 0 && val[i] < result)
            result = val[i];
    return result;
}

long long MaxOfArray( long long val[], int nrElts ) {
    long long result = LLONG_MIN;
    for (int i = 0; i < nrElts; i++)
        if (val[i] > result)
            result = val[i];
    return result;
}

long long MinOfArray( long long val[], int nrElts ) {
    long long result = LLONG_MAX;
    for (int i = 0; i < nrElts; i++)
        if (val[i] > 0 && val[i] < result)
            result = val[i];
    return result;
}

// part 2 processing ========================================

// for counting the nr of elements in a polymer string (part 1)
// or from a calculation table (part 2)
long long glbEltCount[26];

void ClearElementCount() {
    for (int i = 0; i < 26; i++)
        glbEltCount[i] = 0;
}

void PrintElementCount() {
    for (int i = 0; i < 26; i++)
        if (glbEltCount[i] > 0)
            std::cout << "Index: " << i << " models: " << (char)('A' + i) << " contains: " << glbEltCount[i] << std::endl;
    std::cout << std::endl;
}

// calculation table - to calculate how many times a combination is changed into two new combinations,
// according to the rewrite rules of the polymer.
long long glbCombiCount[26][26];

void ClearCombiCount() {
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            glbCombiCount[i][j] = 0;
}

// initialize the calculation table with the input polymer (template)
void StartStep2( PolTemplate &inputPol ) {
    // clear calculation table
    ClearCombiCount();
    for (int i = 0; i < (int)inputPol.length() - 1; i++) {
        char fst = inputPol[i];
        char scd = inputPol[i+1];
        glbCombiCount[(int)(fst - 'A')][(int)(scd - 'A')] += 1;
    }
}

void OneStep2( InsertStream &iData ) {
    long long auxCombiCount[26][26];   // temp copy of calculation table

    // copy glbCombiCount to auxCombiCount
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            auxCombiCount[i][j] = glbCombiCount[i][j];
        }
    }

    for (auto combi : iData) {
        // replace count for original combination by two new ones
        char aux[3]; int ix[3];
        aux[0] = combi.first[0];  ix[0] = (int)(aux[0] - 'A');
        aux[2] = combi.first[1];  ix[2] = (int)(aux[2] - 'A');
        aux[1] = combi.second;    ix[1] = (int)(aux[1] - 'A');
        glbCombiCount[ ix[0] ][ ix[2] ] -= auxCombiCount[ ix[0] ][ ix[2] ];
        glbCombiCount[ ix[0] ][ ix[1] ] += auxCombiCount[ ix[0] ][ ix[2] ];
        glbCombiCount[ ix[1] ][ ix[2] ] += auxCombiCount[ ix[0] ][ ix[2] ];
    }
}

// count all the elements from the polymer in inputPol, using the
// calculation table.
void ConsolidateEltCount( PolTemplate &inputPol ) {
    ClearElementCount();
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            glbEltCount[i] += glbCombiCount[i][j];  // count row input for this element
            glbEltCount[i] += glbCombiCount[j][i];  // column
        }
    }
    // elements are all counted twice, except for the first and the last one of the
    // polymer template. Therefore, add the first and last one, and then divide all by 2
    char fstChar = inputPol[0];
    char lstChar = inputPol.back();
    glbEltCount[(int)(fstChar - 'A')] += 1;
    glbEltCount[(int)(lstChar - 'A')] += 1;
    for (int i = 0; i < 26; i++)
        glbEltCount[i] /= 2;
}

// ============================================================

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    PolTemplate   polTempl;
    InsertStream instrData;
    GetInput( polTempl, instrData, glbProgPhase != PUZZLE );
    std::cout << "Data stats - length of polymer template:    " <<  polTempl.size() << std::endl;
    std::cout << "Data stats - number of insert instructions: " << instrData.size() << std::endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1 : string based naive approach - starts stuttering at around 25 iterations

    PolTemplate tmpPol, newPol;
    newPol = polTempl;
//    PrintPolTemplate( newPol );
    for (int i = 1; i <= 10; i++) {
        tmpPol = newPol;
        newPol.clear();
        OneInsertionStep( tmpPol, instrData, newPol );
//        std::cout << "After step: " << i << " (pol len = " << newPol.length() << ")  " << std::endl;
//        PrintPolTemplate( newPol );

        if (i == 10) {
            // collect element counts by examining the resulting polymer string
            int allNrs[26];
            int cumCount = 0;
            for (int i = 0; i < 26; i++) {
                char C = (char)('A' + i);
                allNrs[i] = NrOfChars( newPol, C );
                cumCount += allNrs[i];
            }
            // get max and min element count and report the difference
            int maxVal = MaxOfArray( allNrs, 26 );
            int minVal = MinOfArray( allNrs, 26 );
            std::cout << "max val: " << maxVal << " min val " << minVal << " diff max - min = " << maxVal - minVal << std::endl;
        }
    }

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2 : based on calculation table approach [ brute force not possible ]

    for (int i = 0; i <= 40; i++) {
        if (i == 0) {
            StartStep2( polTempl );
        } else {
            OneStep2( instrData );
            if (i == 40) {
                ConsolidateEltCount( polTempl );
                // extract max and min values for element counts, and report the difference
                long long maxVal = MaxOfArray( glbEltCount, 26 );
                long long minVal = MinOfArray( glbEltCount, 26 );
                std::cout << "max val: " << maxVal << " min val " << minVal << " diff max - min = " << maxVal - minVal << std::endl;
            }
        }
    }

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}


