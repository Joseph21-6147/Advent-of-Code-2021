// AoC 2021 - day 20
// =================

// date:  2021-12-20
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../flcTimer.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef vector<string> PictureStream;

struct sPicture {
    int           nPicSize;   // square picture
    PictureStream vPic;       // pixel data
    string        sIEA;       // the corresponding enhancement string for this picture
    int           nrEnhancements = 0;       // how may enhancement steps done?
};

// ==========   DATA INPUT FUNCTIONS

// Image Enhancement Algorithm strings: hard coded
void GetIEA( string &iea ) {
    iea.clear();
    switch (glbProgPhase) {
        case EXAMPLE:
            iea.append( "..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#.." );
            iea.append( "#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####" );
            iea.append( ".#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#.." );
            iea.append( "#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#" );
            break;
        case TEST:
        case PUZZLE:
            iea.append( "#.#.##.#.#....#.#.#......###.#....####.#...###..#...#.#.###.#...#.#...##..#.#.#.##..#......#..#...#..#.###...####....#.#....#..." );
            iea.append( "#.##.#####.##..#####..###..###.#.##.#####..#..#..######..######.####.####....#.#..####..#...##..#..#.#.##.##.##.##.#.##..##..###" );
            iea.append( "....###.###.##..#...#.##..#.#..####.......#...###..#....##..#..##.#..##..#..##.###..##.#.##...#..###.###...###...#.#######.....#" );
            iea.append( ".##..#......#....#.#..##..##..#..####.#.#.##..##.##.#..#.##..#.......#.####.#.##..#..........#.#.#..##.##......##..#.##..#.####." );
            break;
    }
}

