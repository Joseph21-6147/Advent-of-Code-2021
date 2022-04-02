// AoC 2021 - day 22
// =================

// date:  2021-12-22
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../flcTimer.h"
#include "vector_types.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

#define MIN_COORDINATE  -50    // at least (for part 1)
#define MAX_COORDINATE   50    // at most

struct sUnitCube {
    vllong3d origin;       // unit cube is represented by it's origin and
                       // extends from there by one in +x, +y, +z directions
    bool bIsOn = false;
};
typedef vector<sUnitCube> CubeStream;

struct sCuboid {
    vllong3d minPoint, maxPoint;   // minPoint has all the lowest x, y and z values, maxPoint the highest
};
struct sInstruction {
    sCuboid cuboid;
    bool bTurnOn = false;
};
typedef vector<sInstruction> InstrStream;

// ==========   OUTPUT FUNCTIONS

void PrintVector( vllong3d &v, bool bPadding = true ) {
    auto nr_paddings = [=]( int n ) {
        string result;
        if (n <= -100) result = "";    else
        if (n <=  -10) result = " ";   else
        if (n <=   -1) result = "  ";  else
        if (n <=    9) result = "   "; else
        if (n <=   99) result = "  ";  else
                       result = " ";
        return bPadding ? result : "";
    };
    cout << "[" << nr_paddings( v.x ) << v.x <<
            "," << nr_paddings( v.y ) << v.y <<
            "," << nr_paddings( v.z ) << v.z << "]";
}

// output to console for testing
void PrintInstrStream( InstrStream &iData ) {
    for (int i = 0; i < (int)iData.size(); i++) {
        cout << "Instruction # " << i << " = switch " << (iData[i].bTurnOn ? "ON " : "OFF") << " for minvec: ";
        PrintVector( iData[i].cuboid.minPoint, false );
        cout << " and maxvec: ";
        PrintVector( iData[i].cuboid.maxPoint, false );
        cout << endl;
    }
    cout << endl;
}

// ==========   DATA INPUT FUNCTIONS

void SplitToken( const string &token_in, string &x_token, string &y_token, string &z_token ) {

    size_t splitIndex = token_in.find( "," );
    string tmp;
    x_token = token_in.substr( 0, splitIndex  ); if (x_token[0] != 'x') cout << "ERROR: SplitToken() --> failing x_token" << endl;
    tmp     = token_in.substr( splitIndex + 1 );
    splitIndex = tmp.find( "," );
    y_token = tmp.substr( 0, splitIndex  ); if (y_token[0] != 'y') cout << "ERROR: SplitToken() --> failing y_token" << endl;
    z_token = tmp.substr( splitIndex + 1 ); if (z_token[0] != 'z') cout << "ERROR: SplitToken() --> failing z_token" << endl;
}

void ParseRange( const string &token, char cCheck, int &minVal, int &maxVal ) {

    if (token[0] != cCheck) cout << "ERROR: ParseRange() --> expected " << cCheck << " but got " << token[0] << endl;
    size_t splitIndex = token.find( "=" );
    string sRelevantPart = token.substr( splitIndex + 1 );
    splitIndex = sRelevantPart.find( ".." );
    minVal = atoi( sRelevantPart.substr( 0, splitIndex  ).c_str() );
    maxVal = atoi( sRelevantPart.substr( splitIndex + 2 ).c_str() );
}

