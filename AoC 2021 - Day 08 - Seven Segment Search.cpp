#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <Windows.h>

#include "../flcTimer.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgFase {     // what programming fase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgFase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

struct sData {
    string pattern[10];   // 10 unique patterns (1x2, 1x3, 1x4, 3x5, 3x6, 1x7)
    string values[4];     // strings to decode

    char key[7];          // if key[0] == 'b' --> each 'a' must be mapped to b segment
};

typedef vector<sData> DataStream;

// ========== data input functions

bool ReadDataLine( ifstream &dFileStream, sData &d ) {

    string trash;
    bool bAbort = false;
    for (int j = 0; j < 10; j++) {          // read 10 unique patterns
        bAbort = !(dFileStream >> d.pattern[j]);
    }
    dFileStream >> trash;
    for (int j = 0; j < 4; j++) {           // read 4 digit strings
        bAbort = !(dFileStream >> d.values[j]);
    }
    for (int j = 0; j < 7; j++)             // initialize key array
        d.key[j] = ' ';
    return !bAbort;
}

// this function can be used to input the data from the text file
void ReadDataStream( string sFileName, DataStream &vData ) {
    ifstream dataFileStream( sFileName );    // open filestream
    vData.clear();
    bool bDone = dataFileStream.eof();
    while (!bDone) {
        sData d;
        bool bSuccess = ReadDataLine( dataFileStream, d );
        if (bSuccess) {
            vData.push_back( d );
            bDone = dataFileStream.eof();
        } else {
            bDone = true;
        }
    }
    dataFileStream.close();
}

// ========== output functions

// output to console for testing
void PrintDataLine( sData &Datum ) {
    for (int i = 0; i < 10; i++) {
        cout << Datum.pattern[i] << " ";
    }
    cout << " ||| ";
    for (int i = 0; i < 4; i++) {
        cout << Datum.values[i] << " ";
    }
    cout << endl;
}

void PrintDataStream( DataStream &vData ) {
    for (auto &elt : vData)
        PrintDataLine( elt );
}

// ========== data processing functions

// assume!! statArray is size 10 and contains counts of values afterwards
// only used for part 1
void GetDataStats( DataStream &vData, int statArr[], bool bOutput ) {
    for (int i = 0; i < (int)vData.size(); i++) {
        for (int j = 0; j < 4; j++) {
            int len = vData[i].values[j].length();
            statArr[ len ] += 1;
        }
    }
    if (bOutput) {
        for (int i = 0; i < 10; i++) {
            cout << "Countarray nr " << i << " # " << statArr[i];
            switch ( i ) {
                case 0:
                case 1: cout << " <must be 0!>"; break;
                case 2: cout << " digit 1" ; break;
                case 3: cout << " digit 7" ; break;
                case 4: cout << " digit 4" ; break;
                case 7: cout << " digit 8" ; break;
                case 5: // don't care that much since it's not asked in part 1
                case 6:
                case 8:
                case 9:cout << " <unknown>" ; break;
            }
            cout << endl;
        }
    }
}

// ========== program phasing

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( DataStream &vData ) {
    switch (glbProgFase) {
        case EXAMPLE: ReadDataStream( "input.example.txt", vData ); break;
        case TEST:    ReadDataStream( "input.test.txt"   , vData ); break;
        case PUZZLE:  ReadDataStream( "input.txt"        , vData ); break;
    }
}

// ========== puzzle solutions

// remove all elements from a that are present in b, leave the rest
string difference( string a, string b ) {
    string result;
    for (int i = 0; i < (int)a.length(); i++) {
        bool bKeep = true;
        for (int j = 0; j < (int)b.length(); j++) {
            if (a[i] == b[j]) bKeep = false;
        }
        if (bKeep)
            result.append( 1, a[i] );
    }
//    cout << "difference: " << a << " and " << b << " is " << result << endl;
    return result;
}

// remove all elements from a that are NOT present in b
string in_common( string a, string b ) {
    string result;
    for (int i = 0; i < (int)a.length(); i++) {
        bool bKeep = false;
        for (int j = 0; j < (int)b.length(); j++) {
            if (a[i] == b[j]) bKeep = true;
        }
        if (bKeep)
            result.append( 1, a[i] );
    }
//    cout << "in_common: " << a << " and " << b << " is " << result << endl;
    return result;
}

// how many times occurs char c in string a?
int nr_occurrences( string a, char c ) {
    int result = 0;
    for (int i = 0; i < (int)a.length(); i++) {
        if (a[i] == c) result += 1;
    }
//    cout << "nr_occurences char " << c << " in " << a << " is " << result << endl;
    return result;
}

// returns next pattern from datum that has length.
// the search starts from patIndex, which is updated upon finding
string FindNextPatternOfLength(sData &datum, int length, int &patIndex ) {
    string result;

    bool found = false;
    for (int j = patIndex; j < 10 && !found; j++) {
        if (length == (int)datum.pattern[j].length()) {
            found = true;
            result = datum.pattern[j];
            patIndex = j;
        }
    }

    return result;
}