// no file reading today, just dump all the data into code
void GetPicture( PictureStream &pic ) {
    pic.clear();
    switch (glbProgPhase) {
        case EXAMPLE:
            pic.push_back( "#..#." );
            pic.push_back( "#...." );
            pic.push_back( "##..#" );
            pic.push_back( "..#.." );
            pic.push_back( "..###" );
            break;
        case TEST: // no content (yet)
            break;
        case PUZZLE:
            pic.push_back( ".#..####..#.####.####...#.##..##.##....#..#####..#.#......##....#.#.#..####...###..#..#.#.##.##...##" );
            pic.push_back( "#...##.......##..#....##.####.....####..#.#.....#.#.#........#.#######.#.##...####.#.#...#.###...###" );
            pic.push_back( "###..##.#.#.#.#....##....#.#.#.##..#.##..##.##.##.#.##...##..####.....###.#.#.#.#....#......###..##." );
            pic.push_back( "###......#.##.....#......##..######.######.##..........#.#..#.#.########.####.###.#.#...#.##.##.##.." );
            pic.push_back( ".###.#.#......######.####......#..##....#..#...###...#####..####..###.#####..###...####.#.###.#.###." );
            pic.push_back( "#.#..##.....#...###...#.##.#..#.#.#.......#.##.###....###.###.#..###...#.#..#...#..###..##.###.##..." );
            pic.push_back( "#####.#.##..#.##..####.....#.#.#.#....###....##.######.##...###....###.##.#...#....####.##.#.###..#." );
            pic.push_back( ".##...##.#....##.#..#......####.#..#.##...##...#.##.....#.#.#.###.###.##.#.##.#.###..#..#..###......" );
            pic.push_back( ".....#....#..#..####.####..#..#........#....#.#.##.######.#..#...##.###.#.#....#..#.#....##....##.##" );
            pic.push_back( "..#...####.#.#.....##.#.##.#.#...##..####.#.###.#..#.###...#.#.....###....#..#..#.#########..####.##" );
            pic.push_back( ".#..###.#.....#..#.....#.##.##..#.#####..############.#..#.#.###........#.##....#...#.#.#....#.#..##" );
            pic.push_back( ".#.#..#####.####.#.##...#.#.##....##..#.#.#..#.#.##.##.#..##....##.##.###....#.##..#.#####..####..##" );
            pic.push_back( "#...##......#####..##...#.....##..#..##......#.#######.#.##..###.#..###..#.#.#.#.#.###.#.##.#.####.." );
            pic.push_back( "##..#...####.#..#....#..#..##......##.##....##.#..#.#.##.###.#.#.....#.#..##.###.#####.#..#.#####.##" );
            pic.push_back( "#..###.#.###.##......#.####.....####.##....#.....###.#.#######.#..##.#.###.##...###....##.#.####...#" );
            pic.push_back( "..#.###.##.#.#.#..##..####.....#.##.##.##....##.####..#.###..####...#...#.#..##.##...#...#.#.##.####" );
            pic.push_back( "##...#.##.#######..##.##.#####.......#..#..##.#....#.####..#.##.##...###.#....#....#.##.##.####..##." );
            pic.push_back( "##...##.###.#.#.#.#.##.##....#.####.###..#..#.#.##..###.##..#.#.#.##.#####.####..###...#.....###..#." );
            pic.push_back( "#..#.#####...#...#...#...##.##.##..##.###..###.####.......#.#####.###..###.#.###....#.####...#.#.#.." );
            pic.push_back( "#######..####.#.........#.##.###.##.##..####...#....#..###.##....#.#.....##.####..##.#.##..#.#.#...#" );
            pic.push_back( "..###.#..#.#..#.#.#..#...#.#....#..###.#.###.#..#..#...#####..#......###.#.#..#..#.#.##....#.#.#..#." );
            pic.push_back( "#.....###...#...###.....###....#.################.##..##..#...###.##.#....#....##.#.#.#..##.#.##..##" );
            pic.push_back( ".#.###....##.#.##..#.#######..####....#.####.#.#.####..##.#####..#...#.#.######.###..#.##...#..##.##" );
            pic.push_back( "..#.#.##.#.#.#......#.#...#..#...#.##.#..##.#####.###....###.####..####.#....##.#########.....##..#." );
            pic.push_back( ".#.##..#.##...#.##.#.##.#.#.#.##...###.##.#.#####..##.#..##.#.##.#.....#..##.##..##..#..#.#.##.##..." );
            pic.push_back( ".#...#.##......##.#..##.#.#...#.##.##.####...###..#..###..........#.#.#.#.##.##.##.##..####..#.#.###" );
            pic.push_back( "........###..##...##..##.#.###....#.#......##..####.#####....#...#...##.....##.##.#..#...#####...###" );
            pic.push_back( "#..###.#.##.#.#.##.....#.###.#.###.#####.#..#.#..##..###..##.##...#...##.#...#.#..#..#.##...#.##...#" );
            pic.push_back( "##....##...#####.....#..#..#.....##.##..#......###.##...#..#.######.#..#.######..#..#.#.....#....#.." );
            pic.push_back( "#...#.#..##..##.#....#.#..#...##.#####..####.###.##...#.###...#.##.#.##.#.#...###.##.####..###.#.###" );
            pic.push_back( ".#..##.#.###..####.#..#..##.#..#.##.#.#...#.##.#.###.##..###.#.##..##..#..#.#.###.##.###.#..#..##.#." );
            pic.push_back( "#....#.####..##.#..#.###.####..#.######.#.##.#.###...##...#.#..#.##.##....###...#.#..#.#...#..######" );
            pic.push_back( "###.#..##..#######.##..#.#..#.###..##.#..####.#.###.#.#.##.#....#......#...#.#.##.#...###.#.##.#...." );
            pic.push_back( ".#.#####..##..#.##.....##..##.#....##.##.##..##.####.#...##..##...#...####..####.#.##...#...#....#.." );
            pic.push_back( "...#.###...##..##...#.#..####..#.###..###.###.......#####..##..###.##..##.#.....####....#.###.#..###" );
            pic.push_back( "#.#..##..#####.#.##.#..####....#...#..####.#.##....#..#..#.##.##.#.##..#..#.#.#..#####..##...#.#..#." );
            pic.push_back( "..#..###...#.#......##########..#.....#.....#.###....###..#.#..###.#..###..#.#...#.##....###...#.#.#" );
            pic.push_back( "#.#...#.##..#..##...#######.##.##.#####...#####..#...#....#....#..#.#.##.#...........###.##.###...##" );
            pic.push_back( ".#..#.#.###..#....##..#..##.##..#.##..####..#...####.##..#..#####...#.###..#..##.#...##..#.#.#.###.#" );
            pic.push_back( "###..#..#####..#....#.##....##.#...........###..##...##.##..#....##.###..##.#.#.#..#..##..###...##.." );
            pic.push_back( ".#...##.#.#...##.###..##.#.#.########..####.#.#.#.##.#..#.#####...#.#...####...#.##.##.###...#....#." );
            pic.push_back( "#####..#.#..######...#....##.#..#.##..#...#####...#....####..###.#..#..###......###.#.....###.######" );
            pic.push_back( ".##.#....#...#.##.#.######..#....##..#.###...##..######.#####..##.####.##...#.....###..###.##..#..##" );
            pic.push_back( "..###.##...#.###.#..####....###....#.##.#..##..#..#.#...#.##.......####.#....##.#....#...###...#.#.." );
            pic.push_back( "##..##.###..#....##.##...#....#.#####.#...#.#.#.###.#.##.#####.#...##....#....##..##.##..##.##.#..##" );
            pic.push_back( "#...###.#.#.##..........##...#..#####.#...###.##...#...###.##.##...####.##.........#.#######.#.#.#.." );
            pic.push_back( "..#....##...##.#.#..##.#...#.######.##.###.###..#..##.#........##..###...#.##..#######.##.....##.##." );
            pic.push_back( ".#.##.##.#..#...#.#.##.#..##.##..#..###.##.###.#.###..##.###.#..##.#.....###.#.#.###...#########.#.#" );
            pic.push_back( "..#.##.#..###..##..#..##.#...#..##..##..#..#.#....#..##.#...#...##..#....#..##.#...#.##.#.########.." );
            pic.push_back( "#..####..####..##.#.##..####.#.#.#..#.....###.###..#....#.#####..####.#.....#...#....###.#.#.......#" );
            pic.push_back( ".###.#...#.###..#......#..#..####.##...###...##...##...#.#..#....###......###.....#.####...##.#.##.#" );
            pic.push_back( "##..#.#.......##..####.#..#...#.##..###...#.....##..#....#..###.......#####...##.###.#####..#..###.." );
            pic.push_back( "..#.#.##.#.#.##.##.####.##.#.#.......#..##.#.#.##....#.##.####..#######..##.#....#..#.##....####...." );
            pic.push_back( ".#..#..#........####..#.#.#.######.###.#...#.#.#..#.#......##..#.#...####..#.#.#.#..#....##.#.#..#.#" );
            pic.push_back( ".###.#...#......###.######.#....#######..#.##...###..#.##..#..###.##.#.....#.######..##.#.#...####.." );
            pic.push_back( "#.#.##..###....#.....#..###..#.#.###.#...##.#.#..##.#.#####.#..##.....#.#####.#..#..##.####.##....#." );
            pic.push_back( ".#####....###..#..##..###.####...###..#.######..#####.##..#.####....#.#..###.#.#...#..#.#.##.#.#..##" );
            pic.push_back( "..#####.#..#.######..#.##.##..#.##...#.####...####..#..#.###....##.#...#.##.#.#.#.#......#...##..##." );
            pic.push_back( ".#.#....#...#####..#.#.....#.#####..#..#..........##.#..###...#.#...#########....##.##..##.#..##..##" );
            pic.push_back( "##....#.#...##.....##.#.#..#...####.###.......#####.#..####.#..###..##..##.#..##.#.###....#.##..#.#." );
            pic.push_back( "#.#..#..#.#..#.#####..#..###.#.###..##.#.#.##...####..#.#...#.##...##.####..##.###....######.#.##..." );
            pic.push_back( "###..#.#..##.....#..##.#.#.##.##.##.##.#.##.##..###..##.....###..###.###..###.#..#....#.#......#...#" );
            pic.push_back( "##....#.#..#.####.##......##...###.#...#.###..##.#..#.##..##...#.##.........##.####.###..######..###" );
            pic.push_back( "####..#....##.#####.#...#.##.....#..#....######..#......###.#..##....#.#.###.###.#######.#.#....###." );
            pic.push_back( "#####.#.#####.###.#..#.#######.#.#.#.####.#####.####.##..#.########..###..#.#####.#.#.##..#..##...#." );
            pic.push_back( "#.......#....#.##.#...##.....#...#######..#.##...##.#...##.#.#.##..###.##..#.#.#..#.#.###...###..#.." );
            pic.push_back( ".###.##......#..#....#.#.####.#.##.##.#..##...###.#...#.#...##.###.#.###.###..#..#.#..#..#...###...#" );
            pic.push_back( "####.#.##..##.##.#..##....##....##...#...#...#.######.#...#...##.#.##..#.##.##..##..#.##...##..##.#." );
            pic.push_back( "###.###.##.#...##.##....#..####.#...#.#....#######..##.#.##.###..###.#..#..#..#.##.#..###..#.#######" );
            pic.push_back( "#.##...#########...#.#.#..##.#....##..#.#..##.#..####....##.#.#.###.#..#..#.#..##...#..###.###.#...#" );
            pic.push_back( "##..#.##....#....####...#.#.#...###.#....##.#.###...#..#.####..##.#.###...###..###..#.##..#.##.##..." );
            pic.push_back( "###.#.##..#.#.#.#######.####.#.####.#####...#....#..##.###..#...#.#..####..###.##.....#....##.....#." );
            pic.push_back( ".######..#.##.#####.#.##.#...#...#..#####....#.....#.####.###..#...#.##..###.#.##.###.#...#.#..##.#." );
            pic.push_back( ".#.#.##.###..#..#..###.#.#.#.#####..##.#...#..#.....#.###....####..#.#..#.#......#.##.#.###.#...#..." );
            pic.push_back( "#####..#.##..####.###...##...#.###..#.##.###..####..####.#.###########.##.#...###.###.######..#..#.#" );
            pic.push_back( ".##.#.##...#.##.##.#.##..#..##.##..#....#.#.##.##.....#.#.#..#..####.###.#..#..###.....#.###...##.#." );
            pic.push_back( "#####....###...#..#..##.#..#..#.##.#.#.#..##.##.####.###..###.#.#.##..##.#...###.#..#.##.#.#.#.#..##" );
            pic.push_back( "####...#.##...##.#.....#...#####.##...###.#.#.#.#..##.#...#.#..#.#.###...#.##.#...#..###.##.####...." );
            pic.push_back( "##.####.#.##.###..#...#..####.###....###.#.#.####.#..##.#.#.#.##.#.#####.##..#..##.#.#######...#..##" );
            pic.push_back( ".....#...#.#####...#........#.#...####.#.#.#.#.#..##.#.##.##.........#...###...###.#.....#...#....##" );
            pic.push_back( "......##.#####..##...##.####.#.###.##..#.#..#..##.####....###.#.......#.##...#...#.###.##.#.#####..#" );
            pic.push_back( "....##.###..#####.##.#..####.#.....#.##.###.##..#######.###..##...#...##....###.##..###...#.....#..#" );
            pic.push_back( "####.#.#....##...####..#.#.#.##..#.#.#.....#...#.#.##.#..##......###.#...##...######..#####.#....###" );
            pic.push_back( "##..#......#..##.###...#..#.#..###.###.##...####.....#.#...#..##....#...#.#...##....#..##......##.#." );
            pic.push_back( "#..###....##.###.##.#......#.....###...#..#####..###..#..#.#.##..#.##....###..##.#...####..##.#####." );
            pic.push_back( "...##..#.###..#...##.##.###.###.#....#.###.####.#...#.#####.##.#...#..##.####.#.##.#.#...#.#..#.#..#" );
            pic.push_back( ".#......#####..#..###.###.......#.##.#..#....##.###.##.#....##.#.#######..##..#....#..#.###.##.#####" );
            pic.push_back( "...#..##..#..####.###...#...##..####.#####..#..###...##.##.##...#..#.###...##.#....#.####..##..#.###" );
            pic.push_back( "####.#.#..##..###..#...###...##...#.#####.###.#.#..#.##..##...##..###.....###.##.###....#..##.###..#" );
            pic.push_back( "#.###.######..#.#..###..##....####.###.###.###.#####..###.#.#..###.#.####.###...##.##.###..##.#.#.#." );
            pic.push_back( "####.....#......#......#.#.#####.#...#..#.##...#####..#..########..#####.##..###...####..#..#.#.#.##" );
            pic.push_back( "..#...#...#####...##.#.#.#..##..#..#.#......##.#####...#...###..###...#.####.#.#.#.#.#.....#..##.###" );
            pic.push_back( ".###...#...#.###.#.#...#..##..#..###.###.#.#..##..#.....##...##.####...###..#.....#.##...##..#.##..." );
            pic.push_back( "###...####.#.#......#####.#.##.#####.#.#####...##.##..#...#.####..#..##.#.##.#...#..#.#.#.#.#.###..#" );
            pic.push_back( "....#####.###.##..#.#..#####..###..#..###..###..#...####.####.##..####.##..#.#.###.###..###..######." );
            pic.push_back( "##.##..#....#.#..#.##.##.###....#.#########.#.#...#..###.#..###..#....##.#..#.##..###.###.#...##...." );
            pic.push_back( "#.##.##.###.....####.###.#.#...#####..#.##.###.##.#....##.##.###.#...#.###..#..##.####.#...#...##..." );
            pic.push_back( ".....#####.........#..#..#...##....#....###.#.###.#######..##....###.##.........###.#.##..##...###.." );
            pic.push_back( "..###..####.##.#.#..##..#..#.#.##.####...##..#..###...#......#..######.####....###.#.###.#.##.##.#.." );
            pic.push_back( ".####.#####.##.#.####......###....#.#......#.#.##...##...#..#.#..##...#..##.#...##..###.###.##.##.##" );
            break;
    }
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintIEA( string &iData ) {
    cout << "IAE: " << endl << iData << endl << endl;
}

// output to console for testing
void PrintPictureStream( PictureStream &dData ) {
    for (int i = 0; i < (int)dData.size(); i++)
        cout << "Picture - index: " << i << " = " << dData[i] << endl;
    cout << endl;
}

// for testing example cases - NOTE this function turned out to be needed for part 2 as well :)
void DisplayPicture( PictureStream &dData ) {
    PrintPictureStream( dData );
}

// ==========   PROGRAM PHASING

// populates input data
void GetInput( sPicture &pic, bool bDisplay = false ) {

    GetIEA( pic.sIEA );
    GetPicture( pic.vPic );
    pic.nrEnhancements = 0;
    if (pic.vPic.size() != pic.vPic[0].length()) {
        cout << "ERROR: GetInput() --> not square..." << endl;
    }
    pic.nPicSize = pic.vPic.size();

    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintIEA( pic.sIEA );
        PrintPictureStream( pic.vPic );
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

// pictures need to be square. difference between newsize and orgsize must be even number
void EmbedPicture(  sPicture &orgPic, sPicture &newPic, char cDefaultPix ) {

    // copy relevant info from inPic to outPic
    newPic.nPicSize        = orgPic.nPicSize + 2;
    newPic.nrEnhancements  = orgPic.nrEnhancements;
    newPic.sIEA            = orgPic.sIEA;

    // grab convenience variables
    int orgSize = orgPic.nPicSize;
    int newSize = newPic.nPicSize;

    // fill new pic with default pixels
    newPic.vPic.clear();
    for (int y = 0; y < newSize; y++) {
        string aux = "";
        for (int x = 0; x < newSize; x++) {
            aux.append( string( 1, cDefaultPix ));
        }
        newPic.vPic.push_back( aux );
    }

    int startX = (newSize - orgSize) / 2;
    int startY = startX;
    // copy old pic into new pic
    for (int y = 0; y < orgSize; y++) {
        for (int x = 0; x < orgSize; x++) {
            newPic.vPic[y + startY][x + startX] = orgPic.vPic[y][x];
        }
    }
}

// converts a string with only '0' and '1' chars to corresponding int value
int BinToInt( string &binString ) {
    int factor = 1, result = 0;
    for (int i = binString.length() - 1; i >= 0; i--) {
        if (binString[i] == '1') result += factor;
        factor *= 2;
    }
    return result;
}

// return the pixel '.' or '#' at (x, y). If any is out of bounds (picSize),
// then cDefaultPix is returned.
char GetPixel( int picSize, PictureStream &p, int x, int y, char cDefaultPix ) {
    char result;
    if (x < 0 || x >= picSize ||
        y < 0 || y >= picSize) {
        result = cDefaultPix;
    } else {
        result = p[y][x];
    }
    return result;
}

// enhance the pixel from orgPic at (indexX, indexY) using the IEA string
// of the sPicture object to get the right pixel
char EnhancePixel( sPicture &orgPic, int indexX, int indexY, char cDefaultPix ) {

    string sIndex = "";
    int nIndex = 0;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            char tmp1 = GetPixel( orgPic.nPicSize, orgPic.vPic, indexX + x, indexY + y, cDefaultPix );
            char tmp2 = ' ';
            switch (tmp1) {
                case '.': tmp2 = '0'; break;
                case '#': tmp2 = '1'; break;
                default: cout << "ERROR: EnhancePixel() --> invalid pixel value: " << tmp1 << endl;
            }
            sIndex.append( string( 1, tmp2 ));
        }
    }
    nIndex = BinToInt( sIndex );

//    cout << "EnhancePixel() --> (" << indexX << ", " << indexY << ") - 3x3 string = " << sIndex << " - calculated index = " << nIndex << endl;

    return orgPic.sIEA[nIndex];
}

