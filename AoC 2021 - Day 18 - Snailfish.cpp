// AoC 2021 - day 18
// =================

// date:  2021-12-18
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

#include "../flcTimer.h"
#include "../generic_tree.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef std::string sData;
typedef std::vector<sData> DataStream;

// ==========   DATA INPUT FUNCTIONS

void MakeExample( DataStream &vData ) {
    vData.clear();
    sData datum;
    char choice;

    cout << "Which example? (0-6, a-e, !, A-E) ";
    cin >> choice;
    switch (choice) {
        case '0': datum = "[1,2]";                                                         vData.push_back( datum ); break;
        case '1': datum = "[[1,2],3]";                                                     vData.push_back( datum ); break;
        case '2': datum = "[9,[8,7]]";                                                     vData.push_back( datum ); break;
        case '3': datum = "[[1,9],[8,5]]";                                                 vData.push_back( datum ); break;
        case '4': datum = "[[[[1,2],[3,4]],[[5,6],[7,8]]],9]";                             vData.push_back( datum ); break;
        case '5': datum = "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]";                     vData.push_back( datum ); break;
        case '6': datum = "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]"; vData.push_back( datum ); break;
        // explode testing
        case 'a': datum = "[[[[[9,8],1],2],3],4]";                 vData.push_back( datum ); break;
        case 'b': datum = "[7,[6,[5,[4,[3,2]]]]]";                 vData.push_back( datum ); break;
        case 'c': datum = "[[6,[5,[4,[3,2]]]],1]";                 vData.push_back( datum ); break;
        case 'd': datum = "[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]"; vData.push_back( datum ); break;
        case 'e': datum = "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]";     vData.push_back( datum ); break;
        // combined testing
        case '!': datum = "[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]"; vData.push_back( datum ); break;
        // sum testing
        case 'A': datum = "[[[[1,1],[2,2]],[3,3]],[4,4]]"; vData.push_back( datum ); break;
        case 'B':
            datum = "[1,1]";  vData.push_back( datum );
            datum = "[2,2]";  vData.push_back( datum );
            datum = "[3,3]";  vData.push_back( datum );
            datum = "[4,4]";  vData.push_back( datum );
            break;
        case 'C':
            datum = "[1,1]";  vData.push_back( datum );
            datum = "[2,2]";  vData.push_back( datum );
            datum = "[3,3]";  vData.push_back( datum );
            datum = "[4,4]";  vData.push_back( datum );
            datum = "[5,5]";  vData.push_back( datum );
            break;
        case 'D':
            datum = "[1,1]";  vData.push_back( datum );
            datum = "[2,2]";  vData.push_back( datum );
            datum = "[3,3]";  vData.push_back( datum );
            datum = "[4,4]";  vData.push_back( datum );
            datum = "[5,5]";  vData.push_back( datum );
            datum = "[6,6]";  vData.push_back( datum );
            break;
        case 'E':
            datum = "[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]";              vData.push_back( datum );
            datum = "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]";                      vData.push_back( datum );
            datum = "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]";              vData.push_back( datum );
            datum = "[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]";  vData.push_back( datum );
            datum = "[7,[5,[[3,8],[1,4]]]]";                                  vData.push_back( datum );
            datum = "[[2,[2,2]],[8,[8,1]]]";                                  vData.push_back( datum );
            datum = "[2,9]";                                                  vData.push_back( datum );
            datum = "[1,[[[9,3],9],[[9,0],[0,7]]]]";                          vData.push_back( datum );
            datum = "[[[5,[7,4]],7],1]";                                      vData.push_back( datum );
            datum = "[[[[4,2],2],6],[8,7]]";                                  vData.push_back( datum );
            break;
        case 'F':
            datum = "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]"; vData.push_back( datum );
            datum = "[[[5,[2,8]],4],[5,[[9,9],0]]]";                     vData.push_back( datum );
            datum = "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]";                 vData.push_back( datum );
            datum = "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]";                 vData.push_back( datum );
            datum = "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]";             vData.push_back( datum );
            datum = "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]";             vData.push_back( datum );
            datum = "[[[[5,4],[7,7]],8],[[8,3],8]]";                     vData.push_back( datum );
            datum = "[[9,3],[[9,9],[6,[4,9]]]]";                         vData.push_back( datum );
            datum = "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]";             vData.push_back( datum );
            datum = "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]";         vData.push_back( datum );
            break;

        default:  datum = "<incorrect choice>";
    }
}

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
        case EXAMPLE: MakeExample(                   vData ); break;
        case TEST:    ReadData( "input.test.txt"   , vData ); break;
        case PUZZLE:  ReadData( "input.txt"        , vData ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintDataStream( vData );
    }
}

