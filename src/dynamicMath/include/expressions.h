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
        [[nodiscard]] std::string toString() const override;
    protected:
        double value;
    };

    class Variable : public StaticComponent
    {
    public:
        static StaticComponent *create();
        Variable() = default;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Addition : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        using BinaryExpression::BinaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Subtraction : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        using BinaryExpression::BinaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Multiplication : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        using BinaryExpression::BinaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Division : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        using BinaryExpression::BinaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Power : public BinaryExpression
    {
    public:
        static BinaryExpression *create(Expression *_left, Expression *_right);
        using BinaryExpression::BinaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Sine : public UnaryExpression
    {
    public:
        static UnaryExpression *create(Expression *_inner);
        using UnaryExpression::UnaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };

    class Cosine : public UnaryExpression
    {
    public:
        static UnaryExpression *create(Expression *_inner);
        using UnaryExpression::UnaryExpression;
        [[nodiscard]] double evaluate(double x) const override;
        [[nodiscard]] std::string toString() const override;
    };
}

#endif
