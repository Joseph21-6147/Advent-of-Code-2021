#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// there's a text file containing one column of only integer numbers
// open it and get the input in a std::vector
void ReadData( vector<int> &vData, string sFileName ) {

    ifstream dataStream( sFileName );
    vData.clear();
    int nDatum;
    while (dataStream >> nDatum ) {
        vData.push_back( nDatum );
    }
    dataStream.close();
}

int main()
{
    vector<int> inputData;
    ReadData( inputData, "input.txt" );

    int answer1 = 0, answer2 = 0;
    for (int i = 0; i < (int)inputData.size(); i++) {
        if (i > 0) {
            if (inputData[i] > inputData[i - 1])
                answer1 += 1;
        }

        if (i > 3) {
            int s1 = inputData[i - 1] + inputData[i - 2] + inputData[i - 3];
            int s2 = inputData[i]     + inputData[i - 1] + inputData[i - 2];
            if (s2 > s1)
                answer2 += 1;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return 0;
}


