#ifndef GRAPHERTOOLKIT_EXPRESSIONS_H
#define GRAPHERTOOLKIT_EXPRESSIONS_H

#include "dynamicMath.h"

namespace DynMath::Expressions
{
    class Constant : public StaticComponent
    {
    public:
        static StaticComponent *create(double _value);
        explicit Constant(double _value);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    protected:
        double value;
    };

    class Variable : public StaticComponent
    {
    public:
        static StaticComponent *create();
        Variable();
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Addition : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        Addition(Expression *_left, Expression *_right);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Subtraction : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        Subtraction(Expression *_left, Expression *_right);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Multiplication : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        Multiplication(Expression *_left, Expression *_right);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Division : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        Division(Expression *_left, Expression *_right);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Power : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        Power(Expression *_left, Expression *_right);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Sine : public UnaryExpression
    {
    public:
        static UnaryExpression *create(Expression *_inner);
        explicit Sine(Expression *_inner);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };

    class Cosine : public UnaryExpression
    {
    public:
        static UnaryExpression *create(Expression *_inner);
        explicit Cosine(Expression *_inner);
        [[nodiscard]] double evaluate(double x) const override;
        std::string toString() override;
    };
}

#endif
