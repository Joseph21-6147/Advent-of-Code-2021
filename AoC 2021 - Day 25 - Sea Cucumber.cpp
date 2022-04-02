// AoC 2021 - day 25
// =================

// date:  2021-12-25
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

typedef vector<string> DataStream;

int glbChartWidth  = 0;
int glbChartHeight = 0;

#define EAST  '>'
#define SOUTH 'v'
#define EMPTY '.'

struct sCucumber {
    char type;   // EAST or SOUTH
    int x, y;   // position on map

    bool canMove;
};

sCucumber *glbMap = nullptr;

// ==========   DATA INPUT FUNCTIONS

// no file reading today, just dump all the data into code
void GetChartData( DataStream &chart ) {
    chart.clear();
    switch (glbProgPhase) {
        case EXAMPLE:
            chart.push_back( "...>..." );
            chart.push_back( "......." );
            chart.push_back( "......>" );
            chart.push_back( "v.....>" );
            chart.push_back( "......>" );
            chart.push_back( "......." );
            chart.push_back( "..vvv.." );
            break;
        case TEST:
            chart.push_back( "v...>>.vv>" );
            chart.push_back( ".vv>>.vv.." );
            chart.push_back( ">>.>v>...v" );
            chart.push_back( ">>v>>.>.v." );
            chart.push_back( "v>v.vv.v.." );
            chart.push_back( ">.>>..v..." );
            chart.push_back( ".vv..>.>v." );
            chart.push_back( "v.v..>>v.v" );
            chart.push_back( "....v..v.>" );
            break;
        case PUZZLE:
            chart.push_back( ".>vv...>>.>.v...v..>.vv>......>>.v>.vv.v......v.v.vv.vvv...>v.v.>v..>>....vv......>....>>.>v.>......>v>.v>v...v>.>..>.....>>.v...>....>>.v." );
            chart.push_back( "..v..vv>v..vvv>..>>v.>.vv..v>vv>>.v..>v...v...v.>v..v.>...>v.v.v..v...v.v.vv..>v...>v.>..>.>.v>>vv..v.v..>.v>vvv.vv>.>>v....>>..>>v..v>v..." );
            chart.push_back( ">...v>>>..>.>vv.....v>.v.>>>>>.v..v...vv>.v>v..v..v>..>.>..>.>v..>>..vv..>>..v..>v..>v.v>>.>.>>v....vvv.>.>..v.v.>v..>>>vv>..v..v>..>v.>..v" );
            chart.push_back( "vv.>v>>.....>.v..>.>>v.>v....>>>vv.v..>.>....>>v...v>.>v>>..v..v.>vvv>..v...v......>>..v>>>.v....vv.>v.>v>v>v>..>v>......>.v>..v.>>.v.v.>>>" );
            chart.push_back( "v.vvv>.v...v..>>.v.vvv>.vv...v...>.>.v>>.>.>v..v..v.......vvv>..v...vv........vv.>.>v>...v>>v....>.>v>.v>v.>>>>>v..v>>>v.vv.>...>.v>v>.>v.." );
            chart.push_back( "....>....>.v.v..v.v>>.....vv..vvv>>..v.>.v>...>>...v.>>.v>>.>>>>..v>v>.>.>..v..>v.>.>.v..v>...vv>v...v.>>v>..v...>vvv.v.>>...>>vv>..>.>...v" );
            chart.push_back( "..>>>>.v..>v..v....>>>>>.>....>>v.>>..>.v.>v..>..>>>..v.vv.>.>v.>>.v..>...vv.v>.v.vv..>..>>v>>..>>vvv..>.>>v...vv.v.>>..v>..>>.>v..>v.>...>" );
            chart.push_back( "..v.v>vv>vvv.>.>>.>.v.>...v>.v>v...vv>.v..>>.v.v>...>.>...>>...>>>>.v..v..v..v...v..vv.vvvv.>.>....v.>....v>..>vv.>v.....>.>>vv..>.v....v>v" );
            chart.push_back( ">..v..>>v>..vv.>vv.vv..v>>>...>vv....vv>.>>.vv>>>>>v.v......v>>v.>vvvv.....>v..v.v>..v..v....vv>..v...vv.....v.....>>>vv..>.>..v.v.>v.....>" );
            chart.push_back( "v.>v...>vvv.>.>....vvv>>.>>v>..>.>v>v.>>.>.>..>>.>v..>>..>v...>>.>.vv>.......vv.>v>>>v>>v>..vvv>v>.>vv.>..>.vvv>>>..>>>.vv...>...v>.v>>>.>>" );
            chart.push_back( "..>>.>v>v..>..v>>...v.>..v....v>>>..v>vvv.v>...v>.v>...v.>>.vvv...v>...v.....vv>..vv.v.>..vv>.........>vvv.v>.>.>...>.>v>vv.v>>v.>vv...v>v>" );
            chart.push_back( "v..>>........>>.>.v..v..>>v>.v.v..v>..v.>vvvv.>.vv......>v>>.>.>..v..vvv.v.v.>.v..>v..>.v..v>...v>v>.v>.v.>>vv>.vv....>v..>....v>.>.vv>.v.." );
            chart.push_back( ".v..vvv.>vv.vv..>.>.>>....>vv..v..vvv.v..>.>.>.v>v.vv....vv.>v>vv.v.v.v..>v>v.v>..v....>>.v...>.....v..>.>vv........v....>v>..>.>>>v.v..v.>" );
            chart.push_back( "......>>.v..>>>v>>v>..>.>>>>...v.>>..>v>v>...>>vv>.>vv..>.....v.>.>v>..>...>>.v..v>vv.v.>.>>..v.>v..v.>>vv>v.>v>.>vv>.v>..v>..vvv....v...>v" );
            chart.push_back( ">...>v.>....vv.....v>>...v..>>...v...vv>.v>>...vvv>.>>..>v.v>..>>v>.>v..vvv>>.>v.>...>v>...>vv>>.>>>>v.v.>>v.....v..v...v.>.>>>v.>..v>>>.v." );
            chart.push_back( "..v>..>v.>..>vv..>>v..>.v.>vv...>v.v..>vv.v....v.>v>...v..>>.>v.>.v>v.........>v>..v..vv.>v.v.>v...vvvv.>v>....>v.v>..v..v.>v.>vv>.vv.v>v>." );
            chart.push_back( ">v.v>vv.v>.v....>.>.>.vv..v.vv>....v..>vv>>..>v.>v>.>.vv..>v>vv>>..>..>v>..>.vv.v>.vv>.>..v.vv>..vv.>.>..v.>.v>.v..v>......>.>..>>v.vvv..v." );
            chart.push_back( ">v.>v..vv.>.v>v.>.vv>vv..vvv.v>vv..v.....>.v>v>v.>.....v>....>v.>vv>>v>vvv>....v>vv.>vv........>v>>..>.>v.vv>v.......v...>..>>....>v.>.v.>." );
            chart.push_back( ".>>..v.>>.vv.v.vv>..>....>....>..vv....>..vv..>...vvv.>>>.v.....v...>>>.>.>vvv>.>..>>..>...v.v..v>..v>>.v.>>..v>..v..>.v.v.>.>v>>>v.v.>vv>." );
            chart.push_back( ".>>....vv.>v>v..>..v.>v>vvv>.>..>..>>vv...v>v>v..vvv.>v>v>v.v...v>....v.>.>vv...>..>..>.vv>..>v.>>.>..vv.v.>....v.v.>v>>>v>..>vvv..vvv..>.." );
            chart.push_back( ".v..v..vvv.v..vvv...>v..vv...>..v..>>.v...v.>>>>.>>vv.v>..>v..v.>>v.>....v.v>.>v..vv.>v>>...>.vv..v.v.v>v>v.v....>.vv.>v..vvv..v>v>...v>v.>" );
            chart.push_back( "...v>.>v......>.v..>v..>>..>..>vv........v>>>>>.v....vv.v.v.>v..>.>v..>.>.>>>.>.>>>v.>..>>>..>>>.>>vv....v>..>.v..v>v.>v.>v>v.>>>.>>>>.>.>." );
            chart.push_back( "...vv.v>...v..>>.>vv>v>.>>.v>..>vv.>v>>v>...>..>>.v.>..>.v.....v.v.......v..>>...>.>.v..vv..>vv.>.vv.>.v.>v...vv...vvv..v>....v.>v.v>>.v.v." );
            chart.push_back( "..>.vvv>v.v>v.v.>>vv..>.>.v>....>..v>.v..>>>v.>..v>v..>.v>>.v...>....>>v....v..>>....v.v..>..>..>v..>>....v..>vvv.>>>.>...>>>.....>v.>vvvv>" );
            chart.push_back( ">.>>>v..>.>v.>v.>.........>.v..vv>>vv...>.v.v.>...vv....>.vv>....>>.v.>.v.v.vv.>..v...>>>>v..vvv.>.v>.>.v>v>>>>>v>>.v..v..>.>>..>...v.>v>>." );
            chart.push_back( ">.>.>v>>..>.>v>v.v....v.>...v>v>v>vv.>v>.v.........>.>v..>.v..>>>vvv.v.>>.v...vv>v>>.v.v>>.>.>>v.>v.....>>.v..>vv..v.v.v.>>v>>>.>vv.>>vvvv>" );
            chart.push_back( "v>.>....v.>v...v>>v.>vv..v....>>.v.>..v......>.>vv.........v..v>.>...vv>..v....>v>>.v...>.vv..>v..>>v...v.v>......>.>v>.v>>v>>.>.>v....>>>>" );
            chart.push_back( ">v.>v.v>v...>..>vv...v>.vv.>>>>v.v..v...v....v>v>vv>vvv.>...>>vv..v>>..>>..vv.>vv>>...v...>v...v>...v..>>v.v.v>v.v..v.>vv>..>>>...v.vvvv.>." );
            chart.push_back( "......v>>....>.v>>.>..>...vvv....v.>v.vv>>...v..>...v...vvv>>v.>>>.>>..vv....>vv.>>.....>..>.>v..>>.v>>>.>vv.....v..>.vv>.....>.v>..vv.>.v." );
            chart.push_back( "v..>v.>..>....>>..v..vv...v>..v>>>..>.v>.>.v>..>v.......vv..>...v>.>.v>..v>.vvvvv........v>.>>.>...>....vvv>>vv.>.>.>...>.>>vv>.>..>v>.v..v" );
            chart.push_back( "..v>>...vv.>vvvv.>....v...v>>...v>..v....>v.........>>..v>>>>..>.>..>..>v......v..>v.v..vv>>..>.>..>>....>>.....vv..v>..v..>.vvvv.>.>vvv.v." );
            chart.push_back( ".>..v..>>.v>vv.v.vv>>v>....>.>.>>....v.v>v>.....vv.>.vv....>v..>.>>>v>.>>v..v.vv>..>.v....vvv.>>v>>.>>>...vv...vv.>>>.>vv>>...>.v.v>....v.." );
            chart.push_back( ">.v.>vv...vvvv.>.>v.>..v.....>v.>>v>.v>..>.v>>.....>.>.....vv>>v....v...v...v..vv..v.....v>....v...v.>...>.>v>.>..>vv.v>v.>>.v..v>.>...v>.." );
            chart.push_back( ".v..vv..v...v...>.....v>.>.v>>.>..v..vv.>>v...v>v>>v.>v....vv>>vv>...v>vv>.v....>v.v>>.>>v>vv>.vv.v>v.>v.v>..v........v.>..vv>......>......" );
            chart.push_back( ">>.>..>..vv>..v..v>v.>v.>..v.>..v.>...>>v>..v.>.>.>>.v.v.>>vv>v.>v.>...>>v>v>>v....>..>....>v......v..v.>>.vv>.>...>>vv....v>.v>..>...v>.>." );
            chart.push_back( "...v..>.>v>.>v..vv.v...v.v.>..>.>..>.>vvv...>..v.v..v>v...>..v...>.>>.v.v>>>>v.v>.v.v....v....>v>.>v.......>v>>.v>...>v>..>v.>v...>v>v.>.v." );
            chart.push_back( ">.>.vvvv.v>>...>...>>.>>>..v......>v..>.vv.>>.v..v.v...v..vvvv.>.vv...v.vv.v....>.v..>v>>.>>v>>>.v>...v.>vvv.v>.vv>..>....v>>..v...>>.v.>>v" );
            chart.push_back( "v...>.vvv.>.v>>>v..>v......>>.v...>>.v.>>vv.v.v.>>.v..>vv>>.v...>>..v.>........v.v..>>..v.>>>.....>...v.v.>.>>.v.vv.v>v..v.v.>.>>....v.v.>v" );
            chart.push_back( ".>v>...>.v>.v..>..>.>.>v.>.vvvvv...v...v.>.v>..v.>.vvv...>>v>>>v.v.vv...>v....>vvv>.v.v..>.v.v>..v.v>v.>vv..v.>v.>v...>>.v>>vv>v>vvv>>vv.>." );
            chart.push_back( "v>..v..vvv.vv>v.v>..>>.>..>..>...v.v>..>>..v.v...>v.>...>>vv.>vv..v>>..>>>>>.v.v.v...>>.>...>>...vv>..>>>..>..>.>v.v..vv.v......>..>>v>>v.>" );
            chart.push_back( ">v.v.>v>>......v....v.>.vv>v.v>...>..>>.v>..v.vvv...........>>>vv...>.>.v.>>v..>.>>>>vv....>.>..v.>vv>..>.v...>v>.....vvv.......>.vv>>v>v>." );
            chart.push_back( "vv>...v...>.>v..>.>v......>>>>vv...>v..vvv..>v..v>...>...>..vv>.>..v>v..>..>>..v..>>v..v....>.>..vv.>vvv>.vv..vv...v>v.>>vv...v.v.>.vv>vv.." );
            chart.push_back( ">vv>..>>v>.vv..>vvv>..v..v.>>>.v..>v..v..v>vvvvvvv>v..>.>v>..>>.>>v..v.v>..v.....vvv.vv..>>.>...v.....v>....v>..>..>.>vv..>>...v.>v.v...>v>" );
            chart.push_back( "vv.v.vvv....>.>.v>.>.v.>v.>.>.v.vvv...v...>vvv.>>..v>.v.......>...v>>v.v.....v.>>.....v>v..v.v..>v.....>>.>>.>.v>.....v>....v..vvv.vv.v..v." );
            chart.push_back( "...vv>v>vv.>v.>.v>.vv>.>v.vvvv.>...v...>v>......>..v.>..v..>..v...v.>v...>>.vvvvvv.v..v..v.vv.>.v...>v...v.vv..v>..v.v>>.vv...v>>>vv....v.." );
            chart.push_back( ".>.v....>>.v...>.v.v>.v.....v..vv>..v.v.v..v.>>...v..vv..>>>.>.v....v.>......v.v>.>>v..>.>..v..>..>..vv..v.>>>v.....>.>..>.>>>>v>v.>v>.vv.>" );
            chart.push_back( ">....v.....v>.>.>v>.>>.>>....>v.v>v...>>.vv.v.v>>..v.v>v>.v..vv.v.v.>v......>..>..vv.>>..vv>.>.v...>v..>.vv>......>v.v.>>...........vv.>.v>" );
            chart.push_back( ">....>>vv>v>vvv..v..v...vv.v..vv........v.....>vvv.v.v.vv>>.vv>...v>>>vv..>.vv.>.v.....>>...>>>......v>v......v.>>.vv..v.>.v.>>...>.v>>.v>." );
            chart.push_back( ">v.>.>.>vv>...v..v...>..v.v..>>.vv.v..vv.>.>>v>...>v.v>.v..>..>>.v>>>....v.....v>.vv...v.>vv.v.>vv>v..>>.v....>.v>v.>v.>.vvv.>v..>.>.>>.v.v" );
            chart.push_back( "vv.>.v..v.v..>v....v>.vv..>v.vvv.>v.>v>...v>>v..v>.>>...v>.........v.>.>v>..>....>>....>v...vvv.v>>>v>.>v.>...v...v...v..v...>.>v.v>.>vv..v" );
            chart.push_back( ">vv...v>vv..>>v......v>v.>.....>.vvv.>......v.v>v>>.>...>>..vv>.v>v>>.v.v.v..v.v.>v>vv.>vvvv...v..>..>.v.v..vv.>....v....>.>vvv>>>....>>>>." );
            chart.push_back( ".....>.......>..v>.>.....>.v>>.>>v.vv.vv..v>...v.vv...v.v>>..v.>.v>v...v.vv.v.>.>v..>...v.v>.v>.v>.....vv...v....>>vvv>.>vv>.v.v>v...>....." );
            chart.push_back( "......>>v..>..v..v.>v>>.>>.....>.vv.>.....v.>>>.>>..vv.>.>..>...v..vv>>...vv...>.v.>..>.>...>vv....>>....>..>v..v..>.vv..v>.vv>.>>>..>vv..." );
            chart.push_back( "...>..v>v.>.v.>.>v>v.>vv..>.>...v.>.>vv.....v>.>...>.>...vv>v......v..v>.>.vv..v.v>.>..>.v..v>vv>...v..>v>.v.v.....>vvv.....vv..v.v.v.vv.v." );
            chart.push_back( ">>vv.v>..v..vv.>..v..>v>.>vv.v>>>v....v.vv..v>..v..>.>....v.>.>>v..v....>.v.v>>..>>..v.>vv>>>.v.v>>.v...v...>.vv.vv..>.v.>v.>>v.vv.v..>..v>" );
            chart.push_back( ".vv..v.>....>>>>>vv...>..vv>.>..>>>v.v.........>>.v.>.>>>>..>.vv>.v>...>....>vv>.v..vvv>v.v..vv>.vvv...>>v>>>....v....v.>v>..v....v....v.>." );
            chart.push_back( "...vv.v.>>.>..v>v.>>.vv....>.>v..v...>..>..>>..v>.>vvv.vv>...vv.v>v..>....>v>.>v.v>v.>.v>v.>v>......>.>..v.vv.>vv..vv.>>v....>vv.>>v..v>v.." );
            chart.push_back( ">..>.>..>.>...>.vvv...vv..v.>v>....v>>>...vv...>..vv>..v...v...>>.>v..>..v.>..v>>...v....vv>vv>.>..>>.v...v.>.vv>.>v>.>.v....>...v...v.>.>>" );
            chart.push_back( ">>.>.v.>.>v....v.v>..v.vvv.>>>..>vv...v....>vv.......>.>>.>v.>>.....>>.>.>.v.>>...>>.......v>..>.v.>.v...>.v>..>.>>v.>>.....v..>.....>>...." );
            chart.push_back( ">>v.>>v..v.v.>.vv.>v......>v...>>vvv...>v>.>v>.>.vv>>.v>v.v...>..>.v..v>>vv>..v.>>vv>>.>..>.>..vv.>>v.>>>..vv.v....>..v.>....vv..>..>....>>" );
            chart.push_back( ">..vv.vvv..v>.....>.vv>v>>>>v..>v.v>>....v>...v>vv>.v...>...>v.v.....v.v..>v...vv>....>....vv>...>..v.>.v..>.v.>.>vv..v.vv.....>..>v.>.vv>>" );
            chart.push_back( ">.>....v.>>v..v.......v.>..v...v.>v>>v..>vv.v..v>>.>v.>v..v..v.>>.v..>vv...>..>v>..>>v>v>.v.v...v>.vv...>..vvv..v>....>>>...>.v>v>v.>>>v>.v" );
            chart.push_back( "..v.vv..v...>..>>..>>>vv>.>>.>.vvv.>v.>>.....>>>..v..>vvv>.>v>vv>...>...>.>>..vv>.vv.>>..v.v.v>v.v..>.......>...>.>.>.v.v....vvvv.>..v..v.v" );
            chart.push_back( "..>vvvv..>v..>>>.v>.>v..v..vv.v>..>v.v.v.v>.>>..v.>.v>.v.v..>...v.>>...>>>..v.>v.....vv>>>vv>vv.>>..v>.>...>vvv..>>v.>..>vv...>....>.v>v>>." );
            chart.push_back( "....v.>.>>vv..v>>>.vv.vv>>.vv>.v>v..vv.vv.v..>v>.v.>vvv>..v>v>>>v>..vvv..>..v.>v.v.>v.v.>.v>...v.>vv.v....vv...>..vv.>vv..v..vv>>>..v.v..vv" );
            chart.push_back( "v........v.vv>..>>v.v......vv.>>v>.vvv.>>v>>.v...vvv>>....>.>vv>>..v..v..v...v...v...v.>..>..v>v.v.v.>>>v.>vv..>>..v.v>>vvv....vv>..v>>..v." );
            chart.push_back( "..vv>...>.>vvv...>.>.>v.>.>.....v..>....v>.>>>.....>.>.>....>.>>>..>vv>>>>>..v.>v..........>..v>..v.>.vv>>.v.v...>.v...vv>.>v.>.v...>....>v" );
            chart.push_back( ".>>v.>v.vv>>..>.vv>>>v...v..v>>....>.>..>>v..>..v.....v...>.vv.>.>v.v..vv.v.>.>..v.>v.v.>.>>.>v>vv..>>v>....>..v.v...v..>.....>..v..v..>>>>" );
            chart.push_back( ".>..>>v.>.>.>v.v..>.>...>v....v.>>>>vv>....>>.>vv.>>...v>vvv>v>>v..v..>v>..vv.v..v>...v...>v....>v>.....>v.....v....>v.>>..>>v.>v..v..vvv>." );
            chart.push_back( ">...>..v.>...>>.....>....>..vv>vv>v>>v.v.>v...>.v..>.v.v>...vv..>.........vv>>v.vvvv.vv.v>v>>.v>>>v>.>>>v>.>...vv>.v>....>v>..v.v.>v..vv.>v" );
            chart.push_back( ".v.>.vv....vvv....vv..>..v.....>.>>.....v..>...>.vv.....vv>v......>.v....v....>..v>vv.>.v.>vv>.vv.v.>...>v....v>v......v>>..v.vv>.v.>>>.>>." );
            chart.push_back( "vv..>>>......v>.....>....>.vvv>v..>>>vv.>....>v>>v.v>..>vv.>>.>v.vv>>>>>vvv>>v..vv>....vv...v..>..v...v>>v.v>..>.>.vv>...>...v...>.>>....v." );
            chart.push_back( ".......>>.>v>.>v.....>.v...vv..v>>v...>...v>.>.>..v>.....v.>.>.v.v.v...>>.>.v..v.....v.vvvvv.v>....v>vv>.v..>.....v.>v>.vv>vv....v.vv.>..v." );
            chart.push_back( ">.v.v....>v.>v>.>>vv.v...>..v.>.vv>..>v.>vvv...>.>>vv>.v.v...v..v.>>..>..v...v....v.vv....v.>>.vvvv...v....v.>.>.v.>>vvv>.>..vv>..vv.>.vv.v" );
            chart.push_back( "...>>>....>v...>v....>v.>>vv.v>...>vv.>.>vv>..>>>.>>...>>v>>..>..v..v...v>>.vvv.>..v>v...>.>.v.......v.....>...v.>...v>...>>.>.>>>....v..>." );
            chart.push_back( ".v>.v...v>...>.>......>...>.v.>..>v.>..v>.>.v.v..>.vv>>v.......>>v.....v>.v.v>..v>>v>....vv>>.>....>>v>>.v>.....>>.v..>v..v..v..>v>>..v.vv." );
            chart.push_back( "..v.vvv>v>.>v..>>v.vv>.>>v.>>vvv>..>v.vvv..>..>v..v....>v.>>..v.>.>v>.>...v.>>.>..v>..>.vv..v....>>>..v..>..v>>vv..v.>.v.>...v..>.>.v.v.v.." );
            chart.push_back( ">.v>v..v..v.>>.v.v>.v.v.vvvv.>v>..>..>.>..>>>>.v>.>..v..vv.>>.>.v>.v.>v>vv>......v>v..>.>.v>vv..v.v...v>v>.>v>v.....v.>.>..v.v>>..vvv>>>..v" );
            chart.push_back( ">..v>vv....>.>..>...>vv.>.>>......>vv>.v..>v>v>vv..v..>.>..>>...v>vv.....v.>.>..>>v.>>>v>.v..vv>....v...>>>>.>>v>..>>>>>v>..vvv>v>>.v.>.>.v" );
            chart.push_back( ".>>.v>.v>..>...v..v.>v.>v>...>>.v.vv..v.v.>v>>>........>v>..>.>>>v.v>v>..>..v..>v>>..v.v>vv...v..v>..v..>.v>.vv>..v.>v..>>>>v.>.>v>v..>..>." );
            chart.push_back( ">..v>.v.>>v>v.>..v..>>v..vv>>...>.v>....>>.>v.vvv>v>>>..>.>v...v...>..v.v>.>vv...v...>>.v>v......v>v.vv.....>vvvv.vv.>.>.vv...>v>v.>...vv.>" );
            chart.push_back( ".v.>...>.>..v.vv.v....>v...>.>.>>.vv.>.>.v..>..>...v.>v>..>...>.>v...vvvv>vv.>>v..>.>....vv>..>v...v.>.>.>>.>>vv..v.........v.>.vv.vvv>...." );
            chart.push_back( "v....>v>v>>v.>>>.v>..>.>>>v.>>v..>...vv>.v.>>>v.....>v>>...>>>v...v....>v.vv>>>...>......>>.v>.....>..>>v..v.>v>..v>...>vv..>v..>v...v>...." );
            chart.push_back( ">v.>.v>.vv>...v....v>v..v>>>>>v>.v>....>>>.>..>.>>vv.>...v.vvvvv.vv..>v.v..>..v..v.v.v.....>v>v>.vv>>...v.>>..>.>.>...>>.vv>.v..v.>vv.>..vv" );
            chart.push_back( "vv...vv>vv>>...>>.....>..v>..vvvv>...>v>>vv>v...v..vv...v>vv>v>....>vv>......v>.>>.v.vv>.>.>>.vv>v..v>.v>.v.v.vv.....v....v..>>.vvv...>..>>" );
            chart.push_back( "v.>>..>>..v>v>...vvv..vv>.>>v...>....>..>v...>>>v..v>v>v.v>>...>.>.v...>.v..v>>..v.....>...v.v.>..v.....v>.>....v.v...vv...v...v.v...>vv.vv" );
            chart.push_back( ">.>v...>>.v>>..vvv.>>..>.....>v..v.>>........>..>.v...vv>v..>>.....vvv.>v.>.>>>v..v...vvvvv.v...v.>v.v.v..v>.vv>>vv...>..>.v...v>.>>v.>>v>>" );
            chart.push_back( ".v..vv.v..v.....v>..>.>.v..>..>.v>>vv.v..>v>..>...>>v>>.v..v....v>v..v..>.>..v>..v>>>>.....>v>...v.>v.v..>..>v>.vv>.>v>..>.>.>.>>..>..>vvvv" );
            chart.push_back( "..vv>>.>v...>..vv.>>v.v.>>vv>>.v>>.>>vvv...>>vv.>.vv....vv.>v...v....vv.v>v.....>..>vv.......>v....>>vvvvv.vv.....vv>.>..>v.>.v>.v>.vv>v>.." );
            chart.push_back( ">>...>...>>>>...>>v.>>.>.>..>v>.v>vv.>v>.......v>..>vvv.....vvv>.v>v>>vv.>.....>v....>>>v.v>...>v.>....vv..>v.>.vv...>>>v>v..>>>.>v>.>>>.>>" );
            chart.push_back( ".>>....v>vv.......v..>...v....>...>..v>.v.....v..>......>v>>.>v.v>>.>....v.v..v.>.>v>.>.>.>.>>.v..v>......v....v>>>.v..vvv.>v>v.>.>v.>v.v>>" );
            chart.push_back( ".vv>>.v....vv.v....v.v.v>.v.......>>v.v...>.>vv>..v..vv..>vvv...vvvv..v.v>..v.v.vv>>..>>>>...v>.v.v.vv.>>>vvv>.v..>vv..>v.>>v>>v>.v..>.>>.>" );
            chart.push_back( "...v..vv>>.v.v>.v...>........>>.....v...v.>.v.v..v..vv..>.v>.v>....v..v..>..>...v.vv.v.........>v>..vv>>vvv..>vv.>>.>.v>v..v...v.v>.>v.v..." );
            chart.push_back( ">..>..v>.....>vv.v>>v.v..v...>.>v....v...>....v>>.v>.>v.>.>.v>.......v>.>v>..>>>.>.v.>...>..>>>v..vvv>..>.>.v>vvv...vv>...>>.v.vv..v>.....>" );
            chart.push_back( ".>.>......>.>..>v..v>>v.>..>..>>..>.v.>>.>..>.v..>.v.>..v.>....v>>..>v..v>>....>v>>...>.>..>>.....>>vv.>v...>>v>..v.>>v>>vv.>v.>.>v......>." );
            chart.push_back( "v.>.>..>.....v>..>>..v>vv>>.>>>>>>..>.vv...>>......>...>.>v.v>v..v..>>...v.>v..v>>......>v..vv..>.>.>.>..>..vv..>>..v....v>v.v...>>...vv..." );
            chart.push_back( "v.v.>..vv...>...v....>vv......>>....>v>>v..vv>....v.>v.>........v>v...vv.v..v.v.>....v.v....v>v.>vv..>vvvv..v...v.>>v.>.>........>.>.>v>v.v" );
            chart.push_back( "v>v..>v..>>...........>>....>.v.....v..v...v.vv..>...v>.vv>>vv>........>v.>>v.>v.v>v>>v.>..>...vv.>>v.v>.v>v>.vv>..>v.vv.>>.v...>v.>v.v>vv." );
            chart.push_back( "vv.v>.vv......v>>..>>>.>v.>>vv>.>v.>>.>v>.>v.>>.>v.>...>.>..v.>.v>v>.vvv.vv...vvv..v>..>vv.v.>....vvv.>>vv....>.....vv.>>.....v.>.vv..v>.vv" );
            chart.push_back( ".>...vvv.....>...>..>>vvv.>...>>...>..v.......>v.vv..>...v.>v>.>.v.v..>.>..>>v.>v>>..v..>.vv.v.vvv.v.>>v.>....>vvvv>vv>v.>vv.>>..v........>" );
            chart.push_back( "..>>..v>...v.....vvv.v>>.>...v.......>...v>>.>..>....>.....>.>vv>...vv>v.>..v>v>v>>>..v>.>v>>.>...>......v.vv.>v.>>..v.v.>v..v..>...>v..v.v" );
            chart.push_back( "....>.>>>v..>.>.v>>>.vv...vv.v.v>.>..>v>>vv....>..v...>>..>.v>>>....>...>.v>vv......v...>v.>...>>vv.....v.>............v.vv>.v.v>.>v>>v.v.v" );
            chart.push_back( "....vv..>.>..>>>..v.>...vv.>.v>.>v....vv>v...v>...v..vvvv....>>.>.v.>>vv.vv>.v...v...>.>vv>.>.v..>..v...>>...>...>...>.v.>>v>>.vv.v>.v.>..>" );
            chart.push_back( ">>.>..v.v...>..>.v...>v>.vv>>.v>.>v.v>...v>>v...v>....>..>v.>v>vv>v..vv..>.v>>>v>v>>.v.v>vvv.....>.v.v.>..>v.>>vv..v>.v>.v>v.>.v.>v.v..>..v" );
            chart.push_back( "....v..>.v.>..>v...vv.v>>v.....>>>..>..v.>...v.v>>>.>>....v..>>>..v..>>..>>.>>>.>v>...v....v>vvvv...v...>v>.v>.>.v.v>.v.v>v.vv..v..v..v.>>>" );
            chart.push_back( ".>v.>v....v.vv>>v>>v.v..v.v>>....v.v>.>.>..>..v.>v.>...>vv.>.>>.....>>v..>v.....>vv.v..>.>.v>v>v.v.>.>....vv....v.vv.........>....v.v.>>..>" );
            chart.push_back( "v..>>>.vv.>>.v>.>.>>>.vv.v>>..v...vv>>vv.v>.>>vvv.v....>>.vv>v.>.vv..v>>.>>v......>.>vvv>..vv.v.v.....v...>.v.v.v..>vv......v.>.>...v.>.>v." );
            chart.push_back( "v>.>vvv...>.vvv>>>.v..vv...vv>>..vv..vv....>..v.....v.vv>.v>....>>.>.v..>>.vvv.....v...v...vv>v.v>.vv>>v.........>v...>v>vv.>vv...v>.vv>.>." );
            chart.push_back( "...>..>.v...>....>v..v>..>>....vv.v>>>v.>.>vv.>.....>>>.>v...v>v.>.v..v...vvv.>...vv..vv.v>...v.v>vvv......v>.v..>v>>>v>v.vv.v>v.>...>...v." );
            chart.push_back( ">.>..>v>...v.>>.>v....>vv>>v.vv..>....vv>.>>.v>.v.>>v.>v.......v.vv>>...>.>...>.vvv>..>.v..>..>..>.v.vvv.>..>>>.v....>v.vv.>...v..>vvvv>..v" );
            chart.push_back( "v..v.v.>..v..>>.>.>vv>..>..v.>..>v...>vv>.vvv.....v>>>.vvv.v..>...v>....v..v....v..>>v.>vv>>v..v.>>......>........v...v..>v.>....vv>...v.>>" );
            chart.push_back( ".>>..v.>......v>>.v...vvvvv.v..vvv.>.v.>>.vv..v..vvvv..v>.>vv.v.v>>..v>...>v.....>...v.v....vvv.>v...>..>.v>v>v..v.>>..vv>v>>>>..v>.>v..v>." );
            chart.push_back( ">v>v.v>>>.>v.v>..v.>>..v.>v..vv.v>>.>.v>>v..v.>.>v>.....v.v.>......>>vv...>..v.>>>>..v.v..>v..>vv>.>....v.>.vv......v>..>v.v.>.>.v>v..vv.>." );
            chart.push_back( ".v>...v..vvv..>.v......>v.......>..v>.vv>>..>..v>..>>>vvv>v.v>vv>v.v.......>>.>.v>.>>v..>.v>...>....>.>.v>.v>...v>v>>..vv..vv.>...v..>>.>>." );
            chart.push_back( ".>v>.vv..>..v..vv.>...vvv.....v>v>v>v>>>v...>v.v.>..v>v...>..v..>.v>.>>>.>>.vv.......>..>.>v..>vv.vv>...v....>..v.vv.>..>.v.>>vvv.v.>..>>.v" );
            chart.push_back( ".>v>..vvv.>>..>>.v.>.>>.>.vv.>>>v>.>.v.v....v.>.v.v.v.>>....v...v.....>v>>..v...vv.v>>..>>......>v>..v..vv>v.>>vv.>..>.vv>..>..>v...>>vv..>" );
            chart.push_back( "..v....>v...>v>..v..>v......>>>.>>v.>.v>....v.v>v.v..>>..>.>v..vvv.>v..>.>v>.....v.vv>v.v.>>>>.>v...vv.>v.v.v.>v.v..vv>.vv.v..>...>v.v..vv>" );
            chart.push_back( ".vv>v...>.>v.>...>vv.vvv.>vv>.vv...v.>>v>>..>vvvv.v.>>v.....>.>.>.v.v.>.....>.>>v...v....>>v.>...v...v.v..vv...v..v...>..>>..v.v.>..>v.>.vv" );
            chart.push_back( ".>.>v.vv.>...>.>>vv>..vvv...vv>.>......v.v.>vv>>v>..>..>.....v.vv.>vv.v>..v.>vv.v>.....>.v.vv...>v...>>.v>....vv..>v>.>>>v>v.>.v.v.>......>" );
            chart.push_back( "v.>.>vv>v..v....>.>..>>.>.....>v>.>vv>.v>.>v>>>.>..>vv>vvv>>.>>>...vvv>v>>.v....v..>.v...v>>.......>....>v>>.v........v>.>vv.v..>v..>.v>>.>" );
            chart.push_back( "v.>v>>vv>....>.>v...v.>v.....vvv.v.v..>.v..vvv.v>.>.vv>>vv>vv>v....v..>..>.v>.>...>.v..>>v...v.v>v.v>>v.v.>.>..>>.v>vv>..v...v.v.>.v...>.v." );
            chart.push_back( "vv>.>>>..>vv>v.>vvv.v.v...>...>>vvvv>v>.v>..v....>>>..v.vv...>......>v>.>.>...>>...>.v>v.>..vv..>.v.v.>v.v.>v....v...>v.>..vv>.>....vv.>..v" );
            chart.push_back( ">>vv.......v.v.>.>..>v.>>..v.v.v>.v.>.v.vvvvv..>v.v..v.>..>.>..v..v>...>>v>.>>..v.>.v..v>>v..>>.v>.vv>..>v>.vv..>..vvv.>...vvvv>v>>v..>>>.." );
            chart.push_back( ">v.v.v>vv.>.v.v.>>.v..v>.>v>>v..vv..>>v.>vvv.>v>v>.v.v..v>v.v..>vv..>.>..>....>>>.v....>..>....v>..vvvvvv>.v....>.>v>vv>.v>...>v.v>vvv....v" );
            chart.push_back( ".>.>.>>v.>v>..>v...vv.v>>.>vv.>>vv.v>...>.>......v.>...>..>>>.>>..>>v>.v.vv.vvv..v>.>.vv.>>>..>>vv.vvv..>v.>.....>.v....v.v>v.>v.v.vv.>.v.v" );
            chart.push_back( "..v>>.vv>v>v>vvv>v.....v...>vv.v>.>......>>v..vv.vv.v.v>.v>v>v...v.....>.v..>>>..>.>....>.>>>v>.vv........vv..>.vv..v..>.>.v..>>....>v.>.>>" );
            chart.push_back( "v.v.v>>>.v.v.v.>..vv.vv.vv>....v.v>..>.vvvvvv...v>.v>.>vv....v...>>.v.v>vv..v.v>v>.....v..>..>..>>...vv.>vv>..vv>..v.>vv>>.>.....>>>...v.v>" );
            chart.push_back( "vv..v>...v....v>.....v>>>.>.v.v.>.>.>.v>v>.vv......v.v>v.v>.>v>>v..v..>...v>.>>...>v..>>..>vv>>>v>.v.>>>.>>..>v.v....>..v.>>....>v>....>..." );
            chart.push_back( ">>v.>.>>..vv>.>..>...>...>>..vv>>.>.>..v>.>v..>>.v>>..v....>v.>..>.>v...v...>.v>...v.>.v....>vvvv>.....>>..>..>v>.vv>v>.>v>v..v>vv.v..>v..>" );
            chart.push_back( "v.>>v..v..v...v>>....>v.>>....v..v....>>..>>>v>>>vv.>>.v>..>v.>vv>>v.vv.v.v.>..>.v>.v..>...>.....vv.>..v.....v..v.v....>.>..v>.vv..v>.>..vv" );
            chart.push_back( "v>....v>>..>..v.>v>..vv.>v.v>v.v.>vv.vv..v>..v>.v...>v.......>v....vv>..>>>.v.>..>..v.>..v.>>>v.vv>....v>v>..>.v>...vv>....v.vv...v.v>.>..." );
            chart.push_back( "v..>.vv.>v.>>..>.>.v>vvv.>>v.>..>v.>.>..v>>v...v.>..v>v..>>>.>>.>.v.>v.>.v>v.v......>.v..>...>v.>v.vv.vv..v>vv>..>>v>..v...v>v...vv....v>.." );
            chart.push_back( ">.>.>..v....>.vv.vvv.>>.>vv.>>..>.>v>v..vv.v>>..>v.....v.>.v>>..v>v..v>>..v>.v.v..>.>.vvv>..>v.>>.>....>..>..>vv>>...v>..>.....v>>v....>v>v" );
            chart.push_back( "...v.>>..>v.v.......>.>.>vv>>>.>vvv>v.>......>>...>..>>vvv.v>...>>v...>v..>>>...v.v.vv.>>v>.vvv.>..v.>.>>v.v>..>....v>v>>v.>v..v>.....>...." );
            chart.push_back( ">>..v.v>>>v>..>.>.vv..>....v.vvv.>v..>>.>>...v...v.v.>...>v.vv>..>>>..vv>v...>.v.v>v.>>...>v.v....>>>>>.>>>.vv....>.....>..>..vvv>.v>v>..>v" );
            chart.push_back( "vvv.>...>...>..>v....>.v...vv>....>.....>..v.>v..v.>.....>..vvv.>....>.>.>.>>v.v.v...v.>>vv.vv>.v.>vv..>v>vv..>vv.>.>.....>.>....v..>.v>.v." );
            chart.push_back( ">>>.>v....>.vv.>v..v...>>.>v.v>vvv>v..v..v.v.>....v>..>v>.v>vvvv>...>.>v..>....>>...>>.>....>.v.....>vv>...vv.>v.v.v>vvv.v...vv..>.v>>>...." );
            break;
    }
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintDataStream( DataStream &dData ) {
    for (int i = 0; i < (int)dData.size(); i++)
        cout << "Data - index: " << i << " = " << dData[i] << endl;
    cout << endl;
}

// output to console for testing
void PrintMap() {
    if (glbMap == nullptr) {
        cout << "map non-existen" << endl;
    } else {
        for (int y = 0; y < glbChartHeight; y++) {
            for (int x = 0; x < glbChartWidth; x++) {
                cout << glbMap[ y * glbChartWidth + x ].type;
           }
            cout << endl;
        }
        cout << endl;
    }
}

// ==========   PROGRAM PHASING

// populates input data
void GetInput( DataStream &cData, bool bDisplay = false ) {

    GetChartData( cData );
    glbChartHeight = cData.size();
    glbChartWidth  = cData[0].length();
    // create array of cucumber pointers
    glbMap = new sCucumber[ glbChartHeight * glbChartWidth ];
    // and fill it
    for (int y = 0; y < glbChartHeight; y++) {
        for (int x = 0; x < glbChartWidth; x++) {
            sCucumber aux;
            aux.type = cData[y][x];
            aux.x = x;
            aux.y = y;
            aux.canMove = false;
            glbMap[ y * glbChartWidth + x ] = aux;
        }
    }

    // display to console if so desired (for debugging)
    if (bDisplay) {
//        PrintDataStream( cData );
        PrintMap();
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

int NextSpaceX( char type, int posX, int posY ) {
    int result = -1;
    switch (type) {
        case EAST:  result = (posX + 1) % glbChartWidth; break;
        case SOUTH: result =  posX                     ; break;
        default: cout << "ERROR: NextSpaceX() --> invalid type: " << type << endl;
    }
    return result;
}

int NextSpaceY( char type, int posX, int posY ) {
    int result = -1;
    switch (type) {
        case SOUTH: return (posY + 1) % glbChartHeight; break;
        case EAST:  return  posY                      ; break;
        default: cout << "ERROR: NextSpaceY() --> invalid type: " << type << endl;
    }
    return result;
}

bool NextSpaceOpen( char type, int posX, int posY ) {
    int x = NextSpaceX( type, posX, posY );
    int y = NextSpaceY( type, posX, posY );
    return glbMap[ y * glbChartWidth + x ].type == EMPTY;
}

void DoMove( char type, int posX, int posY ) {
    if ( glbMap[ posY * glbChartWidth + posX ].type != type ||
        !glbMap[ posY * glbChartWidth + posX ].canMove) {
            cout << "ERROR: DoMove() --> ... " << type << endl;
    } else {
        int x = NextSpaceX( type, posX, posY );
        int y = NextSpaceY( type, posX, posY );
        glbMap[    y * glbChartWidth +    x ].type = type;
        glbMap[ posY * glbChartWidth + posX ].type = EMPTY;
        glbMap[ posY * glbChartWidth + posX ].canMove = false;
    }
}

// the nr of cucumbers that moved is returned
int OneStepPerType( char type ) {
    // check which ones can move
    for (int y = 0; y < glbChartHeight; y++) {
        for (int x = 0; x < glbChartWidth; x++) {
            sCucumber *aux = &glbMap[ y * glbChartWidth + x ];
            if (aux->type == type) {
                aux->canMove = NextSpaceOpen( aux->type, x, y );
            }
        }
    }
    int nNrMoved = 0;
    // move the ones that can move
    for (int y = 0; y < glbChartHeight; y++) {
        for (int x = 0; x < glbChartWidth; x++) {
            sCucumber *aux = &glbMap[ y * glbChartWidth + x ];
            if (aux->type == type && aux->canMove) {
                nNrMoved += 1;
                DoMove( aux->type, x, y );
            }
        }
    }
    return nNrMoved;
}

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    DataStream chart;
    GetInput( chart, glbProgPhase != PUZZLE );
    cout << "Data stats - chart is " << glbChartWidth << " x " << glbChartHeight << endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    bool bQuit = false;
    int nQuitStep;
    for (int i = 0; /* i <= 100 && */ !bQuit; i++) {
        if (i > 0) {
            int nrMoved = 0;
            nrMoved += OneStepPerType( EAST  );
            nrMoved += OneStepPerType( SOUTH );
            bQuit = (nrMoved == 0);
            nQuitStep = i;
        }
//        if ((i > 0 && i <= 5) || (i % 10 == 0) || (i >= 55)) {
//            cout << "Step: " << i << endl;
//            PrintMap();
//        }
    }
    cout << "First step without movement: " << nQuitStep << endl;

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

// Not allowed to do part 2 since I'm missing out on stars... :/

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}





