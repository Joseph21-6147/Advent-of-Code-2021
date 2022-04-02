// AoC 2021 - day 15
// =================

// date:  2021-12-15
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

#include "../flcTimer.h"

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

int glbMapWidth  = -1;
int glbMapHeight = -1;

// example dimensions 10x10, puzzle dimensions 100x100
typedef std::string sData;   // in the form "1163751742" - all individual cell risk levels (on entrance)
typedef std::vector<sData> DataStream;

// ==========   DATA INPUT FUNCTIONS

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

// populates data vector(s), depending on the program phase (glbProgPhase)
void GetInput( DataStream &vData, bool bDisplay = false ) {
    // read the right input file and set the map dimensions
    switch (glbProgPhase) {
        case EXAMPLE: ReadData( "input.example.txt", vData ); glbMapWidth =  10; glbMapHeight =  10; break;
        case TEST:    ReadData( "input.test.txt"   , vData ); glbMapWidth = 100; glbMapHeight =   5; break;
        case PUZZLE:  ReadData( "input.txt"        , vData ); glbMapWidth = 100; glbMapHeight = 100; break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintDataStream( vData );
    }
}

// ==========  PUZZLE SPECIFIC SOLUTIONS

// =====================/   A* algo from javid video   /=============================

// this sNode struct combines process info and content. This is not good programming practice.
// To keep the demo simple this should do the trick.
struct sNode {
    bool bVisited  = false;        // have we searched the node before?
    float fGlobalGoal;             // Distance to goal so far
    float fLocalGoal;              // Distance to goal if we took the alternative
    int x, y;                      // nodes position in 2D space
    std::vector<sNode*> vecNeighbours;  // connections to neighbours
    sNode* parent;                 // node connecting to this node that offers
    // added for AoC 21
    short riskLevel;
};

sNode *nodes     = nullptr;   // will hold an array of glbMapWidth x glbMapHeight nodes [ in a grid ]

sNode *nodeStart = nullptr;   // set to start and end point to search a path between
sNode *nodeEnd   = nullptr;

void InitializeGraph_part1( DataStream &vData ) {
    // Create a 2D array of nodes - this is for convenience of rendering and construction
    // and is not required for the algorithm to work - the nodes could be placed anywhere
    // in any space, in multiple dimensions...
    if (nodes != nullptr) delete nodes;

    nodes = new sNode[glbMapWidth * glbMapHeight];
    for (int x = 0; x < glbMapWidth; x++)
        for (int y = 0; y < glbMapHeight; y++) {
            nodes[y * glbMapWidth + x].x         = x;
            nodes[y * glbMapWidth + x].y         = y;
            nodes[y * glbMapWidth + x].bVisited  = false;
            nodes[y * glbMapWidth + x].parent    = nullptr;

            nodes[y * glbMapWidth + x].riskLevel = (short)(vData[y][x] - '0');
    }

    // Create connections - in this case nodes are on a regular grid
    for (int x = 0; x < glbMapWidth; x++)
        for (int y = 0; y < glbMapHeight; y++) {
            if (y >                0) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y - 1) * glbMapWidth + (x + 0)]);
            if (y < glbMapHeight - 1) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y + 1) * glbMapWidth + (x + 0)]);
            if (x >                0) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y + 0) * glbMapWidth + (x - 1)]);
            if (x < glbMapWidth  - 1) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y + 0) * glbMapWidth + (x + 1)]);
        }

    // Manually position the start and end markers so they are not nullptr
    nodeStart = &nodes[                 0  * glbMapWidth +                0  ];
    nodeEnd   = &nodes[ (glbMapHeight - 1) * glbMapWidth + (glbMapWidth - 1) ];
}

