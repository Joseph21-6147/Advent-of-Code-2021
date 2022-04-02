// AoC 2021 - day 13
// =================

// date:  2021-12-13
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../flcTimer.h"

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

struct sDot {
    int x = -1, y = -1;

    // had to insert this operator due to compiler complaints... :(
    bool operator == (const sDot &rhs) {
       return (this->x == rhs.x && this->y == rhs.y);
    }
};
typedef std::vector<sDot> DotStream;

struct sInstr {
    int index = -1;
    bool horizontal;   // either horizontal or vertical
};
typedef std::vector<sInstr> InstrStream;

// ==========   DATA INPUT FUNCTIONS

void ReadOneDotLine( const std::string &token, DotStream &dData ) {
    // parse the coordinates out of token
    // convert to numbers and insert into dot list
    sDot dot;
    size_t splitIndex = token.find( "," );
    dot.x = atoi( token.substr( 0, splitIndex  ).c_str() );
    dot.y = atoi( token.substr( splitIndex + 1 ).c_str() );
    dData.push_back( dot );
}

// only the "x=327" (example) part is passed in token
void ReadOneInstrLine( const std::string &token, InstrStream &iData ) {
    // parse instruction out of token
    // convert appropriately and insert into instruction list
    sInstr instr;
    size_t splitIndex = token.find( "=" );
    instr.horizontal = (token.substr( 0, splitIndex ) == "y");
    instr.index = atoi( token.substr( splitIndex + 1 ).c_str() );
    iData.push_back( instr );
}

// this function is used to input the data from the text file
void ReadData( const std::string &sFileName, DotStream &dData, InstrStream &iData ) {
    // open filestream
    std::ifstream dataFileStream( sFileName );
    // read data stream
    dData.clear();
    iData.clear();
    bool bDone = dataFileStream.eof();
    std::string token[3];
    while (!bDone) {
        // get next token and determine what to do
        if (dataFileStream >> token[0]) {
            if (token[0][0] == 'f') {
                // it's an instruction line - read next two tokens and process the last one
                dataFileStream >> token[1] >> token[2];
                ReadOneInstrLine( token[2], iData );
            } else {
                // it's a dot (coordinate) line - process it
                ReadOneDotLine( token[0], dData );
            }
            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintDotStream( DotStream &dData ) {
    for (int i = 0; i < (int)dData.size(); i++)
        std::cout << "Dot - index: " << i << " is at ( " << dData[i].x << ", " << dData[i].y << " )" << std::endl;
    std::cout << std::endl;
}

// output to console for testing
void PrintInstrStream( InstrStream &iData ) {
    for (int i = 0; i < (int)iData.size(); i++) {
        std::cout << "Fold instruction - " << (iData[i].horizontal ? "HOR" : "VRT") << " at : " << iData[i].index << std::endl;
    }
    std::cout << std::endl;
}

// for testing example cases - NOTE this function turned out to be needed for part 2 as well :)
void DisplayDots( DotStream &dData ) {
    // first determine the dimensions (sizes) of the map to display
    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;
    for (auto elt : dData) {
        if (elt.x < minX) minX = elt.x;
        if (elt.y < minY) minY = elt.y;
        if (elt.x > maxX) maxX = elt.x;
        if (elt.y > maxY) maxY = elt.y;
    }
    // now display in these sizes
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // performancewise this is a disaster, but it works
            std::cout << (std::any_of( dData.begin(), dData.end(), [=]( sDot d ){ return d.x == x && d.y == y; }) ? "#" : " ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgPhase)
void GetInput( DotStream &dData, InstrStream &iData, bool bDisplay = false ) {
    switch (glbProgPhase) {
        case EXAMPLE: ReadData( "input.example.txt", dData, iData ); break;
        case TEST:    ReadData( "input.test.txt"   , dData, iData ); break;
        case PUZZLE:  ReadData( "input.txt"        , dData, iData ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintDotStream( dData );
        PrintInstrStream( iData );
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

void FoldOnce( const DotStream &input, DotStream &output, bool bHor, int nIndex ) {
    if (bHor) {   // fold horizontally
        for (auto e : input) {
            sDot tmp = e;
            if (tmp.y > nIndex) {
                int nDiff = e.y - nIndex;
                tmp.y = nIndex - nDiff;
            }
            output.push_back( tmp );
        }
    } else {    // fold vertically
        for (auto e : input) {
            sDot tmp = e;
            if (tmp.x > nIndex) {
                int nDiff = e.x - nIndex;
                tmp.x = nIndex - nDiff;
            }
            output.push_back( tmp );
        }
    }
    // make output vector unique, since it might contain doubles now
    std::sort( output.begin(), output.end(), []( sDot a, sDot b ){ return
         (a.x < b.x) || (a.x == b.x && a.y < b.y);
    });
    output.erase( std::unique( output.begin(), output.end()), output.end());
}

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    DotStream     dotData;
    InstrStream instrData;
    GetInput( dotData, instrData, glbProgPhase != PUZZLE );
    std::cout << "Data stats - #dots         " <<   dotData.size() << std::endl;
    std::cout << "Data stats - #instructions " << instrData.size() << std::endl;

    // dyn. alloc an array of DotStream type objects, one larger than the # of instructions
    DotStream *aux = new DotStream[ instrData.size() + 1];
    // set 0'th element to initial data
    aux[0] = dotData;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    for (int i = 0; i < 1; i++) {
        sInstr instruction = instrData[i];
        FoldOnce( aux[i], aux[i + 1], instruction.horizontal, instruction.index );
        std::cout << "# of dots: " << aux[i + 1].size() << std::endl;
    }

    tmr.TimeReport( "Timing 1 : " );   // ==============================^^^^^

// ========== part 2

    for (int i = 0; i < (int)instrData.size(); i++) {
        sInstr instruction = instrData[i];
        FoldOnce( aux[i], aux[i + 1], instruction.horizontal, instruction.index );
    }

    tmr.TimeReport( "Timing 2 [ without displaying ] : " );   // ==============================^^^^^

    DisplayDots( aux[instrData.size()] );

    tmr.TimeReport( "Timing 2 [ with displaying ] : " );   // ==============================^^^^^

    delete []aux;

    return 0;
}


