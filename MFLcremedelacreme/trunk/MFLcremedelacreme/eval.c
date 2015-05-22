#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "exp.h"
#include "eval.h"
#include "value.h"
#include "symtab.h"
/*
* Private variable: variableTable
* -------------------------------
* This table keeps track of the values for each variable.
*/

static symtabADT variableTable;

/* Private function prototypes */

static int EvalCompound(expADT exp);

/* Exported entries */valueADT Eval(expADT exp, environmentADT env){	exptypeT type;	type = ExpType(exp);	switch (type){	case FuncExp:
		//rekursivt anrop
	case IfExp:
		//rekursivt anrop
	case CallExp:
		//rekursivt anrop
	case ConstExp:
		return NewIntegerValue((ExpInteger(exp)));
	case IdentifierExp:
		return NewIntegerValue(GetIdentifierValue(ExpIdentifier(exp)));
	case CompoundExp:		return NewIntegerValue((EvalCompound(exp)));	}}
void InitVariableTable(void)
{
	variableTable = NewSymbolTable();
}

int GetIdentifierValue(string name)
{
	int *ip;

	ip = Lookup(variableTable, name);
	if (ip == UNDEFINED)  Error("%s is undefined", name);
	return (*ip);
}

void SetIdentifierValue(string name, int value)
{
	int *ip;

	ip = New(int *);
	*ip = value;
	Enter(variableTable, name, ip);
}

/* Private functions */

static int EvalCompound(expADT exp)
{
	char op;
	int lhs, rhs;

	op = ExpOperator(exp);
	if (op == '=') {
		rhs = EvalExp(ExpRHS(exp));
		SetIdentifierValue(ExpIdentifier(ExpLHS(exp)), rhs);
		return (rhs);
	}
	lhs = EvalExp(ExpLHS(exp));
	rhs = EvalExp(ExpRHS(exp));
	switch (op) {
	case '+': return (lhs + rhs);
	case '-': return (lhs - rhs);
	case '*': return (lhs * rhs);
	case '/': return (lhs / rhs);
	default:  Error("Illegal operator");
	}
}
