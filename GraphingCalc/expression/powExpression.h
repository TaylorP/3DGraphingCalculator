//Copyright Marmoset Software, 2012
//Written by Taylor Petrick
//
//3D Graphing Calculator Project

#ifndef POWER_EXPRESSION_H
#define POWER_EXPRESSION_H

#include "baseExpression.h"
#include <math.h>

///A exponent expression that raises the left side to the power of the right side
class PowerExpression : public BaseExpression
{
    
public:

    ///Constructs a power expression
    PowerExpression(BaseExpression* pLeft, BaseExpression* pRight)
    {
        mLeft = pLeft;
        mRight = pRight;
    }

    ///Destructs the expression
    ~PowerExpression()
    {
        delete mLeft;
        delete mRight;
    }

    ///Evaluates this expression, i.e compute mLeft^mRight
    inline virtual float evaluate(float pX, float pY)
    {
        return pow(mLeft->evaluate(pX, pY),mRight->evaluate(pX, pY));
    }

    ///Prints the expression
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