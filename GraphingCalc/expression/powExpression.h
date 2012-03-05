#ifndef POWER_EXPRESSION_H
#define POWER_EXPRESSION_H

#include "baseExpression.h"
#include <math.h>

class PowerExpression : public BaseExpression
{
	public:

		PowerExpression(BaseExpression* pLeft, BaseExpression* pRight)
		{
			mLeft = pLeft;
			mRight = pRight;
		}

		~PowerExpression()
		{
			delete mLeft;
			delete mRight;
		}

		inline virtual float evaluate(float pX, float pY)
		{
			return pow(mLeft->evaluate(pX, pY),mRight->evaluate(pX, pY));
		}

		virtual void printExpression()
		{
			printf("(");
			mLeft->printExpression();
			printf(" ^ ");
			mRight->printExpression();
			printf(")");
		}
};

#endif