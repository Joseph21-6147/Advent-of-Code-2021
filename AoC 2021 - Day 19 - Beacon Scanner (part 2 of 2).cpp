// AoC 2021 - day 19
// =================

// date:  2021-12-30
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../flcTimer.h"
#include "../vector_types.h"

using namespace std;

#define DEBUG  true

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

// to store beacons - just a list of coordinates in 3d
typedef vector<vi3d> BeaconStream;

// to store scanners and their properties
struct sScanner {
    int nID        = -1;       // which scanner is it?
    int nOrtIndex  = -1;       // if the orientation is changed, what scanner's orientation is adopted?
    vi3d vPosition = vi3d( 0, 0, 0 );           // where is the scanner itself in 3D space?
    BeaconStream vBeacons;    // where are all the beacons that this scanner can see
                              // these are relative to position and orientation (!!!)
};
typedef vector<sScanner> ScannerStream;

// ==========   AUX FUNCTIONS

// produces a padding of white spaces depending on the value of n
string Padding( int n, bool bPadding = true ) {
    string result;
    if (n <= -1000) result = "";     else
    if (n <=  -100) result = " ";    else
    if (n <=   -10) result = "  ";   else
    if (n <=    -1) result = "   ";  else
    if (n <=     9) result = "    "; else
    if (n <=    99) result = "   ";  else
    if (n <=   999) result = "  ";   else
                    result = " ";
    return bPadding ? result : "";
}

string PrintVectorToString( vi3d &v, bool bPadding = true ) {
    string result;
    result.append( "[" + Padding( v.x, bPadding ) + to_string( v.x ) +
                   "," + Padding( v.y, bPadding ) + to_string( v.y ) +
                   "," + Padding( v.z, bPadding ) + to_string( v.z ) + "]" );
    return result;
}

void PrintVector( vi3d &v, bool bPadding = true ) { cout << PrintVectorToString( v, bPadding ); }

void PrintBeaconList( BeaconStream beaconList ) {
    cout << "Beacon list: " << endl;
    for (int i = 0; i < (int)beaconList.size(); i++) {
        cout << " # " << i << " " << PrintVectorToString( beaconList[i] ) << endl;
    }
}

void PrintScanner( sScanner &s, bool bOutputBeacons ) {
    cout << "Scanner - ID: " << s.nID                              << " - Orientation index: " << s.nOrtIndex
         << " - Position: "  << PrintVectorToString( s.vPosition ) << " - # Beacon coords: "   << s.vBeacons.size();
    // print beacon list as well?
    if (bOutputBeacons)  {
        cout << endl;
        PrintBeaconList( s.vBeacons );
    }
    cout << endl;
}

// ==========   DATA INPUT FUNCTIONS

void ReadOneBeaconLine( const string &line, BeaconStream &vData ) {
    // parse line into tokens, and get coordinates from them
    // convert to numbers and insert into beacon list
    vi3d tmpBeacon;
    size_t splitIndex;
    string token, remainder1, remainder2;
    // get (token for) x coord
    splitIndex  = line.find( "," );
    token       = line.substr( 0, splitIndex  );
    remainder1  = line.substr( splitIndex + 1 );
    tmpBeacon.x = atoi( token.c_str() );
    // get (tokens for) y and z coord
    splitIndex  = remainder1.find( "," );
    token       = remainder1.substr( 0, splitIndex  );
    remainder2  = remainder1.substr( splitIndex + 1 );
    tmpBeacon.y = atoi( token.c_str() );
    tmpBeacon.z = atoi( remainder2.c_str() );
    // error checking of the result
    if (tmpBeacon.x < -1000 || tmpBeacon.x > 1000) cout << "WARNING WARNING: ReadOneBeaconLine() --> x component too far away: " << tmpBeacon.x << endl;
    if (tmpBeacon.y < -1000 || tmpBeacon.y > 1000) cout << "WARNING WARNING: ReadOneBeaconLine() --> y component too far away: " << tmpBeacon.y << endl;
    if (tmpBeacon.z < -1000 || tmpBeacon.z > 1000) cout << "WARNING WARNING: ReadOneBeaconLine() --> z component too far away: " << tmpBeacon.z << endl;
    // push beacon to list
    vData.push_back( tmpBeacon );
}