// this function is used to input the data from the text file
void ReadData( const string &sFileName, InstrStream &iData ) {
    // open filestream
    ifstream dataFileStream( sFileName );
    // read data stream
    iData.clear();

    bool bDone = dataFileStream.eof();
    while (!bDone) {

        // read one line (consisting of two tokens, the other three are used later on)
        sInstruction aux;
        string token[5];
        if (dataFileStream >> token[0] >> token[1]) {
            if (token[0] == "on") {
                aux.bTurnOn = true;
            } else if (token[0] == "off") {
                aux.bTurnOn = false;
            } else {
                cout << "ERROR: ReadData() --> encountered unknown first token: " << token[0] << endl;
            }
            SplitToken( token[1], token[2], token[3], token[4] );
            int minValue, maxValue;
            // get x min and max values into cuboid
            ParseRange( token[2], 'x', minValue, maxValue );
            aux.cuboid.minPoint.x = minValue;
            aux.cuboid.maxPoint.x = maxValue;
            // get y min and max values into cuboid
            ParseRange( token[3], 'y', minValue, maxValue );
            aux.cuboid.minPoint.y = minValue;
            aux.cuboid.maxPoint.y = maxValue;
            // get z min and max values into cuboid
            ParseRange( token[4], 'z', minValue, maxValue );
            aux.cuboid.minPoint.z = minValue;
            aux.cuboid.maxPoint.z = maxValue;

            iData.push_back( aux );

            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgPhase)
void GetInput( InstrStream &iData, bool bDisplay = false ) {

    switch (glbProgPhase) {
        case EXAMPLE: ReadData( "input.example.txt", iData ); break;
        case TEST:    ReadData( "input.test.txt"   , iData ); break;
        case PUZZLE:  ReadData( "input.txt"        , iData ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintInstrStream( iData );
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

// ==========   Class Cuboid

// this code is inspired by Masfo - thanks!!
class Cuboid {
    public:
        Cuboid() = default;   // why is this useful?
        Cuboid( vllong3d _minCoord, vllong3d _maxCoord, long long _on, bool _bEmpty = false ) : minCoord( _minCoord ), maxCoord( _maxCoord ), on( _on ), bEmpty( _bEmpty ) {}

    private:
        vllong3d minCoord, maxCoord;
        long long on = -1;
        bool bEmpty = false;

    public:
        long long volume() {
            // why is the 1 + even necessary?
            return (1 + maxCoord.x - minCoord.x) *
                   (1 + maxCoord.y - minCoord.y) *
                   (1 + maxCoord.z - minCoord.z);
        }

        // the on value of the intersection is - per definition - the negative of the cuboid intersected with
        Cuboid intersection( Cuboid &other ) {
            // if 'this' cuboid fully fits within b, it is completely cancelled
            // so return empty cuboid
            if (minCoord.x > other.maxCoord.x || maxCoord.x < other.minCoord.x ||
                minCoord.y > other.maxCoord.y || maxCoord.y < other.minCoord.y ||
                minCoord.z > other.maxCoord.z || maxCoord.z < other.minCoord.z)
                // is this gonna work like this?
                return Cuboid( { 0, 0, 0 }, { 0, 0, 0 }, -1, true );

            return Cuboid{ { max( minCoord.x, other.minCoord.x ), max( minCoord.y, other.minCoord.y ), max( minCoord.z, other.minCoord.z ) },
                           { min( maxCoord.x, other.maxCoord.x ), min( maxCoord.y, other.maxCoord.y ), min( maxCoord.z, other.maxCoord.z ) }, -other.on, false };
        }

        long long GetOnVal() { return on; }

        bool IsOn() {  return (on ==  1); }
        bool IsOff() { return (on == -1); }
        bool IsEmpty() { return bEmpty; }

        string ToString() {
            string s;
            s.append( "On/Off = " + to_string( on ) + " " );
            auto print_vector = [=]( vllong3d v ) {
                string sRes;
                auto nr_paddings = [=]( int n ) {
                    string result;
                    if (n <= -100) result = ""   ; else
                    if (n <=  -10) result = " "  ; else
                    if (n <=   -1) result = "  " ; else
                    if (n <=    9) result = "   "; else
                    if (n <=   99) result = "  " ; else
                                   result = " "  ;
                    return result;
                };
                sRes.append( "[" + nr_paddings( v.x ) + to_string( v.x ) +
                             "," + nr_paddings( v.y ) + to_string( v.y ) +
                             "," + nr_paddings( v.z ) + to_string( v.z ) + "]" );
                return sRes;
            };
            s.append( print_vector( minCoord ));
            s.append( print_vector( maxCoord ));
            return s;
        }
};

// this code is inspired by Masfo - thanks!!
long long count_cuboids( vector<Cuboid> &cuboidList ) {
    // build a list of intersected cuboids, having flag on to be -1 (for off) or 1 (for on)
    vector<Cuboid> intersected_cuboids;
    for (auto &cuboid : cuboidList)
    {
        vector<Cuboid> add;
        // if original cuboid was on, add it to the "add" list
        if (cuboid.IsOn())
            add.emplace_back(cuboid);

        // how exactly does this work?
        for (auto &othercuboid : intersected_cuboids) {

            // what is this for syntactic construction? Something with std::optional<T>...
//            if (auto interS = cuboid.intersection(othercuboid); interS)
//                add.emplace_back(*interS);

            // my own attempt here !!
            Cuboid interS = cuboid.intersection( othercuboid );
            if (!interS.IsEmpty())
                add.emplace_back( interS );
        }
        for (const auto &newcuboid : add)
            intersected_cuboids.emplace_back(newcuboid);
    }

    // now count the grand total over the intersected cuboid list
    long long volume = 0;
    for (auto &cuboid : intersected_cuboids)
        volume += cuboid.volume() * cuboid.GetOnVal();

    return volume;
}

// Aux function to transform the input as I organized it into the form that Masfo's code expected
void TransformInput( int nrElts, InstrStream &iData, vector<Cuboid> &v1, vector<Cuboid> &v2 ) {
    v1.clear();
    v2.clear();
    int nRange = (nrElts == -1 ? iData.size() : nrElts);

    for (int i = 0; i < nRange; i++) {
        sInstruction aux = iData[i];
        Cuboid tmp( aux.cuboid.minPoint, aux.cuboid.maxPoint, ( aux.bTurnOn ? +1 : -1 ));
        v2.push_back( tmp );
        if ( aux.cuboid.minPoint.x >= MIN_COORDINATE && aux.cuboid.minPoint.x <= MAX_COORDINATE &&
             aux.cuboid.maxPoint.x >= MIN_COORDINATE && aux.cuboid.maxPoint.x <= MAX_COORDINATE )
            v1.push_back( tmp );
    }
}

int main()
{
    glbProgPhase = TEST;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    InstrStream instrData;
    // get input data, depending on the glbProgPhase (example, test, puzzle)
    GetInput( instrData, false );

    vector<Cuboid> cuboidList1;
    vector<Cuboid> cuboidList2;

    TransformInput( -1, instrData, cuboidList1, cuboidList2 );

    cout << "Data stats - #instructions " << instrData.size() << endl;
    cout << "Cuboid list 1 # elts: " << cuboidList1.size() << endl;
    cout << "Cuboid list 2 # elts: " << cuboidList2.size() << endl;

    TransformInput( -1, instrData, cuboidList1, cuboidList2 );

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    long long llAnswer1 = count_cuboids( cuboidList1 );
    cout << "Answer 1 = " << llAnswer1 << endl;

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    long long llAnswer2 = count_cuboids( cuboidList2 );
    cout << "Answer 2 = " << llAnswer2 << endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}





