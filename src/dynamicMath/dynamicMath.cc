#include "include/dynamicMath.h"
#include "include/expressions.h"

#include <map>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <bits/stdc++.h>
#include "../debugging/include/debugging.h"

using namespace DynMath;

// This is a bunch of const stuff required for the parser to work

const std::map<std::string, UnaryExpression* (*)(Expression *)> unaryExpressions = {
    {"sin", &Expressions::Sine::create},
    {"cos", &Expressions::Cosine::create},
    {"tan", &Expressions::Tangent::create},
    {"ln", &Expressions::NaturalLogarithm::create},
    {"log", &Expressions::LogarithmBase10::create}
};

const std::map<std::string, BinaryExpression* (*)(Expression *, Expression *)> binaryExpressions = {
    {"+", &Expressions::Addition::create},
    {"-", &Expressions::Subtraction::create},
    {"*", &Expressions::Multiplication::create},
    {"/", &Expressions::Division::create},
    {"^", &Expressions::Power::create}
};

const std::string variable = "x";

const std::map<std::string, double> specialConstants = {
    {"e", M_E},
    {"pi", M_PI}
};

const std::unordered_set<std::string> parentheses = {"(", ")"};

const std::map<int, std::unordered_set<std::string>> priorityMap = {
    {0, {"+", "-"}},
    {1, {"*", "/"}},
    {2, {"^"}},
    {3, {"sin", "cos", "tan", "log", "ln"}}
};

constexpr bool isNumeric(const std::string& src)
{
    if (src.empty()) return false;
    if (src.contains('-'))
    {
        if (std::ranges::count(src, '-') != 1 || src.at(0) != '-')
        {
            return false;
        }
    }
    return std::ranges::all_of(src, [](const char &c) { return isdigit(c) || c == '.' || c == '-'; });
}

UnaryExpression::UnaryExpression(Expression *_inner)
{
    inner = _inner;
}

BinaryExpression::BinaryExpression(Expression *_left, Expression *_right)
{
    left = _left;
    right = _right;
}

UnaryExpression::~UnaryExpression()
{
    logInfo("Deleting unary...");
    delete inner;
}

BinaryExpression::~BinaryExpression()
{
    logInfo("Deleting binary...");
    delete left;
    delete right;
}

/*
 * Splits the initial string into substrings, separating into characters/strings mentioned in structures above
 */
std::vector<std::string> Util::tokenize(const std::string& src)
{
    std::vector<std::string> out = {};

    size_t start = 0;
    size_t len = 0;

    while (start + len <= src.size())
    {
        bool parenteseFlag = false;
        std::string sub = src.substr(start, len);
        if (sub == "-")
        {
            if (out.empty() || out.back() == "(" || unaryExpressions.contains(out.back()) || binaryExpressions.contains(out.back()))
            {
                out.emplace_back("-1");
                out.emplace_back("*");
                start += len;
                len = 0;
                ++len;
                continue;
            }
        }
        if (unaryExpressions.contains(sub) ||
            binaryExpressions.contains(sub) ||
            sub == variable ||
            specialConstants.contains(sub) ||
            parentheses.contains(sub) ||
            (isNumeric(sub) &&
            (start + len >= src.size() || (!isdigit(src.at(start + len)) && src.at(start + len) != '.'))))
        {
            out.push_back(sub);
            start += len;
            len = 0;
        }
        ++len;
    }

    if (start != src.size())
    {
        logWarning(std::format("Unrecognized token at position {}", start));
        throw std::invalid_argument(std::format("Unrecognized token at position {}", start));
    }

    logInfo(std::format("start: {}, len: {}", start, len));

    return out;
}

bool verifyUnaryOperationCompliance(const std::vector<std::string>::iterator tokensStart, const std::vector<std::string>::iterator tokensEnd)
{
    for (auto i = tokensStart; i < tokensEnd; ++i)
    {
        if (unaryExpressions.contains(*i))
        {
            if (i + 1 >= tokensEnd || *(i + 1) != "(")
            {
                return false;
            }
        }
    }
    return true;
}

/*
 * Counts how "deep" a given token is in the expression via counting the parentheses
 */
int evaluateTokenDepth(const std::vector<std::string>::iterator tokensStart, const int index)
{
    const auto limit = tokensStart + index;
    const int depth = std::count(tokensStart, limit, "(") - std::count(tokensStart, limit, ")");
    if (depth < 0) throw std::invalid_argument("Illegal expression depth");
    return depth;
}

/*
 * Gets token's depth evaluated before multiplying it by 3 to allow for extra room to allow for precision within the given depth
 */
