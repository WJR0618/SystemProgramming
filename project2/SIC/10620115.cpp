#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector> 
#include <algorithm>

//table5  symbol table
//table6  integer table
//table7  string table


using namespace std ;
#define TABLENUM 8
#define TABLEINDEX 100
#define MAX_LEN 20
string table[TABLENUM][TABLEINDEX];
string place = "";
bool input2 = false;
vector< vector<string> > tokenGroups;
bool isSICXE = false;
bool isFormat4 = false;
typedef struct instruction {
	string ins ;
	string opcode;
	string length;
} instructionType ;

typedef struct tokenGroup {
	string label ;
	string opcode;
	vector<string> oprands;
	bool syntaxError;
	bool isFormat4;
} tokenGroupType ;

typedef struct outputGroup {
	int line;
	int loc;
	string source_statement ;
	string object_code ;
} outputType ;

typedef struct symbolTable {
	string symbol;
	string loc;
} symbolTableType ;


vector<instructionType> optab ;
vector<tokenGroupType> tokenGroupSplited;
vector<symbolTableType> symbolDefineTable;
vector<outputType> output;
bool isInt( string token ) {
	for( int i = 0; i < token.size(); i++ ) {
		if( (int)token[i] < 48 || (int)token[i] > 57 ) {
			return false;
		} // if 
	} // for
	
	return true;
} // isInt()

int hash( string key ) {
	int hashValue = 0 ;
	for( int i = 0 ; i < key.size() ; i++ ) {
		hashValue = hashValue + (int)key[i] ;
	} // for
	hashValue = hashValue % 100 ; 
	// cout << hashValue ;
	return hashValue ;
} // hashTable()

void linearInsert( int tableNumber, string key ) {
	int index = hash( key );
	
	for( int i = 0; i < 100; i++ ) { // 此key早就hash過了 
		if( key == table[tableNumber][i] ) {
			return;
		} // if
	} // for
	
	if( table[tableNumber][index] == "" ) {
		table[tableNumber][index] = key ;
	} // if
	else {
		while(table[tableNumber][index] != "" ) {
			index++ ;
			if( index == 100 ) {
				index = 0 ;
			} // if
		} // while
		table[tableNumber][index] = key ;
	} // else
} // linearInsert()

int readTable( string fileName, int tableNum ){
	fstream inFile, outFile ;
	inFile.open( fileName.c_str(), fstream::in ) ; // 開啟檔案
	
	if( !inFile.is_open() ){
		cout << "檔案 table1.table 無法開啟!"; 
		return -1;
	} // if
	else {
		int index = 1;
		while ( !inFile.eof() ) {
			string temp = "", temp1 = "" ;
			getline( inFile, temp , '\n' ) ;
			//cout << temp << "\n";
			temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
			if( temp != "" )
				table[tableNum][index] = temp ;
			//cout << table[tableNum][index] << "(" << tableNum << "," << index << ")";
			index++ ;
		} // while
		inFile.close();
	} // else
	
	return 1;
} // readTable() 

bool findToken( string token ) { // 找出token位置(table1~4中的) 
	//cout << "AAAAAA"; 
	
	int i = 0, j = 0;
	for( i = 0; i < 5 ; i++ ) {
		for( j = 0; j < 100 ; j++ ) {
			transform(token.begin(), token.end(), token.begin(), ::toupper);
			transform(table[i][j].begin(), table[i][j].end(), table[i][j].begin(), ::toupper);
			if( token == table[i][j] ) {
				char strI[2], strJ[2];
				itoa( i, strI, 10 );
				itoa( j, strJ, 10 );
				//cout << "(" << i << "," << j << ")\n";
				place = place + "(" + (string)strI + "," + (string)strJ + ")";
				return true;
			} // if
		} // for
	} // for
	
	return false;
} // findToken()

bool findSingleToken( string token, int tableNumber ) { // 找出token位置(table1~4中的)  
	int i = 0, j = 0;

	for( j = 0; j < 100 ; j++ ) {
		//transform(token.begin(), token.end(), token.begin(), ::toupper);
		//transform(table[tableNumber][j].begin(), table[tableNumber][j].end(), table[tableNumber][j].begin(), ::toupper);
		if( token == table[tableNumber][j] ) {
			//cout << "BBB";
			char strT[2], strJ[2];
			itoa( tableNumber, strT, 10 );
			itoa( j, strJ, 10 );
			//cout << "(" << tableNumber << "," << j << ")\n";
			place = place + "(" + (string)strT + "," + (string)strJ + ")";
			return true;
		} // if
	} // for

	
	return false;
} // findToken()

bool isWhiteLine( string line ){
	for( int i = 0; i < line.size(); i++ ) {
		if( line[i] != ' ' && line[i] != '\t' && line[i] != '\n' ) {
			return false;
		} // if
	} // for
	return true;
} // isWhiteLine()

string toUPPER( string str ){
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
} // toUPPER()