// ==========  PUZZLE SPECIFIC SOLUTIONS

// should be an enum...
#define UNKNOWN 0
#define ATOMIC  1
#define PAIR    2

struct sElt {
    int nType  = UNKNOWN;
    int nValue = 0;
    int nLevel = 0;
};

// convenience getters and setters that can be addressed with a void *
// useful in combination with generic_tree module
int GetType(  void *eltPtr ) { return ((sElt *)eltPtr)->nType;  }
int GetValue( void *eltPtr ) { return ((sElt *)eltPtr)->nValue; }
int GetLevel( void *eltPtr ) { return ((sElt *)eltPtr)->nLevel; }

void SetType(  void *eltPtr, int nType  ) { ((sElt *)eltPtr)->nType = nType; }
void SetValue( void *eltPtr, int nValue ) { ((sElt *)eltPtr)->nValue = nValue; }
void SetLevel( void *eltPtr, int nLevel ) { ((sElt *)eltPtr)->nLevel = nLevel; }

generic_tree glbTree;
typedef generic_tree_node sNode;

// Is this one still being used? Check !!!
sElt MakeElt( int nType, int nVal, int nLevel ) {
    sElt result;
    result.nType  = nType;
    result.nValue = nVal;
    result.nLevel = nLevel;
    return result;
}

void *MakeEltDynamic( int nType, int nVal, int nLevel ) {
    sElt *result = new sElt;
    result->nType  = nType;
    result->nValue = nVal;
    result->nLevel = nLevel;
    return (void *)result;
}

void PrintElt( sElt &elt ) {
    switch( elt.nType ) {
        case UNKNOWN: cout << "UNKNOWN, "; break;
        case ATOMIC:  cout << "ATOMIC,  "; break;
        case PAIR:    cout << "PAIR,    "; break;
        default:      cout << "** ERROR ** "; break;
    }
    cout << "value = " << elt.nValue << ", ";
    cout << "level = " << elt.nLevel << ", ";
}
// overload so that it accepts void * argument as well
void PrintElt( void *eltPtr ) { PrintElt( *((sElt *)eltPtr) ); }

void DisposeElt( void *eltPtr ) {
    delete ((sElt *)eltPtr);
}

// prototype / forward declaration
sNode *ParsePair( string &in, int nLevel, bool bOutput = false );

// keeps track of indentation level
int nIndentCntr = 0;

// generic indentation function
void Indent() {
    for (int j = 0; j < nIndentCntr; j++) {
        for (int i = 0; i < 4; i++)
            cout << " ";
    }
}


// parsing ----------

// parses a token from input of length len, and returns the two result std::strings as parameters
// token and remainder
void ParseTokenLen( string &worksString, int len, string &token, string &remainder ) {
    token     = worksString.substr( 0, len );
    remainder = worksString.erase(  0, len );
}

sNode *ParseRegNr( string &in, int nLevel, bool bOutput = false ) {
    string token, remainder;
    ParseTokenLen( in, 1, token, remainder );
    in = remainder;
    int result = (int)(token[0] - '0');
    if (bOutput) { Indent(); cout << "parsing RegNr - token: " << token << " value " << result << endl; }
    return glbTree.MakeNode( MakeEltDynamic( ATOMIC, result, nLevel ));
}

sNode *ParseElt( string &in, int nLevel, bool bOutput = false ) {
    sNode *result = nullptr;
    char c = in[0];
    if (c >= '0' && c <= '9') {
        result = ParseRegNr( in, nLevel, bOutput );
    } else {
        result = ParsePair( in, nLevel, bOutput );
    }
    return result;
}

