#ifndef NUMBER_EXPRESSION_H
#define NUMBER_EXPRESSION_H

#include "baseExpression.h"

class NumberExpression : public BaseExpression
{
	protected:
		float mValue;

	public:

		NumberExpression(float pValue)
		{
			mLeft = 0;
			mRight = 0;
			mValue = pValue;
		}

		~NumberExpression()
		{

		}

		inline virtual float evaluate(float pX, float pY)
		{
			return mValue;
		}

		virtual void printExpression()
		{
			printf("%f",mValue);
		}
};

#endif