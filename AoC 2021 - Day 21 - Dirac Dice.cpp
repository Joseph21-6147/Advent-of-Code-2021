// AoC 2021 - day 21
// =================

// date:  2021-12-21
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#include "../flcTimer.h"

using namespace std;

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

int glbTurn = 0;        // how many turns are played
int playerToTurn = 1;   // can be 1 or 2 representing player 1 or 2
int nrDiceThrows = 0;
int playerPos[2];       // NOTE there's no position 0. [1-10] and wrapping around
int playerScore[2];
int winningPlayer = -1;

// ==========   DATA INPUT FUNCTIONS

// input:
// Player 1 starting position: 4
// Player 2 starting position: 9   // example is 8

// no file reading today, just dump all the data into code
void GetData() {
    glbTurn = 1;
    playerToTurn = 1;
    playerPos[0] = 4;  // note that 0 is not a position
    switch (glbProgPhase) {
        case EXAMPLE: playerPos[0] = 4; playerPos[1] = 8; break;
        case TEST   : playerPos[0] = 4; playerPos[1] = 8; break;
        case PUZZLE : playerPos[0] = 4; playerPos[1] = 9; break;
    }
    playerScore[0] = 0;
    playerScore[1] = 0;
    winningPlayer = -1;
}

// ==========   OUTPUT FUNCTIONS


// output to console for testing
void PrintGameState() {
    cout << "Current turn: " << glbTurn;
    cout << " Player to turn: " << playerToTurn;
    cout << " Positions - player 1: " << playerPos[0] << ", player 2: " << playerPos[1];
    cout << endl;
}

// ==========   PROGRAM PHASING

// populates input data
void GetInput( bool bDisplay = false ) {

    GetData();
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintGameState();
    }
}

// ==========   PUZZLE SPECIFIC SOLUTIONS

class DetermDie {
    public:
        DetermDie() {}

        int ThrowOnce() {
            m_current_throw += 1;
            if (m_current_throw > 100)
                m_current_throw = 1;
            m_total_rolls += 1;
            return m_current_throw;
        }

        int ThrowTrice() {
            int result = ThrowOnce() + ThrowOnce() + ThrowOnce();
            m_total_rolls += 3;
            return result;
        }

        int GetTotalRolls() {
            return m_total_rolls;
        }
    private:
        int m_current_throw = 0;
        int m_total_rolls = 0;
};

void PlayGame( int nrTurns, bool bUntilWin = false ) {
    DetermDie die;
    bool bWonFlag = false;

    for (glbTurn = 1; glbTurn <= nrTurns || (bUntilWin && !bWonFlag); glbTurn++) {
        for (playerToTurn = 1; playerToTurn <= 2 && !bWonFlag; playerToTurn++) {
            int nThrow[3] = { die.ThrowOnce(), die.ThrowOnce(), die.ThrowOnce() };
            playerPos[playerToTurn - 1] += ((nThrow[0] + nThrow[1] + nThrow[2]) % 10);
            if (playerPos[playerToTurn - 1] > 10)
                playerPos[playerToTurn - 1] = (playerPos[playerToTurn - 1] % 11) + 1;

            playerScore[playerToTurn - 1] += playerPos[playerToTurn - 1];
//            cout << "Player " << playerToTurn << " rolls " << nThrow[0] << "+" << nThrow[1] << "+" << nThrow[2];
//            cout << " and moves to space " << playerPos[playerToTurn - 1] << " for a total score of " << playerScore[ playerToTurn - 1];
//            cout << endl;

            if (bUntilWin) {
                bWonFlag = playerScore[ playerToTurn - 1] >= 1000;
                winningPlayer = playerToTurn;
            }
        }
    }
    cout << "Total die rolls: " << die.GetTotalRolls() << endl;
    cout << "Winning: " << winningPlayer << endl;
    int losingPlayer = (winningPlayer == 1) ? 2 : 1;
    cout << "Losing: " << losingPlayer << " losing score: " << playerScore[ losingPlayer - 1 ] << endl;
    cout << "Answer part 1: " << (die.GetTotalRolls() * playerScore[ losingPlayer - 1 ]) << endl;
}

struct sDiracRoll {
    int value;
    int frequency;
};
vector<sDiracRoll> rolls = {
    { 3, 1 },
    { 4, 3 },
    { 5, 6 },
    { 6, 7 },
    { 7, 6 },
    { 8, 3 },
    { 9, 1 }
};