// newPic must be a an empty sPicture object / previous contents is lost
void EnhancePicture( sPicture &orgPic, sPicture &newPic ) {

//cout << "EnhancePicture() --> at start. Original picture: " << endl;
//PrintPictureStream( orgPic.vPic );
//cout << endl;

    char cDefaultPixel = '.';
    if (orgPic.sIEA[0] == '#' && orgPic.sIEA.back() == '.') {   // the default pixel is alternating per enhancement step...
        cDefaultPixel = (orgPic.nrEnhancements % 2 == 0) ? '.' : '#';
    }

    // first put org picture in a slightly larger temp picture to enable enhancement
    sPicture tmpPic;
    EmbedPicture( orgPic, tmpPic, cDefaultPixel );

    // copy relevant info from tmpPic to newPic
    newPic.nPicSize        = tmpPic.nPicSize;
    newPic.nrEnhancements  = tmpPic.nrEnhancements;
    newPic.sIEA            = tmpPic.sIEA;

    int tmpSize = tmpPic.nPicSize;
    int newSize = newPic.nPicSize;
    newPic.vPic.clear();

    // fill new pic with '+' (dummy for debugging)
    for (int y = 0; y < newSize; y++) {
        string aux = "";
        for (int x = 0; x < newSize; x++) {
            aux.append( string( 1, '+' ));
        }
        newPic.vPic.push_back( aux );
    }
    // enhance temp picture into new picture
    for (int y = 0; y < tmpSize; y++) {
        for (int x = 0; x < tmpSize; x++) {
            // enhancement per pixel
            char tmpPixel = EnhancePixel( tmpPic, x, y, cDefaultPixel );
            newPic.vPic[y][x] = tmpPixel;
        }
    }
    // advance counter of the newPic object to keep track # of enhancement steps
    newPic.nrEnhancements += 1;

//cout << "EnhancePicture() --> at finish: New Picture" << endl;
//PrintPictureStream( newPic.vPic );
//cout << endl;
}

