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
static expADT ReadE(scannerADT scanner);
static expADT readT(scannerADT scanner);
static expADT readC(scannerADT scanner);
static expADT readF(scannerADT scanner);

/*
* Implementation notes: ParseExp
* ------------------------------
* This function just calls ReadE to read an expression and then
* checks to make sure no tokens are left over.
*/

expADT ParseExp(scannerADT scanner)
{
	expADT exp;

	exp = ReadE(scanner);
	if (MoreTokensExist(scanner)) {
		Error("ParseExp: %s unexpected", ReadToken(scanner));
	}
	return (exp);
}

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
	if (IsPlusMinusOperator(token)){
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
	if (IsTimesDivOperator(token)){
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
		rhs = ReadE(scanner);
		if (!StringEqual(ReadToken(scanner), ")")){
			Error("Unbalanced parentheses");
		}
		exp = NewCallExp(exp, rhs);		//C -> F (E)
	}
	else {
		SaveToken(scanner, token);
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
	expADT exp, lhs, rhs, ifPart, elsePart;
	string token;
	char relop;

	token = ReadToken(scanner);
	if (StringEqual(token, "(")) {
		exp = ReadE(scanner);				//F -> (E)
		if (!StringEqual(ReadToken(scanner), ")")) {
			Error("Unbalanced parentheses");
		}
	}
	else if (StringEqual(token, "if"))
	{
		lhs = ReadE(scanner);
		token = ReadToken(scanner);
		if (IsRealOp(token)){
			rhs = ReadE(scanner);
			if (StringEqual(ReadToken(scanner), "then")){
				ifPart = ReadE(scanner);
				if (StringEqual(ReadToken(scanner), "else")){
					elsePart = ReadE(scanner);
					exp = NewIfExp(lhs, token, rhs, ifPart, elsePart); //F -> if E RelOp E then E else E
				}
			}
		}

	}
	else if (StringEqual(token, "func")){
		if (StringEqual(token, "(")) {
			token = ReadToken(scanner);
			if (!StringEqual(ReadToken(scanner), ")")) {
				Error("Unbalanced parentheses");
			}
			if (StringEqual(token, "{")) {
				rhs = ReadE(scanner);
				if (!StringEqual(ReadToken(scanner), "}")) {
					Error("Unbalanced parentheses");
				}
			}
			exp = NewFuncExp(token, rhs); // F -> func (identifier) { E }
		}
	}
	else if (isalpha(token[0])){
		exp = NewIdentifierExp(token); // F -> identifier
	}
	else if (isdigit(token[0])){
		exp = NewIntegerExp(StringToInteger(token)); // F -> integer
	}
	else {
		SaveToken(scanner, token);
	}
	return(exp);
}


static bool IsPlusMinusOperator(string token){
	if (StringLength(token) != 1){
		return FALSE;
	}
	switch (token[0]){
	case '+':
	case '-':
		return TRUE;
	default:
		return FALSE;
	}
}


static bool IsTimesDivOperator(string token){
	if (StringLength(token) != 1){
		return FALSE;
	}
	switch (token[0]){
	case '*':
	case '/':
		return TRUE;
	default:
		return FALSE;
	}
}

static bool IsRealOp(string token){
	if (StringLength(token) != 1){
		return FALSE;
	}
	switch (token[0]){
	case '=':
	case '<':
	case '>':
		return TRUE;
	default:
		return FALSE;
	}
}