void InitializeGraph_part2( DataStream &vData ) {
    // Create a 2D array of nodes - this is for convenience of rendering and construction
    // and is not required for the algorithm to work - the nodes could be placed anywhere
    // in any space, in multiple dimensions...

    if (nodes != nullptr) delete [] nodes;
    // resize the map to 5 times the dimensions of the input data
    int orgMapWidth  = glbMapWidth ; glbMapWidth  *= 5;
    int orgMapHeight = glbMapHeight; glbMapHeight *= 5;

    // create new - 5x5 times larger - map
    nodes = new sNode[glbMapWidth * glbMapHeight];

    // convenience lambda to calculate the correct index, given the tile coordinates and coordinates within that tile
    auto aux_index = [=]( int tileX, int tileY, int celX, int celY ) {
        return (((tileY * orgMapHeight) + celY) * glbMapWidth + (tileX * orgMapWidth) + celX);
    };

    // iterate 5x5 tiles
    for (int tile_x = 0; tile_x < 5; tile_x++) {
        for (int tile_y = 0; tile_y < 5; tile_y++) {

            // iterate elements within each tile
            for (int x = 0; x < orgMapWidth; x++)
                for (int y = 0; y < orgMapHeight; y++) {
                    nodes[ aux_index( tile_x, tile_y, x, y ) ].x         = (tile_x * orgMapWidth ) + x;
                    nodes[ aux_index( tile_x, tile_y, x, y ) ].y         = (tile_y * orgMapHeight) + y;
                    nodes[ aux_index( tile_x, tile_y, x, y ) ].bVisited  = false;
                    nodes[ aux_index( tile_x, tile_y, x, y ) ].parent    = nullptr;
                    // calculate the correct risk level - see puzzle specs!
                    short tiled_value = (short)(vData[y][x] - '0') + tile_x + tile_y;
                    if (tiled_value > 9)
                        tiled_value = tiled_value - 10 + 1;
                    nodes[ aux_index( tile_x, tile_y, x, y ) ].riskLevel = tiled_value;
            }
        }
    }

    // Create connections - in this case nodes are on a regular grid
    for (int x = 0; x < glbMapWidth; x++)
        for (int y = 0; y < glbMapHeight; y++) {
            if (y >                0) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y - 1) * glbMapWidth + (x + 0)]);
            if (y < glbMapHeight - 1) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y + 1) * glbMapWidth + (x + 0)]);
            if (x >                0) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y + 0) * glbMapWidth + (x - 1)]);
            if (x < glbMapWidth  - 1) nodes[y * glbMapWidth + x].vecNeighbours.push_back( &nodes[(y + 0) * glbMapWidth + (x + 1)]);
        }

    // Manually position the start and end markers so they are not nullptr
    nodeStart = &nodes[                 0  * glbMapWidth +                0  ];
    nodeEnd   = &nodes[ (glbMapHeight - 1) * glbMapWidth + (glbMapWidth - 1) ];
}

// function to calculate the distance - for convenience
float distance_AStar( sNode* a, sNode* b ) {
    // ASSUMPTION, a is current node under analysis and b is one of it's neighbours
    // the distance is determined by the risk level to enter location b
    return (float)b->riskLevel;
}