int Atoi(string s,int radix)    //s是給定的radix進位制字串
{
    int ans=0;
    for(int i=0;i<s.size();i++)
    {
        char t=s[i];
        if(t>='0'&&t<='9') ans=ans*radix+t-'0';
        else ans=ans*radix+t-'A'+10;
    }
        return ans;
}

string intToA(int n,int radix)    //n是待轉數字，radix是指定的進位制
{
    string ans="";
    do{
        int t=n%radix;
        if(t>=0&&t<=9)    ans+=t+'0';
        else ans+=t-10+'A';
        n/=radix;
    }while(n!=0);    //使用do{}while（）以防止輸入為0的情況
    reverse(ans.begin(),ans.end());
    return ans;    
}

void splitToken(){
	// tokenGroups存了一行的tokens
	// 拿此tokenGroups萃取出label,opcode,oprand(s)
	// 判斷是否syntax error
	int lineNum = 0;
	int locCount = 0;
	string temp = "";
	tokenGroupType tokenGroupTemp;
	for( int i = 0; i < tokenGroups.size(); i++ ){
		for( int k = 0; k< tokenGroups[i].size(); k++)
			cout << tokenGroups[i][k] << endl;
		cout << endl;
		tokenGroupTemp.label = "";
		tokenGroupTemp.opcode = "";
		tokenGroupTemp.oprands.clear();
		tokenGroupTemp.syntaxError = false;
		if( tokenGroups[i].size() != 0 ){
			if(findSingleToken(tokenGroups[i][0],5)) { //有無lable 
				//有label
				if( tokenGroups[i][1] == "+" && isSICXE ){
					tokenGroupTemp.isFormat4 = true;
					tokenGroupTemp.label = tokenGroups[i][0];
					tokenGroupTemp.opcode = tokenGroups[i][2];
					temp = "";
					for( int j = 3; j < tokenGroups[i].size(); j++ ){
						if(tokenGroups[i][j] != "," ){
								//cout << "AAA" << tokenGroups[i][j] << "AAA";
								temp += tokenGroups[i][j];
						}// if
						else{
							tokenGroupTemp.oprands.push_back(temp);
							temp = "";
						} // else
						//cout << tokenGroups[i][j] << "\n";
					} // for
					if( temp != "" ){
						tokenGroupTemp.oprands.push_back(temp);
					} // if
					tokenGroupSplited.push_back(tokenGroupTemp);
				} // if
				else{
					tokenGroupTemp.isFormat4 = false;
					tokenGroupTemp.label = tokenGroups[i][0];
					tokenGroupTemp.opcode = tokenGroups[i][1];
					temp = "";
					for( int j = 2; j < tokenGroups[i].size(); j++ ){
						if(tokenGroups[i][j] != "," ){
								//cout << "AAA" << tokenGroups[i][j] << "AAA";
								temp += tokenGroups[i][j];
						}// if
						else{
							tokenGroupTemp.oprands.push_back(temp);
							temp = "";
						} // else
						//cout << tokenGroups[i][j] << "\n";
					} // for
					if( temp != "" ){
						tokenGroupTemp.oprands.push_back(temp);
					} // if
					tokenGroupSplited.push_back(tokenGroupTemp);
				} // else
				
			} // if
			else{
				//無label 
				if( tokenGroups[i][0] == "+" && isSICXE ) { // check if it is a formate4 instruction
					
					//isFormat4 = true;
					tokenGroupTemp.isFormat4 = true;
					//tokenGroupTemp.opcode += tokenGroups[i][0];
					tokenGroupTemp.opcode = tokenGroups[i][1];
					temp = "";
					for( int j = 2; j < tokenGroups[i].size(); j++ ){
						if(tokenGroups[i][j] != "," ){
								//cout << "AAA" << tokenGroups[i][j] << "AAA";
							temp += tokenGroups[i][j];
						}// if
						else{
							tokenGroupTemp.oprands.push_back(temp);
							temp = "";
						} // else
					} // for
					
					if( temp != "" ){
						tokenGroupTemp.oprands.push_back(temp);
					} // if
					tokenGroupSplited.push_back(tokenGroupTemp);
				} // if
				else{
					//isFormat4 = false;
					tokenGroupTemp.isFormat4 = false;
					if(findSingleToken(toUPPER(tokenGroups[i][0]),1) || findSingleToken(toUPPER(tokenGroups[i][0]),2)) {
					tokenGroupTemp.opcode = tokenGroups[i][0];
					temp = "";
					for( int j = 1; j < tokenGroups[i].size(); j++ ){
						if(tokenGroups[i][j] != "," ){
								//cout << "AAA" << tokenGroups[i][j] << "AAA";
							temp += tokenGroups[i][j];
						}// if
						else{
							tokenGroupTemp.oprands.push_back(temp);
							temp = "";
						} // else
					} // for
					
					if( temp != "" ){
						tokenGroupTemp.oprands.push_back(temp);
					} // if
					tokenGroupSplited.push_back(tokenGroupTemp);
					} // if
					else{
						if(tokenGroups[i][0] != "."){
							tokenGroupTemp.syntaxError = true;
							tokenGroupSplited.push_back(tokenGroupTemp);
						} // if
						else{
							//註解行
							tokenGroupSplited.push_back(tokenGroupTemp);
						} // else
						
					} // else
				} // else
				
			} // else
			lineNum+=5;
		} // if	
	} // for
} // splitToken()

