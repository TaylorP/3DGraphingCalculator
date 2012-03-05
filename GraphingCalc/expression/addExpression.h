#ifndef ADD_EXPRESSION_H
#define ADD_EXPRESSION_H

#include "baseExpression.h"

class AddExpression : public BaseExpression
{
	public:

		AddExpression(BaseExpression* pLeft, BaseExpression* pRight)
		{
			mLeft = pLeft;
			mRight = pRight;
		}

		~AddExpression()
		{
			delete mLeft;
			delete mRight;
		}

		inline virtual float evaluate(float pX, float pY)
		{
			return mLeft->evaluate(pX, pY) + mRight->evaluate(pX, pY);
		}

		virtual void printExpression()
		{
			printf("(");
			mLeft->printExpression();
			printf(" + ");
			mRight->printExpression();
			printf(")");
		}
};

#endif