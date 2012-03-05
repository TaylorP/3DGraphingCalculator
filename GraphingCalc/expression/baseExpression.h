#ifndef BASE_EXPRESSION_H
#define BASE_EXPRESSION_H
#include <stdio.h>

class BaseExpression
{
	protected:
		
		BaseExpression* mLeft;
		BaseExpression* mRight;

	public:

		BaseExpression()
		{
			mLeft = mRight = 0;
		}

		BaseExpression(BaseExpression* pLeft, BaseExpression* pRight)
		{
			mLeft = pLeft;
			mRight = pRight;
		}

		~BaseExpression()
		{
			if(mLeft)
				delete mLeft;
			if(mRight)
				delete mRight;
		}

		inline virtual float evaluate(float pX, float pY) = 0;

		virtual void printExpression() = 0;
};

#endif