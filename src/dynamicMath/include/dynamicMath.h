#ifndef GRAPHERTOOLKIT_DYNAMICMATH_H
#define GRAPHERTOOLKIT_DYNAMICMATH_H

#include <string>
#include <vector>
#include <map>

namespace DynMath
{
    /*
     * Base of base classes, doesn't do anything
     */
    class Expression
    {
    public:
        virtual ~Expression() = default;
        [[nodiscard]] virtual double evaluate(double x) const = 0;
        [[nodiscard]] virtual std::string toString() const = 0;
    };

    /*
     * For anything that doesn't need recursion to be parsed
     */
    class StaticComponent : public Expression
    {
    public:
        ~StaticComponent() override = default;
    };
    /*
     * Only contains the "inner" field, uses it's eval result to get its own
     */
    class UnaryExpression : public Expression
    {
    public:
        explicit UnaryExpression(Expression *_inner);
        ~UnaryExpression() override;
    protected:
        Expression *inner;
    };

    /*
     * Unlike UnaryOperation, has two fields and needs results from both to eval itself
     */
    class BinaryExpression : public Expression
    {
    public:
        BinaryExpression(Expression *_left, Expression *_right);
        ~BinaryExpression() override;
    protected:
        Expression *left;
        Expression *right;
    };

    namespace Util
    {
        /*
         * Splits the initial string into substrings, separating into characters/strings mentioned in structures above
         */
        std::vector<std::string> tokenize(const std::string& src);

        /*
         * Just serves as an entrypoint for parseRecursive which isn't exposed via header
         */
        Expression *parse(std::vector<std::string> tokens);

        /*
         * Just creates a map from x to expression evaluation for a given expression
         */
        std::map<double, double> *prepareGraph(const Expression *expr, double minX, double maxX, double step);
    }
}

#endif