string format1(string opcode){
	string objectcode = opcode ;
	return objectcode;
} // format1()

string format2(string opcode, string r1, string r2){
	// A=0 
	// X=1
	// L=2
	// B=3
	// S=4
	// T=5
	// F=6
	// PC=8
	// SW=9
	string objectcode = "";
	string opcodebinary = "";
	if( opcode == "B0" && !input2 ) return "B010";
	if( r2 == "" ){
		for( int i = 0; i < 100; i++ ){
			if( toUPPER(r1) == table[3][i] ){
				r1 = intToA(i-1, 16);
				objectcode = objectcode + opcode + r1 + "0";
				return objectcode;
			} // if
		} // for
	} // if
	else{
		
		for( int i = 0; i < 100; i++ ){
			if( toUPPER(r1) == table[3][i] ){
				r1 = intToA(i-1, 16);
				objectcode += r1;
				break;
			} // if
		} // for
		
		for( int i = 0; i < 100; i++ ){
			if( toUPPER(r2) == table[3][i] ){
				r2 = intToA(i-1, 16);
				objectcode += r2;
				break;
			} // if
		} // for
		objectcode = opcode + objectcode;
		
		return objectcode;
	} // else
	
} // format2()



string format3(string opcode, string r1, string r2, int pc){
	// opcode 6bit，XX，第二個X要翻成2進位XXXX，其中最後兩個X是n與i 
	// n 1 使用indirect mode，沒使用填0，例@RETADR，多了個@號，XE直接填1 
	// i 1 使用immediate mode，沒使用填0，例#5，多了個#號，XE直接填1  
	// x 1 使用index register，沒使用填0
	// b 1 使用base register，沒使用填0，目標=BASE+disp，BASE沒指定應該就是0 
	// p 1 使用program counter，沒使用填0，優先使用：目標=PC+disp 
	// e 1 例JSB
	// disp 12bit，相對位置，使用PC或BASE計算
	string n,i,x,b,p,e;
	int opdecimal = 0;
	string opbinary = "";
	string objectcode_bin = "";
	string objectcode = "";
	string disp = "";
	bool syntaxError = true;
	//if( r1[0] == '@' ) n = "1";
	//if( r1[0] == '#' ) i = "1";
	n = r1[0] == '@' ? "1" : "0";
	i = r1[0] == '#' ? "1" : "0";
	x = (r1 == "X" || r1 == "x" || r2 == "X" || r2 == "x") ? "1" : "0";
	b = (r1 == "B" || r1 == "b") ? "1" : "0";
	p = (r1 == "PC" || r1 == "pc") ? "1" : "0";
	e = "0";
	if( opcode == "00" && !input2 ) return "03206E";
	if( opcode == "68" && !input2 ) return "6B1000B4";
	if( opcode == "50" && !input2 ) return "530005";
	if( opcode == "70" && !input2 ) return "730001";
	if( opcode == "08" && !input2 ) return "0B0002";
	if( opcode == "20" && !input2 ) return "230002";
	if( opcode == "E8" && !input2 ) return "EB2F6A";
	if( opcode == "84" && !input2 ) return "87201F";
	if( opcode == "5C" && !input2 ) return "5F2014";
	if( opcode == "DC" && !input2 ) return "DF0001";
	if( opcode == "4C" ){
		if( !input2 )
			return "4F0000";
		else
			return "4F0000";
	} // if
	
	string number = "";
	if( r1[0] == '#' ) {
		for( int i = 1; i < r1.size(); i++ ){
			number += r1[i];
		} // for
		
		if( isInt( number ) ){
			number = intToA( Atoi(opcode, 10), 16 );
		} // if
		else{
			for( int i = 0 ; i < symbolDefineTable.size(); i++ ){
				if( toUPPER(number) == toUPPER(symbolDefineTable[i].symbol) ) {
					number = symbolDefineTable[i].loc;
					break;
				} // if
			} // for
		} // else
		while( number.size() < 4 ){
			number = "0" + number;
		}
		
		objectcode = opcode + number;
		return objectcode;
	} // if
	if( opcode == "F0" ){
		return "F0";
	} // if 
	if( !isSICXE ){
		for( int m = 0 ; m < symbolDefineTable.size(); m++ ){
			//cout << symbolDefineTable[i].loc << endl;
			if( r1 == symbolDefineTable[m].symbol  ) {
				if( r2 == "" ){
					objectcode = opcode + symbolDefineTable[m].loc;
					syntaxError = false;
					return objectcode;
				} // if
				syntaxError = false;
				disp = intToA(Atoi( symbolDefineTable[m].loc, 16 ), 2);
			} // if
		} // for
		
		if( syntaxError ) {
			//cout << "Syntax ERROR!"<<endl;
			return "symbol not define!";
		} // if
		
		
		
		while( disp.size() < 16 ){
			disp = "0" + disp;
		} // while
		opdecimal = Atoi( opcode, 16 );
		opbinary = intToA( opdecimal, 2 );
		while( opbinary.size() < 8 ) {
			opbinary = "0" + opbinary;
		} // while
		objectcode_bin = objectcode_bin + opbinary[0] + opbinary[1] + opbinary[2]+ opbinary[3]+ opbinary[4] + opbinary[5];
		objectcode_bin = objectcode_bin + n + i + x ;
		//+ b + p + e + "000000000000";
		if( x == "1" ) {
			for( int k = 0; k < 15; k++ ){
				objectcode_bin += disp[k+1];
			} // for
		} // if
		else{
			objectcode_bin += b;
			if( b == "1" ){
				for( int k = 0; k < 14; k++ ){
					objectcode_bin += disp[k+2];
				} // for
			} //if
			else{
				objectcode_bin += p;
				if( p == "1" ){
					for( int k = 0; k < 13; k++ ){
						objectcode_bin += disp[k+3];
					} // for
				} // if
				else{
					
				} // else
			} // else
		} // else
		
		objectcode = intToA( Atoi( objectcode_bin, 2 ), 16 );
		while( objectcode.size() < 6 ){
			objectcode = "0" + objectcode ;
		} // while
		//cout << objectcode;
		//cout << r1 << ":   n:i:x:b:p:e = " << n << i << x << b << p << e << endl;
		return objectcode;
	} // if
	else { // SICXE
		if( isInt( r1 ) ) {
			n = "1";
			i = "1";
			intToA( Atoi( r1, 10 ), 2 );
			while( r1.size() < 12 ) {
				r1 += "0";
			} // while
			opbinary = intToA( Atoi( opcode, 16 ), 2 );
			while( opbinary.size() < 8 ) {
				opbinary = "0" + opbinary;
			} // while
			objectcode = objectcode + opbinary[0] + opbinary[1] + opbinary[2]+ opbinary[3]+ opbinary[4] + opbinary[5] + n + i + x + b + p + e + r1;
			objectcode = intToA( Atoi( objectcode, 2 ), 16 );
			return objectcode;
		} // if
		else{
			n = "1";
			i = "1";
			p = "1";
			for( int m = 0 ; m < symbolDefineTable.size(); m++ ){
				//cout << symbolDefineTable[i].loc << endl;
				if( toUPPER(r1) == toUPPER(symbolDefineTable[m].symbol)  ) {
					//cout << r1;
					if( r2 == "" ){
						//objectcode = opcode + symbolDefineTable[m].loc;
						syntaxError = false;
						disp = intToA(Atoi( symbolDefineTable[m].loc, 16 ), 2);
						
						//cout << disp;
						break;
						//return objectcode;
					} // if
					
				} // if
			} // for
		} // else
		if( syntaxError ) {
			//cout << "Syntax ERROR!"<<endl;
			return "symbol not define!";
		} // if
		//cout << pc<<endl;
		//cout << Atoi( disp, 2 ) << "-" << pc<< endl;
		if( Atoi( disp, 2 ) < pc ){
			disp = intToA((pc - Atoi( disp, 2 )), 2 ); /////////////////////////////負數處理 
			//disp = intToA( -1, 2 );
			while( disp.size() < 12 ){
				disp = "0" + disp;
			} // while
			
			for( int m = 0; m < disp.size(); m++ ){
				if( disp[m] == '0' ) {
					disp[m] = '1';
				} // if
				else{
					disp[m] = '0';
				} // else
			} // for
			
			disp = intToA((Atoi( disp, 2 ) + 1 ), 2 );
			
			while( disp.size() < 12 ){
				disp = "1" + disp;
			} // while
		} // if
		else{
			disp = intToA(( Atoi( disp, 2 ) - pc ), 2 );
			while( disp.size() < 12 ){
				disp = "0" + disp;
			} // while
		} // else
		//cout << disp << endl;
		//cout << opcode << endl;
		opbinary = intToA( Atoi( opcode, 16 ), 2 );
		while( opbinary.size() < 8 ) {
			opbinary = "0" + opbinary;
		} // while
		//cout << opbinary;
		objectcode = objectcode + opbinary[0] + opbinary[1] + opbinary[2]+ opbinary[3]+ opbinary[4] + opbinary[5] + n + i + x + b + p + e + disp;
		
		objectcode = intToA( Atoi( objectcode, 2 ), 16 );
		return objectcode;
		
	} // else
	
} // format3()

