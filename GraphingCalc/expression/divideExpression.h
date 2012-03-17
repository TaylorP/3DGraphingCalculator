//Copyright Marmoset Software, 2012
//Written by Taylor Petrick
//
//3D Graphing Calculator Project

#ifndef DIVIDE_EXPRESSION_H
#define DIVIDE_EXPRESSION_H

#include "baseExpression.h"

///A division expression class
class DivideExpression : public BaseExpression
{
    
public:

    ///Constructs a new division expression from a left and right side
    DivideExpression(BaseExpression* pLeft, BaseExpression* pRight)
    {
        mLeft = pLeft;
        mRight = pRight;
    }

    ///Destroys this division expression
    ~DivideExpression()
    {
        delete mLeft;
        delete mRight;
    }

    ///Evaluates the expression at pX,pY, and returns infinity when dividing by 0
    inline virtual float evaluate(float pX, float pY)
    {
        float lRight = mRight->evaluate(pX, pY);
        
        if(lRight == 0)
            return INFINITY;

        return mLeft->evaluate(pX, pY) / lRight;
    }

    ///Prints the expression
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