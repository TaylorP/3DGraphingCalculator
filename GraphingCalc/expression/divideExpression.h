#ifndef DIVIDE_EXPRESSION_H
#define DIVIDE_EXPRESSION_H

#include "baseExpression.h"

class DivideExpression : public BaseExpression
{
	public:

		DivideExpression(BaseExpression* pLeft, BaseExpression* pRight)
		{
			mLeft = pLeft;
			mRight = pRight;
		}

		~DivideExpression()
		{
			delete mLeft;
			delete mRight;
		}

		inline virtual float evaluate(float pX, float pY)
		{
			float lRight = mRight->evaluate(pX, pY);
			if(lRight == 0)
				return 0;

			return mLeft->evaluate(pX, pY) / lRight;
		}

		virtual void printExpression()
		{
			printf("(");
			mLeft->printExpression();
			printf(" / ");
			mRight->printExpression();
			printf(")");
		}
};

#endif