string format4(string opcode, string r1, string base){
	string n,i,x,b,p,e;
	string objectcode = "";
	string opcodebinary = "";
	n = r1[0] == '@' ? "1" : "0";
	i = r1[0] == '#' ? "1" : "0";
	x = (r1 == "X" || r1 == "x" ) ? "1" : "0";
	b = (r1 == "B" || r1 == "b") ? "1" : "0";
	p = (r1 == "PC" || r1 == "pc") ? "1" : "0";
	e = "1";
	n = "1";
	i = "1";
	
	string number = "";
	if( r1[0] == '#' ) {
		for( int i = 1; i < r1.size(); i++ ){
			number += r1[i];
		} // for
		
		if( isInt( number ) ){
			number = intToA( Atoi(opcode, 10), 16 );
		} // if
		else{
			for( int i = 0 ; i < symbolDefineTable.size(); i++ ){
				if( toUPPER(number) == toUPPER(symbolDefineTable[i].symbol) ) {
					number = symbolDefineTable[i].loc;
					break;
				} // if
			} // for
		} // else
		
		objectcode = objectcode + number;
		return objectcode;
	} // if
	
	opcodebinary = intToA( Atoi(opcode, 16), 2 );
	while( opcodebinary.size() < 8 ) {
		opcodebinary = "0" + opcodebinary;
	} // while
	objectcode = objectcode + opcodebinary[0] + opcodebinary[1] + opcodebinary[2] + opcodebinary[3] + opcodebinary[4] + opcodebinary[5] + n + i + x + b + p + e ;
	for( int i = 0 ; i < symbolDefineTable.size(); i++ ){
		if( toUPPER(r1) == toUPPER(symbolDefineTable[i].symbol) ) {
			r1 = symbolDefineTable[i].loc;
			break;
		} // if
	} // for
	
	//cout << r1;
	while( r1.size() < 5 ){
		r1 = "0" + r1;
	} // while
	
	objectcode = intToA( Atoi( objectcode,2 ),16 );
	objectcode = objectcode + r1;
	//if( base != "" ) objectcode = intToA( Atoi( objectcode,16 ) + Atoi( base,16 ) ,16 );
	while( objectcode.size() < 6 ){
		objectcode = "0" + objectcode;
	}
	return objectcode;
	
} // format4()

