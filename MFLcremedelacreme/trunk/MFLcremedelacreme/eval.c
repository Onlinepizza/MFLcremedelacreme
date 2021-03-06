#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "exp.h"
#include "env.h"
#include "eval.h"
#include "value.h"
#include "symtab.h"


/* Private function prototypes */

static int EvalCompound(expADT exp);

/* Exported entries */valueADT Eval(expADT exp, environmentADT env){	exptypeT type;	expADT lhs = NULL, rhs = NULL, ifpart = NULL, elsepart = NULL, funcExp = NULL, arg = NULL, body = NULL;	valueADT value = NULL, identifier = NULL;	environmentADT closure = NULL;	char op;	string id;	int larg, rarg;	type = ExpType(exp);	switch (type){	case FuncExp:
		closure = NewClosure(env);
		return NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), closure);
	case IfExp:
		closure = NewClosure(env);
		op = GetIfRelOp(exp);
		larg = GetIntValue(Eval(GetIfLHSExpression(exp), closure));		rarg = GetIntValue(Eval(GetIfRHSExpression(exp), closure));

		switch (op){
		case '=':
			if (larg == rarg){
				return Eval(GetIfThenPart(exp), env);
			}
			else{
				return Eval(GetIfElsePart(exp), env);
			}
		case '>':
			if (larg > rarg){
				return Eval(GetIfThenPart(exp), env);
			}
			else{
				return Eval(GetIfElsePart(exp), env);
			}
		case '<':
			if (larg < rarg){
				return Eval(GetIfThenPart(exp), env);
			}
			else{
				return Eval(GetIfElsePart(exp), env);
			}
		}
		
	case CallExp:
		funcExp = GetCallExp(exp);
		arg = GetCallActualArg(exp);
		value = Eval(funcExp, env);

		if (ValueType(value) == FuncValue){			
			closure = GetFuncValueClosure(value);
			id = GetFuncValueFormalArg(value);
			DefineIdentifier(closure, id, arg, env);
			return Eval(GetFuncValueBody(value), closure);
		}
		else{
			return value;
		}
	case ConstExp:
		return NewIntegerValue((ExpInteger(exp)));
	case IdentifierExp:
		id = ExpIdentifier(exp);
		identifier = GetIdentifierValue(env, id);
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
