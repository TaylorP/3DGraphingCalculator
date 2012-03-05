#ifndef VARIABLE_EXPRESSION_H
#define VARIABLE_EXPRESSION_H

#include "baseExpression.h"

class VariableExpression : public BaseExpression
{
	protected:
	
		bool mVariable;

	public:
		VariableExpression()
		{
			mVariable = false;
		}

		VariableExpression(bool pNumber)
		{
			mVariable = pNumber;
		}

		~VariableExpression()
		{
			delete mLeft;
			delete mRight;
		}

		inline virtual float evaluate(float pX, float pY)
		{
			if(mVariable)
				return pX;
			else
				return pY;
		}

		virtual void printExpression()
		{
			if(mVariable)
				printf("x");
			else
				printf("y");
		}
};

static VariableExpression* gXVar = new VariableExpression(true);
static VariableExpression* gYVar = new VariableExpression(false);

#define XVAR gXVar
#define YVAR gYVar

#endif