#include "scanadt.h"
#include "genlib.h"
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
			if (token == 'l' || token == "load"){

			}
			else if (token == 'd' || token == "define"){

			}
			else if(token == 'h' || token == "help"){

			}
			else if (token == 'q' || token == "quit"){
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
