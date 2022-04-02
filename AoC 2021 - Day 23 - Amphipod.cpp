// AoC 2021 - day 23
// =================

// date:  2021-12-23
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
    PUZZLE,
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

#define AMBER  'A'
#define BRONZE 'B'
#define CUPPER 'C'
#define DESERT 'D'
#define EMPTY  '.'

struct sSit {
    char hallway[11]    = { EMPTY };
    char sideroom[4][4] = { EMPTY };
};
typedef vector<sSit> sitStream;

// ==========   DATA INPUT FUNCTIONS

void GetExample( sSit &s ) {
    // clear the hallway
    for (int i = 0; i < 11; i++) s.hallway[i] = EMPTY;
    // put amphipods in side rooms
    s.sideroom[0][0] = AMBER ; s.sideroom[0][1] = DESERT; s.sideroom[0][2] = DESERT; s.sideroom[0][3] = BRONZE;
    s.sideroom[1][0] = DESERT; s.sideroom[1][1] = BRONZE; s.sideroom[1][2] = CUPPER; s.sideroom[1][3] = CUPPER;
    s.sideroom[2][0] = CUPPER; s.sideroom[2][1] = AMBER ; s.sideroom[2][2] = BRONZE; s.sideroom[2][3] = BRONZE;
    s.sideroom[3][0] = AMBER ; s.sideroom[3][1] = CUPPER; s.sideroom[3][2] = AMBER;  s.sideroom[3][3] = DESERT;
}

// inserted wrt part 1
//  #D#C#B#A#
//  #D#B#A#C#

//###B#C#B#D###   // example part 2
//  #D#C#B#A#
//  #D#B#A#C#
//  #A#D#C#A#


void GetTest( sSit &s ) {
    for (int i = 0; i < 11; i++) s.hallway[i] = EMPTY;
    s.sideroom[0][0] = AMBER ; s.sideroom[0][1] = BRONZE; s.sideroom[0][2] = EMPTY; s.sideroom[0][3] = EMPTY;
    s.sideroom[1][0] = DESERT; s.sideroom[1][1] = CUPPER; s.sideroom[1][2] = EMPTY; s.sideroom[1][3] = EMPTY;
    s.sideroom[2][0] = CUPPER; s.sideroom[2][1] = BRONZE; s.sideroom[2][2] = EMPTY; s.sideroom[2][3] = EMPTY;
    s.sideroom[3][0] = AMBER ; s.sideroom[3][1] = DESERT; s.sideroom[3][2] = EMPTY; s.sideroom[3][3] = EMPTY;
    // testset is working
//    s.sideroom[0][0] = BRONZE; s.sideroom[0][1] = EMPTY; s.sideroom[0][2] = EMPTY; s.sideroom[0][3] = EMPTY;
//    s.sideroom[1][0] = DESERT; s.sideroom[1][1] = EMPTY; s.sideroom[1][2] = EMPTY; s.sideroom[1][3] = EMPTY;
//    s.sideroom[2][0] = AMBER ; s.sideroom[2][1] = EMPTY; s.sideroom[2][2] = EMPTY; s.sideroom[2][3] = EMPTY;
//    s.sideroom[3][0] = CUPPER; s.sideroom[3][1] = EMPTY; s.sideroom[3][2] = EMPTY; s.sideroom[3][3] = EMPTY;

//    s.sideroom[0][0] = AMBER ; s.sideroom[0][1] = BRONZE; s.sideroom[0][2] = EMPTY; s.sideroom[0][3] = EMPTY;
//    s.sideroom[1][0] = BRONZE; s.sideroom[1][1] = CUPPER; s.sideroom[1][2] = EMPTY; s.sideroom[1][3] = EMPTY;
//    s.sideroom[2][0] = CUPPER; s.sideroom[2][1] = AMBER ; s.sideroom[2][2] = EMPTY; s.sideroom[2][3] = EMPTY;
//    s.sideroom[3][0] = DESERT; s.sideroom[3][1] = DESERT; s.sideroom[3][2] = EMPTY; s.sideroom[3][3] = EMPTY;
}

//#############    // example part 1
//#...........#    a 4 + 4 = 8 *    1 =    8
//###B#C#B#D###    b 2 + 2 = 4 *   10 =   40
//                 c 2 + 4 = 6 *  100 =  600
//                 d 4 + 2 = 6 * 1000 = 6000
//  #A#D#C#A#      total                6648 additional energy
//  #########

