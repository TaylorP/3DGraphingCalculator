//Copyright Marmoset Software, 2012
//Written by Taylor Petrick
//
//3D Graphing Calculator Project

#ifndef LN_EXPRESSION_H
#define LN_EXPRESSION_H

#include "baseExpression.h"
#include <math.h>

///A natural log expression that computes log(left), with base e
class LnExpression : public BaseExpression
{
    
public:
    
    ///Constructs a power expression
    LnExpression(BaseExpression* pLeft, BaseExpression* pRight)
    {
        mLeft = pLeft;
        mRight = pRight;
    }
    
    ///Destructs the expression
    ~LnExpression()
    {
        delete mLeft;
        delete mRight;
    }
    
    ///Evaluates this expression, i.e compute mLeft^mRight
    inline virtual float evaluate(float pX, float pY)
    {
        return log(mLeft->evaluate(pX,pY));
    }
    
    ///Prints the expression
    virtual void printExpression()
    {
        printf("ln(");
        mLeft->printExpression();
        printf(")");
    }
};

#endif