// returns first pattern of length in datum
string FindFirstPatternOfLength(sData &datum, int length ) {
    int dummy = 0;
    return FindNextPatternOfLength( datum, length, dummy );
}

//This is how the segments are identified, a-g or 0-7:
//
//      aaaa        0000
//     b    c      1    2
//     b    c      1    2
//      dddd        3333
//     e    f      4    5
//     e    f      4    5
//      gggg        6666

// This function analyses the patterns in Datum, and creates a decoding scheme in the
// key array of Datum
// Look at the notes (separate doc) - this is hard to understand without them!
void AnalyseOneLine( sData &Datum ) {

//    PrintDataElement( Datum );

    // get all known digitstrings
    string first1 = FindFirstPatternOfLength( Datum, 2 ); // cout << "first1 = " << first1 << endl;
    string first7 = FindFirstPatternOfLength( Datum, 3 ); // cout << "first7 = " << first7 << endl;
    string first4 = FindFirstPatternOfLength( Datum, 4 ); // cout << "first4 = " << first4 << endl;
    string first8 = FindFirstPatternOfLength( Datum, 7 ); // cout << "first8 = " << first8 << endl;

    // determine segment A (= 0), using the difference between pattern of 7 and pattern of 1
    string result = difference( first7, first1 );
    if (result.length() != 1) cout << "ERROR ";
    Datum.key[0] = result[0];
//    cout << "Segment A (= 0): " << Datum.key[0] << endl;

    // look for the string representing digit 6
    bool bFound = false;
    int localPatIndex = 0;
    string first6;
    while (!bFound) {
        first6 = FindNextPatternOfLength( Datum, 6, localPatIndex );

        string tmp = in_common( first6, first1 );
        if (tmp.length() == 1) { // found
            bFound = true;
        } else {
            localPatIndex += 1;
        }
    }

    // determine segment F and C - using the difference approach again
    result = in_common( first6, first1 );
    if (result.length() != 1) cout << "ERROR ";
    Datum.key[5] = result[0];  // segment F
//    cout << "Segment F (= 5): " << Datum.key[5] << endl;

    result = difference( first8, first6 );
    if (result.length() != 1) cout << "ERROR ";
    Datum.key[2] = result[0];
//    cout << "Segment C (= 2): " << Datum.key[2] << endl;

    // look for the string representing digit 3
    bFound = false;
    localPatIndex = 0;
    string first3;
    while (!bFound) {
        first3 = FindNextPatternOfLength( Datum, 5, localPatIndex );

        string tmp = in_common( first3, first1 );
        if (tmp.length() == 2) { // found a 3 string
            bFound = true;
        } else {
            localPatIndex += 1;
        }
    }

    // look for the string representing digit 5
    bFound = false;
    localPatIndex = 0;
    string first5;
    while (!bFound) {
        first5 = FindNextPatternOfLength( Datum, 5, localPatIndex );

        string tmp = in_common( first5, string( 1, Datum.key[2] ));
        if (tmp.length() == 0) { // found a 5 string
            bFound = true;
        } else {
            localPatIndex += 1;
        }
    }
    // Determine segment B - using the difference approach between digit patterns 5 and 3
    result = difference( first5, first3 );
    if (result.length() != 1) cout << "ERROR ";
    Datum.key[1] = result[0];
//    cout << "Segment B (= 1): " << Datum.key[1] << endl;

    // find string representing digit 4 and determine segment D from it
    result.clear();
    result.append( 1, Datum.key[1] );
    result.append( 1, Datum.key[2] );
    result.append( 1, Datum.key[5] );
    string tmp = difference( first4, result );
    if (tmp.length() != 1) cout << "ERROR ";
    Datum.key[3] = tmp[0];
//    cout << "Segment D (= 3): " << Datum.key[3] << endl;

    // look for the string representing digit 2
    bFound = false;
    localPatIndex = 0;
    string first2;
    while (!bFound) {
        first2 = FindNextPatternOfLength( Datum, 5, localPatIndex );

        string aux1 = difference( first2, first3 );
        string aux2 = difference( first2, first5 );
        if (aux1.length() == 1 && aux2.length() == 2) { // found
            bFound = true;
        } else {
            localPatIndex += 1;
        }
    }
    // determine segment E using difference between digit 2 and digit 3 patterns
    result = difference( first2, first3 );
    if (result.length() != 1) cout << "ERROR ";
    Datum.key[4] = result[0];
//    cout << "Segment E (= 4): " << Datum.key[4] << endl;

    // determine segment G - the only one remainging, just subtract all
    // known segments A-F from the digit 8 pattern
    tmp.clear();
    for (int k = 0; k <= 5; k++)
        tmp.append( 1, Datum.key[k] );

    result = difference( first8, tmp );
    if (result.length() != 1) cout << "ERROR ";
    Datum.key[6] = result[0];
//    cout << "Segment G (= 6): " << Datum.key[6] << endl;
}