void GetPuzzle( sSit &s ) {
    for (int i = 0; i < 11; i++) s.hallway[i] = EMPTY;
    s.sideroom[0][0] = BRONZE; s.sideroom[0][1] = DESERT; s.sideroom[0][2] = DESERT; s.sideroom[0][3] = CUPPER;
    s.sideroom[1][0] = AMBER ; s.sideroom[1][1] = BRONZE; s.sideroom[1][2] = CUPPER; s.sideroom[1][3] = AMBER;
    s.sideroom[2][0] = BRONZE; s.sideroom[2][1] = AMBER ; s.sideroom[2][2] = BRONZE; s.sideroom[2][3] = DESERT;
    s.sideroom[3][0] = CUPPER; s.sideroom[3][1] = CUPPER; s.sideroom[3][2] = AMBER;  s.sideroom[3][3] = DESERT;
}

//#############   // part 2 puzzle
//#...........#
//###C#A#D#D###
//  #D#C#B#A#
//  #D#B#A#C#
//  #B#A#B#C#
//  #########

// ==========   OUTPUT FUNCTIONS

void PrintSituation( sSit &s ) {
    cout << "#############" << endl;
    string aux;
    aux.append( "#" );
    for (int i = 0; i < 11; i++) aux.append( string( 1, s.hallway[i] ));
    aux.append( "#" );
    cout << aux << endl; aux.clear();
    aux.append( "###" ); aux.append( string( 1, s.sideroom[0][3] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[1][3] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[2][3] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[3][3] ));
    aux.append( "###" );
    cout << aux << endl; aux.clear();
    aux.append( "  #" ); aux.append( string( 1, s.sideroom[0][2] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[1][2] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[2][2] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[3][2] ));
    aux.append( "#  " );
    cout << aux << endl; aux.clear();
    aux.append( "  #" ); aux.append( string( 1, s.sideroom[0][1] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[1][1] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[2][1] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[3][1] ));
    aux.append( "#  " );
    cout << aux << endl; aux.clear();
    aux.append( "  #" ); aux.append( string( 1, s.sideroom[0][0] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[1][0] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[2][0] ));
    aux.append( "#"   ); aux.append( string( 1, s.sideroom[3][0] ));
    aux.append( "#  " );
    cout << aux << endl; aux.clear();
    cout << "  #########  " << endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgPhase)
void GetInput( sitStream &sData, bool bDisplay = false ) {

    sSit tmp;
    sData.clear();
    switch (glbProgPhase) {
        case EXAMPLE: GetExample( tmp ); break;
        case TEST:    GetTest(    tmp ); break;
        case PUZZLE:  GetPuzzle(  tmp ); break;
    }
    sData.push_back( tmp );
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintSituation( sData[0] );
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

// Model the positions:
//    0 .. 10 --> all positions in the hallway from left to right
//    11, 12, 13, 14  --> from bottom to top position of first / leftmost side room (A-room)
//    15, 16, 17, 18  --> same B-room
//    19, 20, 21, 22  --> same C-room
//    23, 24, 25, 26   -> same D-room


bool IsHallway( int pos ) {
    if (pos < 0 || pos > 26) cout << "ERROR: IsHallway() --> called with invalid pos argument: " << pos << endl;

    return (pos >= 0 && pos <= 10);
}

bool IsSideroom( int pos ) {
    if (pos < 0 || pos > 26) cout << "ERROR: IsSideroom() --> called with invalid pos argument: " << pos << endl;

    return (pos >= 11 && pos <= 26);
}

char GetType( sSit &curSit, int pos ) {
    if (!IsHallway( pos ) && !IsSideroom( pos )) cout << "ERROR: GetType() --> called with invalid pos argument: " << pos << endl;

    char result = EMPTY;
    if (IsHallway( pos ))
        result = curSit.hallway[pos];
    else if (IsSideroom( pos )) {
        int siderIx = (pos - 11) / 4;
        int inrmIx  = (pos - 11) % 4;
        result = curSit.sideroom[siderIx][inrmIx];
    }
    return result;
}

void SetType( sSit &curSit, int pos, char type ) {
    if (!IsHallway( pos ) && !IsSideroom( pos )) cout << "ERROR: SetType() --> called with invalid pos argument: " << pos << endl;
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: SetType() --> called with invalid type argument: " << type << endl;

    if (IsHallway( pos ))
        curSit.hallway[pos] = type;
    else if (IsSideroom( pos )) {
        int siderIx = (pos - 11) / 4;
        int inrmIx  = (pos - 11) % 4;
        curSit.sideroom[siderIx][inrmIx] = type;
    }
}

bool IsOccupied( sSit &curSit, int pos ) {
    if (!IsHallway( pos ) && !IsSideroom( pos )) cout << "ERROR: IsOccupied() --> called with invalid pos argument: " << pos << endl;

    return GetType( curSit, pos ) != EMPTY;
}

bool IsSolved( sSit &curSit ) {
    bool flag = true;
    // if solved the hallway must be empty ...
    for (int i = 0; i < 11; i++)
        flag &= curSit.hallway[i] == EMPTY;
    // ... and all amphipods must be in their right sideroom
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
            flag &= (curSit.sideroom[j][k] == (char)(AMBER + j) ||
                     curSit.sideroom[j][k] == EMPTY);
        }
    }
    return flag;
}

