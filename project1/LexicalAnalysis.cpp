#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector> 
#include <algorithm>


using namespace std ;
#define TABLENUM 8
#define TABLEINDEX 100
string table[TABLENUM][TABLEINDEX];
string place = "";

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

string findSingleToken( string token, int tableNumber ) { // 找出token位置(table1~4中的) 
	//cout << "AAAAAA"; 
	
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
		} // if
	} // for

	
	return token;
} // findToken()



int main(){
	fstream inFile, outFile ;
	string fileName;
	string line = "";
	char ch;
	string token, temp;
	
	
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
	//fileName = "SIC_input1.txt";
	inFile.open( (fileName + ".txt").c_str(), fstream::in );
	
	outFile.open( (fileName +"_output.txt").c_str(), fstream::out ) ;
	while( inFile.get(ch) ) {
		//cout << ch ;
		line = line + ch;
		if( ch == ' ' || ch == '\t' || ch == '\n' ) { //看到White space 
			if( temp != "" ) {
				token = temp;
				//cout << token << "\n";
				if ( isInt(token) ) {
					linearInsert( 6, token ); 
					findSingleToken( token, 6 );
				} // if
				else {
					if( !findToken( token ) ) {
						linearInsert( 5, token ); 
						findSingleToken( token, 5 );
					} // if
				} // else
			} // if
			
			if( ch == '\n' ) {
				cout << line ;
				outFile << line;
				cout << place << endl;
				outFile << place << endl;
				line = "";
				place = "";
			} // if
			
			while( inFile.get(ch) ) {
				//cout << ch ;
				line = line + ch;
				if( ch == '\n' ) {
					cout << line ;
					outFile << line;
					cout << place << endl;
					outFile << place << endl;
					line = "";
					place = "";
				} // if
				
				if( ch != ' ' && ch != '\t' && ch != '\n' ){
					break;
				} //if
			} // while
			
			temp = "";
		} // if
		//,+-* /:;?'.=#@
		if( ch == ',' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':' ||
			ch == '?' || ch == ';' || ch == '=' || ch == '#' || ch == '@' ) { //看到delimeter
			if( temp != "" ) {
				token = temp;
				//cout << token << "\n";
				if ( isInt(token) ) {
					linearInsert( 6, token ); 
					findSingleToken( token, 6 );
				} // if
				else {
					if( !findToken( token ) ) {
						linearInsert( 5, token ); 
						findSingleToken( token, 5 );
					} // if
				} // else
				temp = "";
			} //if
			else {
				temp = ch;
				token = temp;
				//cout << token << "\n";
				findToken( token );
				
				while( inFile.get(ch) ) {
					//cout << ch ;
					line = line + ch;
					if( ch != ' ' && ch != '\t' && ch != '\n' ){
						break;
					} //if
				} // while
				
				if( ch == '\n' ) {
					cout << line ;
					outFile << line;
					cout << place << endl;
					outFile << place << endl;
					line = "";
					place = "";
					ch = '\0';
				} // if
				temp = "";
				
				
			} // else
			
		} // if
		if( ch == '\'' ) {
			if( temp != "" ) {
				if( temp == "C" || temp == "c" ) {
					temp = ch;
					token = temp;
					//cout << token << "\n";
					findToken( token );
					temp = "";
					while( inFile.get(ch) ) { //放入 string table
						//cout << ch ;
						line = line + ch;
						if( ch != '\'' ){
							temp = temp + ch;
						} //if
						else{
							break;
						} // else
					} // while
					token = temp;
					//cout << token << "\n";
					linearInsert( 7, token ); 
					findSingleToken( token, 7 );
					findToken( token );
					temp = "";
					
					
				} // if
				if( temp == "X" || temp == "x" ) {
					temp = ch;
					token = temp;
					//cout << token << "\n";
					findToken( token );
					temp = "";
					while( inFile.get(ch) ) { //放入 string table
						//cout << ch ;
						line = line + ch;
						if( ch != '\'' ){
							temp = temp + ch;
						} //if
						else{
							break;
						} // else
					} // while
					token = temp;
					//cout << token << "\n";
					linearInsert( 6, token ); 
					findSingleToken( token, 6 );
					findToken( token );
					temp = "";
					
					
				} // if
				
			} //if
			
		} // if
		if( ch == '.' ) {
			if( temp != "" ) {
				token = temp;
				//cout << token << "\n";
				if ( isInt(token) ) {
					linearInsert( 6, token ); 
					findSingleToken( token, 6 );
				} // if
				else {
					if( !findToken( token ) ) {
						linearInsert( 5, token ); 
						findSingleToken( token, 5 );
					} // if
				} // else
			} //if
			
			token = ch ;
			//cout << token << "\n";
			findToken( token );
			inFile.get(ch);
			cout << ch;
			line = line + ch;
			while( ch != '\n' ){
				inFile.get(ch);
				//cout << ch ;
				line = line + ch;
			} // while
			if( ch == '\n' ) {
				cout << line ;
				outFile << line;
				cout << place << endl;
				outFile << place << endl;
				line = "";
				place = "";
			} // if
			ch = '\0';
			temp = "";
		} // if

		temp = temp + ch;
		
	} // while
	inFile.close();
	outFile.close();
	
} // main