typedef struct sPair {
    long long p1, p2;
} my_pair;

typedef struct sGameSit {
    int who;
    int board0, board1;
    int score0, score1;

    // needed for application of this struct in a std::map as a key
    bool operator < ( const sGameSit &rhs ) const {
        // this five-level distinction is needed to get the map function correctly
        return ( this->who <  rhs.who ||
                (this->who == rhs.who && ( this->board0 <  rhs.board0 ||
                                          (this->board0 == rhs.board0 && ( this->board1 <  rhs.board1 ||
                                                                          (this->board1 == rhs.board1 && ( this->score0 <  rhs.score0 ||
                                                                                                          (this->score0 == rhs.score0 && this->score1 < rhs.score1))))))));
    }
} my_situation;

typedef map<my_situation, my_pair> my_memo;
my_memo glbMemo;

my_pair solve1( int who, int player0, int player1, int score0, int score1 ) {
    my_pair ans = { 0, 0 };
    int players[2] = { player0, player1 };
    int scores[ 2] = {  score0,  score1 };

    // check if either player has won the game
    if (scores[0] >= 21) {
        ans = { 1, 0 };
    } else if (scores[1] >= 21) {
        ans = { 0, 1 };
    } else {
        // check if the answer is in the memoization
        my_situation sit = { who, player0, player1, score0, score1 };
        my_memo::iterator it = glbMemo.find( sit );
        if (it != glbMemo.end()) {
            ans = (*it).second;
        } else {

            // calculate the answer and store in the memoization
            for (int i = 0; i < (int)rolls.size(); i++) {
                sDiracRoll roll = rolls[i];
                int add  = roll.value;
                int ways = roll.frequency;
                players[who] = (players[who] + add - 1) % 10 + 1;
                scores[ who] += players[who];

                my_pair now = solve1( 1 - who, players[0], players[1], scores[0], scores[1] );

                // store answer in memoization
                my_situation newSit = { 1 - who, players[0], players[1], scores[0], scores[1] };
                glbMemo.insert( make_pair( newSit, now ));

                ans.p1 += ways * now.p1;
                ans.p2 += ways * now.p2;

                scores[ who] = (who == 0) ?  score0 :  score1;
                players[who] = (who == 0) ? player0 : player1;
            }
        }
    }
    return ans;
}

my_pair solve2( int who, int player0, int player1, int score0, int score1 ) {
    my_pair ans = { 0, 0 };
    int players[2] = { player0, player1 };
    int scores[ 2] = {  score0,  score1 };

    // check if either player has won the game
    if (scores[0] >= 21) {
        ans = { 1, 0 };
    } else if (scores[1] >= 21) {
        ans = { 0, 1 };
    } else {

        // calculate the answer
        for (int i = 0; i < (int)rolls.size(); i++) {
            sDiracRoll roll = rolls[i];
            int add  = roll.value;
            int ways = roll.frequency;
            players[who] = (players[who] + add - 1) % 10 + 1;
            scores[ who] += players[who];

            my_pair now = solve2( 1 - who, players[0], players[1], scores[0], scores[1] );

            ans.p1 += ways * now.p1;
            ans.p2 += ways * now.p2;

            scores[ who] = (who == 0) ?  score0 :  score1;
            players[who] = (who == 0) ? player0 : player1;
        }

    }
    return ans;
}

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    GetInput( false );

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    PlayGame( 4, true );

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    int nWho    = 0;
    int nStart0 = 4;
    int nStart1 = 9;
    int nScore0 = 0;
    int nScore1 = 0;
    my_pair answer1 = solve1( nWho, nStart0, nStart1, nScore0, nScore1 );

    cout << "Part 2 - p1 = " << answer1.p1 << ", p2 = " << answer1.p2 << endl;

    tmr.TimeReport( "Timing 2 (with memoization): " );   // ==============================^^^^^

    my_pair answer2 = solve2( nWho, nStart0, nStart1, nScore0, nScore1 );

    cout << "Part 2 - p1 = " << answer2.p1 << ", p2 = " << answer2.p2 << endl;

    tmr.TimeReport( "Timing 2 (without memoization): " );   // ==============================^^^^^
    return 0;
}