int evaluateTokenPriority(const std::vector<std::string>::iterator tokensStart, const int index)
{
    if (!unaryExpressions.contains(*(tokensStart + index)) && !binaryExpressions.contains(*(tokensStart + index)))
    {
        return -1;
    }
    int depth = evaluateTokenDepth(tokensStart, index) * priorityMap.size();
    for (const auto& pair : priorityMap)
    {
        if (pair.second.contains(*(tokensStart + index)))
        {
            depth += pair.first;
            logInfo(std::format("Evaluated {} priority as {}", *(tokensStart + index), depth));
            return depth;
        }
    }
    return -1;
}

/*
 * Self-explanatory. Used to determine if there's only one valid token
 */
int countParentheses(const std::vector<std::string>::iterator tokensStart, const std::vector<std::string>::iterator tokensEnd)
{
    return std::count_if(tokensStart, tokensEnd, [](const std::string& s) { return parentheses.contains(s); });
}

/*
 * Searches for the first non-operation token. The parseRecursive relies on this to not recurse infinitely
 */
int findFirstStatic(const std::vector<std::string>::iterator tokensStart, const std::vector<std::string>::iterator tokensEnd)
{
    for (auto i = tokensStart; i < tokensEnd; ++i)
    {
        if (isNumeric(*i) || specialConstants.contains(*i) || *i == variable) return std::distance(tokensStart, i);
    }
    return -1;
}

/*
 * Searches for the token with lowest priority (not depth!) and returns the index of the rightmost one with respect to iterator start
 */
int findEntryPoint(const std::vector<std::string>::iterator tokensStart, const std::vector<std::string>::iterator tokensEnd)
{
    int point = -1;
    int32_t minPriority = INT32_MAX;
    for (auto i = tokensStart; i < tokensEnd; ++i)
    {
        const int priority = evaluateTokenPriority(tokensStart, std::distance(tokensStart, i));
        if (priority < 0) continue;
        if (priority <= minPriority)
        {
            minPriority = priority;
            point = std::distance(tokensStart, i);
        }
    }
    logInfo(std::format("Found entry point on {}", point));
    return point;
}

/*
 * Behold, this abomination
 *
 * Recursively transforms the token list into an expression structure
 */
Expression *parseRecursive(const std::vector<std::string>::iterator tokensStart, const std::vector<std::string>::iterator tokensEnd)
{
    // Shouldn't happen on a valid input
    if (tokensStart == tokensEnd)
    {
        logWarning("Received empty token vector");
        return nullptr;
    }

    const int entryPoint = findEntryPoint(tokensStart, tokensEnd);

    // No operations left to parse but there still are tokens
    if (entryPoint < 0)
    {
        // Exactly one token other than the parentheses
        if (std::distance(tokensStart, tokensEnd) == countParentheses(tokensStart, tokensEnd) + 1)
        {
            // If there's only one valuable token left, and it's a constant/variable
            if (const int firstStatic = findFirstStatic(tokensStart, tokensEnd); firstStatic >= 0)
            {
                std::string token = *(tokensStart + firstStatic);
                logInfo(std::format("Found static component: {}", token));
                if (token == variable) return Expressions::Variable::create();
                if (specialConstants.contains(token)) return Expressions::Constant::create(specialConstants.at(token));
                if (isNumeric(token))
                {
                    double value = std::stod(token);
                    return Expressions::Constant::create(value);
                }
            }
        }
        throw std::invalid_argument(std::format("Invalid static token"));
    }

    std::string token = *(tokensStart + entryPoint);
    logInfo(std::format("Entry token: {}", token));

    if (unaryExpressions.contains(token))
    {
        Expression *inner = parseRecursive(tokensStart + entryPoint + 1, tokensEnd);
        if (inner == nullptr) throw std::invalid_argument("Incomplete expression");
        return unaryExpressions.at(token)(inner);
    }

    if (binaryExpressions.contains(token))
    {
        Expression *left = parseRecursive(tokensStart, tokensStart + entryPoint);
        Expression *right = parseRecursive(tokensStart + entryPoint + 1, tokensEnd);
        if (left == nullptr || right == nullptr) throw std::invalid_argument("Incomplete expression");
        return binaryExpressions.at(token)(left, right);
    }
    throw std::invalid_argument(std::format("No valid operation found for {}", token));
}

Expression *Util::parse(std::vector<std::string> tokens)
{
    if (std::ranges::count(tokens, "(") != std::ranges::count(tokens, ")"))
    {
        throw std::invalid_argument("Incorrect amount of parentheses");
    }
    if (!verifyUnaryOperationCompliance(tokens.begin(), tokens.end()))
    {
        throw std::invalid_argument("Improper use of unary operations. Please use parentheses");
    }
    logInfo(std::format("Received tokens: {}", tokens));
    return parseRecursive(tokens.begin(), tokens.end());
}

std::map<double, double> *Util::prepareGraph(const Expression *expr, const double minX, const double maxX, const double step)
{
    if (step == 0) return {};

    auto *out = new std::map<double, double>();

    double x = minX;

    while (x <= maxX)
    {
        out -> emplace(x, expr -> evaluate(x));
        x += step;
    }

    return out;
}
