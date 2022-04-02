// AoC 2021 - day 16
// =================

// date:  2021-12-16
// by:    Joseph21 (Joseph21-6147)

#include <iostream>
#include <vector>

#include "../flcTimer.h"

// ==========   PROGRAM PHASING

enum eProgPhase {     // what programming phase are you in - set at start of main()
    EXAMPLE = 0,
    TEST,
    PUZZLE
} glbProgPhase;

// ==========   DATA STRUCTURES          <<<<< ========== adapt to match columns of input file

typedef std::string DataStream;  // hexadecimal char's encoding 4 bits each

DataStream inputData;

// ==========   DATA INPUT FUNCTIONS

void GetExampleData( std::string &output ) {
    output.clear();
//    output = "D2FE28";                          // literal packet
//    output = "38006F45291200";                  // operator packet example 1
//    output = "EE00D40C823060";                  // operator packet example 2
//    output = "8A004A801A8002F478";              // operator packet example 3
//    output = "620080001611562C8802118E34";      // operator packet example 4
//    output = "C0015000016115A2E0802F182340";    // operator packet example 5
//    output = "A0016C880162017C3686B18A3D4780";  // operator packet example 6

//    output = "C200B40A82";                      // part 2 example # 1
//    output = "04005AC33890";                    // part 2 example # 2
//    output = "880086C3E88112";                  // part 2 example # 3
//    output = "CE00C43D881120";                  // part 2 example # 4
//    output = "D8005AC2A8F0";                    // part 2 example # 5
//    output = "F600BC2D8F";                      // part 2 example # 6
//    output = "9C005AC2F8F0";                    // part 2 example # 7
    output = "9C0141080250320F1802104A08";      // part 2 example # 8
}

void GetTestData( std::string &output ) {
    output.clear();
    output = "EE00D40C823060";
}

void GetPuzzleData( std::string &output ) {
    output.clear();
    output.append( "420D610055D273AF1630010092019207300B278BE5932551E703E608400C335003900AF040290500" );
    output.append( "9923003880856201E95C00B60198D400B50034400E20101DC00E10024C00F1003C400B0002126971" );
    output.append( "40249D049F0F8952A8C6009780272D5D074B5741F3F37730056C0149658965E9AED7CA8401A5CC45" );
    output.append( "BB801F0999FFFEEE0D67050C010C0036278A62D4D737F359993398027800BECFD8467E3109945C10" );
    output.append( "08210C9C442690A6F719C48A351006E9359C1C5003E739087E80F27EC29A0030322BD2553983D272" );
    output.append( "C67508E5C0804639D4BD004C401B8B918E3600021D1061D47A30053801C89EF2C4CCFF39204C53C2" );
    output.append( "12DABED04C015983A9766200ACE7F95C80D802B2F3499E5A700267838803029FC56203A009CE134C" );
    output.append( "773A2D3005A77F4EDC6B401600043A35C56840200F4668A71580043D92D5A02535BAF7F9A89CF97C" );
    output.append( "9F59A4F02C400C249A8CF1A49331004CDA00ACA46517E8732E8D2DB90F3005E92362194EF5E630CA" );
    output.append( "5E5EEAD1803E200CC228E70700010A89D0BE3A08033146164177005A5AEEB1DA463BDC667600189C" );
    output.append( "9F53A6FF6D6677954B27745CA00BCAE53A6EEDC60074C920001B93CFB05140289E8FA4812E071EE4" );
    output.append( "47218CBE1AA149008DBA00A497F9486262325FE521898BC9669B382015365715953C5FC01AA80021" );
    output.append( "11721D4221007E13C448BA600B4F77F694CE6C01393519CE474D46009D802C00085C578A71E40010" );
    output.append( "98F518639CC301802B400E7CDDF4B525C8E9CA2188032600E44B8F1094C0198CB16A29180351EA1D" );
    output.append( "C3091F47A5CA0054C4234BDBC2F338A77B84F201232C01700042A0DC7A8A0200CC578B10A65A0006" );
    output.append( "01048B24B25C56995A30056C013927D927C91AB43005D127FDC610EF55273F76C96641002A4F0F8C" );
    output.append( "01CCC579A8D68E52587F982996F537D600" );
}

