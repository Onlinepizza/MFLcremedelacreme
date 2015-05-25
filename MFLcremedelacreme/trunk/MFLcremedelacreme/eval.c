#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "exp.h"
#include "env.h"
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

void InitVariableTable(void);
int GetIdValue(string name);
void SetIdentifierValue(string name, int value);
static int EvalCompound(expADT exp);

/* Exported entries */valueADT Eval(expADT exp, environmentADT env){	exptypeT type;	expADT lhs, rhs, ifpart, elsepart;	environmentADT closure;	char op;	type = ExpType(exp);	switch (type){	case FuncExp:
		closure = NewClosure(env);
		//rekursivt anrop
	case IfExp:
		closure = NewClosure(env);
		op = GetIfRelOp(exp);
		switch (op)
		{
		case '=':
			if (Eval(GetIfLHSExpression(exp), closure) == Eval(GetIfRHSExpression(exp), closure)){
				Eval(GetIfThenPart(exp), closure);
			}
			else{
				Eval(GetIfElsePart(exp), closure);
			}
		case '>':
			if (Eval(GetIfLHSExpression(exp), closure) > Eval(GetIfRHSExpression(exp), closure)){
				Eval(GetIfThenPart(exp), closure);
			}
			else{
				Eval(GetIfElsePart(exp), closure);
			}
		case '<':
			if (Eval(GetIfLHSExpression(exp), closure) < Eval(GetIfRHSExpression(exp), closure)){
				Eval(GetIfThenPart(exp), closure);
			}
			else{
				Eval(GetIfElsePart(exp), closure);
			}
		}
	case CallExp:
		closure = NewClosure(env);
		//rekursivt anrop
	case ConstExp:
		return NewIntegerValue((ExpInteger(exp)));
	case IdentifierExp:
		return NewIntegerValue(GetIdValue(ExpIdentifier(exp)));
	case CompoundExp:		return NewIntegerValue((EvalCompound(exp, env)));	}}
void InitVariableTable(void)
{
	variableTable = NewSymbolTable();
}

int GetIdValue(string name){
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

static int EvalCompound(expADT exp, environmentADT env)
{
	char op;
	int lhs, rhs;
	environmentADT closure;
	closure = NewClosure(env);

	op = ExpOperator(exp);
	if (op == '=') {
		rhs = EvalExp(ExpRHS(exp));
		SetIdentifierValue(ExpIdentifier(ExpLHS(exp)), rhs);
		return (rhs);
	}
	lhs = Eval(ExpLHS(exp), closure);
	rhs = Eval(ExpRHS(exp), closure);
	switch (op) {
	case '+': return (lhs + rhs);
	case '-': return (lhs - rhs);
	case '*': return (lhs * rhs);
	case '/': return (lhs / rhs);
	default:  Error("Illegal operator");
	}
}
