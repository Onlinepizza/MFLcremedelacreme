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


/* Private function prototypes */

static int EvalCompound(expADT exp);

/* Exported entries */valueADT Eval(expADT exp, environmentADT env){	exptypeT type;	expADT lhs = NULL, rhs = NULL, ifpart = NULL, elsepart = NULL, funcExp = NULL, arg = NULL, body = NULL;	valueADT value, identifier;	environmentADT closure;	char op;	string id;	type = ExpType(exp);	switch (type){	case FuncExp:
		closure = NewClosure(env);
		return NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), closure);
	case IfExp:
		closure = NewClosure(env);
		op = GetIfRelOp(exp);
		switch (op){
		case '=':
			if (GetIntValue(Eval(GetIfLHSExpression(exp), closure)) == GetIntValue(Eval(GetIfRHSExpression(exp), closure))){
				return Eval(GetIfThenPart(exp), closure);
			}
			else{
				return Eval(GetIfElsePart(exp), closure);
			}
		case '>':
			if (GetIntValue(Eval(GetIfLHSExpression(exp), closure)) > GetIntValue(Eval(GetIfRHSExpression(exp), closure))){
				return Eval(GetIfThenPart(exp), closure);
			}
			else{
				return Eval(GetIfElsePart(exp), closure);
			}
		case '<':
			if (GetIntValue(Eval(GetIfLHSExpression(exp), closure)) < GetIntValue(Eval(GetIfRHSExpression(exp), closure))){
				return Eval(GetIfThenPart(exp), closure);
			}
			else{
				return Eval(GetIfElsePart(exp), closure);
			}
		}
		
	case CallExp:
		value = Eval(GetCallExp(exp), env);
		funcExp = GetCallExp(exp);
		if (ValueType(value) == FuncValue){
			arg = GetCallActualArg(exp);
			closure = GetFuncValueClosure(value);
			id = GetFuncValueFormalArg(value);
			DefineIdentifier(closure, funcExp, arg, env);
			return Eval(GetFuncValueBody(value), closure);
		}
		else{
			return value;
		}
	case ConstExp:
		return NewIntegerValue((ExpInteger(exp)));
	case IdentifierExp:
		identifier = GetIdentifierValue(env, ExpIdentifier(exp));
		closure = GetFuncValueClosure(identifier);
		body = GetFuncValueBody(identifier);
		return Eval(body, closure);
	case CompoundExp:		return NewIntegerValue((EvalCompound(exp, env)));	}}

/* Private functions */

static int EvalCompound(expADT exp, environmentADT env)
{
	char op;
	int lhs, rhs;
	op = ExpOperator(exp);

	lhs = GetIntValue(Eval(ExpLHS(exp), env));
	rhs = GetIntValue(Eval(ExpRHS(exp), env));
	switch (op) {
	case '+': return (lhs + rhs);
	case '-': return (lhs - rhs);
	case '*': return (lhs * rhs);
	case '/':
		if (rhs < 1){
			Error("division by zero");
		}
		return (lhs / rhs);
	default:  Error("Illegal operator");
	}
}