bool IsForbidden( int pos ) {
    if (!IsHallway( pos ) && !IsSideroom( pos )) cout << "ERROR: IsForbidden() --> called with invalid pos argument: " << pos << endl;

    return (pos == 2 || pos == 4 || pos == 6 || pos == 8);
}

int GetSideroomEntrance( char type ) {
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: GetSideroomEntrance() --> called with invalid type argument: " << type << endl;

    return (int)(type - AMBER) * 2 + 2;
}

// gives the entrance position of the side room that corresponds with pos
int GetSideroomExit( int pos ) {
    if (!IsSideroom( pos )) cout << "ERROR: GetSideroomExit() --> called with non-sideroom pos argument: " << pos << endl;

    return (((pos - 11) / 4) + 1) * 2;
}

bool IsRightSideroom( int pos, char type ) {
    if (!IsHallway( pos ) && !IsSideroom( pos )) cout << "ERROR: IsRightSideroom() --> called with invalid pos argument: "  << pos << endl;
    if (!IsSideroom( pos ))                      cout << "ERROR: IsRightSideroom() --> argument is no side room position: " << pos << endl;
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: IsRightSideroom() --> called with invalid type argument: " << type << endl;

    int nStart = (int)(type - AMBER) * 4 + 11;
    return (nStart <= pos && pos <= (nStart + 3));
}

// only works if pos is in hallway
bool MustGoRight( char type, int pos ) {
    if (!IsHallway( pos )) cout << "ERROR: MustGoRight() --> argument is not in hallway: "  << pos << endl;
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: MustGoRight() --> called with invalid type argument: " << type << endl;

    return (pos < GetSideroomEntrance( type ));
}

bool MustGoLeft(  char type, int pos ) {
    if (!IsHallway( pos )) cout << "ERROR: MustGoLeft() --> argument is not in hallway: "  << pos << endl;
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: MustGoLeft() --> called with invalid type argument: " << type << endl;

    return (pos > GetSideroomEntrance( type ));
}

// returns true if otherPos is in between myPos and the entrance of the side room of type
// only works if myPos and otherPos are in hallway
bool InBetweenSRentrance( char type, int myPos, int otherPos ) {
    if (!IsHallway( myPos    )) cout << "ERROR: InBetweenSRentrance() --> argument myPos is not in hallway: "     << myPos << endl;
    if (!IsHallway( otherPos )) cout << "ERROR: InBetweenSRentrance() --> argument otherPos is not in hallway: "  << otherPos << endl;
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: InBetweenSRentrance() --> called with invalid type argument: " << type << endl;

    int srEntrance = GetSideroomEntrance( type );
    return (srEntrance < otherPos && otherPos < myPos) ||
           (srEntrance > otherPos && otherPos > myPos);
}

bool IsDeadlock( sSit &curSit ) {
    bool bResult = false;
    // check all types in hallway
    for (int i = 3; i <= 7; i += 2) {
        char curType = GetType( curSit, i );
        if (curType != EMPTY) {
            bool bRightBlocked = false;
            bool bLeftBlocked  = false;
            // look right
            for (int j = i + 2; j <= 7 && !bResult; j += 2) {
                char otherType = GetType( curSit, j );
                if (otherType != EMPTY) {
                    bRightBlocked = MustGoRight(   curType, i ) && InBetweenSRentrance(   curType, i, j );
                    bLeftBlocked  = MustGoLeft(  otherType, j ) && InBetweenSRentrance( otherType, j, i );
                    bResult = (bRightBlocked && bLeftBlocked);
                }
            }
            // look left
            for (int j = i - 2; j >= 3 && !bResult; j -= 2) {
                char otherType = GetType( curSit, j );
                if (otherType != EMPTY) {
                    bLeftBlocked  = MustGoLeft(    curType, i ) && InBetweenSRentrance(   curType, i, j );
                    bRightBlocked = MustGoRight( otherType, j ) && InBetweenSRentrance( otherType, j, i );
                    bResult = (bRightBlocked && bLeftBlocked);
                }
            }
        }
    }
//    return bResult;
    return false;
}