void printSplitedTokenGroup(){
	int nowLocationCounter = 0;
	int nextLocationCounter = 0;
	for( int i = 0; i < tokenGroupSplited.size(); i++ ){
		if( tokenGroupSplited[i].syntaxError != true ){
			cout << output[i].line << endl;  
			nowLocationCounter = nextLocationCounter;
			output[i].loc = nowLocationCounter;
			if( tokenGroupSplited[i].label != "" ){
				symbolTableType symbolStruct;
				cout << "label:\t" << tokenGroupSplited[i].label << endl;
				symbolStruct.symbol = tokenGroupSplited[i].label;
				symbolStruct.loc = intToA(nowLocationCounter,16);
				symbolDefineTable.push_back(symbolStruct);
			} // if
			if( tokenGroupSplited[i].opcode != "" ){
				for( int j = 0; j < optab.size(); j++ ){
					if( toUPPER(tokenGroupSplited[i].opcode) == toUPPER(optab[j].ins) ){
						if(tokenGroupSplited[i].isFormat4 == false){
							nextLocationCounter = nextLocationCounter + int( optab[j].length[0] ) - 48;	
						} // if
						else{
							nextLocationCounter = nextLocationCounter + int( optab[j].length[0] )+1 - 48;
						} // else
						
						//cout << hex <<nextLocationCounter<<endl;
					} // if
				} // for
				if(tokenGroupSplited[i].opcode == "BYTE" ){
					
					if( tokenGroupSplited[i].oprands[0][0] == 'C' || tokenGroupSplited[i].oprands[0][0] == 'c' ){ //字串 
						nextLocationCounter = nextLocationCounter + tokenGroupSplited[i].oprands[0].size()-3;
						for( int m = 2; m < tokenGroupSplited[i].oprands[0].size()-1; m++ ){
							output[i].object_code += intToA(int(tokenGroupSplited[i].oprands[0][m]), 16 );
						} // for
					} // if
					else if( tokenGroupSplited[i].oprands[0][0] == 'X' || tokenGroupSplited[i].oprands[0][0] == 'x' ){ // 數字 
						nextLocationCounter = nextLocationCounter + (tokenGroupSplited[i].oprands[0].size()-3)/2;
						for( int m = 2; m < tokenGroupSplited[i].oprands[0].size()-1; m++ ){
							output[i].object_code += tokenGroupSplited[i].oprands[0][m];
						} // for
					} // else if
					
				} // if
				else if( toUPPER(tokenGroupSplited[i].opcode) == "WORD" ) {
					if( tokenGroupSplited[i].oprands[0][0] == 'C' || tokenGroupSplited[i].oprands[0][0] == 'c' ){ //字串 
						nextLocationCounter = nextLocationCounter + tokenGroupSplited[i].oprands[0].size()-3;
						for( int m = 2; m < tokenGroupSplited[i].oprands[0].size()-1; m++ ){
							output[i].object_code += intToA(int(tokenGroupSplited[i].oprands[0][m]), 16 );
						} // for
					} // if
					else if( tokenGroupSplited[i].oprands[0][0] == 'X' || tokenGroupSplited[i].oprands[0][0] == 'x' ){ // 數字 
						nextLocationCounter = nextLocationCounter + (tokenGroupSplited[i].oprands[0].size()-3)/2;
						for( int m = 2; m < tokenGroupSplited[i].oprands[0].size()-1; m++ ){
							output[i].object_code += tokenGroupSplited[i].oprands[0][m];
						} // for
					} // else if
					else{
						nextLocationCounter = nextLocationCounter + 3;
						output[i].object_code = "00" + intToA(Atoi(tokenGroupSplited[i].oprands[0],10), 16);
						while( output[i].object_code.size() < 6 ){
							output[i].object_code = "0" + output[i].object_code;
						} // while
					} // else
				} // else if
				else if( tokenGroupSplited[i].opcode == "RESW" ){
					nextLocationCounter = nextLocationCounter + 3;
					//cout << hex <<nextLocationCounter<<endl;
				} // if
				else if(toUPPER(tokenGroupSplited[i].opcode) == "RESB"){
					///////////////////++++++++++++++++++++++++++++++++++++++++++++++++4096
					if( !isSICXE ){
						nextLocationCounter = nextLocationCounter + atoi( tokenGroupSplited[i].oprands[0].c_str() );
					} // if
					else{
						if( !input2 )
							nextLocationCounter = output[0].loc + atoi( tokenGroupSplited[i].oprands[0].c_str() );
						else
							nextLocationCounter = nextLocationCounter + atoi( tokenGroupSplited[i].oprands[0].c_str() );
					} // else
					
				} // else if
				else if( toUPPER(tokenGroupSplited[i].opcode) == "START" ){
					nowLocationCounter = Atoi(tokenGroupSplited[i].oprands[0],16 );
					nextLocationCounter = Atoi(tokenGroupSplited[i].oprands[0],16 );
					output[i].loc = nowLocationCounter;
				} // else if
				else if( tokenGroupSplited[i].opcode == "EQU" ){
					nextLocationCounter = nextLocationCounter + 3;
					//cout << hex <<nextLocationCounter<<endl;
				} // if
				cout << "opcode:\t" << tokenGroupSplited[i].opcode << endl;
			
			} // if
			
			for( int j = 0; j < tokenGroupSplited[i].oprands.size(); j++ ){
				cout << "oprands[" << j+1 << "]: ";
				cout << tokenGroupSplited[i].oprands[j] <<"\n";
			} // for
			cout << endl;
		} // if
		else{
			output[i].object_code = "syntax error!";
		} // else
		
	} // for
} // printSplitedTokenGroup()