void HexToBinString( std::string &in, std::string &out ) {
    out.clear();
    for (int i = 0; i < (int)in.length(); i++) {
        switch (in[i]) {
            case '0': out.append( "0000" ); break;
            case '1': out.append( "0001" ); break;
            case '2': out.append( "0010" ); break;
            case '3': out.append( "0011" ); break;
            case '4': out.append( "0100" ); break;
            case '5': out.append( "0101" ); break;
            case '6': out.append( "0110" ); break;
            case '7': out.append( "0111" ); break;
            case '8': out.append( "1000" ); break;
            case '9': out.append( "1001" ); break;
            case 'A': out.append( "1010" ); break;
            case 'B': out.append( "1011" ); break;
            case 'C': out.append( "1100" ); break;
            case 'D': out.append( "1101" ); break;
            case 'E': out.append( "1110" ); break;
            case 'F': out.append( "1111" ); break;
            default: std::cout << "ERROR: HexToBinString() --> unexpected hex char encountered: " << in[i] << std::endl;
        }
    }
}

// ==========   OUTPUT FUNCTIONS

// output to console for testing
void PrintDataStream( DataStream &vData ) {
    std::cout << "Data (size) " << vData.length() << " (content) " << vData << std::endl << std::endl;
}

// ==========   PROGRAM PHASING

// populates data vector(s), depending on the program phase (glbProgPhase)
void GetInput( DataStream &vData, bool bDisplay = false ) {
    // read the right input file and set the map dimensions
    switch (glbProgPhase) {
        case EXAMPLE: GetExampleData( vData ); break;
        case TEST:    GetTestData(    vData ); break;
        case PUZZLE:  GetPuzzleData(  vData ); break;
    }
    // display to console if so desired (for debugging)
    if (bDisplay) {
        PrintDataStream( vData );
    }
}

// ==========  PUZZLE SPECIFIC SOLUTIONS

/* PARSE SPECIFICATIONS
 * --------------------
 *
 * packet			    <header>.{ <literal_body> | <operator_body> }
 *
 * header			    <version>.<typeID>
 *
 * version			    fixed  3
 * typeID			    fixed  3   		// if 4 then literal packet, else operator packet
 *
 * literal_body		    <lit_group>+		// + means "1 or more times"
 * lit_group		    fixed 1.fixed 4 	// if fixed 1 == 0 --> last group for literal, if 1 not last group
 *
 * operator_body		<length_typeID>
 * 			            { <total_length_bits>.<packet>+ | 	// nr of bits is known
 * 			            <nr_sub_packets>.<packet>+	} 	// nr of sub packets is known
 *
 * length_typeID		fixed  1		// if 0 --> bit specified, if 1 --> nr packets specified
 * total_length_bits	fixed 15		// used for bit specified packet(s)
 * nr_sub_packets		fixed 11		// used for nr specified packet(s)
 */


// necessary prototypes
long long ParsePacket( std::string &worksString, bool bOutput = false );

// ----- Generic functions -----

// keeps track of indentation level
int nIndentCntr = 0;

// generic indentation function
void Indent() {
    for (int j = 0; j < nIndentCntr; j++) {
        for (int i = 0; i < 4; i++)
            std::cout << " ";
    }
}

int BinToInt( std::string &binString ) {
    int factor = 1, result = 0;
    for (int i = binString.length() - 1; i >= 0; i--) {
        if (binString[i] == '1') result += factor;
        factor *= 2;
    }
    return result;
}