bool RangeHasValue( sSit &curSit, int strtPos, int stopPos, char type ) {
    if (!IsHallway( strtPos ) && !IsSideroom( strtPos )) cout << "ERROR: RangeHasValue() --> called with invalid strtPos argument: "  << strtPos << endl;
    if (!IsHallway( stopPos ) && !IsSideroom( stopPos )) cout << "ERROR: RangeHasValue() --> called with invalid stopPos argument: "  << stopPos << endl;
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: RangeHasValue() --> called with invalid type argument: " << type << endl;

    if (strtPos > stopPos)
        cout << "WARNING: RangeHasValue() --> startPos / stopPos out of order: " << endl;
    bool result = true;
    for (int i = strtPos; i <= stopPos; i++) {
        result &= (GetType( curSit, i ) == type);
    }
    return result;
}

int CanMove( sSit &curSit, int curPos, vector<int> &newPositions ) {
    if (!IsHallway( curPos ) && !IsSideroom( curPos )) cout << "ERROR: CanMove() --> called with invalid curPos argument: "  << curPos << endl;

//cout << "CanMove() --> curPos: " << curPos;

    int nrMoves = 0;
    char type = GetType( curSit, curPos );

    if (type == EMPTY) cout << "ERROR: CanMove() --> curPos is empty: " << curPos << endl;

    if (IsHallway( curPos )) {  // only option is to move into the right side room

//cout << ", in hallway";

        // check if the path upto the entrance is clear
        int srEntry = GetSideroomEntrance( type );
        bool bPathIsFree = true;
        if (MustGoLeft( type, curPos )) {
            for (int i = curPos - 1; i >= srEntry; i--) {
                bPathIsFree &= !IsOccupied( curSit, i );
            }
        } else if (MustGoRight( type, curPos )) {
            for (int i = curPos + 1; i <= srEntry; i++) {
                bPathIsFree &= !IsOccupied( curSit, i );
            }
        } else {
            cout << "ERROR: CanMove() --> need not go left neither go right ...?? " << curPos << endl;
        }
        // check if there's space to move there
        if (bPathIsFree) {
            int  srmIx = 11 + (int)(type - AMBER) * 4;

            // bottom position is free and pathway is not occupied
            if (!IsOccupied( curSit, srmIx + 0 ) &&                                                       RangeHasValue( curSit, srmIx + 1, srmIx + 3, EMPTY )) {
                nrMoves += 1; newPositions.push_back( srmIx + 0 );
            } else
            // position right above bottom is free, ... Check if there's another species below it... (and the pathway is clear)
            if (!IsOccupied( curSit, srmIx + 1) && RangeHasValue( curSit, srmIx + 0, srmIx + 0, type ) && RangeHasValue( curSit, srmIx + 2, srmIx + 3, EMPTY )) {
                nrMoves += 1; newPositions.push_back( srmIx + 1 );
            } else
            // position right below top is free, ... Check if there's another species below it... (and the pathway is clear)
            if (!IsOccupied( curSit, srmIx + 2) && RangeHasValue( curSit, srmIx + 0, srmIx + 1, type ) && RangeHasValue( curSit, srmIx + 3, srmIx + 3, EMPTY )) {
                nrMoves += 1; newPositions.push_back( srmIx + 2 );
            } else
            // Top position is free, ... Check if there's another species below it... !!!
            if (!IsOccupied( curSit, srmIx + 3) && RangeHasValue( curSit, srmIx + 0, srmIx + 2, type )) {
                nrMoves += 1; newPositions.push_back( srmIx + 3 );
            }
        }
    } else if (IsSideroom( curPos )) { // only option is to move into the hallway

//cout << ", in sideroom";

        int offsettedPos = (curPos - 11) % 4;

        if (offsettedPos == 0 && IsRightSideroom( curPos, type )) {
            // the current position is the bottom of the side room AND this is the correct side room for it --> don't move
            nrMoves = 0;
        } else if (offsettedPos == 1 && IsRightSideroom( curPos, type ) && RangeHasValue( curSit, curPos - 1, curPos - 1, type )) {
            // the same but one position higher
            nrMoves = 0;
        } else if (offsettedPos == 2 && IsRightSideroom( curPos, type ) && RangeHasValue( curSit, curPos - 2, curPos - 1, type )) {
            // the same but one position higher
            nrMoves = 0;
        } else if (offsettedPos == 3 && IsRightSideroom( curPos, type ) && RangeHasValue( curSit, curPos - 3, curPos - 1, type )) {
            // the same but one position higher
            nrMoves = 0;
        } else if (offsettedPos == 0 && IsOccupied( curSit, curPos + 1 )) {
            // the current position is the bottom of the side room AND the position above that is blocked...
            nrMoves = 0;
        } else if (offsettedPos == 1 && IsOccupied( curSit, curPos + 1 )) {
            // the same but one position higher
            nrMoves = 0;
        } else if (offsettedPos == 2 && IsOccupied( curSit, curPos + 1 )) {
            // the same but one position higher
            nrMoves = 0;

        } else {
            // should I discriminate between top and bottom positions? I.e. if curpos is bottom position, the
            // amphipod could just move to the top position
            // THIS IS STILL A POSSIBILITY - the rules are not explicit on this

            // try to the left of the side room exit point
            bool bIsBlocked = false;
            for (int i = GetSideroomExit( curPos ) - 1; i >= 0 && !bIsBlocked; i--) {
                bIsBlocked = IsOccupied( curSit, i );
                if (!bIsBlocked && !IsForbidden( i )) {
                    nrMoves += 1;
                    newPositions.push_back( i );
                }
            }
            // try to the right of the side room exit point
            bIsBlocked = false;
            for (int i = GetSideroomExit( curPos ) + 1; i <= 10 && !bIsBlocked; i++) {
                bIsBlocked = IsOccupied( curSit, i );
                if (!bIsBlocked && !IsForbidden( i )) {
                    nrMoves += 1;
                    newPositions.push_back( i );
                }
            }
        }
    } else {
        cout << "ERROR: CanMove() --> curPos is not in hallway neither in side room: " << curPos << endl;
    }

//    cout << " - nr possible moves found = " << nrMoves << ": ";
//    for (int i = 0; i < (int)newPositions.size(); i++) {
//        cout << newPositions[i] << " ";
//    }
//    cout << endl;
    return nrMoves;
}