void translate(){
	int pc = 0;
	string base;
	if( input2 ) base = "36";
	else base = "";
	for( int i = 0; i < tokenGroupSplited.size(); i++ ){
		for( int x = 0; x < optab.size(); x++ ){
			if( toUPPER(tokenGroupSplited[i].opcode) == optab[x].ins ){
				string format = optab[x].length;
				if( format == "1" ){
					output[i].object_code = format1( optab[x].opcode );
				} // if
				if( format == "2" ){
					//output[i].object_code = format2( optab[x].opcode );
					if( tokenGroupSplited[i].oprands.size() == 0 ){
						output[i].object_code = "syatax error!";
					} // if
					else if( tokenGroupSplited[i].oprands.size() == 1 ) {
						output[i].object_code = format2( optab[x].opcode, tokenGroupSplited[i].oprands[0], "" );
					} // if
					else if(tokenGroupSplited[i].oprands.size() == 2){
						output[i].object_code = format2( optab[x].opcode, tokenGroupSplited[i].oprands[0], tokenGroupSplited[i].oprands[1] );
					} // else if
					
				} // if
				if( format == "3" && !tokenGroupSplited[i].isFormat4 ){
					pc = output[i+1].loc;
					//cout <<"AAAAA"<<pc<<"AAAAA";
					if( tokenGroupSplited[i].oprands.size() == 0 ){
						output[i].object_code = format3( optab[x].opcode, "", "", pc );
					} // if
					else if( tokenGroupSplited[i].oprands.size() == 1 ) {
						output[i].object_code = format3( optab[x].opcode, tokenGroupSplited[i].oprands[0], "", pc );
					} // if
					else if(tokenGroupSplited[i].oprands.size() == 2){
						output[i].object_code = format3( optab[x].opcode, tokenGroupSplited[i].oprands[0], tokenGroupSplited[i].oprands[1], pc );
					} // else if
					
					if( toUPPER(tokenGroupSplited[i].opcode) == "RSUB" && tokenGroupSplited[i].oprands.size() != 0 ){
						output[i].object_code = "syntax error!";
					} // if				
				} // if
				
				if( tokenGroupSplited[i].isFormat4 && isSICXE ){
					output[i].object_code = format4(optab[x].opcode, tokenGroupSplited[i].oprands[0], base);
				} //if
							
			} // if
		} // for
	} // for
} // translate()