// this function is used to input the data from the text file
void ReadData( const string &sFileName, ScannerStream &dData ) {
    int nCurScanner = -1;
    sScanner auxScanner;
    bool bMustPush = false;

    // open filestream
    ifstream dataFileStream( sFileName );
    // read data stream
    dData.clear();
    bool bDone = dataFileStream.eof();
    string token[4];
    while (!bDone) {
        // get next token and determine what to do
        if (dataFileStream >> token[0]) {
            // take second character to identify scanner title lines
            if (token[0][1] == '-') {
                // it's a scanner title line - if necessary process current scanner data before, getting next one
                if (bMustPush) {
                    dData.push_back( auxScanner );
                }
                // read next three tokens and process the one with the scanner id
                dataFileStream >> token[1] >> token[2] >> token[3];
                nCurScanner = atoi( token[2].c_str() );
                // initialize data for this (new) scanner
                auxScanner.nID       = nCurScanner;
                auxScanner.nOrtIndex = nCurScanner;
                auxScanner.vBeacons.clear();
                bMustPush = true;
            } else {
                // it's a beacon line - process it
                ReadOneBeaconLine( token[0], auxScanner.vBeacons );
            }
            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }
    dData.push_back( auxScanner );
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintScannerStream( ScannerStream &sData, bool bBeaconOutput = false ) {
    for (int i = 0; i < (int)sData.size(); i++)
        PrintScanner( sData[i], bBeaconOutput );
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgPhase)
void GetInput( ScannerStream &dData, bool bDisplay = false, bool bVerbose = false ) {
    switch (glbProgPhase) {
        case EXAMPLE: ReadData( "input.example.txt", dData ); break;
        case TEST:    ReadData( "input.test.txt"   , dData ); break;
        case PUZZLE:  ReadData( "input.txt"        , dData ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintScannerStream( dData, bVerbose );
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

struct sOrientation {
    int  x_sign = 0;      // either -1 or +1
    char x_axis = '*';    // either 'x', 'y' or 'z'
    int  y_sign = 0;
    char y_axis = '*';
    int  z_sign = 0;
    char z_axis = '*';
};

// even permutations - require an even number of steps (0 or 2)
// odd permutations - require an odd number of steps (1)
// x, y, z - even
// x, z, y - odd
// y, x, z - odd
// y, z, x - even
// z, x, y - even
// z, y, x - odd
// this is useful, because even permutations must have even nr of - signs and
// odd permutations must have odd nr of - signs

bool OrtIsEven( sOrientation &ort ) {
    return ((ort.x_axis == 'x'&& ort.y_axis == 'y' && ort.z_axis == 'z') ||
            (ort.x_axis == 'y'&& ort.y_axis == 'z' && ort.z_axis == 'x') ||
            (ort.x_axis == 'z'&& ort.y_axis == 'x' && ort.z_axis == 'y'));
}

bool OrtIsOdd( sOrientation &ort ) {
    return ((ort.x_axis == 'x'&& ort.y_axis == 'z' && ort.z_axis == 'y') ||
            (ort.x_axis == 'y'&& ort.y_axis == 'x' && ort.z_axis == 'z') ||
            (ort.x_axis == 'z'&& ort.y_axis == 'y' && ort.z_axis == 'x'));
}

int NrNegatives( sOrientation &ort ) {
    int nResult = 0;
    if (ort.x_sign == -1) nResult += 1;
    if (ort.y_sign == -1) nResult += 1;
    if (ort.z_sign == -1) nResult += 1;
    return nResult;
}

bool CheckOrientation( sOrientation &ort ) {
    bool bResult = false;
    if (OrtIsEven( ort )) {
        bResult = (NrNegatives( ort ) % 2 == 0);
    } else if (OrtIsOdd( ort )) {
        bResult = (NrNegatives( ort ) % 2 == 1);
    } else {
//        cout << "ERROR: CheckOrientation() --> unknown parity encountered - x = " << ort.x_axis << ", y = " << ort.y_axis << ", z = " << ort.z_axis << endl;
    }
    return bResult;
}

void FlipOrientation( sOrientation &ort ) {
    ort.x_sign *= -1;
    ort.y_sign *= -1;
    ort.z_sign *= -1;
}

// works out the orientation transform from b space to a space
// example, if ort = { -1, z, +1, x, -1, y }
// then the transform from b to a is { -1 * b_z, +1 * b_x, -1 * b_y }
bool GetRawOrientation( vi3d &a1, vi3d &a2, vi3d &b1, vi3d &b2, sOrientation &ort ) {
    bool bResult = false;
    // get difference vectors between corresponding beacons a1-a2 and b1-b2
    vi3d difVecA = a1 - a2;
    vi3d difVecB = b1 - b2;

    if (difVecA.x == 0 || difVecA.y == 0 || difVecA.z == 0) {
//        cout << "WARNING: GetRawOrientation() --> Failed, can't make an orientation with a 0 vector component..." << endl;
        bResult = false;
    } else if (difVecA.x == difVecA.y || difVecA.y == difVecA.z || difVecA.z == difVecA.x) {
//        cout << "WARNING: GetRawOrientation() --> Failed, can't make an orientation with identical vector components..." << endl << endl << endl;
        bResult = false;
    } else {
        bResult = true;
        // get axes and signedness part of the orientation mapping
        if (abs( difVecA.x ) == abs( difVecB.x )) { ort.x_axis = 'x'; ort.x_sign = (difVecA.x == difVecB.x ? +1 : -1); } else
        if (abs( difVecA.x ) == abs( difVecB.y )) { ort.x_axis = 'y'; ort.x_sign = (difVecA.x == difVecB.y ? +1 : -1); } else
        if (abs( difVecA.x ) == abs( difVecB.z )) { ort.x_axis = 'z'; ort.x_sign = (difVecA.x == difVecB.z ? +1 : -1); } else { /* cout << "GetRawOrientation() -->> X failure... :( " << endl; */ bResult = false; }

        if (abs( difVecA.y ) == abs( difVecB.x )) { ort.y_axis = 'x'; ort.y_sign = (difVecA.y == difVecB.x ? +1 : -1); } else
        if (abs( difVecA.y ) == abs( difVecB.y )) { ort.y_axis = 'y'; ort.y_sign = (difVecA.y == difVecB.y ? +1 : -1); } else
        if (abs( difVecA.y ) == abs( difVecB.z )) { ort.y_axis = 'z'; ort.y_sign = (difVecA.y == difVecB.z ? +1 : -1); } else { /* cout << "GetRawOrientation() -->> Y failure... :( " << endl; */ bResult = false; }

        if (abs( difVecA.z ) == abs( difVecB.x )) { ort.z_axis = 'x'; ort.z_sign = (difVecA.z == difVecB.x ? +1 : -1); } else
        if (abs( difVecA.z ) == abs( difVecB.y )) { ort.z_axis = 'y'; ort.z_sign = (difVecA.z == difVecB.y ? +1 : -1); } else
        if (abs( difVecA.z ) == abs( difVecB.z )) { ort.z_axis = 'z'; ort.z_sign = (difVecA.z == difVecB.z ? +1 : -1); } else { /* cout << "GetRawOrientation() -->> Z failure... :( " << endl; */ bResult = false; }
    }
    return bResult;
}

// works out the orientation transform from b space to a space
// example, if ort = { -1, z, +1, x, -1, y }
// then the transform from b to a is { -1 * b_z, +1 * b_x, -1 * b_y }
bool GetOrientation( vi3d &a1, vi3d &a2, vi3d &b1, vi3d &b2, sOrientation &ort ) {

    if (GetRawOrientation( a1, a2, b1, b2, ort )) {

        // error checking !!
        if (!CheckOrientation( ort )) {
            cout << endl << "WARNING: GetOrientation() --> encountered apparently invalid orientation, gonna flip...";
            FlipOrientation( ort );
            if (CheckOrientation( ort ))
                cout << " OK!" << endl;
            else
                cout << " UNSUCCESFULL --> error persistent!!!" << endl;
        }
        return true;
    }
    return false;
}

// returns point p transformed into another orientation (denoted by ort)
vi3d ReorientateForward( vi3d &p, const sOrientation &ort, bool bDebug = false ) {

    if (bDebug) {
        cout << "ReorientateForward: ( " << (ort.x_sign < 0 ? "-1 " : "+1") << " * " << ort.x_axis << ", "
                                         << (ort.y_sign < 0 ? "-1 " : "+1") << " * " << ort.y_axis << ", "
                                         << (ort.z_sign < 0 ? "-1 " : "+1") << " * " << ort.z_axis << " ) " << endl;

        if (ort.x_sign != -1 && ort.x_sign != 1) cout << "ReorientateForward() --> invalid x sign value: " << ort.x_sign << endl;
        if (ort.y_sign != -1 && ort.y_sign != 1) cout << "ReorientateForward() --> invalid y sign value: " << ort.y_sign << endl;
        if (ort.z_sign != -1 && ort.z_sign != 1) cout << "ReorientateForward() --> invalid z sign value: " << ort.z_sign << endl;
    }

    int tmp_x, tmp_y, tmp_z;
    switch (ort.x_axis) {
        case 'x': tmp_x = ort.x_sign * p.x; break;
        case 'y': tmp_x = ort.x_sign * p.y; break;
        case 'z': tmp_x = ort.x_sign * p.z; break;
        default: cout << "Reorientate() --> error in x" << endl;
    }
    switch (ort.y_axis) {
        case 'x': tmp_y = ort.y_sign * p.x; break;
        case 'y': tmp_y = ort.y_sign * p.y; break;
        case 'z': tmp_y = ort.y_sign * p.z; break;
        default: cout << "Reorientate() --> error in y" << endl;
    }
    switch (ort.z_axis) {
        case 'x': tmp_z = ort.z_sign * p.x; break;
        case 'y': tmp_z = ort.z_sign * p.y; break;
        case 'z': tmp_z = ort.z_sign * p.z; break;
        default: cout << "Reorientate() --> error in z" << endl;
    }
    return vi3d( tmp_x, tmp_y, tmp_z );
}

// returns point p reversely transformed using orientation (denoted by ort)
vi3d ReorientateReverse( vi3d &p, const sOrientation &ort, bool bDebug = false ) {

    if (bDebug) {
        cout << "ReorientateReverse: ( " << (ort.x_sign < 0 ? "-1 " : "+1") << " * " << ort.x_axis << ", "
                                         << (ort.y_sign < 0 ? "-1 " : "+1") << " * " << ort.y_axis << ", "
                                         << (ort.z_sign < 0 ? "-1 " : "+1") << " * " << ort.z_axis << " ) " << endl;

        if (ort.x_sign != -1 && ort.x_sign != 1) cout << "ReorientateReverse() --> invalid x sign value: " << ort.x_sign << endl;
        if (ort.y_sign != -1 && ort.y_sign != 1) cout << "ReorientateReverse() --> invalid y sign value: " << ort.y_sign << endl;
        if (ort.z_sign != -1 && ort.z_sign != 1) cout << "ReorientateReverse() --> invalid z sign value: " << ort.z_sign << endl;
    }

    int tmp_x, tmp_y, tmp_z;

    switch (ort.x_axis) {
        case 'x': tmp_x = ort.x_sign * p.x; break;
        case 'y': tmp_y = ort.x_sign * p.x; break;
        case 'z': tmp_z = ort.x_sign * p.x; break;
        default: cout << "ReorientateReverse() --> error in x" << endl;
    }
    switch (ort.y_axis) {
        case 'x': tmp_x = ort.y_sign * p.y; break;
        case 'y': tmp_y = ort.y_sign * p.y; break;
        case 'z': tmp_z = ort.y_sign * p.y; break;
        default: cout << "ReorientateReverse() --> error in y" << endl;
    }
    switch (ort.z_axis) {
        case 'x': tmp_x = ort.z_sign * p.z; break;
        case 'y': tmp_y = ort.z_sign * p.z; break;
        case 'z': tmp_z = ort.z_sign * p.z; break;
        default: cout << "ReorientateReverse() --> error in z" << endl;
    }
    return vi3d( tmp_x, tmp_y, tmp_z );
}

// calculate the origin of scanner B with respect to the origin of scanner A using two beacons
// that are known to correspond, and the orientation mapping from B to A
vi3d GetOffset( vi3d &a, vi3d &b, sOrientation &ort, bool bDebug = false ) {
    if (bDebug) cout << "GetOffset() --> input vectors " << PrintVectorToString( a ) << ", " << PrintVectorToString( b ) << endl;
    // first transform b conform ort
    vi3d trfdB = ReorientateForward( b, ort );
    if (bDebug) cout << "           transformed vector " << PrintVectorToString( trfdB ) << endl;
    vi3d result = (a - trfdB);
    // return the difference vector between a and the transformed b
    if (bDebug) cout << "                output vector " << PrintVectorToString( result ) << endl;

    return result;
}

// this struct stores a match between two beacons from different scanners
struct sBeaconMatch {
    int  nScnrA;
    int  nBcnIdxA;
    vi3d vBcnPosA;
    int  nScnrB;
    int  nBcnIdxB;
    vi3d vBcnPosB;

    // needed for uniqueing
    bool operator == ( const sBeaconMatch &rhs ) { return (this->nScnrA == rhs.nScnrA && this->nBcnIdxA == rhs.nBcnIdxA &&
                                                           this->nScnrB == rhs.nScnrB && this->nBcnIdxB == rhs.nBcnIdxB); }
    // needed for sorting
    bool operator < ( const sBeaconMatch &rhs ) {
        // this four-level distinction is needed to get the unique function to work correctly
        return ( this->nScnrA <  rhs.nScnrA ||
                (this->nScnrA == rhs.nScnrA && ( this->nBcnIdxA <  rhs.nBcnIdxA ||
                                                (this->nBcnIdxA == rhs.nBcnIdxA && ( this->nScnrB <  rhs.nScnrB ||
                                                                                    (this->nScnrB == rhs.nScnrB && this->nBcnIdxB < rhs.nBcnIdxB))))));
    }
};


// returns the nr of overlapping scanners between A and B are overlapping by at least a number of distances (point pairs).
// this function works on matching pairs of beacons. Disadvantage is that you don't know if beacon A1
// matches with B1 or with B2
int FindOverlappingScanners2( ScannerStream &scannerData, int nScnrA, int nScnrB, vector<sBeaconMatch> &vBeaconMatchList, bool bDebug = false ) {
    int result;
    struct sDistance {
        int scanner;
        int beacon1, beacon2;
        int distanceSqrd;
    };

    auto dist_smaller = [=]( sDistance &a, sDistance &b ) {
        return a.distanceSqrd < b.distanceSqrd;
    };

    // lambda function to calculate the squared distance between two points
    auto squared_dist = [=]( vi3d &a, vi3d &b ) {
        return ((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y) +
                (a.z - b.z) * (a.z - b.z));
    };

    auto make_dist_list = [=] ( sScanner &scnr, int scnrIx, vector<sDistance> &resltList ) {

        if (bDebug) cout << "Getting distance list for scanner: " << scnrIx << endl;
        // iterate all unique combination of three beacons within the current scanner
        for (int i = 0; i < (int)scnr.vBeacons.size(); i++) {
            vi3d pointA = scnr.vBeacons[i];
            for (int j = i + 1; j < (int)scnr.vBeacons.size(); j++) {
                vi3d pointB = scnr.vBeacons[j];
                // calculate the (squared) distance between point A and B
                int aux0 = squared_dist( pointA, pointB );
                // create the distance struct and push it at the back of the result list
                sDistance dist = { scnrIx, i, j, aux0 };
                resltList.push_back( dist );

                if (bDebug) {
                    cout << "scanner: " << scnrIx << " bcn ix A: " << i << " point A: " << PrintVectorToString( pointA )
                                                  << " bcn ix B: " << j << " point B: " << PrintVectorToString( pointB ) << " dist^2: "  << aux0 << endl;
                }
            }
        }
    };

    // get the (sorted) distance list of scanner A
    vector<sDistance> distsA;
    sScanner scnrA = scannerData[ nScnrA ];
    make_dist_list( scnrA, nScnrA, distsA );
    sort( distsA.begin(), distsA.end(), dist_smaller );

    // get the (sorted) distance list of scanner B
    vector<sDistance> distsB;
    sScanner scnrB = scannerData[ nScnrB ];
    make_dist_list( scnrB, nScnrB, distsB );
    sort( distsB.begin(), distsB.end(), dist_smaller );

    if (bDebug) {
        cout << "# distances first scanner: " << (int)distsA.size() << " # distances second scanner: " << (int)distsB.size() << endl;
    }
    int ixA = 0;
    int ixB = 0;
    bool bDone = false;
    result = 0;
    while (!bDone) {
        if (distsA[ixA].distanceSqrd == distsB[ixB].distanceSqrd) {

            // insert this match into the match list, but check if this orientation, or the flipped one is valid
            sOrientation tmp;
            if (GetRawOrientation( scnrA.vBeacons[ distsA[ ixA ].beacon1 ], scnrA.vBeacons[ distsA[ ixA ].beacon2 ],
                                   scnrB.vBeacons[ distsB[ ixB ].beacon1 ], scnrB.vBeacons[ distsB[ ixB ].beacon2 ], tmp )) {
                // if a1-b1 vs a2-b2 yields a valid orientation, then store a1-b1 and a2-b2 as a match
                // otherwise store a1-b2 and a2-b1 as a match
                if (CheckOrientation( tmp )) {
                    vBeaconMatchList.push_back( { nScnrA, distsA[ ixA ].beacon1, scnrA.vBeacons[ distsA[ ixA ].beacon1 ],
                                                  nScnrB, distsB[ ixB ].beacon1, scnrB.vBeacons[ distsB[ ixB ].beacon1 ] } );
                    vBeaconMatchList.push_back( { nScnrA, distsA[ ixA ].beacon2, scnrA.vBeacons[ distsA[ ixA ].beacon2 ],
                                                  nScnrB, distsB[ ixB ].beacon2, scnrB.vBeacons[ distsB[ ixB ].beacon2 ] } );
                } else {
                    vBeaconMatchList.push_back( { nScnrA, distsA[ ixA ].beacon1, scnrA.vBeacons[ distsA[ ixA ].beacon1 ],
                                                  nScnrB, distsB[ ixB ].beacon2, scnrB.vBeacons[ distsB[ ixB ].beacon2 ] } );
                    vBeaconMatchList.push_back( { nScnrA, distsA[ ixA ].beacon2, scnrA.vBeacons[ distsA[ ixA ].beacon2 ],
                                                  nScnrB, distsB[ ixB ].beacon1, scnrB.vBeacons[ distsB[ ixB ].beacon1 ] } );
                }

                if (bDebug) {
                    cout << "Found match between distance indices (A) " << ixA << " (B) " << ixB;
                    cout << " at points [ from persp. scnr A ]: " << PrintVectorToString( scnrA.vBeacons[ distsA[ ixA ].beacon1 ] );
                    cout << " and: "                              << PrintVectorToString( scnrA.vBeacons[ distsA[ ixA ].beacon2 ] ) << endl;
                }
                result += 1;
            }

            ixA++;
            ixB++;
        } else if (distsA[ixA].distanceSqrd < distsB[ixB].distanceSqrd) {
            ixA++;
        } else if (distsA[ixA].distanceSqrd > distsB[ixB].distanceSqrd) {
            ixB++;
        }
        bDone = (ixA >= (int)distsA.size() || ixB >= (int)distsB.size());
    }

    int nSizeBefore = vBeaconMatchList.size();
    // sort the list - sorting is done according to "<" operator that's defined in sBeaconMatch
    sort( vBeaconMatchList.begin(), vBeaconMatchList.end() );
    // remove duplicates - for uniqueing the "==" operator is needed that's defined in sBeaconMatch
    vBeaconMatchList.erase( unique( begin( vBeaconMatchList ), end( vBeaconMatchList )), end( vBeaconMatchList ));
    int nSizeAfter = vBeaconMatchList.size();

    if (bDebug) {
        cout << "Total matches found: " << result << " - # matched beacons before and after sorting/uniqueing: " << nSizeBefore << " --> " << nSizeAfter << endl;

        for (int i = 0; i < (int)vBeaconMatchList.size(); i++) {
            cout << " scnr: " << vBeaconMatchList[i].nScnrA << " beacon: " << vBeaconMatchList[i].nBcnIdxA << " point: " << PrintVectorToString( vBeaconMatchList[i].vBcnPosA );
            cout << " scnr: " << vBeaconMatchList[i].nScnrB << " beacon: " << vBeaconMatchList[i].nBcnIdxB << " point: " << PrintVectorToString( vBeaconMatchList[i].vBcnPosB );
            cout << endl;
        }
    }

    if (bDebug)
        cout << "Total matches found: " << result << endl;

    // return the number of (unique) beacon matches found between scanners A and B
    return (int)vBeaconMatchList.size();
}


// Returns the number of beacons that are overlapping between scanner A and B. The list of matched beacon's is passed back as a parameter.
// This function works on matching triangles of beacons. Advantage is that you know exactly which beacon of
// scanner A corresponds with beacon of scanner B. [ Disadvantage is that it's more computational expensive ]
int FindBeaconOverlap( ScannerStream &scannerData, int nScnrA, int nScnrB, vector<sBeaconMatch> &vBeaconMatchList, bool bDebug = false, bool bVerbose = false, bool bAdditional = false ) {
    int result;
    // to capture triangles of beacons and the squared distances between their points.
    struct sTriDistance {
        int scanner;
        int beacon[3];
        long long distanceSqrd[3];   // distanceSqrd[i] is the distance from beacon[i] to beacon[(i+1)%3]
    };

    ofstream debugFile; // create a file object
    if (bAdditional)
        debugFile.open("debug.txt");

    // lambda function to calculate the squared distance between two points
    auto squared_dist = [=]( vi3d &a, vi3d &b ) {
        return ((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y) +
                (a.z - b.z) * (a.z - b.z));
    };

    // lambda function to create a triangle distance list for a specific scanner
    auto make_tri_dist_list = [=] ( sScanner &scnr, int scnrIx, vector<sTriDistance> &resltList ) {

        if (bVerbose) cout << "Getting tri distance list for scanner: " << scnrIx << endl;
        // iterate all unique combination of three beacons within the current scanner
        for (int i = 0; i < (int)scnr.vBeacons.size(); i++) {
            vi3d pointA = scnr.vBeacons[i];
            for (int j = i + 1; j < (int)scnr.vBeacons.size(); j++) {
                vi3d pointB = scnr.vBeacons[j];
                for (int k = j + 1; k < (int)scnr.vBeacons.size(); k++) {
                    vi3d pointC = scnr.vBeacons[k];
                    // calculate the three (squared) distances between each pair of the triangle
                    int aux0 = squared_dist( pointA, pointB );
                    int aux1 = squared_dist( pointB, pointC );
                    int aux2 = squared_dist( pointC, pointA );
                    // create the tri distance struct and push it at the back of the result list
                    sTriDistance triDist = { scnrIx, i, j, k, aux0, aux1, aux2 };
                    resltList.push_back( triDist );

                    if (bAdditional) {
                        cout << "scanner: " << scnrIx << " bcn ix A: " << i
                                                      << " bcn ix B: " << j
                                                      << " bcn ix C: " << k << " point A: " << PrintVectorToString( pointA )
                                                                            << " point B: " << PrintVectorToString( pointB )
                                                                            << " point C: " << PrintVectorToString( pointC )
                                                                            << " dist^2: "  << aux0
                                                                            << " dist^2: "  << aux1
                                                                            << " dist^2: "  << aux2 << endl;
                    }
                }
            }
        }
    };

    // get the tri distance list of scanner A
    vector<sTriDistance> distsA;
    sScanner scnrA = scannerData[ nScnrA ];
    make_tri_dist_list( scnrA, nScnrA, distsA );

    // get the tri distance list of scanner B
    vector<sTriDistance> distsB;
    sScanner scnrB = scannerData[ nScnrB ];
    make_tri_dist_list( scnrB, nScnrB, distsB );

    if (bVerbose) {
        cout << "# tri distances first scanner: " << (int)distsA.size() << " # tri distances second scanner: " << (int)distsB.size() << endl;
    }

    // Little lambda - returns number in range [0, 5] if the squared distances in dstSqrdA and dstSqrdB
    // are permutations of each other. The number returned signals which permutation it is.
    // Returns -1 if no permutation
    auto is_permutation = [=]( long long dstSqrdA[], long long dstSqrdB[] ) {
        int result = -1;
        // check all possible permutations
        if (dstSqrdA[0] == dstSqrdB[0] && dstSqrdA[1] == dstSqrdB[1] && dstSqrdA[2] == dstSqrdB[2]) { result = 0; } else
        if (dstSqrdA[0] == dstSqrdB[0] && dstSqrdA[1] == dstSqrdB[2] && dstSqrdA[2] == dstSqrdB[1]) { result = 1; } else
        if (dstSqrdA[0] == dstSqrdB[1] && dstSqrdA[1] == dstSqrdB[0] && dstSqrdA[2] == dstSqrdB[2]) { result = 2; } else
        if (dstSqrdA[0] == dstSqrdB[1] && dstSqrdA[1] == dstSqrdB[2] && dstSqrdA[2] == dstSqrdB[0]) { result = 3; } else
        if (dstSqrdA[0] == dstSqrdB[2] && dstSqrdA[1] == dstSqrdB[0] && dstSqrdA[2] == dstSqrdB[1]) { result = 4; } else
        if (dstSqrdA[0] == dstSqrdB[2] && dstSqrdA[1] == dstSqrdB[1] && dstSqrdA[2] == dstSqrdB[0]) { result = 5; };   // if none match, then -1 is returned

        return result;
    };

    // to check if the coordinates lie within the visible ranges
    auto confidence_high_point = [=]( vi3d &p ) {
        auto in_between = [=]( int n, int low, int high ) { return (low <= n && n <= high); };
        return in_between( p.x, -1000, 1000 ) &&
               in_between( p.y, -1000, 1000 ) &&
               in_between( p.z, -1000, 1000 );
    };

    auto confidence_high_tri = [=]( sTriDistance &d ) {
        bool bConfident = true;
        for (int i = 0; i < 3; i++) {
            vi3d tmpBeacon = scannerData[d.scanner].vBeacons[d.beacon[i]];
            bConfident &= confidence_high_point( tmpBeacon );
        }
        return bConfident;
    };

    // look for matches between tri distances of A and tri distances of B
    result = 0;
    for (int i = 0; i < (int)distsA.size(); i++) {
        for (int j = 0; j < (int)distsB.size(); j++) {
            // only match if all the points in both A and B triangles are trustworthy
            // and the one is a permutation of the other
            bool bConfidentTriA = confidence_high_tri( distsA[i] );
            bool bConfidentTriB = confidence_high_tri( distsB[j] );
            int nPerm = is_permutation( distsA[i].distanceSqrd, distsB[j].distanceSqrd );
            if (nPerm != -1 && bConfidentTriA && bConfidentTriB) {

                // get the corresponding points of scanner B into the aux index array in the correctly permuted order
                int auxIx[3] = { -1 };
                switch (nPerm) {
                    // this permutation took me a while to figure out (pen/paper)
                    case 0: auxIx[0] = 0; auxIx[1] = 1; auxIx[2] = 2; break;
                    case 1: auxIx[0] = 1; auxIx[1] = 0; auxIx[2] = 2; break;
                    case 2: auxIx[0] = 2; auxIx[1] = 1; auxIx[2] = 0; break;
                    case 3: auxIx[0] = 1; auxIx[1] = 2; auxIx[2] = 0; break;
                    case 4: auxIx[0] = 2; auxIx[1] = 0; auxIx[2] = 1; break;
                    case 5: auxIx[0] = 0; auxIx[1] = 2; auxIx[2] = 1; break;
                    default: cout << "FindBeaconOverlap() --> shit here " << endl;
                }

                if (bAdditional) {
                    debugFile << "Found match between triangle distance indices (A) " << i << " and (B) " << j;
                    debugFile << " with permutation value: " << nPerm << "Corresponding points are: " << endl;
                    for (int k = 0; k < 3; k++) {
                        debugFile << "   point list A: " << PrintVectorToString( scnrA.vBeacons[ distsA[ i ].beacon[       k ]] );
                        debugFile << "   point list B: " << PrintVectorToString( scnrB.vBeacons[ distsB[ j ].beacon[ auxIx[k]]] );
                        debugFile << endl;
                    }
                }
                // push three pairs of corresponding points into the beacon match list
                for (int k = 0; k < 3; k++) {
                    vBeaconMatchList.push_back( { nScnrA, distsA[ i ].beacon[       k ], scnrA.vBeacons[ distsA[ i ].beacon[       k ]],
                                                  nScnrB, distsB[ j ].beacon[ auxIx[k]], scnrB.vBeacons[ distsB[ j ].beacon[ auxIx[k]]] } );
                }
                result += 1;
            }
        }
    }

    int nSizeBefore = vBeaconMatchList.size();
    // sort the list - sorting is done according to "<" operator that's defined in sBeaconMatch
    sort( vBeaconMatchList.begin(), vBeaconMatchList.end() );
    // remove duplicates - for uniqueing the "==" operator is needed that's defined in sBeaconMatch
    vBeaconMatchList.erase( unique( begin( vBeaconMatchList ), end( vBeaconMatchList )), end( vBeaconMatchList ));
    int nSizeAfter = vBeaconMatchList.size();

    if (bDebug) {
        cout << "Total tri matches found: " << result << " - # matched beacons before and after sorting/uniqueing: " << nSizeBefore << " --> " << nSizeAfter << endl;

        if (bVerbose) {
            for (int i = 0; i < (int)vBeaconMatchList.size(); i++) {
                cout << " scnr: " << vBeaconMatchList[i].nScnrA << " beacon: " << vBeaconMatchList[i].nBcnIdxA << " point: " << PrintVectorToString( vBeaconMatchList[i].vBcnPosA );
                cout << " scnr: " << vBeaconMatchList[i].nScnrB << " beacon: " << vBeaconMatchList[i].nBcnIdxB << " point: " << PrintVectorToString( vBeaconMatchList[i].vBcnPosB );
                cout << endl;
            }
        }
    }

    if (bAdditional)
        debugFile.close();

    // return the number of (unique) beacon matches found between scanners A and B
    return (int)vBeaconMatchList.size();
}

// To store mappings from one scanner to another. A list is created that is used to transform all scanners into (eventually) the orientation of scanner 0
struct sMapping {
    int fromOrt,
          toOrt;
    sOrientation trfrm;
    vi3d fromBcn,
           toBcn;
    bool bRemovable;          // after an orientation is processed, it is removed from the mapping list
};

struct sTransform {
    int echelon;    // process the tree from back to front (= from leaves to root
    int fromOrt;    // this way all orientations will end up as orientation 0
    int toOrt;
    char cType;     // is the transform 'F' (forward) or 'R' (reverse)?
    sOrientation ort;
    vi3d fromBeacon;
    vi3d toBeacon;
};
vector<sTransform> transformTree = {
//    echelon     forward/reverse
//    |   from    |    <--orientation------------>    <--from point---->    <--to point------>
//    |   |   to  |
    { 0,  1,  0, 'F', {  1, 'y', -1, 'z', -1, 'x' }, { -611, -406,  507 }, { -377,  550,  716 }},
    { 0,  2,  0, 'F', { -1, 'y', -1, 'z',  1, 'x' }, { -661, -763, -685 }, { -377,  550,  716 }},
    { 0,  5,  0, 'F', { -1, 'z',  1, 'y',  1, 'x' }, {  690,  764,  759 }, { -673,  637, -463 }},
    { 0,  6,  0, 'F', { -1, 'y', -1, 'x', -1, 'z' }, {  570, -708, -669 }, { -377,  550,  716 }},
    { 0, 15,  0, 'F', {  1, 'z',  1, 'y', -1, 'x' }, {  576,  631, -465 }, { -377,  550,  716 }},
    { 0, 21,  0, 'F', { -1, 'z',  1, 'x', -1, 'y' }, { -883,  800,  504 }, {  821,  322, -697 }},
    { 0, 22,  0, 'F', { -1, 'x',  1, 'y', -1, 'z' }, { -387,  735, -519 }, {  405, -463,  594 }},
    { 0, 27,  0, 'F', { -1, 'y',  1, 'x',  1, 'z' }, { -584,  455, -474 }, { -377,  550,  716 }},
    { 0, 31,  0, 'F', {  1, 'y',  1, 'x', -1, 'z' }, { -469, -786,  725 }, {  405, -463,  594 }},
    { 1,  7,  1, 'F', { -1, 'y', -1, 'x', -1, 'z' }, { -589,  632, -556 }, {  492,  605, -667 }},
    { 1, 16,  1, 'F', { -1, 'x',  1, 'y', -1, 'z' }, {  341,  480, -430 }, { -393,  569, -783 }},
    { 1, 18,  1, 'F', { -1, 'z',  1, 'y',  1, 'x' }, {  586,  653,  430 }, { -458, -493, -747 }},
    { 1, 32,  1, 'F', { -1, 'x',  1, 'y', -1, 'z' }, { -862, -551,  669 }, { -393,  569, -783 }},
    { 1, 11,  2, 'F', { -1, 'z', -1, 'y', -1, 'x' }, { -848,  870, -519 }, {  464,  437,  812 }},
    { 1, 25,  2, 'F', {  1, 'z',  1, 'x',  1, 'y' }, { -819, -717,  730 }, {  646, -807,  485 }},
    { 1, 19,  5, 'F', { -1, 'z', -1, 'y', -1, 'x' }, { -380,  632, -579 }, { -527, -618,  452 }},
    { 1,  4, 21, 'R', {  1, 'z',  1, 'y', -1, 'x' }, {  780, -444,  760 }, {  626,  828,  344 }},
    { 1, 23, 22, 'F', {  1, 'y',  1, 'x', -1, 'z' }, {  553,  913, -698 }, {  820, -741,  761 }},
    { 1, 26, 22, 'F', { -1, 'z',  1, 'y',  1, 'x' }, {  360,  663, -726 }, {  635, -555, -793 }},
    { 2,  9,  7, 'F', { -1, 'z', -1, 'y', -1, 'x' }, {  554,  772, -348 }, { -767, -876, -603 }},
    { 2,  3, 19, 'R', { -1, 'y', -1, 'z',  1, 'x' }, { -539,  440,  712 }, { -436,  572, -578 }},
    { 2,  8,  4, 'F', {  1, 'z',  1, 'x',  1, 'y' }, {  577, -689,  716 }, {  691, -464,  520 }},
    { 2, 29,  4, 'F', {  1, 'z',  1, 'y', -1, 'x' }, {  664, -649,  866 }, { -352, -473, -587 }},
    { 2, 12, 23, 'R', { -1, 'y', -1, 'z',  1, 'x' }, { -454,  491, -678 }, { -509, -378, -516 }},
    { 3, 10, 12, 'R', {  1, 'x', -1, 'y', -1, 'z' }, { -628,  838,  754 }, { -468,  406, -817 }},
    { 3, 14, 12, 'F', { -1, 'z', -1, 'y', -1, 'x' }, {  467, -723, -700 }, { -509, -378, -516 }},
    { 3, 17, 12, 'F', { -1, 'y',  1, 'x',  1, 'z' }, { -531,  377,  793 }, { -509, -378, -516 }},
    { 3, 20, 12, 'F', {  1, 'z', -1, 'x', -1, 'y' }, {  450, -652,  686 }, { -593,  845,  571 }},
    { 4, 13, 14, 'R', {  1, 'z',  1, 'x',  1, 'y' }, {  719,  758,  705 }, { -540,  634,  673 }},
    { 4, 28, 14, 'F', { -1, 'x', -1, 'y',  1, 'z' }, {  523, -436, -581 }, {  718,  525,  642 }},
    { 4, 30, 17, 'F', {  1, 'z', -1, 'x', -1, 'y' }, { -536, -675, -691 }, { -814,  515, -501 }},
    { 4, 24, 20, 'F', {  1, 'y', -1, 'x',  1, 'z' }, { -552,  404, -633 }, { -693,  453,  644 }}
};

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    ScannerStream scannerData;
    GetInput( scannerData, glbProgPhase == TEST, true );
    cout << "Data stats - #scanners: " << scannerData.size() << endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1


    // iterate over all transforms of the transform tree, working up to the root = orientation 0
    for (int i = (int)transformTree.size() - 1; i >= 0; i--) {

        // for testing - print the complete transform node from the transform tree
        sTransform curTrfm = transformTree[i];
//        cout << "=====>>   Processing transform index: " << i << endl
//             << "             from orientation: " << curTrfm.fromOrt << " to orientation: " << curTrfm.toOrt
//             << " Orientation: ( " << (curTrfm.ort.x_sign < 0 ? "-1" : "+1") << " * " << curTrfm.ort.x_axis << ", "
//                                   << (curTrfm.ort.y_sign < 0 ? "-1" : "+1") << " * " << curTrfm.ort.y_axis << ", "
//                                   << (curTrfm.ort.z_sign < 0 ? "-1" : "+1") << " * " << curTrfm.ort.z_axis << " ) " << endl;
//        cout << "             from point: " << PrintVectorToString( curTrfm.fromBeacon )
//             <<               " to point: " << PrintVectorToString( curTrfm.toBeacon   ) << endl;


        // process the scanners that have the current "from" orientation
        for (int j = 0; j < (int)scannerData.size(); j++) {
//            cout << "Checking scanner index: " << j << " with orientation index: " << scannerData[j].nOrtIndex << " ... ";

            if (scannerData[j].nOrtIndex == curTrfm.fromOrt) {

//                cout << "content before: " << endl;
//                PrintScanner( scannerData[j], true );

                switch (curTrfm.cType) {
                    case 'F':   // forward transform
//                        cout << "    >>>>> FWD transform ";

                        // transform all beacon coordinates of this scanner to the new space (toOrt)
                        for (int k = 0; k < (int)scannerData[j].vBeacons.size(); k++) {
                            scannerData[j].vBeacons[k] = ReorientateForward( scannerData[j].vBeacons[k], curTrfm.ort );
                        }
                        // reorientate and then update scanner origin to the new space
                        scannerData[j].vPosition = ReorientateForward( scannerData[j].vPosition, curTrfm.ort );
                        scannerData[j].vPosition += GetOffset( curTrfm.toBeacon, curTrfm.fromBeacon, curTrfm.ort );
                        // finally signal that scanner has been set to different orientation
                        scannerData[j].nOrtIndex = curTrfm.toOrt;
                        break;
                    case 'R':   // reverse transform
//                        cout << "    <<<<< REV transform ";

                        // transform all beacon coordinates of this scanner to the new space (toOrt)
                        for (int k = 0; k < (int)scannerData[j].vBeacons.size(); k++) {

                            scannerData[j].vBeacons[k] = ReorientateReverse( scannerData[j].vBeacons[k], curTrfm.ort );
                        }

                        // first subtract offset from position
                        scannerData[j].vPosition -= GetOffset( curTrfm.toBeacon, curTrfm.fromBeacon, curTrfm.ort );
                        // reorientate and then update scanner origin to the new space
                        scannerData[j].vPosition = ReorientateReverse( scannerData[j].vPosition, curTrfm.ort );
                        // finally signal that scanner has been set to different orientation
                        scannerData[j].nOrtIndex = curTrfm.toOrt;
                        break;
                    default:
                        cout << "ERROR: main() --> encountered invalid tranformation type: " << curTrfm.cType << endl;
                        break;
                }
//                cout << "content after: " << endl;
//                PrintScanner( scannerData[j], true );
            } else {
//                cout << "skipped." << endl;
            }
        }
    }


    BeaconStream absoluteList;
    // Build a list of all points with absolute coordinates
    for (int i = 0; i < (int)scannerData.size(); i++) {

        if (scannerData[i].nOrtIndex != 0)
            cout << "WARNING: main() --> nOrtIndex not equal 0 for scanner " << i << endl;

        for (int j = 0; j < (int)scannerData[i].vBeacons.size(); j++) {
            // add the origin of the scanner to the beacon coordinates to get the absolute location w.r.t. scanner 0
            absoluteList.push_back( scannerData[i].vBeacons[j] + scannerData[i].vPosition );
        }
    }
    sort( begin( absoluteList ), end( absoluteList ));
    int nSizeBefore = absoluteList.size();
    absoluteList.erase( unique( begin( absoluteList ), end( absoluteList )), end( absoluteList ));
    int nSizeAfter = absoluteList.size();

//    for (int i = 0; i < (int)absoluteList.size(); i++) {
//        PrintVector( absoluteList[i] );
//        cout << endl;
//    }

    cout << "Answer part 1 - beacon list size before removing duplicates: " << nSizeBefore << " and after removing duplicates: " << nSizeAfter << endl;

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^


// ========== part 2

    int nLargestManhattan = INT_MIN;
    for (int i = 0; i < (int)scannerData.size(); i++) {
        vi3d pointA = scannerData[i].vPosition;
        for (int j = i + 1; j < (int)scannerData.size(); j++) {
            vi3d pointB = scannerData[j].vPosition;

            int nManhattan = abs( pointA.x - pointB.x ) +
                             abs( pointA.y - pointB.y ) +
                             abs( pointA.z - pointB.z );

            if (nLargestManhattan < nManhattan) {
                nLargestManhattan = nManhattan;
//                cout << "Current largest Manhattan distance = " << nLargestManhattan << endl;
            }
        }
    }

    cout << "Answer part 2 - largest Manhattan distance between any two scanners: " << nLargestManhattan << endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}