// counts and returns the nr of '#' pixels in pic
int NrPixelsLit( sPicture &pic ) {
    int nResult = 0;
    int picSize = pic.nPicSize;
    for (int y = 0; y < picSize; y++) {
        for (int x = 0; x < picSize; x++) {
            if (pic.vPic[y][x] == '#')
                nResult += 1;
        }
    }
    return nResult;
}

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    sPicture picData;
    GetInput( picData, glbProgPhase != PUZZLE );
    cout << "Data stats - size of iae " << picData.sIEA.size() << endl;
    cout << "Data stats - size of pic " << picData.vPic.size() << endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    sPicture workData1, workData2;
    workData1 = picData;
//    DisplayPicture( workData1.vPic );

    for (int i = 0; i < 2; i++) {
//        cout << "main() --> enhancing for step " << i << endl << endl;
        EnhancePicture( workData1, workData2 );
        workData1 = workData2;
//        DisplayPicture( workData1.vPic );
    }

    cout << "Answer 1 - # pixels lit: " << NrPixelsLit( workData1 ) << endl << endl;


    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    // get fresh copy of input data
    workData1 = picData;
//    DisplayPicture( workData1.vPic );

    for (int i = 0; i < 50; i++) {
//        cout << "main() --> enhancing for step " << i << endl << endl;
        EnhancePicture( workData1, workData2 );
        workData1 = workData2;
//        DisplayPicture( workData1.vPic );
    }

    cout << "Answer 2 - # pixels lit: " << NrPixelsLit( workData1 ) << endl << endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}





