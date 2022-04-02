// AoC 2021 - day 12
// =================

// date:  2021-12-12
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <algorithm>

#include "../flcTimer.h"

// ==========   PROGRAM PHASING

enum eProgFase {     // what programming fase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgFase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

struct sData {    // data consists of a list of strings, each containing one edge in the graph
    std::string from;
    std::string to;
};
typedef std::vector<sData> DataStream;    // list of edges of the paths graph

struct gData {   // list of caves
    std::string ID;
    bool bSmall;
    std::vector<std::string> vNeighbors;
};
typedef std::map<std::string, gData> CaveStream;  // map implementation
typedef std::map<std::string, gData>::iterator CaveIterator;  // iterator type on CaveStream

typedef std::vector<std::string> CaveList;  // for neighbor lists and paths

// ==========   DATA INPUT FUNCTIONS

bool IsSmallCave( const std::string &cID ) {
    return ('a' <= cID[0] && cID[0] <= 'z');  // hack - just take first char as representative
}

bool IsLargeCave(const std::string &cID ) {
    return !IsSmallCave( cID );
}

bool ReadOneEdgeLine( std::ifstream &dFileStream, DataStream &eData, CaveStream &cData ) {
    bool bAbort = false;
    std::string token;
    if (dFileStream >> token) {

        // parse two caves out of token
        sData edge;
        size_t hyphenIndex = token.find( "-" );
        edge.from = token.substr( 0, hyphenIndex );
        edge.to   = token.substr( hyphenIndex + 1 );
        // insert into edge list
        eData.push_back( edge );

        // insert both caves into cave map
        gData node;
        node.ID = edge.from; node.bSmall = IsSmallCave( node.ID ); cData.insert( make_pair( node.ID, node ));
        node.ID = edge.to  ; node.bSmall = IsSmallCave( node.ID ); cData.insert( make_pair( node.ID, node ));
    } else {
        bAbort = true;
    }
    return !bAbort;
}

// this function can be used to input the data from the text file
void ReadData( const std::string &sFileName, DataStream &eData, CaveStream &cData ) {
    // open filestream
    std::ifstream dataFileStream( sFileName );
    // read data stream
    eData.clear();
    cData.clear();
    bool bDone = dataFileStream.eof();
    while (!bDone) {
        if (ReadOneEdgeLine( dataFileStream, eData, cData )) {
            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }
}

// ==========   OUTPUT FUNCTIONS

// returns a list (in neighbors) of all the neighbors of node with caveID
// ASSUMPTION - neighbor pairs are not occurring more than once in input data !
void GetAllNeighbors( DataStream &eData, const std::string &caveID, CaveList &neighbors ) {
    neighbors.clear();
    for (auto elt : eData) {
        if (elt.from == caveID) neighbors.push_back( elt.to   );
        if (elt.to   == caveID) neighbors.push_back( elt.from );
    }
}

void PrintCaveList( const CaveList &caveList ) {
    for (auto elt : caveList)
        std::cout << elt << (elt == caveList.back() ? "" : ",");
    std::cout << std::endl;
}

// output to console for testing
void PrintEdgeStream( DataStream &eData ) {
    for (int i = 0; i < (int)eData.size(); i++)
        std::cout << "Edge - index: " << i << " from: " << eData[i].from << " to: " << eData[i].to << std::endl;
    std::cout << std::endl;
}

// output to console for testing
void PrintCaveStream( CaveStream &cData ) {
    CaveIterator it;
    for (it = cData.begin(); it != cData.end(); it++) {
        gData elt = (*it).second;
        std::cout << "Cave - index: " << (*it).first << " ID: " << elt.ID << (elt.bSmall ? " small" : " large");
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( DataStream &eData, CaveStream &cData, bool bDisplay = false ) {
    // get all edge data from file, and assemble both an edge list (eData)
    // and a node list (cData) from it.
    switch (glbProgFase) {
        case EXAMPLE: ReadData( "input.example.txt", eData, cData ); break;
        case TEST:    ReadData( "input.test.txt",    eData, cData ); break;
        case PUZZLE:  ReadData( "input.txt",         eData, cData ); break;
    }
    // populate neighbor lists for each node
    for (auto &elt : cData)
        GetAllNeighbors( eData, elt.second.ID, elt.second.vNeighbors );
    // display to console if desired (for debugging)
    if (bDisplay) {
        PrintEdgeStream( eData );
        PrintCaveStream( cData );
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

// see pseudo code in notes document for path enumeration algo

std::queue<CaveList> myQueue;    // working queue of incomplete paths
std::vector<CaveList> myPaths;   // result set of complete paths

// returns true if id is already contained in path
bool InPath( CaveList &path, const std::string &id ) {
    return any_of( path.begin(), path.end(), [=]( const std::string &s ) { return id == s; } );
}

// returns the nr of occurrences of id in path
int NrInPath( CaveList &path, const std::string &id ) {
    return count_if( path.begin(), path.end(), [=]( const std::string &s ) { return id == s; } );
}

// returns true if there is a duplicate in path
// SNAG - function should return true only based on the small caves.
bool DuplicateInPath( CaveList &path ) {
    bool bFound = false;
    CaveList::iterator it;
    for (it = path.begin(); it != path.end() && !bFound; it++) {
        if (IsSmallCave( (*it) ))
            bFound = any_of( it + 1, path.end(), [=]( const std::string &s ) { return (*it) == s; } );
    }
    return bFound;
}

// Argument puzzlePart must be 1 or 2
void GetPaths( DataStream &eData, CaveStream &cData, int puzzlePart, bool bDisplay = false ) {
    while (!myQueue.empty()) myQueue.pop(); // there is no clear() for std::queue...
    myPaths.clear();

// 1. Add [ a path containing only the ] root node to the queue
    CaveList curPath;
    curPath.push_back( "start" );
    myQueue.push( curPath );

// 2. Loop on the queue as long as it's not empty
    while (!myQueue.empty()) {

//    a. Get and remove the path at the top of the queue(current)
        curPath = myQueue.front();
        myQueue.pop();
        // extract last node from current path, and grab iterator to node list
        std::string curNode = curPath.back();
        CaveIterator curIt = cData.find( curNode );

//    b. For every neighbor of the current node that can be visited [ see specs below ]:
        for (auto elt : (*curIt).second.vNeighbors) {
            CaveIterator it = cData.find( elt );
            if (it == cData.end()) {
                std::cout << "ERROR - couldn't find cave - " << elt << std::endl;
            } else {
                gData curNeighbor = (*it).second;
                // Criteria to include neighbor into path:
                //   *  "start" is not allowed (since the algo starts with it).
                //   *  "end" is only allowed once
                //   *  big caves are always allowed, and multiple times
                //   *  small caves are always allowed once, and not more than one of them is allowed twice
                bool bAddNode;
                if (curNeighbor.ID == "start") {
                    bAddNode = false;
                } else if (curNeighbor.ID == "end" && NrInPath( curPath, "end" ) == 0) {
                    bAddNode = true;
                } else if (!curNeighbor.bSmall) {
                    bAddNode = true;
                } else {
                    int nOccur = NrInPath( curPath, curNeighbor.ID );
                    switch (nOccur) {
                        case  0: bAddNode = true;  break;
                        // for puzzlepart 2 there's an extra criterium - small cave can be inserted if there's
                        // no other small cave inserted twice yet
                        case  1: bAddNode = (puzzlePart == 1) ? false : !DuplicateInPath( curPath ); break;
                        default: bAddNode = false; break;
                    }
                }
                // If criterium is fulfilled, extend the path. If it's finished, put it in myPaths, otherwise
                // enqueue into myQueue again for finishing (or disposal) later on.
                if (bAddNode) {
                    CaveList pathCopy = curPath;
                    pathCopy.push_back( (*it).second.ID );

                    if ((*it).second.ID == "end") {
                        myPaths.push_back( pathCopy );
                    } else {
                        myQueue.push( pathCopy );
                    }
                } // there was no way to validly extend the current path - it is exposed.
            }
        }
    }
    if (bDisplay) {
        for (auto elt : myPaths) {
            PrintCaveList( elt );
        }
    }
}

int main()
{
    glbProgFase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgFase (example, test, puzzle)
    DataStream edgeData;
    CaveStream caveData;
    GetInput( edgeData, caveData, true );
    std::cout << "Data stats - #edges " << edgeData.size() << std::endl;
    std::cout << "Data stats - #caves " << caveData.size() << std::endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    GetPaths( edgeData, caveData, 1, false );
    std::cout << std::endl << "Paths found: " << myPaths.size() << std::endl;

    tmr.TimeReport( "Timing 1 : " );   // ==============================^^^^^

// ========== part 2

    GetPaths( edgeData, caveData, 2, false );
    std::cout << std::endl << "Paths found: " << myPaths.size() << std::endl;

    tmr.TimeReport( "Timing 2 : " );   // ==============================^^^^^

    return 0;
}