// converts a std::string representing bin nr to an int (adapted to long long later on) of that value
long long BinStringToLLong( std::string &binString ) {
    long long factor = 1, result = 0;
    for (int i = binString.length() - 1; i >= 0; i--) {
        if (binString[i] == '1') result += factor;
        factor *= 2;
    }
    return result;
}

// parses a token from input of length len, and returns the two result std::strings as parameters
// token and remainder
void ParseTokenLen( std::string &worksString, int len, std::string &token, std::string &remainder ) {
    token     = worksString.substr( 0, len );
    remainder = worksString.erase(  0, len );
}

// ----- Atom tokens parsing -----

// part 1 - global var to collect sum of all version nrs
int nGlbVersionSum = 0;

int ParseVersion( std::string &worksString, bool bOutput = false ) {
    std::string token, remainder;
    ParseTokenLen( worksString, 3, token, remainder );
    worksString = remainder;
    int result = BinStringToLLong( token );

    nGlbVersionSum += result;

    if (bOutput) { Indent(); std::cout << "Version parsing - token: " << token << " value " << result << std::endl; }

    return result;
}

// returns a std::string denoting the function per type ID
std::string PacketType( int nTypeID ) {
    switch (nTypeID) {
        case  0: return "SUM";     break;
        case  1: return "PRODUCT"; break;
        case  2: return "MIN";     break;
        case  3: return "MAX";     break;
        case  4: return "LITERAL"; break;
        case  5: return "GREATER"; break;
        case  6: return "SMALLER"; break;
        case  7: return "EQUAL";   break;
    }
    return "**ERROR**";
}

int ParseTypeID( std::string &worksString, bool bOutput = false ) {
    std::string token, remainder;
    ParseTokenLen( worksString, 3, token, remainder );
    worksString = remainder;
    int result = BinStringToLLong( token );

    if (bOutput) { Indent(); std::cout << "TypeID parsing - token: " << token << " value " << result << " meaning " << PacketType( result ) << std::endl; }

    return result;
}

int ParseLengthTypeID( std::string &worksString, bool bOutput = false ) {
    std::string token, remainder;
    ParseTokenLen( worksString, 1, token, remainder );
    worksString = remainder;
    int result = BinStringToLLong( token );

    if (bOutput) { Indent(); std::cout << "LengthTypeID parsing - token: " << token << " value " << result << std::endl; }

    return result;
}

int ParseTotalLengthBits( std::string &worksString, bool bOutput = false ) {
    std::string token, remainder;
    ParseTokenLen( worksString, 15, token, remainder );
    worksString = remainder;
    int result = BinStringToLLong( token );

    if (bOutput) { Indent(); std::cout << "TotalLengthBits parsing - token: " << token << " value " << result << std::endl; }

    return result;
}

int ParseNrSubPackets( std::string &worksString, bool bOutput = false ) {
    std::string token, remainder;
    ParseTokenLen( worksString, 11, token, remainder );
    worksString = remainder;
    int result = BinStringToLLong( token );

    if (bOutput) { Indent(); std::cout << "NrSubPackets parsing - token: " << token << " value " << result << std::endl; }

    return result;
}

// ----- Compound tokens parsing -----

void ParseHeader( std::string &worksString, int &nVersion, int &nTypeID, bool bOutput = false ) {
    nVersion = ParseVersion( worksString, bOutput );
    nTypeID  = ParseTypeID(  worksString, bOutput );
}

// returns true if last literal group in literal
bool ParseLiteralGroup( std::string &worksString, std::string &output, bool bOutput = false ) {
    std::string token, remainder;
    ParseTokenLen( worksString, 5, token, remainder );
    worksString = remainder;
    output = token.substr( 1, 4 );

    bool bIsLastGroup = (token[0] == '0');
    if (bOutput) { Indent(); std::cout << "LiteralGroup parsing - token: " << output << " last group in literal: " << (bIsLastGroup ? "YES" : "NO") << std::endl; }

    return bIsLastGroup;
}