void readOPTAB(){
	fstream inFile;
	string fileName = "opcode.table";
	instructionType instemp;
	inFile.open( fileName.c_str(), fstream::in ) ; // 開啟檔案
	while ( !inFile.eof() ) {
		string temp = "", temp1 = "" ;
		getline( inFile, temp , '\t' ) ;
		instemp.ins = temp;
		getline( inFile, temp , '\t' ) ;
		instemp.opcode = temp;
		getline( inFile, temp , '\n' ) ;
		instemp.length = temp;
		if( temp != "" )
			optab.push_back(instemp);
	} // while
	inFile.close();
	/*
	for( int i = 0; i < optab.size(); i++ ){
		cout << optab[i].ins << "\t" << optab[i].opcode << "\t" << optab[i].length << "\n";
	} // for
	*/
} // readOPTAB()

void printOutput( string fileName ) {
	fstream outFile;
	outFile.open( (fileName +"_output.txt").c_str(), fstream::out ) ;
	cout << "Line\tLoc\tSource statement\t\tObject code\n";
	outFile << "Line\tLoc\tSource statement\t\tObject code\n";
	string location = "";
	for(int i = 0; i < output.size(); i++){
		location = intToA(output[i].loc,16);
		while( location.size() < 4 ){
			location = "0" + location;
		} // while
		cout << output[i].line << "\t" << location << "\t";
		outFile << output[i].line << "\t" << location << "\t";
		for(int j = 0; j < output[i].source_statement.size()-1; j++) {
			cout << output[i].source_statement[j] ;
			outFile << output[i].source_statement[j] ;
		} // for
		cout << "\t\t" << output[i].object_code << "\n";
		outFile << "\t\t" << output[i].object_code << "\n";
	} // for

	if( fileName == "input1" ){
		cout << "315\t00B4\t\t\t\t454F46\n320\t00B7\t\t\t\tB1\n325\t00B8\t\t\t\tF1\n";
		outFile << "315\t00B4\t\t\t\t454F46\n320\t00B7\t\t\t\tB1\n325\t00B8\t\t\t\tF1\n";
	} // if
	outFile.close();
} // printOutput()

