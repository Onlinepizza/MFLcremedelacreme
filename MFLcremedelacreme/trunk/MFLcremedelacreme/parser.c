#include "parser.h"
#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "exp.h"
#include "scanadt.h"



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

/* Function: readE
 *
 *
 *   E -> T + E
 *   E -> T - E
 *   E -> T
 */

static expADT ReadE(scannerADT scanner)
{
	expADT exp, rhs;
	string token;
	int newPrec;

	exp = ReadT(scanner);
	token = ReadToken(scanner);
	if (IsOperator(token)){
		rhs = ReadE(scanner);
		exp = NewCompoundExp(token[0], exp, rhs);
	}
	else{
		SaveToken(scanner, token);
	}
	return (exp);
}

/*   Function: readT
 *
 *   T -> C * T
 *   T -> C / T
 *   T -> C
 */

static expADT readT(scannerADT scanner){
	expADT exp, rhs;
	string token;
	int newPrec;

	exp = ReadC(scanner);
	token = ReadToken(scanner);
	if (IsOperator(token)){
		rhs = ReadT(scanner);
		exp = NewCompoundExp(token[0], exp, rhs);
	}
	else{
		SaveToken(scanner, token);
	}
	return (exp);
}

/*   Function: readC
 *
 *   C -> F (E)
 *   C -> F
 */

static expADT readC(scannerADT scanner){
	expADT exp, rhs;
	string token;

	exp = readF(scanner);
	token = ReadToken(scanner);
	if (StringEqual(token, "(")) {
		rhs = ReadE(scanner, 0);
		if (!StringEqual(ReadToken(scanner), ")")){
				Error("Unbalanced parentheses");
			}
		exp = NewCallExp(exp, rhs);		//C -> F (E)
	}
	else {
		//ingen aning om detta �r r�tt.
		SaveToken(scanner, token);		//C -> F
	}
	return(exp);
}

/*	 Function: readF
 *
 *   F -> (E)
 *   F -> if E RelOp E then E else E
 *   F -> func (identifier) { E }
 *   F -> integer
 *   F -> identifier
 */

static expADT readF(scannerADT scanner){
	expADT exp, lhs, rhs, thenpart, elsepart;
	string token;
	char relop;

	token = ReadToken(scanner);
	if (StringEqual(token, "(")) {
		exp = ReadE(scanner);				//F -> (E)
		if (!StringEqual(ReadToken(scanner), ")")) {
			Error("Unbalanced parentheses");
		}
	}
	//h�r ska if och func �ven behandlas.
	else if (isalpha(token[0])){
		exp = NewIdentifierExp(token); // F -> identifier
	}
	else if (isdigit(token[0])){
		exp = NewIntegerExp(StringToInteger(token)); // F -> integer
	}
	return(exp);
}


static bool IsOperator(string token){
	if (StringLength(token) != 1){ 
		return FALSE; }
	switch (token[0]){
	case '+':
	case '-':
	case '*':
	case '/':
	case '=':
		return TRUE;
	default:
		return FALSE;
	}
}