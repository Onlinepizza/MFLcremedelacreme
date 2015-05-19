#include <stdio.h>
#include "simpio.h"
#include "print.h"
#include "exp.h"
#include "value.h"

void PrintExp(expADT exp){
	exptypeT type;
		type = ExpType(exp);
	switch (type){
	case FuncExp:
		printf("%s",GetFuncFormalArg(exp));
		PrintExp(GetFuncBody(exp));
		break;
	case IfExp:
		//Osäker på I vilken ordning print bör ske.
		//F -> if E RelOp E then E else E
		PrintExp(GetIfLHSExpression(exp));
		printf("%c", GetIfRelOp(exp));
		PrintExp(GetIfRHSExpression(exp));
		PrintExp(GetIfThenPart(exp));
		PrintExp(GetIfElsePart(exp));
		break;
	case CallExp:
		PrintExp(GetCallExp(exp));
		PrintExp(GetCallActualArg(exp));
		break;
	case ConstExp:
		printf("%d", ExpInteger(exp));
		break;
	case IdentifierExp:
		printf("%s", ExpIdentifier(exp));
		break;
	case CompoundExp:
		PrintExp(ExpLHS(exp));
		printf("%c", ExpOperator(exp));
		PrintExp(ExpRHS(exp));
		break;
	}
}

void PrintValue(valueADT value){
	valuetypeT type;
	type = ValueType(value);
	switch (type)
	{
	case FuncValue:
		//oklart i vilken ordning detta ska att ske.
		printf("%s", GetFuncValueFormalArg(value));
		PrintExp(GetFuncValueBody(value));
		//environmentADT GetFuncValueClosure(valueADT value); //vet ej hur man hanterar returntypen.
		break;
	case IntValue:
		printf("%d", GetIntValue(value));
		break;
	}
}