struct sMove {
    char type;        // Amber, Bronze, Cupper or Desert
    int fromPos;      // moves from ...
    int toPos;        // ... to ...

    int nCost = 0;
    int nCumCost = 0;
};
typedef vector<sMove> MoveStream;

int GetDistance( int posA, int posB ) {
    if (!IsHallway( posA ) && !IsSideroom( posA )) cout << "ERROR: GetDistance() --> called with invalid posA argument: "  << posA << endl;
    if (!IsHallway( posB ) && !IsSideroom( posB )) cout << "ERROR: GetDistance() --> called with invalid posB argument: "  << posB << endl;

    int hw_posA, hw_posB;
    int result = 0;
    switch (posA) {
        case 11: case 12: case 13: case 14: result += (15 - posA); hw_posA = 2; break;
        case 15: case 16: case 17: case 18: result += (19 - posA); hw_posA = 4; break;
        case 19: case 20: case 21: case 22: result += (23 - posA); hw_posA = 6; break;
        case 23: case 24: case 25: case 26: result += (27 - posA); hw_posA = 8; break;
        default: hw_posA = posA;
    }
    switch (posB) {
        case 11: case 12: case 13: case 14: result += (15 - posB); hw_posB = 2; break;
        case 15: case 16: case 17: case 18: result += (19 - posB); hw_posB = 4; break;
        case 19: case 20: case 21: case 22: result += (23 - posB); hw_posB = 6; break;
        case 23: case 24: case 25: case 26: result += (27 - posB); hw_posB = 8; break;
        default: hw_posB = posB;
    }
    result += abs( hw_posA - hw_posB );
    return result;
}

int GetEnergy( char type ) {
    if (type != AMBER && type != BRONZE && type != CUPPER && type != DESERT && type != EMPTY) cout << "ERROR: GetEnergy() --> called with invalid type argument: " << type << endl;

    switch (type) {
        case AMBER : return    1;
        case BRONZE: return   10;
        case CUPPER: return  100;
        case DESERT: return 1000;
    }
    return -1;
}

