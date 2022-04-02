#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../flcTimer.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgFase {     // what programming fase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgFase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef string sData;               // data consists of a list of strings, each containing opening and closing braces (){}<>[]
typedef vector<sData> DataStream;

// ==========   DATA INPUT FUNCTIONS

// populate the example data in this function
void ExampleData( DataStream &vData ) {     // <<<<< ========== adapt to match columns of input file
    vData.clear();
    vData = {   "[({(<(())[]>[[{[]{<()<>>",
                "[(()[<>])]({[<{<<[]>>(",
                "{([(<{}[<>[]}>{[]{[(<()>",
                "(((({<>}<{<{<>}{[]{[]{}",
                "[[<[([]))<([[{}[[()]]]",
                "[{[{({}]{}}([{[{{{}}([]",
                "{<[[]]>}<{[{[{[]{()[[[]",
                "[<(<(<(<{}))><([]([]()",
                "<{([([[(<>()){}]>(<<{{",
                "<{([{{}}[<[[[<>{}]]]>[]]" };
}

// this function is to input the data from the text file
void ReadData( string sFileName, DataStream &vData ) {
    ifstream dataFileStream( sFileName );
    vData.clear();
    sData sDatum;
    while (dataFileStream >> sDatum )      // <<<<< ========== adapt to match columns of input file
        vData.push_back( sDatum );
    dataFileStream.close();
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintDataStream( DataStream &vData ) {
    for (int i = 0; i < (int)vData.size(); i++)
        cout << "Data: " << vData[i] << endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program fase (glbProgFase)
void GetInput( DataStream &vData ) {
    switch (glbProgFase) {
        case EXAMPLE: ExampleData(                vData ); break;
        case TEST:    ReadData( "input.test.txt", vData ); break;
        case PUZZLE:  ReadData( "input.txt",      vData ); break;
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

bool IsClosing( char c ) { return ( c == ')' || c == ']' || c == '>' || c == '}' ); }
bool IsOpening( char c ) { return ( c == '(' || c == '[' || c == '<' || c == '{' ); }

char GetMatch( char cOpen) {
    switch (cOpen) {
        case '(': return ')';
        case '[': return ']';
        case '<': return '>';
        case '{': return '}';

        case ')': return '(';
        case ']': return '[';
        case '>': return '<';
        case '}': return '{';
    }
    return 'X';
}

bool IsMatch( char cOpen, char cClose ) {
    return  GetMatch( cOpen ) == cClose;
}

#define STACK_SIZE 128

class MyStack {
    public:
    //    MyStack() {};
        void push( char c ) {
            mySP += 1;
            myStack[mySP] = c;
        }
        char pop() {
            mySP -= 1;
            return myStack[mySP+1];
        }
        char size() {
            return mySP + 1;
        }
        bool empty() {
            return size() == 0;
        }
        void print() {
            for (int i = mySP; i >= 0; i--)
                cout << " " << myStack[i];
            cout << endl;
        }
        string convert() {
            string result;
            for (int i = mySP; i >= 0; i--)
                result.append( 1, GetMatch( myStack[i] ));
            return result;
        }
    private:
        char myStack[STACK_SIZE];
        int  mySP = -1;
};

// returns an error value > 0 upon corruption
// returns 0 if no corruption. The invalid character is passed in cError
int ProcessOneLine1( string &line, char &cError, bool bOutput ) {

    // hard coded scores - see description of puzzle
    auto get_error_score = [=]( char c ) {
        switch (c) {
            case ')': return     3;
            case ']': return    57;
            case '}': return  1197;
            case '>': return 25137;
        }
        return -1;
    };

    // parse input using auxiliary stack ds. Quit & report upon error
    MyStack cStack;
    bool bError = false;
    int nErrorScore = 0;
    for (int i = 0; i < (int)line.length() && !bError; i++) {
        if (IsOpening( line[i] ))
            cStack.push( line[i] );
        else if (IsClosing( line[i] )) {
            char tmp = cStack.pop();
            if (!IsMatch( tmp, line[i] )) {
                if (bOutput)
                    cout << "ERROR encountered - expected: " << GetMatch( tmp ) << ", but found: " << line[i] << " instead." << endl;
                bError = true;
                cError = line[i];
                nErrorScore = get_error_score( line[i] );
            }
        }
    }
    return nErrorScore;
}

// calculate score - see description of the puzzle
// NOTE: if you use ints or long you'll get overflow problems
long long ClosingPoints( string s ) {

    // from the description of the puzzle
    auto point_value = [=]( char c ) {
        switch (c) {
            case ')': return 1;
            case ']': return 2;
            case '}': return 3;
            case '>': return 4;
        }
        return 0;
    };

    long long result = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        result *= 5;
        result += point_value( s[i] );
    }
    return result;
}

int ProcessOneLine2( string &line, string &sIncomplete ) {
    MyStack cStack;
    // build up stack to determine what's missing
    for (int i = 0; i < (int)line.length(); i++) {
        if (IsOpening( line[i] ))
            cStack.push( line[i] );
        else if (IsClosing( line[i] )) {
            cStack.pop();
        }
    }
    // create a string of what's missing
    if (cStack.empty()) {
        cout << "WARNING: line is NOT incomplete... " << endl;
    } else {
        sIncomplete.clear();
        sIncomplete = cStack.convert();
    }

    return sIncomplete.length();
}

int main()
{
    glbProgFase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgFase (example, test, puzzle)
    DataStream inputData;
    GetInput( inputData );
//    PrintDataStream( inputData );
    int nSize   = inputData.size();
    cout << "data stats - #elts " << nSize << endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    // parse all lines for errors
    int nSyntaxErrorScore = 0;
    for (int i = 0; i < (int)inputData.size(); i++) {
        char tmp;
        nSyntaxErrorScore += ProcessOneLine1( inputData[i], tmp, false );
    }
    cout << "Total syntax error score: " << nSyntaxErrorScore << endl;

    tmr.TimeReport( "Timing 1 : " );   // ==============================^^^^^

// ========== part 2

    vector<long long> vScores;
    for (int i = 0; i < (int)inputData.size(); i++) {
        char tmp;
        if (ProcessOneLine1( inputData[i], tmp, false ) == 0) {
            string aux;
            ProcessOneLine2( inputData[i], aux );
            long long nClosePts = ClosingPoints( aux );
//            cout << "Line: " << inputData[i] << " - needed to complete " << aux << " which represents: " << nClosePts << endl;
            vScores.push_back( nClosePts );
        }
    }
    // sort the scores and pick the middle one
    sort( vScores.begin(), vScores.end());
    int pick = (vScores.size() - 1) / 2;
    cout << "middle is: " << pick << " value " << vScores[pick] << endl;

    tmr.TimeReport( "Timing 2 : " );   // ==============================^^^^^

    return 0;
}


