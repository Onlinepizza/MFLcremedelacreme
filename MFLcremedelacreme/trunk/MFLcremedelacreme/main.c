#include "scanadt.h"
#include "genlib.h"
#include "strlib.h"
#include "exception.h"

main(){
	scannerADT scanner;
	string line, token;

	scanner = NewScanner();
	GetScannerSpaceOption(scanner, IgnoreSpaces);

	while (TRUE){
		try
		line = GetLine();
		SetScannerString(scanner, line);
		token = ReadToken(scanner);

		if (token == ':'){
			token = ReadToken(scanner);
			if (token == 'l' || StringEqual(token, "load")){

			}
			else if (token == 'd' || StringEqual(token, "define")){

			}
			else if(token == 'h' || StringEqual(token, "help")){

			}
			else if (token == 'q' || StringEqual(token, "quit")){
				exit(0);
			}
			else{
				Error("Invalid input");
			}

		}
		else{

		}

	} endtry
}
