// returns true if scanner A and B are overlapping by at least a number of distances (point pairs).
// this function works on matching pairs of beacons. Disadvantage is that you don't know if beacon A1
// matches with B1 or with B2
bool FindOverlappingScanners2( ScannerStream &scannerData, int nScnrA, int nScnrB, int nAtLeastOverlapping ) {
    int result;
    struct sDistance {
        int scanner;
        int beacon1, beacon2;
        int distanceSqrd;
    };

    auto dist_smaller = [=]( sDistance &a, sDistance &b ) {
        return a.distanceSqrd < b.distanceSqrd;
    };

    vector<sDistance> distsA, distsB;

    sScanner scnrA = scannerData[ nScnrA ];
    sScanner scnrB = scannerData[ nScnrB ];
    // get the distance list of scanner A
    if (DEBUG) cout << "Getting distance list for scanner: " << nScnrA << endl;
    for (int i = 0; i < (int)scnrA.vBeacons.size(); i++) {
        for (int j = i + 1; j < (int)scnrA.vBeacons.size(); j++) {
            vi3d pointA = scnrA.vBeacons[i];
            vi3d pointB = scnrA.vBeacons[j];
            int aux = (pointA.x - pointB.x) * (pointA.x - pointB.x) +
                      (pointA.y - pointB.y) * (pointA.y - pointB.y) +
                      (pointA.z - pointB.z) * (pointA.z - pointB.z);
            sDistance dist = { nScnrA, i, j, aux };

            if (DEBUG)
                cout << "scanner: " << nScnrA << " bcn ix A: " << i
                                              << " bcn ix B: " << j << " point A: " << PrintVectorToString( pointA )
                                                                    << " point B: " << PrintVectorToString( pointB ) << " distance (squared): " << aux << endl;
            distsA.push_back( dist );
        }
    }
    // get the distance list of scanner B
    if (DEBUG) cout << "Getting distance list for scanner: " << nScnrB << endl;
    for (int i = 0; i < (int)scnrB.vBeacons.size(); i++) {
        for (int j = i + 1; j < (int)scnrB.vBeacons.size(); j++) {
            vi3d pointA = scnrB.vBeacons[i];
            vi3d pointB = scnrB.vBeacons[j];
            int aux = (pointA.x - pointB.x) * (pointA.x - pointB.x) +
                      (pointA.y - pointB.y) * (pointA.y - pointB.y) +
                      (pointA.z - pointB.z) * (pointA.z - pointB.z);
            sDistance dist = { nScnrA, i, j, aux };

            if (DEBUG) {
                cout << "scanner: " << nScnrB << " bcn ix A: " << i
                                              << " bcn ix B: " << j << " point A: " << PrintVectorToString( pointA )
                                                                    << " point B: " << PrintVectorToString( pointB ) << " distance (squared): " << aux << endl;
            }
            distsB.push_back( dist );
        }
    }
    sort( distsA.begin(), distsA.end(), dist_smaller );
    sort( distsB.begin(), distsB.end(), dist_smaller );

    if (DEBUG) {
        cout << "# distances first scanner: " << (int)distsA.size() << " # distances second scanner: " << (int)distsB.size() << endl;
    }
    int ixA = 0;
    int ixB = 0;
    bool bDone = false;
    result = 0;
    while (!bDone) {
        if (distsA[ixA].distanceSqrd == distsB[ixB].distanceSqrd) {
            if (DEBUG) {
                cout << "Found match between distance indices (A) " << ixA << " (B) " << ixB;
                cout << " at points [ from persp. scnr A ]: " << PrintVectorToString( scnrA.vBeacons[ distsA[ ixA ].beacon1 ] );
                cout << " and: "                              << PrintVectorToString( scnrA.vBeacons[ distsA[ ixA ].beacon2 ] ) << endl;
            }
            result += 1;
            ixA++;
            ixB++;
        } else if (distsA[ixA].distanceSqrd < distsB[ixB].distanceSqrd) {
            ixA++;
        } else if (distsA[ixA].distanceSqrd > distsB[ixB].distanceSqrd) {
            ixB++;
        }
        bDone = (ixA >= (int)distsA.size() || ixB >= (int)distsB.size());
    }
    if (DEBUG)
        cout << "Total matches found: " << result << endl;

    return (result >= nAtLeastOverlapping);
}

