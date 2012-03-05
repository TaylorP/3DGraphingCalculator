#ifndef MULTIPLY_EXPRESSION_H
#define MULTIPLY_EXPRESSION_H

#include "baseExpression.h"

class MultiplyExpression : public BaseExpression
{
	public:

		MultiplyExpression(BaseExpression* pLeft, BaseExpression* pRight)
		{
			mLeft = pLeft;
			mRight = pRight;
		}

		~MultiplyExpression()
		{
			delete mLeft;
			delete mRight;
		}

		inline virtual float evaluate(float pX, float pY)
		{
			return mLeft->evaluate(pX, pY) * mRight->evaluate(pX, pY);
		}

		virtual void printExpression()
		{
			printf("(");
			mLeft->printExpression();
			printf(" * ");
			mRight->printExpression();
			printf(")");
		}
};

#endif