sNode *ParsePair( string &in, int nLevel, bool bOutput ) {

    string token, remainder;
    sNode *elt1 = nullptr;
    sNode *elt2 = nullptr;
    sNode *result;

    auto char_check = [=]( char check, char expected ) {
        if (check != expected) {
            cout << "ERROR: ParsePair() --> " << expected << " expected instead of " << check << endl;
            cout << "                        Input string: " << in << endl;
        }
    };

    ParseTokenLen( in, 1, token, remainder );
    char_check( token[0], '[' );
    in = remainder;
    elt1 = ParseElt( in, nLevel + 1, bOutput );

    ParseTokenLen( in, 1, token, remainder );
    char_check( token[0], ',' );
    in = remainder;
    elt2 = ParseElt( in, nLevel + 1, bOutput );

    ParseTokenLen( in, 1, token, remainder );
    char_check( token[0], ']' );
    in = remainder;
    // assemble result node and set parent pointers of siblings
    result = glbTree.MakeNode( MakeEltDynamic( PAIR, 0, nLevel ), elt1, elt2 );
    result->leftElt->parent = result;
    result->rghtElt->parent = result;

    return result;
}

void FixParentPointers( sNode *root ) {
    if (root->leftElt != nullptr) {
        root->leftElt->parent = root;
        FixParentPointers( root->leftElt );
    }
    if (root->rghtElt != nullptr) {
        root->rghtElt->parent = root;
        FixParentPointers( root->rghtElt );
    }
}

void ResetLevels( sNode *root, int nLevel ) {
    if (root != nullptr) {
        SetLevel( root->elt, nLevel );
        ResetLevels( root->leftElt, nLevel + 1 );
        ResetLevels( root->rghtElt, nLevel + 1 );
    }
}

sNode *MakePair( sNode *left, sNode *right, int curLevel ) {
    return glbTree.MakeNode( MakeEltDynamic( PAIR, 0, curLevel ), left, right );
}

// consider curPtr as the root of a tree, and search that tree for the
// first (leftmost) ATOMIC node. Returns nullptr if not found.
sNode *FindFirstLeftInTree( sNode *curPtr ) {

//    cout << "  FindFirstLeftInTree() called with following pointer info: ";
//    PrintNode( curPtr );

    sNode *result;
    if (curPtr == nullptr) {
        result = nullptr;
    } else if (GetType( curPtr->elt ) == ATOMIC) {
        result = curPtr;
    } else {
        sNode *tmp = FindFirstLeftInTree( curPtr->leftElt );
        if (tmp == nullptr) {
            tmp = FindFirstLeftInTree( curPtr->rghtElt );
        }
        result = tmp;
    }

//    cout << "FindFirstLeftInTree() is returning the following result pointer info: ";
//    PrintNode( result );

    return result;
}

// consider curPtr as the root of a tree, and search that tree for the
// first (rightmost) ATOMIC node. Returns nullptr if not found.
sNode *FindFirstRightInTree( sNode *curPtr ) {

//    cout << "  FindFirstRightInTree() called with following pointer info: ";
//    PrintNode( curPtr );

    sNode *result;
    if (curPtr == nullptr) {
        result = nullptr;
    } else if (GetType( curPtr->elt ) == ATOMIC) {
        result = curPtr;
    } else {
        sNode *tmp = FindFirstRightInTree( curPtr->rghtElt );
        if (tmp == nullptr) {
            tmp = FindFirstRightInTree( curPtr->leftElt );
        }
        result = tmp;
    }

//    cout << "FindFirstRightInTree() is returning the following result pointer info: ";
//    PrintNode( result );

    return result;
}

// find the next neighbour to the right of the node pointed at by curPtr
sNode *FindRegNrRight( sNode *curPtr ) {

//    cout << "FindRegNrRight() called with following pointer info: ";
//    PrintNode( curPtr );

    sNode *result = nullptr;

    if (curPtr == nullptr) {
        cout << "ERROR: FindRegNrRight() --> how can you ask for the neighbour of a nullptr argument?? " << endl;
    } else {
        if (curPtr->parent == nullptr) {
            result = nullptr;   // if there's no parent, there's no neighbour
        } else {
            // get aux. ptr to parent and to current node
            sNode *scanParPtr = curPtr->parent;
            sNode *scanCurPtr = curPtr;
            // scan up the tree until a left sibling relation is found (or the tree ends)
            bool bFound = false;
            do {
                if (scanParPtr->leftElt == scanCurPtr) {
                    // there's a left sibling relation between parent and curPtr
                    // look for the right neighbour in the right branch of the parent
                    bFound = true;
                    result = FindFirstLeftInTree( scanParPtr->rghtElt );
//                    cout << " option 1... ";
                } else if (scanParPtr->parent == nullptr) {
                    // the scan is terminating since there's no parent to further search
                    bFound = true;
                    result = nullptr;
//                    cout << " option 2... ";
                } else { // it's no trivial case, and there is a parent, so scan the parent
                    scanCurPtr = scanParPtr;
                    scanParPtr = scanParPtr->parent;
//                    cout << " option 3... ";
                }
            } while (!bFound);
        }
    }
//    cout << "FindRegNrRight() is returning the following result pointer info: ";
//    PrintNode( result );

    return result;
}

