#define _CRT_SECURE_NO_WARNINGS
#include "scanadt.h"
#include "genlib.h"
#include "strlib.h"
#include "exception.h"
#include "env.h"
#include "symtab.h"
#include "parser.h"
#include "eval.h"

/*
void loadFunction(scannerADT scanner, symtabADT table){
	FILE * infile;
	string fileName, key = NULL;
	int value = 0;

	fileName = GetLine();
	infile = fopen(fileName, "r");

	if (infile == NULL){
		Error("Can't open file.");
	}
	else {
		while (fscanf(infile, "%s %d", key, parserShit()) != EOF) { //key är oinstansierad.
			Enter(table, key, parserShit());
		}
	}
	fclose(fileName);
}
void defineFunction(scannerADT scanner, symtabADT table){
	string token;
	int value = 0;
	token = ReadToken(scanner);
	//somefunction that does the rest of this shit.

	Enter(table, token, value);
}
*/

void helpFunction(){
	printf("\n\n");

	printf("My name is A.I - helper, I'm going to write out the commands:\n");
	printf("1: => Load - To load an existing file.\n");
	printf("2: => Define - define a variable ex. x = 2 + 1.\n");
	printf("3: => Help - You'r already here, so you know what this one does.\n");
	printf("4: => Exit - This will exit the program.");
}

void quitFunction(){
	exit(0);
}

main(){
	scannerADT scanner;
	string line, token;
	environmentADT env;
	symtabADT table;
	expADT exp;
	valueADT value;

	void(*functionPtr)(scannerADT, symtabADT);

	scanner = NewScanner();
	env = NewEnvironment();
	table = NewSymbolTable();

	GetScannerSpaceOption(scanner, IgnoreSpaces);

	/*Enter(table, 'l', &loadFunction);
	Enter(table, "load", &loadFunction);
	Enter(table, 'd', &defineFunction);
	Enter(table, "define", &defineFunction);*/
	Enter(table, "h", &helpFunction);
	Enter(table, "help", &helpFunction);
	Enter(table, "q", &quitFunction);
	Enter(table, "quit", &quitFunction);


	while (TRUE)try{

		printf("=> ");
		line = GetLine();
		SetScannerString(scanner, line);
		token = ReadToken(scanner);
		

		if (StringEqual(token, ":")){
			token = ReadToken(scanner);
			if (Lookup(table, token) != UNDEFINED){
				functionPtr = Lookup(table, token);
				(*functionPtr)(scanner, table);
			}
			else{
				Error("Invalid input");
			}

		}
		else{
			exp = ParseExp(scanner);
			value = Eval(exp, env);
		}

	} endtry
}