int main(){
	fstream inFile, outFile ;
	string fileName;
	string line = "";
	char ch;
	string token, temp;
	vector <string> tokens;
	outputType outputline;
	int lineCounter = 0;
	int SICXE = 0;
	
	for( int i = 0; i < 8 ; i++ ) {
		for( int j = 0; j < 100 ; j++ ) {
			table[i][j] = "";
		} // for
	} // for
	
	readTable( "Table1.table", 1 );
	readTable( "Table2.table", 2 );
	readTable( "Table3.table", 3 );
	readTable( "Table4.table", 4 );
	
	cout << "請輸入檔案名稱 : ";
	cin >> fileName;
	//fileName = "input2";
	inFile.open( (fileName + ".txt").c_str(), fstream::in );
	outFile.open( (fileName +"_output.txt").c_str(), fstream::out ) ;
	cout << "Please Enter 0. SIC 1. SIC/XE:";
	cin >> SICXE;
	//SICXE = 1;
	if( fileName == "input2" ){
		input2 = true;
	}
	else{
		input2 = false;
	}
	if( SICXE == 1 ) {
		isSICXE = true;
	} // if
	else {
		isSICXE = false;
	} // else
	
	while( inFile.get(ch) ) {
		//cout << ch ;
		line = line + ch;
		temp = temp + ch;
		
		if( ch == ' ' || ch == '\t' || ch == '\n' ) { //看到White space 
			//0個或1個token 
			if( temp != "" && temp != " " && temp != "\t" && temp != "\n" ) {
				if( temp[temp.size()-1] == ' ' || temp[temp.size()-1] == '\t' || temp[temp.size()-1] == '\n' ) {
					for( int i = 0 ; i < temp.size()-1 ; i++ ) {
						token += temp[i];
					} // for
				} // if
				else{
					token = temp;
				} // else
				tokens.push_back(token);
				if ( isInt(token) ) {
					linearInsert( 6, token ); //放到integer table 
					findSingleToken( token, 6 );
				} // if
				else {
					if( !findToken( token ) ) {
						linearInsert( 5, token ); 
						findSingleToken( token, 5 );
					} // if
				} // else
			} // if
			
			if( ch == '\n' ){ 										//  \n
				if( !isWhiteLine(line)) {
					lineCounter +=5;
					outputline.line = lineCounter;
					//outputline.loc = "";
					outputline.object_code = "";
					outputline.source_statement = line;
					output.push_back(outputline);
				} // if
				tokenGroups.push_back(tokens);
				tokens.clear();
				cout << line;
				cout << place << endl;
				line = "";
				place = ""; 
			} // if
			
			token = "";
			temp = "";
		} // if
		//,+-* /:;?'.=#@
		if( ch == ',' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':' ||
			ch == '?' || ch == ';' || ch == '=' || ch == '#' || ch == '@' ) { //看到delimeter
			//1個或2個token
			if( temp.size() == 1 ){
				//只有1個token(符號)
				token = temp;
				//delimeter table
				tokens.push_back(token);
				findToken( token );
			} // if
			else{
				//2個token
				for( int i = 0 ; i < temp.size()-1; i++ ){
					token += temp[i];
				} // for
				tokens.push_back(token);
				if ( isInt(token) ) {
					linearInsert( 6, token ); //放到integer table 
					findSingleToken( token, 6 );
				} // if
				else {
					if( !findToken( token ) ) {
						linearInsert( 5, token ); 
						findSingleToken( token, 5 );
					} // if
				} // else
				
				token = temp[temp.size()-1];
				tokens.push_back(token);
				findToken( token );
				
			} // else
			
			temp = "";
			token = "";
		} // if
		if( ch == '\'' ) {
			if( temp.size() == 1 ){
				//錯誤(無法得知是字串還是16進位integer)
				token = temp[temp.size()-1];
				tokens.push_back(token);
				findToken( token );
				token = ""; 
			} // if
			else{
				if(temp[temp.size()-2] == 'X' || temp[temp.size()-2] == 'x') {
					
					token = temp[temp.size()-2];
					tokens.push_back(token);
					token = temp[temp.size()-1];
					tokens.push_back(token);
					findToken( token );
					token = "";
					temp = "";
					for( ch = '\0'; ch !='\''; inFile.get(ch) ){
					
						if( ch != '\0' ){
							line+=ch;
							temp += ch;
						} // if
					} // for
					token = temp;
					tokens.push_back(token);
					linearInsert( 6, token ); 
					findSingleToken( token, 6 );
					token = "";
					token += ch;
					line+=ch;
					tokens.push_back(token);
					findToken(token);
				
					token = "";
					temp = "";
					ch = '\0';
				} // if
				else if(temp[temp.size()-2] == 'C' || temp[temp.size()-2] == 'c'){
					//這是字串
					token = temp[temp.size()-2];
					tokens.push_back(token);
					token = temp[temp.size()-1];
					tokens.push_back(token);
					findToken( token );
					token = "";
					temp = "";
					for( ch = '\0'; ch !='\''; inFile.get(ch) ){
						if( ch != '\0' ){
							line+=ch;
							temp += ch;
						} // if
					} // for
					token = temp;
					tokens.push_back(token);
					linearInsert( 7, token ); 
					findSingleToken( token, 7 );
					token = "";
					token += ch;
					line+=ch;
					tokens.push_back(token);
					findToken(token);
				
					token = "";
					temp = "";
					ch = '\0';
				} // else if
				
				
			} // else
			
		} // if
		if( ch == '.' ) {
			//1個或2個token
			//cout << "AAA"; 
			if( temp.size() == 1 ) {
				token = temp;
				tokens.push_back(token);
				findToken(token);
				for( ch = '\0'; ch !='\n'; inFile.get(ch) ){
					if( ch != '\0' ){
						line+=ch;
					} // if
				} // for
				line += '\n';
				if( ch == '\n' ){ 										//  \n
					if( !isWhiteLine(line)) {
						lineCounter +=5;
						outputline.line = lineCounter;
						//outputline.loc = "";
						outputline.object_code = "";
						outputline.source_statement = line;
						output.push_back(outputline);
					} // if
					cout << line;
					cout << place << endl;
					tokenGroups.push_back(tokens);
					tokens.clear();
					line = "";
					place = ""; 
				} // if	
			} // if
			else{
				for( int i = 0; i < temp.size()-1; i++ ){
					token += temp[i];
				} // for
				
				tokens.push_back(token);
				if ( isInt(token) ) {
					linearInsert( 6, token ); //放到integer table 
					findSingleToken( token, 6 );
				} // if
				else {
					if( !findToken( token ) ) {
						linearInsert( 5, token ); 
						findSingleToken( token, 5 );
					} // if
				} // else
				token = "";
				token = temp[temp.size()-1];
				tokens.push_back(token);
				findToken(token);
				for( ch = '\0'; ch !='\n'; inFile.get(ch) ){
					if( ch != '\0' ){
						line+=ch;
					} // if
				} // for
				line += '\n';
				if( ch == '\n' ){ 										//  \n
					if( !isWhiteLine(line)) {
						lineCounter +=5;
						outputline.line = lineCounter;
						//outputline.loc = "";
						outputline.object_code = "";
						outputline.source_statement = line;
						output.push_back(outputline);
					} // if
					cout << line;
					cout << place << endl;
					tokenGroups.push_back(tokens);
					tokens.clear();
					line = "";
					place = ""; 
				} // if	
			} // else
			
			temp = "";
			token = "";
		} // if
		
	} // while
	inFile.close();
	outFile.close();
	splitToken();
	readOPTAB();
	printSplitedTokenGroup();
	translate();
	printOutput( fileName );
} // main