// find the next neighbour to the left of the node pointed at by curPtr
sNode *FindRegNrLeft( sNode *curPtr ) {

//    cout << "FindRegNrLeft() called with following pointer info: ";
//    PrintNode( curPtr );

    sNode *result = nullptr;

    if (curPtr == nullptr) {
        cout << "ERROR: FindRegNrLeft() --> how can you ask for the neighbour of a nullptr argument?? " << endl;
    } else {
        if (curPtr->parent == nullptr) {
            result = nullptr;   // if there's no parent, there's no neighbour
        } else {
            // get aux. ptr to parent and to current node
            sNode *scanParPtr = curPtr->parent;
            sNode *scanCurPtr = curPtr;
            // scan up the tree until a right sibling relation is found (or the tree ends)
            bool bFound = false;
            do {
                if (scanParPtr->rghtElt == scanCurPtr) {
                    // there's a right sibling relation between parent and curPtr
                    // look for the left neighbour in the left branch of the parent
                    bFound = true;
                    result = FindFirstRightInTree( scanParPtr->leftElt );
//                    cout << " option 1... ";
                } else if (scanParPtr->parent == nullptr) {
                    // the scan is terminating since there's no parent to further search
                    bFound = true;
                    result = nullptr;
//                    cout << " option 2... ";
                } else { // it's no trivial case, and there is a parent, so scan the parent
                    scanCurPtr = scanParPtr;
                    scanParPtr = scanParPtr->parent;
//                    cout << " option 3... ";
                }
            } while (!bFound);
        }
    }

//    cout << "FindRegNrLeft() is returning the following result pointer info: ";
//    PrintNode( result );

    return result;
}

// the curptr argument is const. The result is returned.
sNode *ExplodePair( sNode *curPtr ) {

    sNode *result;
    if (curPtr == nullptr) {
        cout << "ERROR: ExplodePair() --> called with nullptr argument " << endl;
    } else {
        sNode *left = curPtr->leftElt;
        sNode *rght = curPtr->rghtElt;
        if (curPtr->leftElt == nullptr) cout << "ERROR: ExplodePair() --> called with empty left  element " << endl;
        if (curPtr->rghtElt == nullptr) cout << "ERROR: ExplodePair() --> called with empty right element " << endl;

        if (GetType( left->elt ) != ATOMIC) cout << "ERROR: ExplodePair() --> left  element is not ATOMIC " << endl;
        if (GetType( rght->elt ) != ATOMIC) cout << "ERROR: ExplodePair() --> right element is not ATOMIC " << endl;

        sNode *leftNeighbour = FindRegNrLeft( left );
        if (leftNeighbour != nullptr) {
            SetValue( leftNeighbour->elt, GetValue( leftNeighbour->elt ) + GetValue( left->elt ));
        }

        sNode *rghtNeighbour = FindRegNrRight( rght );
        if (rghtNeighbour != nullptr) {
            SetValue( rghtNeighbour->elt, GetValue( rghtNeighbour->elt ) + GetValue( rght->elt ));
        }

        result = glbTree.MakeNode( MakeEltDynamic( ATOMIC, 0, GetLevel( curPtr->elt )));
        delete curPtr->leftElt; curPtr->leftElt = nullptr;
        delete curPtr->rghtElt; curPtr->rghtElt = nullptr;
    }
    return result;
}

