//Written by Taylor Petrick

#ifndef SUBTRACT_EXPRESSION_H
#define SUBTRACT_EXPRESSION_H


#include "baseExpression.h"

//Expression
class SubtractExpression : public BaseExpression
{
	public:

		SubtractExpression(BaseExpression* pLeft, BaseExpression* pRight)
		{
			mLeft = pLeft;
			mRight = pRight;
		}

		~SubtractExpression()
		{
			delete mLeft;
			delete mRight;
		}

		inline virtual float evaluate(float pX, float pY)
		{
			return mLeft->evaluate(pX, pY) - mRight->evaluate(pX, pY);
		}

		virtual void printExpression()
		{
			printf("(");
			mLeft->printExpression();
			printf(" - ");
			mRight->printExpression();
			printf(")");
		}
};

#endif