// function to calculate the heuristic.
// the more accurate the heuristic is, the more efficient the algorithm works
float heuristic_AStar(sNode* a, sNode* b) {
    return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

// if strictShortest == true, than the whole graph is analyzed guaranteeing the shortest path.
// if it is false, than the first valid path found suffices.
// returns true if a path was found, false otherwise
// the path is left as info in the graph
bool Solve_AStar( bool strictShortest ) {
    // Reset navigation graph - default all node states
    for (int y = 0; y < glbMapHeight; y++)
        for (int x = 0; x < glbMapWidth; x++) {
            nodes[y * glbMapWidth + x].bVisited    = false;
            nodes[y * glbMapWidth + x].fGlobalGoal = INFINITY;
            nodes[y * glbMapWidth + x].fLocalGoal  = INFINITY;
            nodes[y * glbMapWidth + x].parent      = nullptr;
        }

    // Setup starting conditions
    sNode *nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = heuristic_AStar( nodeStart, nodeEnd );

    // Add start node to not tested list - this will ensure it gets tested
    // As the algorithm progresses, newly discovered nodes get added to the
    // list, and will themselves be tested later
    std::list<sNode*> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);

    // Start algorithm loop. If strictShortest is true, the algorithm searches the complete graph.
    // If strictShortest is false the algorithm terminates after finding the first valid path.
    while (!listNotTestedNodes.empty() && (strictShortest || nodeCurrent != nodeEnd )) {
        // sort list in descending order of global goal
        // the sort function takes as argument a lambda function that provides the sorting criterium

        // experiment with no sorting
        listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

        // Front of listNotTestedNodes is potentially the lowest distance node
        // list may also contain nodes that have been visited, so ditch these ...
        while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
            listNotTestedNodes.pop_front();
        // ... or abort because there are no valid nodes left to test
        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->bVisited = true;              // only explore a node once

        // check each of this node's neighbours ...
        for (auto nodeNeighbour : nodeCurrent->vecNeighbours) {

            // ... and only if the neighbour is not visited, add it to the NotTested List
            if (!nodeNeighbour->bVisited)
                listNotTestedNodes.push_back( nodeNeighbour );

            // Calculate the neighbours potential lowest parent distance
            float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance_AStar( nodeCurrent, nodeNeighbour );

            // If choosing to path through this node is a lower distance than what
            // the neighbour currently has set, update the neighbour to use this node
            // as the path source, and set its distance scores as necessary
            if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

                // the best path length to the neighbour being tested has changed, so update the neighbour's score.
                // The heuristic is used to globally bias the path algorithm, so it knows if its getting better or worse.
                // At some point the algo will realise this path is worse and abandon it, and then go and search along the next best path.
                nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic_AStar( nodeNeighbour, nodeEnd );
            }
        }
    }
    return nodeCurrent == nodeEnd;
}

// to output the result of the A* algo
void Print_AStarPath( bool bPrintContent ) {
    int nTotalRisk = 0;
    int stepsUp = 0, stepsDn = 0, stepsLt = 0, stepsRt = 0;
    // Display Path by starting at the end, and following the parent node trail
    // back to the start - the start node will not have a parent path to follow
    if (nodeEnd != nullptr) {
        sNode *p = nodeEnd;
        while (p->parent != nullptr) {
            if (bPrintContent) {
                std::cout << "(" << p->x          << "," << p->y         << ") - risk level " << p->riskLevel         << " <<--- ";      // display node
                std::cout << "(" << p->parent->x  << "," << p->parent->y << ") - risk level " << p->parent->riskLevel << std::endl;           // display parent node
            }

            if (p->x < p->parent->x) stepsLt += 1;
            if (p->y < p->parent->y) stepsUp += 1;
            if (p->x > p->parent->x) stepsRt += 1;
            if (p->y > p->parent->y) stepsDn += 1;

            nTotalRisk += p->riskLevel;
            // Set next node to this node's parent
            p = p->parent;
        }
    }
    std::cout << "Total risk level (excl. start position) = " << nTotalRisk << std::endl;
    std::cout << "# steps - up: " << stepsUp << " left: " << stepsLt << " down: " << stepsDn << " right: " << stepsRt << std::endl;
}

// ============================================================

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    DataStream riskData;
    GetInput( riskData, glbProgPhase != PUZZLE );
    std::cout << "Data stats - number of risk data lines: " << riskData.size() << std::endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    bool bFlag;

    InitializeGraph_part1( riskData );

    tmr.TimeReport( "Timing 1: [ graph init ] " );   // ==============================^^^^^

    bFlag = Solve_AStar( false );
    std::cout << "A* algorithm - " << (bFlag ? "success " : "failure ") << std::endl;
    Print_AStarPath( glbProgPhase != PUZZLE );

    tmr.TimeReport( "Timing 1: [ A* part ] " );   // ==============================^^^^^

// ========== part 2

    InitializeGraph_part2( riskData );

    tmr.TimeReport( "Timing 2: [ graph init ] " );   // ==============================^^^^^

    bFlag = Solve_AStar( false );
    std::cout << "A* algorithm - " << (bFlag ? "success " : "failure ") << std::endl;
    Print_AStarPath( glbProgPhase != PUZZLE );

    tmr.TimeReport( "Timing 2: [ A* part ] " );   // ==============================^^^^^

    return 0;
}


