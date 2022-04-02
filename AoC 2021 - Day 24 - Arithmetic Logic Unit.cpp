#include <iostream>
#include <vector>

#include "../flcTimer.h"

struct sParam {
    int divPrm;
    int addPrm1;
    int addPrm2;
};
std::vector<sParam> vParameters = {
    {  1,  12,  9 },
    {  1,  12,  4 },
    {  1,  12,  2 },
    { 26,  -9,  5 },
    { 26,  -9,  1 },
    {  1,  14,  6 },
    {  1,  14, 11 },
    { 26, -10, 15 },
    {  1,  15,  7 },
    { 26,  -2, 12 },
    {  1,  11, 15 },
    { 26, -15,  9 },
    { 26,  -9, 12 },
    { 26,  -3, 12 }
};

// w is input digit at index digitIndex. z_pre is the z value of the previous digit,
// z_post is the resulting z_value of this digit (w)
// returns true if still valid, false otherwise
bool EvaluateDigitPosition( int w, int digitIndex, int &z_pre, int &z_post ) {
    bool bResult = false;
    switch (vParameters[digitIndex].divPrm) {
        case  1: {
            z_post = z_pre * 26 + w + vParameters[digitIndex].addPrm2;
            bResult = true;
            break;
        }
        case 26: {
            if ((w - vParameters[digitIndex].addPrm1) == (z_pre % 26)) {
                z_post = z_pre / 26;
                bResult = true;
            } else {
                z_post = -1;
                bResult = false;
            }
            break;
        }
        default: {
            std::cout << "ERROR: EvaluateDigitPosition() --> can't determine sequence type" << vParameters[digitIndex].divPrm << std::endl;
        }
    }

    return bResult;
}

bool EvaluateSerialNumber( std::string sSerNr, int &indexToIncrementNext ) {

    bool bSuccess = true;
    int prevZ = 0, postZ;
    for (int i = 0; i < 14 && bSuccess; i++) {
        int auxW = (int)(sSerNr[i] - '0');
        bSuccess = EvaluateDigitPosition( auxW, i, prevZ, postZ );
        if (!bSuccess) {
            indexToIncrementNext = i;
        } else {
            prevZ = postZ;
        }
    }

    // the serial number is valid if all digits didn't fail, and the last one yields 0
    return bSuccess && (postZ == 0);
}

void DecrementAtIndex( std::string &serNr, int index ) {
    if (index >= 14 || index < 0)
        std::cout << "ERROR: DecrementIndex() --> index out of range: " << index << std::endl;
    if (serNr[index] > '1') {
        serNr[index] -= 1;
        for (int i = index + 1; i < 14; i++) serNr[i] = '9';
    } else {
        DecrementAtIndex( serNr, index - 1 );
    }
}

// This is the only thing I needed to add for part 2
void IncrementAtIndex( std::string &serNr, int index ) {
    if (index >= 14 || index < 0)
        std::cout << "ERROR: IncrementAtIndex() --> index out of range: " << index << std::endl;
    if (serNr[index] < '9') {
        serNr[index] += 1;
        for (int i = index + 1; i < 14; i++) serNr[i] = '1';
    } else {
        IncrementAtIndex( serNr, index - 1 );
    }
}

int main()
{
    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

// ========== part 1

    std::string candidateNr;
//    int nProgress;
    bool bFound;


// count downwards, and find the first (thus the largest) valid serial number

    candidateNr = "99999999999999";
//    nProgress = 0;
    bFound = false;
    while (!bFound) {
        int progressIndex;
        bFound = EvaluateSerialNumber( candidateNr, progressIndex );
        if (!bFound) {
            DecrementAtIndex( candidateNr, progressIndex );
        }
//        // rudimentary progress indication
//        nProgress += 1;
//        if (nProgress > 1000000) {
//            cout << candidateNr << "    ---> ";
//            tmr.TimeReport( "lap time per 1*10^6 iterations: " );
//            nProgress = 0;
//        }
    }

    if (bFound) {
        std::cout << "Answer: " << candidateNr << std::endl;
    }

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

// count upwards, and find the first (thus the smallest) valid serial number

    candidateNr = "11111111111111";
//    nProgress = 0;
    bFound = false;
    while (!bFound) {
        int progressIndex;
        bFound = EvaluateSerialNumber( candidateNr, progressIndex );
        if (!bFound) {
            IncrementAtIndex( candidateNr, progressIndex );
        }
//        // rudimentary progress indication
//        nProgress += 1;
//        if (nProgress > 1000000) {
//            cout << candidateNr << "    ---> ";
//            tmr.TimeReport( "lap time per 1*10^6 iterations: " );
//            nProgress = 0;
//        }
    }

    if (bFound) {
        std::cout << "Answer: " << candidateNr << std::endl;
    }

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}