long long ParseLiteralBody( std::string &worksString, bool bOutput = false ) {
    int nGroupCount = 0;
    bool quitFlag = false;
    std::string sLiteral;

    while (!quitFlag) {    // parse one or more LiteralGroups
        nGroupCount += 1;
        std::string aux;
        quitFlag = ParseLiteralGroup( worksString, aux, bOutput );
        sLiteral.append( aux );
    }
    long long nLiteral = BinStringToLLong( sLiteral );

    if (bOutput) { Indent(); std::cout << "LiteralBody parsing - nr of groups " << nGroupCount << " result std::string: " << sLiteral << " result value: " << nLiteral << std::endl; }

    return nLiteral;
}

long long ParseOperatorBody( std::string &worksString, int nTypeId, bool bOutput = false ) {

    long long result;               // final result of evaluating this packet
    std::vector<long long> vIntermRes;   // intermediate result vector

    // first - get the length type id to determine how to parse the subpackets
    int nLengthTypeID = ParseLengthTypeID( worksString, bOutput );

    if (nLengthTypeID == 0) {   // bit specified
        // parse how many bits to isolate for the sub packets parsing
        int nrTotalLengthBits = ParseTotalLengthBits( worksString, bOutput );
        // isolate this bit std::string
        std::string token, remainder;
        ParseTokenLen( worksString, nrTotalLengthBits, token, remainder );
        worksString = remainder;
        // parse the bit std::string in sub packets until the remaining std::string contains only 0's
        do {
            // recursive call on substd::string
            long long aux = ParsePacket( token, bOutput );
            vIntermRes.push_back( aux );
        } while (BinStringToLLong( token ) != 0);

        // do some error checking
        if (bOutput) {
            int nDiscarded = token.length();
            if (nDiscarded > 0) {
                Indent();
                std::cout << "WARNING: ParseOperatorBody() --> discarding " << nDiscarded << " bits (std::string): " << token << std::endl;
            }
        }

    } else if (nLengthTypeID == 1) {                    // nr packets specified
        // determine the number of sub packets to parse ...
        int nrSubPackets = ParseNrSubPackets( worksString, bOutput );
        // ... and parse exactly that number from the input std::string
        for (int i = 0; i < nrSubPackets; i++) {
            // recursive call on substd::string
            long long aux = ParsePacket( worksString, bOutput );
            vIntermRes.push_back( aux );
        }

        if (bOutput) {
            Indent();
            std::cout << "WARNING: ParseOperatorBody() --> parsed " << nrSubPackets << " sub packets" << std::endl;
        }
    } else {
        Indent();
        std::cout << "ERROR: () --> unknown LengthTypeID value: " << nLengthTypeID << std::endl;
    }

// Packets with type ID [ from the puzzle description ]
//   0 are sum packets - their value is the sum of the values of their sub-packets. If they only have a
//     single sub-packet, their value is the value of the sub-packet.
//   1 are product packets - their value is the result of multiplying together the values of their
//     sub-packets. If they only have a single sub-packet, their value is the value of the sub-packet.
//   2 are minimum packets - their value is the minimum of the values of their sub-packets.
//   3 are maximum packets - their value is the maximum of the values of their sub-packets.
//   5 are greater than packets - their value is 1 if the value of the first sub-packet is greater than
//     the value of the second sub-packet; otherwise, their value is 0. These packets always have exactly
//     two sub-packets.
//   6 are less than packets - their value is 1 if the value of the first sub-packet is less than the
//     value of the second sub-packet; otherwise, their value is 0. These packets always have exactly two sub-packets.
//   7 are equal to packets - their value is 1 if the value of the first sub-packet is equal to the
//     value of the second sub-packet; otherwise, their value is 0. These packets always have exactly two sub-packets.

    // the following code assumes that vIntermRes contains at least 1 element, may be more. Check this up front!!
    if (vIntermRes.size() <= 0) {
        Indent();
        std::cout << "ERROR: ParseOperatorBody() --> no sub packet results to process for operator type " << nTypeId << std::endl;
    }
    // for nTypeID 5 - 7 exactly 2 elements should be present in intermediate result vector. Check this up front!!
    if (5 <= nTypeId && nTypeId <= 7 && vIntermRes.size() != 2) {
        Indent();
        std::cout << "ERROR: ParseOperatorBody() --> expected two sub packets, but there are: " << vIntermRes.size() << std::endl;
    }

    switch (nTypeId) {
        case  0: result =         0; for (int i = 0; i < (int)vIntermRes.size(); i++) {                             result += vIntermRes[i]; } break;  // sum packet
        case  1: result =         1; for (int i = 0; i < (int)vIntermRes.size(); i++) {                             result *= vIntermRes[i]; } break;  // product packet
        case  2: result = LLONG_MAX; for (int i = 0; i < (int)vIntermRes.size(); i++) { if (vIntermRes[i] < result) result  = vIntermRes[i]; } break;  // minimum packet
        case  3: result = LLONG_MIN; for (int i = 0; i < (int)vIntermRes.size(); i++) { if (vIntermRes[i] > result) result  = vIntermRes[i]; } break;  // maximum packet
        case  5: result = (vIntermRes[0] >  vIntermRes[1] ? 1 : 0); break;   // greater than packet
        case  6: result = (vIntermRes[0] <  vIntermRes[1] ? 1 : 0); break;   // less than packet
        case  7: result = (vIntermRes[0] == vIntermRes[1] ? 1 : 0); break;   // is equal packet
        case  4: Indent(); std::cout << "ERROR: ParseOperatorBody() --> called with nTypeID == 4..."              << std::endl; break;
        default: Indent(); std::cout << "ERROR: ParseOperatorBody() --> unknown type ID encountered: " << nTypeId << std::endl;
    }
    return result;
}