int DecodeDigit( sData &Datum, string sDig ) {
    int result = -1;

    auto match_numbers = [=]( int i0, int i1, int i2, int i3, int i4, int i5, int i6 ) {
        return ( (nr_occurrences( sDig, Datum.key[ 0 ] ) == i0) &&
                 (nr_occurrences( sDig, Datum.key[ 1 ] ) == i1) &&
                 (nr_occurrences( sDig, Datum.key[ 2 ] ) == i2) &&
                 (nr_occurrences( sDig, Datum.key[ 3 ] ) == i3) &&
                 (nr_occurrences( sDig, Datum.key[ 4 ] ) == i4) &&
                 (nr_occurrences( sDig, Datum.key[ 5 ] ) == i5) &&
                 (nr_occurrences( sDig, Datum.key[ 6 ] ) == i6) );
    };

    if (match_numbers( 1, 1, 1, 0, 1, 1, 1 )) result = 0; else
    if (match_numbers( 0, 0, 1, 0, 0, 1, 0 )) result = 1; else
    if (match_numbers( 1, 0, 1, 1, 1, 0, 1 )) result = 2; else
    if (match_numbers( 1, 0, 1, 1, 0, 1, 1 )) result = 3; else
    if (match_numbers( 0, 1, 1, 1, 0, 1, 0 )) result = 4; else
    if (match_numbers( 1, 1, 0, 1, 0, 1, 1 )) result = 5; else
    if (match_numbers( 1, 1, 0, 1, 1, 1, 1 )) result = 6; else
    if (match_numbers( 1, 0, 1, 0, 0, 1, 0 )) result = 7; else
    if (match_numbers( 1, 1, 1, 1, 1, 1, 1 )) result = 8; else
    if (match_numbers( 1, 1, 1, 1, 0, 1, 1 )) result = 9; else {
        cout << "ERROR encountered in DecodeDigit() " << sDig << endl;
        for (int i = 0; i < 7; i++) {
            cout << "segment nr (A = 0, G = 6): " << i << " is " << Datum.key[i] << endl;
        }
        Sleep( 5000 );
    }

// alternative approach

//    char cSeg2 = Datum.key[2];
//    char cSeg4 = Datum.key[4];
//    char cSeg5 = Datum.key[5];
//
//    switch (sDig.length()) {
//        case 2: result = 1; break;
//        case 3: result = 7; break;
//        case 4: result = 4; break;
//        case 7: result = 8; break;
//
//        case 5:  // determine 2, 3 or 5
//            if ( nr_occurrences( sDig, cSeg5 ) == 0) {
//                result = 2;
//            } else if (nr_occurrences( sDig, cSeg2 ) == 1) {
//                result = 3;
//            } else {
//                result = 5;
//            }
//            break;
//        case 6:  // determine 0, 6 or 9
//            if ( nr_occurrences( sDig, cSeg2 ) == 0) {
//                result = 6;
//            } else if (nr_occurrences( sDig, cSeg4 ) == 0) {
//                result = 9;
//            } else {
//                result = 0;
//            }
//            break;
//        default: cout << "ERROR encountered in DecodeDigit() " << sDig << endl;
//    }
    return result;
}

int main()
{
    flcTimer tmr;
    glbProgFase = PUZZLE;

    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgFase (example, test, puzzle)
    DataStream inputData;
    GetInput( inputData );
    int nSize   = inputData.size();
    cout << "Data stats - #elts " << nSize << endl;

    tmr.TimeReport( "Data input phase: " );   // ====================^^^^^vvvvv

// ========== part 1

//    PrintAllData( inputData );
    int dataStats[10] = { 0 };
    GetDataStats( inputData, dataStats, false );

    cout << endl;
    cout << "answer 1 - nr of digits 1 " << dataStats[2];
    cout <<         " - nr of digits 7 " << dataStats[3];
    cout <<         " - nr of digits 4 " << dataStats[4];
    cout <<         " - nr of digits 8 " << dataStats[7] << endl;
    cout << "answer 1 - TOTAL nr of digits 1, 4, 7 and 8 = " << dataStats[2] + dataStats[3] + dataStats[4] + dataStats[7] << endl;

    tmr.TimeReport( "Timing answer 1: " );   // ==============================^^^^^

    int nCumTotal = 0;
    for (int j = 0; j < (int)inputData.size(); j++) {
        AnalyseOneLine( inputData[j] );
//        cout << "decoded ";
        int nCumSubTotal = 0;
        int nFactor = 1000;
        for (int i = 0; i < 4; i++) {
            int tmp = DecodeDigit( inputData[j], inputData[j].values[i] );
//            cout << tmp  << " " ;

            nCumSubTotal += (tmp * nFactor);
            nFactor = nFactor / 10;
        }
        nCumTotal += nCumSubTotal;
//        cout << endl;
//        Sleep( 5000 );
    }

    cout << "answer 2 - TOTAL: " << nCumTotal << endl;

    tmr.TimeReport( "Timing answer 2: " );   // ==============================^^^^^

    return 0;
}




