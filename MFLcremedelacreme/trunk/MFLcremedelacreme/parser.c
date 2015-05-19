#include "parser.h"
#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "exp.h"
#include "scanadt.h"

expADT ReadE(scannerADT scanner, int prec);
expADT readT(scannerADT scanner);
expADT readC(scannerADT scanner);
//expADT readF(scannerADT scanner);

/*
* Type: expADT
* -------------------
* This type is used to represent the abstract notion of any
* expression in MFL.
*
*   E -> T + E
*   E -> T - E
*   E -> T
*
*   T -> C * T
*   T -> C / T
*   T -> C
*
*   C -> F (E)
*   C -> F
*
*   F -> (E)
*   F -> if E RelOp E then E else E
*   F -> func (identifier) { E }
*   F -> integer
*   F -> identifier
*
*   RelOp -> =
*   RelOp -> <
*   RelOp -> >
*
*/

expADT ReadE(scannerADT scanner, int prec)
{
	expADT exp, rhs;
	string token;
	int newPrec;

	exp = ReadT(scanner);
	while (TRUE) {
		token = ReadToken(scanner);
		newPrec = Precedence(token);
		if (newPrec <= prec) break;
		rhs = ReadE(scanner, newPrec);
		exp = NewCompoundExp(token[0], exp, rhs);
	}
	SaveToken(scanner, token);
	return (exp);
}

expADT readT(scannerADT scanner){
	expADT exp, rhs;
	string token;
	int newPrec;

	exp = ReadC(scanner);
	while (TRUE) {
		token = ReadToken(scanner);
		newPrec = Precedence(token);
		//if (newPrec != 3 ) break;
		rhs = ReadE(scanner, newPrec);
		exp = NewCompoundExp(token[0], exp, rhs);
	}
	SaveToken(scanner, token);
	return (exp);
}

expADT readC(scannerADT scanner){
	expADT exp, rhs;
	string token;

	exp = readF(scanner);
	token = Precedence(scanner);
	if (StringEqual(token, "(")) {
		rhs = ReadE(scanner, 0);
		if (!StringEqual(ReadToken(scanner), ")")){
				Error("Unbalanced parentheses");
			}
		exp =NewCallExp(exp, rhs);
	}
	return(exp);
}

/*expADT readF(scannerADT scanner){
	expADT exp;
	string token;

	token = ReadToken(scanner);
	if (StringEqual(token, "(")) {
		exp = ReadE(scanner, 0);
		if (!StringEqual(ReadToken(scanner), ")")) {
			Error("Unbalanced parentheses");
		}

}*/


int Precedence(string token)
{
	if (StringLength(token) > 1) return (0);
	switch (token[0]) {
	case '=': return (1);
	case '+': case '-': return (2);
	case '*': case '/': return (3);
	default:  return (0);
	}
}