long long ParsePacket( std::string &workString, bool bOutput ) {

    nIndentCntr += 1;
    if (bOutput) {
        Indent();
        std::cout << "---------- Start packet parsing ----------" << std::endl;
    }

    long long result;
    int nVersion, nTypeID;
    ParseHeader( workString, nVersion, nTypeID, bOutput );

    if (nTypeID == 4) {     // literal packet
        result = ParseLiteralBody( workString, bOutput );   // what output must this yield???
    } else {                // operator packet
        result = ParseOperatorBody( workString, nTypeID, bOutput );   // what output must this yield???
    }

    if (bOutput) {
        Indent();
        std::cout << "---------- End   packet parsing ----------" << std::endl;
    }
    nIndentCntr -= 1;
    return result;
}

// =====================================

int main()
{
    glbProgPhase = PUZZLE;

    flcTimer tmr;
    tmr.StartTiming(); // ============================================vvvvv

    // get input data, depending on the glbProgPhase (example, test, puzzle)
    DataStream hexInputData;
    GetInput(  hexInputData, glbProgPhase != PUZZLE );
    std::cout << "Data stats - number of hex char's in input: " << hexInputData.size() << std::endl;

    tmr.TimeReport( "Data input: " );   // ====================^^^^^vvvvv

// ========== part 1

    DataStream binInputData;
    HexToBinString( hexInputData, binInputData );

    if (glbProgPhase != PUZZLE)
        PrintDataStream( binInputData );

    DataStream workData;
    long long lResult;

    // get a working copy of input data
    workData = binInputData;
    lResult = ParsePacket( workData, false );
    std::cout << std::endl << "Sum of all versions = " << nGlbVersionSum << std::endl;

    tmr.TimeReport( "Timing 1: " );   // ==============================^^^^^

// ========== part 2

    // get a fresh copy of input data
    workData = binInputData;
    lResult = ParsePacket( workData, false );
    std::cout << std::endl << "Result of evaluating packet expression = " << lResult << std::endl;

    tmr.TimeReport( "Timing 2: " );   // ==============================^^^^^

    return 0;
}


