#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// ========== data structure needed for this puzzle

// data is a bit "string" 12 bits long
typedef std::string sData;
// choose a vector as the generic container
typedef std::vector<sData> DataStream;

// ========== data input functions

// populate the example data in this function for testing your algo
void MakeExampleData( DataStream &vData ) {

    vData.clear();
    vData = {
        "00100",
        "11110",
        "10110",
        "10111",
        "10101",
        "01111",
        "00111",
        "11100",
        "10000",
        "11001",
        "00010",
        "01010"
    };
}

// populate the first 20 or so records from the input file this function for testing your algo
void MakeTestData( DataStream &vData ) {

    vData.clear();
    vData = {
        "111010101100",
        "100001001100",
        "000111101100",
        "100100000000",
        "001001001110",
        "100110101011",
        "001001100101",
        "010000010110",
        "011011001001",
        "001001000101",
        "001000101011",
        "000111101111",
        "011000000001",
        "111111010100",
        "101101010010",
        "100111100111",
        "101000110100",
        "011101101101",
        "010101110010",
        "001011101010"
    };
}

// this function can be used to input the data from the text file
void ReadData( DataStream &vData, const std::string &sFileName ) {
    std::ifstream dataFileStream( sFileName );
    vData.clear();
    sData sDatum;
    // adapt this line to match the columns of the input text file
    while (dataFileStream >> sDatum ) {
        vData.push_back( sDatum );
    }
}

// output to console for testing
void PrintData( DataStream &vData ) {
    for (auto elt : vData)
    // adapt this line to match the columns of the input text file
        std::cout << elt << std::endl;
    std::cout << std::endl << "Nr of data elements: " << vData.size() << std::endl << std::endl;
}

// ========== programming fase

enum eProgFase {
    EXAMPLE = 0,
    TEST,
    PUZZLE
};

eProgFase glbProgFase;    // what programming phase are you in - set at start of main()
int glbNrOfBits;          // 5 for test data, 12 for puzzle data

// populates vector vData, depending on the program fase (glbProgFase)
void GetInput( DataStream &vData ) {
    switch (glbProgFase) {
        case EXAMPLE: glbNrOfBits =  5; MakeExampleData( vData );              break;
        case TEST:    glbNrOfBits = 12; MakeTestData(    vData );              break;
        case PUZZLE:  glbNrOfBits = 12; ReadData(        vData, "input.txt" ); break;
    }
}

// ========== puzzle solutions in main

// creates and returns a filtered copy of org. The filtering is done on position nBit, and depending on bBitOn flag
DataStream FilterDataStream( DataStream &org, int nBit, bool bBitOn ) {
    DataStream result;
    for (int i = 0; i < (int)org.size(); i++) {
        // skip element unlees bBitOn matches the setting of the bit
        if (( bBitOn && org[i][nBit] == '1') ||
            (!bBitOn && org[i][nBit] == '0'))
            result.push_back( org[i] );
    }
    return result;
}

// returns number of elements in data that have the bit set at nBitPos
int NrOfBitsOn( DataStream &data, int nBitPos ) {
    return count_if( data.begin(), data.end(), [=]( sData e ) { return e[nBitPos] == '1'; } );
}

int NrOfBitsOff( DataStream &data, int nBitPos ) {
    return data.size() - NrOfBitsOn( data, nBitPos );
}

// returns (in char format) the most occuring bit in data at position nBitPos
// if ex equo, '1' is returned
char MostCommonBit( DataStream &data, int nBitPos ) {
    int nrOn  = NrOfBitsOn( data, nBitPos );
    int nrOff = data.size() - nrOn;
    return (nrOn >= nrOff ? '1' : '0');
}

char LeastCommonBit( DataStream &data, int nBitPos ) {
    return MostCommonBit( data, nBitPos ) == '1' ? '0' : '1';
}

int BinaryString2Value( const std::string &binString ) {
    int result = 0;
    for (int j = glbNrOfBits - 1, nFactor = 1; j >= 0; j--, nFactor *= 2) {
        result += (binString[j] == '1' ? nFactor : 0);
    }
    return result;
}

int main()
{
    glbProgFase = PUZZLE;

    // get input data, depending on the glbProgFase (example, test, puzzle)
    DataStream inputData;
    GetInput( inputData );
    // test if the input reading works correctly
    if (glbProgFase == EXAMPLE || glbProgFase == TEST)
        PrintData( inputData );

    // puzzle part 1

    // count the number of bits set per bit position
    int gammaArray[glbNrOfBits] = { 0 };
    int nrElts = (int)inputData.size();;
    for (int j = 0; j < glbNrOfBits; j++)
        gammaArray[j] += NrOfBitsOn( inputData, j );

    // create gamma and epsilon bit strings
    std::string gamma, epsilon;
    for (int j = 0; j < glbNrOfBits; j++) {
        if (gammaArray[j] > (nrElts - gammaArray[j])) {
            gamma.append(   "1" );
            epsilon.append( "0" );
        } else {
            gamma.append(   "0" );
            epsilon.append( "1" );
        }
    }

    // calculate integer value from bitstrings
    int nGammaVal   = BinaryString2Value( gamma );
    int nEpsilonVal = BinaryString2Value( epsilon );

    // output answer 1
//    cout << "gamma (binary)  : " << gamma   << " (decimal): " << nGammaVal   << endl;
//    cout << "epsilon (binary): " << epsilon << " (decimal): " << nEpsilonVal << endl;
    std::cout << "Answer part 1 - product of gamma and epsilon is: " << nGammaVal * nEpsilonVal << std::endl;

    // puzzle part 2 - Oxygen generator rating
    DataStream vWork = inputData;
    // repeatedly filter for MostCommonBit, working from left to right, until only 1 entry left
    for (int i = 0; i < glbNrOfBits && vWork.size() > 1; i++) {
        bool bBitOnFlag = (MostCommonBit( vWork, i ) == '1' );
        DataStream tmp = FilterDataStream( vWork, i, bBitOnFlag );
        vWork = tmp;
    }
    // convert to integer value
    int OGR = BinaryString2Value( vWork[0] ) ;

    // puzzle part 2 - CO2 scrubber rating
    vWork = inputData;
    // repeatedly filter for LeastCommonBit, working from left to right, until only 1 entry left
    for (int i = 0; i < glbNrOfBits && vWork.size() > 1; i++) {
        bool bBitOnFlag = (LeastCommonBit( vWork, i ) == '1' );
        DataStream tmp = FilterDataStream( vWork, i, bBitOnFlag );
        vWork = tmp;
    }
    // convert to integer value
    int SSR = BinaryString2Value( vWork[0] ) ;

    // output answer 2
//    cout << "Oxygen Generator Rating = " << OGR << endl;
//    cout << "CO2 scrubber Rating     = " << SSR << endl;
    std::cout << "Answer part 2 - product of OGR and SSR is: " << OGR * SSR << std::endl << std::endl;

    return 0;
}


