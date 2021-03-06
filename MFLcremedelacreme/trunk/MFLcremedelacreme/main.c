#define _CRT_SECURE_NO_WARNINGS
#include "scanadt.h"
#include "genlib.h"
#include "strlib.h"
#include "exception.h"
#include "env.h"
#include "symtab.h"
#include "parser.h"
#include "eval.h"
#include "print.h"
#include "simpio.h"
#include <ctype.h>

void defineFunction(scannerADT scanner, environmentADT env){
	expADT exp;
	string token, ident;
	valueADT value;
	environmentADT close;

	close = NewClosure(env);

	token = ReadToken(scanner);
	ident = token;

	while (!StringEqual(token, "="))
		token = ReadToken(scanner);

	exp = ParseExp(scanner);

	DefineIdentifier(env, ident, exp, close);
}

void loadFunction(scannerADT scanner, environmentADT env){
	FILE * infile;
	string fileName, read;
	int value = 0;
	read = "";
	printf("\n");
	

	fileName = ReadToken(scanner);
	fileName = Concat(fileName, ReadToken(scanner));
	fileName = Concat(fileName, ReadToken(scanner));
	

	infile = fopen(fileName, "r");

	if (infile == NULL){
		Error("Can't open file.");
	}
	else {

		while (read != NULL) {
			if (read[0] != '#' && read[0] != '\0'){
				SetScannerString(scanner, read);
				defineFunction(scanner, env);
			}
			read = ReadLine(infile);
		}

	}
	printf("\n");
	printf("File loaded.\n");
	fclose(fileName);
}

void helpFunction(){
	system("cls");

	printf("My name is A.I - helper, I'm going to write out the commands:\n");
	printf("1: => Load - To load an existing file.\n");
	printf("2: => Define - define a variable ex. x = 2 + 1.\n");
	printf("3: => Help - You'r already here, so you know what this one does.\n");
	printf("4: => Exit - This will exit the program.");
}

void quitFunction(){
	exit(0);
}

void initTable(symtabADT table){
	Enter(table, "l", &loadFunction);
	Enter(table, "load", &loadFunction);
	Enter(table, "d", &defineFunction);
	Enter(table, "define", &defineFunction);
	Enter(table, "h", &helpFunction);
	Enter(table, "help", &helpFunction);
	Enter(table, "q", &quitFunction);
	Enter(table, "quit", &quitFunction);

}

main(){
	scannerADT scanner;
	string line, token;
	environmentADT env;
	symtabADT table;
	expADT exp;
	valueADT value;
	int result, var;
	void(*functionPtr)(scannerADT, environmentADT);

	scanner = NewScanner();
	env = NewEnvironment();
	table = NewSymbolTable();

	SetScannerSpaceOption(scanner, IgnoreSpaces);

	initTable(table);

	while (TRUE)try{

		printf("=> ");
		line = GetLine();
		SetScannerString(scanner, line);
		token = ReadToken(scanner);

		if (StringEqual(token, ":")){
			token = ReadToken(scanner);
			if (Lookup(table, token) != UNDEFINED){
				functionPtr = Lookup(table, token);
				(*functionPtr)(scanner, env);
			}
			else{
				Error("Invalid input");
			}

		}
		else{
			SaveToken(scanner, token);
			exp = ParseExp(scanner);
			value = Eval(exp, env);
			PrintValue(value);
		}

		printf("\n");
	} endtry
}
