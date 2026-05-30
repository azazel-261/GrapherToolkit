#include "include/expressions.h"

#include <cmath>

/*
 * I promise, there isn't that much to look at here
 * Those are just a bunch of implementations for classes in expressions.h
 */

// Constant

DynMath::Expressions::Constant::Constant(const double _value)
    : value(_value) {}

DynMath::StaticComponent *DynMath::Expressions::Constant::create(const double _value)
{
    return new Constant(_value);
}

double DynMath::Expressions::Constant::evaluate(double x) const
{
    return value;
}

std::string DynMath::Expressions::Constant::toString() const
{
    return std::to_string(value);
}

// Variable

DynMath::StaticComponent *DynMath::Expressions::Variable::create()
{
    return new Variable();
}

double DynMath::Expressions::Variable::evaluate(const double x) const
{
    return x;
}

std::string DynMath::Expressions::Variable::toString() const
{
    return "x";
}

// Addition

DynMath::BinaryExpression *DynMath::Expressions::Addition::create(Expression *_left, Expression *_right)
{
    return new Addition(_left, _right);
}

double DynMath::Expressions::Addition::evaluate(const double x) const
{
    return left -> evaluate(x) + right -> evaluate(x);
}

std::string DynMath::Expressions::Addition::toString() const
{
    return "(" + left -> toString() + ") + (" + right -> toString() + ")";
}

// Subtraction

DynMath::BinaryExpression *DynMath::Expressions::Subtraction::create(Expression *_left, Expression *_right)
{
    return new Subtraction(_left, _right);
}

double DynMath::Expressions::Subtraction::evaluate(const double x) const
{
    return left -> evaluate(x) - right -> evaluate(x);
}

std::string DynMath::Expressions::Subtraction::toString() const
{
    return "(" + left -> toString() + ") - (" + right -> toString() + ")";
}

// Multiplication

DynMath::BinaryExpression *DynMath::Expressions::Multiplication::create(Expression *_left, Expression *_right)
{
    return new Multiplication(_left, _right);
}

double DynMath::Expressions::Multiplication::evaluate(const double x) const
{
    return left -> evaluate(x) * right -> evaluate(x);
}

std::string DynMath::Expressions::Multiplication::toString() const
{
    return "(" + left -> toString() + ") * (" + right -> toString() + ")";
}

// Division

DynMath::BinaryExpression *DynMath::Expressions::Division::create(Expression *_left, Expression *_right)
{
    return new Division(_left, _right);
}

double DynMath::Expressions::Division::evaluate(const double x) const
{
    return left -> evaluate(x) * right -> evaluate(x);
}

std::string DynMath::Expressions::Division::toString() const
{
    return "(" + left -> toString() + ") / (" + right -> toString() + ")";
}

// Power

DynMath::BinaryExpression *DynMath::Expressions::Power::create(Expression *_left, Expression *_right)
{
    return new Power(_left, _right);
}

double DynMath::Expressions::Power::evaluate(const double x) const
{
    return pow(left -> evaluate(x), right -> evaluate(x));
}

std::string DynMath::Expressions::Power::toString() const
{
    return "(" + left -> toString() + ") ^ (" + right -> toString() + ")";
}

// Sine

DynMath::UnaryExpression *DynMath::Expressions::Sine::create(Expression *_inner)
{
    return new Sine(_inner);
}

double DynMath::Expressions::Sine::evaluate(const double x) const
{
    return std::sin(x);
}

std::string DynMath::Expressions::Sine::toString() const
{
    return "sin(" + inner -> toString() + ")";
}

// Cosine

DynMath::UnaryExpression *DynMath::Expressions::Cosine::create(Expression *_inner)
{
    return new Cosine(_inner);
}

double DynMath::Expressions::Cosine::evaluate(const double x) const
{
    return std::cos(x);
}

std::string DynMath::Expressions::Cosine::toString() const
{
    return "cos(" + inner -> toString() + ")";
}
