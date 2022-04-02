#include <iostream>
#include <vector>
#include <fstream>

// define the data structure needed for this puzzle
struct sData {
    std::string sCommand;
    int nValue;
};
typedef std::vector<sData> DataStream;

// open text file containing one column of only integer numbers
void ReadData( DataStream &vData, std::string sFileName ) {
    std::ifstream dataFileStream( sFileName );
    vData.clear();
    sData sDatum;

    while (dataFileStream >> sDatum.sCommand >> sDatum.nValue ) {
        vData.push_back( sDatum );
    }
}

void MakeTestData( DataStream &vData ) {
    vData.clear();
    vData = {
        { "forward", 5 },
        { "down"   , 5 },
        { "forward", 8 },
        { "up"     , 3 },
        { "down"   , 8 },
        { "forward", 2 }
    };
}

int main()
{
    DataStream inputData;
    // Either get testdata, or get puzzle data from file
//    MakeTestData( inputData );
    ReadData( inputData, "input.txt" );

    // test if the input reading works correctly
//    for (auto elt : inputData)
//        cout << elt.sCommand << " " << elt.nValue << endl;

    // puzzle part 1
    int nX     = 0;
    int nDepth = 0;

    for (int i = 0; i < (int)inputData.size(); i++) {
        sData sDatum = inputData[i];
        if (sDatum.sCommand == "forward") {
            nX += sDatum.nValue;
        } else if (sDatum.sCommand == "up") {
            nDepth -= sDatum.nValue;
        } else if (sDatum.sCommand == "down") {
            nDepth += sDatum.nValue;
        } else {
            std::cout << "ERROR - data element contains unknown command: " << sDatum.sCommand << std::endl;
        }
    }

    std::cout << "Answer 1: forward (" << nX << ") * depth (" << nDepth << ") = " << nX * nDepth << std::endl;

    // puzzle part 2
    nX       = 0;
    nDepth   = 0;
    int nAim = 0;

    for (int i = 0; i < (int)inputData.size(); i++) {
        sData sDatum = inputData[i];
        if (sDatum.sCommand == "forward") {
            nX += sDatum.nValue;
            nDepth += nAim * sDatum.nValue;
        } else if (sDatum.sCommand == "up") {
            nAim -= sDatum.nValue;
        } else if (sDatum.sCommand == "down") {
            nAim += sDatum.nValue;
        } else {
            std::cout << "ERROR - data element contains unknown command: " << sDatum.sCommand << std::endl;
        }
    }

    std::cout << "Answer 2: forward (" << nX << ") * depth (" << nDepth << ") = " << nX * nDepth << std::endl;

    return 0;
}