// the curptr argument is const. The result is returned.
sNode *SplitRegNr( sNode *curPtr ) {

//    cout << "SplitRegNr() called with following pointer info: ";
//    PrintNode( curPtr );

    sNode *result;
    if (curPtr == nullptr) {
        cout << "ERROR: SplitRegNr() --> called with nullptr argument " << endl;
    } else {
        if (GetType( curPtr->elt ) != ATOMIC) cout << "ERROR: SplitRegNr() --> called with invalid node type: " << GetType( curPtr->elt ) << endl;

        // create two new atomic nodes, and transform current atomic node into a pair combining the new nodes
        int rounding   = (GetValue( curPtr->elt ) % 2);
        int left_value =  GetValue( curPtr->elt ) / 2;
        int rght_value =  GetValue( curPtr->elt ) / 2 + rounding;
        curPtr->leftElt = glbTree.MakeNode( MakeEltDynamic( ATOMIC, left_value, GetLevel( curPtr->elt ) + 1 ));
        curPtr->rghtElt = glbTree.MakeNode( MakeEltDynamic( ATOMIC, rght_value, GetLevel( curPtr->elt ) + 1 ));
        curPtr->leftElt->parent = curPtr;
        curPtr->rghtElt->parent = curPtr;
        SetType( curPtr->elt, PAIR );
        result = curPtr;
    }

//    cout << "SplitRegNr() is returning the following result pointer info: ";
//    PrintNode( result );

    return result;
}

// checks for situations where explode is needed
// returns false if any reduce action took place, true otherwise
// NOTE - call with address of sNode pointer!!!!!!
bool Reduced1( sNode **treePtrPtr ) {

    sNode *treePtr = (treePtrPtr != nullptr) ? *treePtrPtr : nullptr;

    bool bResult = true;
    if (treePtr != nullptr) {
        if ((GetLevel( treePtr->elt ) == 4) && (GetType( treePtr->elt ) == PAIR)) {
            if (GetType( treePtr->leftElt->elt ) == PAIR) {
                sNode *tmp = ExplodePair( treePtr->leftElt );
                treePtr->leftElt = tmp;
                tmp->parent = treePtr;
                bResult = false;
            } else if (GetType( treePtr->rghtElt->elt ) == PAIR) {
                sNode *tmp = ExplodePair( treePtr->rghtElt );
                treePtr->rghtElt = tmp;
                tmp->parent = treePtr;
                bResult = false;
            }
        }
        if (bResult) {
            bResult = Reduced1( &(treePtr->leftElt) );
        }
        if (bResult) {
            bResult = Reduced1( &(treePtr->rghtElt) );
        }
    }
    return bResult;
}

// checks for situations where split is needed
// returns false if any reduce action took place, true otherwise
// NOTE - call with address of sNode pointer!!!!!!
bool Reduced2( sNode **treePtrPtr ) {

    sNode *treePtr = (treePtrPtr != nullptr) ? *treePtrPtr : nullptr;

    bool bResult = true;
    if (treePtr != nullptr) {
        if (GetType( treePtr->elt ) == ATOMIC && GetValue( treePtr->elt ) >= 10) {
//            sNode *tmp = SplitRegNr( treePtr );
            SplitRegNr( treePtr );
            bResult = false;
        }
        if (bResult) {
            bResult = Reduced2( &(treePtr->leftElt) );
        }
        if (bResult) {
            bResult = Reduced2( &(treePtr->rghtElt) );
        }
    }
    return bResult;
}

void ReduceSFNr( sNode **treePtrPtr, bool bOutput = false, bool bAsString = true ) {
    sNode *curPtr = (treePtrPtr != nullptr) ? *treePtrPtr : nullptr;

    // reduce on explosions where necessary
    bool bReady = false;
    do {
        bool bExplosionsReady = false;
        do {
            bExplosionsReady = Reduced1( &curPtr );
            if (!bExplosionsReady && bOutput) {
                if (bAsString) {
                    cout << "After explode:   ";
                    glbTree.PrintTree( PrintElt );
                    cout << endl;
                } else
                    glbTree.PrintTree( PrintElt );
            }
        } while (!bExplosionsReady);

        bool bSplitsReady = Reduced2( &curPtr );
        if (!bSplitsReady && bOutput) {
            if (bAsString) {
                cout << "After split:     ";
                glbTree.PrintTree( PrintElt );
                cout << endl;
            } else
                glbTree.PrintTree( PrintElt );
        }

        bReady = bExplosionsReady && bSplitsReady;
    } while (!bReady);
}

