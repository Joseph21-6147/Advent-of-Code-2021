// AoC 2021 - day 17
// =================

// date:  2021-12-17
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <vector>
#include <algorithm>

#include "../flcTimer.h"
#include "../vector_types.h"

//using namespace std;

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

struct sRect {
    vllong2d uplt, dnrt;
};

// ==========   DATA INPUT FUNCTIONS

void GetExampleData( vllong2d &startPoint, vllong2d &curPoint, sRect &targetRect, vllong2d &velocity ) {
// target area: x=20..30, y=-10..-5
    startPoint      = {  0,  0 };
    targetRect.uplt = { 20, -5 };
    targetRect.dnrt = { 30,-10 };
    velocity        = {  0,  0 };
    curPoint        = startPoint;
}

void GetTestData( vllong2d &startPoint, vllong2d &curPoint, sRect &targetRect, vllong2d &velocity ) {
    startPoint      = {  0,   0 };    // created my own example here
    targetRect.uplt = {  5,  -5 };
    targetRect.dnrt = { 10, -10 };
    velocity        = {  0,   0 };
    curPoint        = startPoint;
}

void GetPuzzleData( vllong2d &startPoint, vllong2d &curPoint, sRect &targetRect, vllong2d &velocity ) {
// target area: x=185..221, y=-122..-74  << contents of input file
    startPoint      = {   0,    0 };
    targetRect.uplt = { 185,  -74 };
    targetRect.dnrt = { 221, -122 };
    velocity        = {   0,    0 };
    curPoint        = startPoint;
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void Print_vllong2d( vllong2d &v ) {
    std::cout << "( " << v.x << " , " << v.y << " )";
}

void Print_vi2d( vi2d &v ) {
    std::cout << "( " << v.x << " , " << v.y << " )";
}

void PrintCurSituation( vllong2d &startPoint, vllong2d &curPoint, sRect &targetRect, vllong2d &velocity ) {
    std::cout <<  "Start position: ";  Print_vllong2d( startPoint );
    std::cout << " Cur. position: ";   Print_vllong2d( curPoint );
    std::cout << " Target: up-left: "; Print_vllong2d( targetRect.uplt );
    std::cout <<     " down-right: ";  Print_vllong2d( targetRect.dnrt );
    std::cout << " Cur. velocity: ";   Print_vllong2d( velocity );
    std::cout << std::endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program phase (glbProgPhase)
void GetInput( vllong2d &startPoint, vllong2d &curPoint, sRect &targetRect, vllong2d &velocity, bool bDisplay = false ) {
    // read the right input file and set the map dimensions
    switch (glbProgPhase) {
        case EXAMPLE: GetExampleData( startPoint, curPoint, targetRect, velocity ); break;
        case TEST:    GetTestData(    startPoint, curPoint, targetRect, velocity ); break;
        case PUZZLE:  GetPuzzleData(  startPoint, curPoint, targetRect, velocity ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintCurSituation( startPoint, curPoint, targetRect, velocity );
    }
}

// ==========  PUZZLE SPECIFIC SOLUTIONS

// returns quadrant where point is irt target
//  0 | 1 | 2
// ---+---+---
//  3 | 4 | 5    // where 4 is target
// ---+---+---
//  6 | 7 | 8
int GetQuadrant( vllong2d &point, sRect &rect ) {
    int result, tmp_col, tmp_row;
    int x_small = rect.uplt.x, x_large = rect.dnrt.x;
    int y_small = rect.dnrt.y, y_large = rect.uplt.y;

           if (point.x < x_small) { tmp_col = 0;
    } else if (point.x > x_large) { tmp_col = 2;
    } else {                        tmp_col = 1;
    }
           if (point.y < y_small) { tmp_row = 2;
    } else if (point.y > y_large) { tmp_row = 0;
    } else {                        tmp_row = 1;
    }
    result = (tmp_row * 3) + tmp_col;
    return result;
}

bool InBetween( int n, int n_smaller, int n_larger ) {
    if (n_smaller > n_larger) std::cout << "ERROR: InBetween() --> parameters out of order: " << n_smaller << " not smaller than " << n_larger << std::endl;
    return (n_smaller <= n && n <= n_larger);
}

// generates the range 0, 1, 3, 6, 10, 15, 21, 28, 36, recursively defined by
//   *  elt[ 0 ] = 0
//   *  elt[ 1 ] = 1
//   *  elt[ n ] = elt[ n-1 ] + n
class CumSumEnumerator {
    public:
        CumSumEnumerator() {
            m_CumSumVector.push_back( 0 );   // set first two values
            m_CumSumVector.push_back( 1 );
        }

        // index must be >= 0
        long long GetCS( int index ) {
            // error checking
            if (index < 0)
                return -1;
            // if the value asked for is not calculated yet
            if (index >= (int)m_CumSumVector.size()) {
                // extend the vector so that it gets calculated
                for (int i = m_CumSumVector.size(); i <= index; i++) {
                    m_CumSumVector.push_back( m_CumSumVector[ i - 1 ] + i );
                }
            }
            // return the calculated value from the vector
            return m_CumSumVector[index];
        }

    private:
        std::vector<long long> m_CumSumVector;
};

CumSumEnumerator glbCSenum;

// since the velocity decreases (due to gravity) with 1 per step,
// the top of the parabola is given by glbCSenum
long long GetParabolicTop( int yVel ) {
    return glbCSenum.GetCS( yVel );
}

//The probe's x position increases by its x velocity.
//The probe's y position increases by its y velocity.
//Due to drag, the probe's x velocity changes by 1 toward the value 0; that is,
//  it decreases by 1 if it is greater than 0,
//  it increases by 1 if it is less than 0,
//  or does not change if it is already 0.
//Due to gravity, the probe's y velocity decreases by 1.
void DoOneStep( vllong2d &curPoint, vllong2d &velocity ) {
    curPoint.x += velocity.x;
    curPoint.y += velocity.y;
    if (velocity.x > 0)
        velocity.x -= 1;
    else if (velocity.x < 0)
        velocity.x += 1;
    velocity.y -= 1;
}

// =====================================

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    vllong2d  startPoint;
    vllong2d  curPoint;
    sRect     targetRect;
    vllong2d  startVel;
    vllong2d  curVel;

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    GetInput( startPoint, curPoint, targetRect, startVel, glbProgPhase != PUZZLE );

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    // the smallest y coordinate of the target rect is -122. Since any launch follows a strict parabolic
    // trajectory, and it starts from y = 0, it will hit the x axis (where y = 0) again having the same
    // but negative velocity as it started with in positive direction. The next step will decrease the
    // negative velocity with one.
    // So if the start velocity equals the distance between y = 0 and lower part of rectangle, it will
    // overshoot the rectangle by exactly one. Following that if the start velocity is 122 - 1 = 121, it
    // will exactly hit the bottom part of the target rect, AND have the largest positive y velocity.
    // Therefore, the answer is the parabolic top that belongs to the 122 - 1
    long long answer = GetParabolicTop( abs( targetRect.dnrt.y ) - 1 );
    std::cout << "Answer part 1 = " << answer << std::endl;

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    // get list of x velocity values that end exactly in between target range
    std::vector<int> vVerticals;

    bool bContinue = true;
    for (int i = 0; bContinue; i++) {
        int aux = glbCSenum.GetCS( i );
        if (InBetween( aux, targetRect.uplt.x, targetRect.dnrt.x ))
            vVerticals.push_back( i );
        else if (aux > targetRect.dnrt.x)
            bContinue = false;
    }
    std::cout << "Found " << vVerticals.size() << " verticals: ";
    for (int i = 0; i < (int)vVerticals.size(); i++) {
        std::cout << vVerticals[i] << " ";
    }

    std::vector<vllong2d> vHits;

    for (startVel.x = 1; startVel.x <= std::max( vVerticals.back(), (int)targetRect.dnrt.x ); startVel.x++) {
        for (startVel.y = targetRect.dnrt.y; startVel.y <= -targetRect.dnrt.y; startVel.y++) {

            curVel   = startVel;
            curPoint = startPoint;

            bool bStopExperiment = false;
            // since x velocity >= 1, no need to experiment further than right sight of target rect
            for (int i = 0; !bStopExperiment; i++) {
                DoOneStep( curPoint, curVel );

                int nQuadrant = GetQuadrant( curPoint, targetRect );
                switch (nQuadrant) {
                    case 4: // hit in target rectangle
                            vHits.push_back( startVel );
                            break;
                    case 2:
                    case 5:
                    case 6:
                    case 7:
                    case 8: bStopExperiment = true;
                            break;
                    case 0:
                    case 1:
                    case 3: // just continu experiment, target rect may still be reached
                            break;
                }
            }
        }
    }

    std::sort( vHits.begin(), vHits.end(), []( vllong2d a, vllong2d b ) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    });
    vHits.erase( std::unique( std::begin( vHits ), std::end( vHits )), std::end( vHits ));

    std::cout << "Number of hits: " << vHits.size() << std::endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}