int glbLowestCost  = INT_MAX;
int glbDeadlockCnt = 0;
int glbRejectedCnt = 0;

// keeps track of indentation level
int nIndentCntr = 0;

// generic indentation function
void Indent() {
    for (int j = 0; j < nIndentCntr; j++) {
        for (int i = 0; i < 4; i++)
            cout << " ";
    }
}

void DoOneMove( sSit &curSit, MoveStream &moves ) {

    if (IsSolved( curSit )) {
        // success - output moves that lead upto it
        Indent();
        cout << "YESSS!!! - found solution: " << endl;
        // for displaying the moves
        sSit auxSit;
        switch (glbProgPhase) {
            case EXAMPLE: GetExample( auxSit ); break;
            case TEST:    GetTest(    auxSit ); break;
            case PUZZLE:  GetPuzzle(  auxSit ); break;
        }
        PrintSituation( auxSit );

        for (int i = 0; i < (int)moves.size(); i++) {
            sMove aux = moves[i];
            Indent();
            cout << "Move: " <<  i << " type: _" << aux.type << "_ moved from: " << aux.fromPos << " to: " << aux.toPos
                               << " move cost: " << aux.nCost << " cum. cost: " << aux.nCumCost << endl << endl;

            SetType( auxSit, aux.fromPos, EMPTY );
            SetType( auxSit, aux.toPos, aux.type );
            PrintSituation( auxSit );
            cout << endl;
        }

        if (moves.empty()) {
            glbLowestCost = 0;
        } else if (moves.back().nCumCost < glbLowestCost) {
            glbLowestCost = moves.back().nCumCost;
        }

    } else if (IsDeadlock( curSit )) {
        glbDeadlockCnt += 1;
        if (glbDeadlockCnt % 1000000 == 0) {
            Indent();
            cout << "# deadlocks: " << glbDeadlockCnt << endl;
        }
    } else {
        // check for each amphipod what the move possibilities are, and (recursively) move along
        // if there are no possibilities, the recursive branching will die out
        for (int curPos = 0; curPos <= 26; curPos++) {
            char curType = GetType( curSit, curPos );
            if (curType != EMPTY) {
                vector<int> canMoveTo;

                int nrOptions = CanMove( curSit, curPos, canMoveTo );
                if (nrOptions != (int)canMoveTo.size())
                    cout << "WARNING: DoOneMove() --> mismatch between list size and reported number of moves" << endl;

                for (int j = 0; j < (int)canMoveTo.size(); j++) {
                    // make copies of the variable to pass to the recursive call

                    // extend moves list
                    int newPos  = canMoveTo[j];
                    int nrSteps = GetDistance( curPos, newPos );
                    int energy  = GetEnergy( curType );
                    sMove nextMove;
                    nextMove.type    = curType;
                    nextMove.fromPos = curPos;
                    nextMove.toPos   = newPos;
                    nextMove.nCost   = nrSteps * energy;
                    int prevCost = (moves.empty() ? 0 : moves.back().nCumCost);
                    nextMove.nCumCost = prevCost + nextMove.nCost;
                    MoveStream newMvStr( moves );   // use copy constructor...
                    newMvStr.push_back( nextMove );
                    // adapt situation according to this move
                    sSit nextSit( curSit );         // use copy constructor...
                    SetType( nextSit, newPos, curType );
                    SetType( nextSit, curPos, EMPTY   );
                    if (nextMove.nCumCost < glbLowestCost) {
                        nIndentCntr += 1;
//                        Indent(); cout << "DoOneMove() - type: " << curType << " from " << curPos << " to " << newPos << " cost " << nextMove.nCost << endl;
                        // recursive call - only if current cost still lower than current min cost
                        DoOneMove( nextSit, newMvStr );
                        nIndentCntr -= 1;
                    } else {
                        nIndentCntr += 1;
//                        Indent(); cout << "DoOneMove() - rejected" << endl;
                        nIndentCntr -= 1;

                        glbRejectedCnt += 1;
                        if (glbRejectedCnt % 1000000 == 0) {
                            Indent();
                            cout << "# rejections: " << glbRejectedCnt << endl;
                        }
                    }
                }
            }
        }
    }
}


int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    sitStream sitData;

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    GetInput( sitData, true );
    cout << "Data stats - #situations " << sitData.size() << endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1



    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    MoveStream movesList;
    DoOneMove( sitData[0], movesList );
    cout << "Answer: " << glbLowestCost << endl;
    cout << "btw INT_MAX == " << INT_MAX << endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}