// The magnitude of a regular number is just that number.
// The magnitude of a pair is 3 times the magnitude of its left element
// plus 2 times the magnitude of its right element.
int GetTreeMagnitude( sNode *root ) {
    int result = 0;
    if (root != nullptr) {
        if (GetType( root->elt ) == ATOMIC) {
            result = GetValue( root->elt );
        } else if (GetType( root->elt ) == PAIR) {
            result = 3 * GetTreeMagnitude( root->leftElt ) +
                     2 * GetTreeMagnitude( root->rghtElt );
        } else {
            cout << "ERROR: GetTreeMagnitude() --> unknown type encountered: " << GetType( root->elt ) << endl;
            result = -1;
        }
    }
    return result;
}



// ============================================================

int main()
{
    glbProgPhase = EXAMPLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    DataStream snailNrData;
    GetInput( snailNrData, glbProgPhase != PUZZLE );
    std::cout << "Data stats - number of input snail numbers: " << snailNrData.size() << std::endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1


    bool bWannaPrint = false;

    // make a working copy of input data (to keep it untouched)
    DataStream workData = snailNrData;
    // parse the input data and create a tree from it
    cout << "1, ";
    glbTree.root = ParsePair( workData[0], true );
    cout << "2, ";
    FixParentPointers( glbTree.root );
    cout << "3, ";
    glbTree.root->parent = nullptr;

    if (bWannaPrint) {
        // output it
        cout << endl << "Tree 0 before reduction: " << endl << endl;
        glbTree.PrintTree( PrintElt );
        cout << endl;
    }
    // reduce it
    ReduceSFNr( &glbTree.root, false, true );
    if (bWannaPrint) {
        // output it
        cout << endl << "Tree 0 after reduction: " << endl << endl;
        glbTree.PrintTree( PrintElt );
        cout << endl;
    }

    for (int i = 1; i < (int)workData.size(); i++) {
        // the intermediate result is stored in the left tree
        sNode *tmp_left = glbTree.root;
        // an additional snail number is read in the right tree
        sNode *tmp_rght = ParsePair( workData[i], true );
        // combine it (and recalc the levels per node)
        glbTree.root = MakePair( tmp_left, tmp_rght, 0 );

        ResetLevels( glbTree.root, 1 );
        FixParentPointers( glbTree.root );

        if (bWannaPrint) {
            cout << endl << "New tree after addition: " << endl << endl;
            glbTree.PrintTree( PrintElt );
            cout << endl;
        }
        // reduce it
        ReduceSFNr( &glbTree.root, false, true );
        if (bWannaPrint) {
            // output it
            cout << endl << "New tree after reduction: " << endl << endl;
            glbTree.PrintTree( PrintElt );
            cout << endl;
        }
    }

    int nAnswer = GetTreeMagnitude( glbTree.root );
    cout << "Magnitude of final tree is: " << nAnswer << endl;

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    bWannaPrint = false;

    // make a working copy of input data (to keep it untouched)
    workData = snailNrData;

    int nMaxMagnitude = INT_MIN;
    // iterate over all combinations of two snail numbers
    for (int i = 0; i < (int)workData.size(); i++) {
        for (int j = i + 1; j < (int)workData.size(); j++) {

//            cout << "Iterating outer: " << i << " inner: " << j << endl;

            // iterate over combination
            for (int k = 0; k < 2; k++) {

                string outer_disposable = workData[i];
                string inner_disposable = workData[j];

                sNode *tmp_outer = ParsePair( outer_disposable, 0, false );
                sNode *tmp_inner = ParsePair( inner_disposable, 0, false );
                sNode *combi = nullptr;
                if (k == 0) {
                    combi = MakePair( tmp_outer, tmp_inner, 0 );
                } else {
                    combi = MakePair( tmp_inner, tmp_outer, 0 );
                }
                ResetLevels( combi, 1 );
                FixParentPointers( combi );

                if (bWannaPrint) {
                    cout << endl << "New tree after addition: " << endl << endl;
                    glbTree.PrintTree( PrintElt );
                    cout << endl;
                }
                // reduce it
                ReduceSFNr( &combi, false, true );
                if (bWannaPrint) {
                    // output it
                    cout << endl << "New tree after reduction: " << endl << endl;
                    glbTree.PrintTree( PrintElt );
                    cout << endl;
                }

                int nThisMagnitude = GetTreeMagnitude( combi );
                if (nThisMagnitude > nMaxMagnitude)
                    nMaxMagnitude = nThisMagnitude;

                glbTree.DisposeTree( DisposeElt );
            }
        }
    }

    cout << "Max. magnitude over all combinations is: " << nMaxMagnitude << endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}


