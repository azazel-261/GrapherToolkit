#include "include/grapher.h"

#include <cmath>

using namespace DynMath;

constexpr int maxSubdivisionDepth = 12;
constexpr double maxJump = 1000.0;
constexpr double tolerance = 0.01;

bool isValid(const double y)
{
    return std::isfinite(y);
}

Grapher::Grapher(DynMath::Expression *expr) : fn(expr) {}

/*
 * Smart sampling! This thing basically checks if a function is *linear enough* at the range given
 */
void Grapher::subdivide(std::vector<Point> &segment, const double x0, const double y0, const double x1, const double y1, const int depth) const
{
    if (depth >= maxSubdivisionDepth)
    {
        segment.push_back({x1, y1});
        return;
    }
    const double x05 = (x0 + x1) / 2.0;
    const double y05 = fn -> evaluate(x05);

    if (!isValid(y05)) return;

    const double yExpected = (y0 + y1) / 2;

    /*
     * Check for asymptote
     * y0 and y1 have to be large and opposite
     */
    if (std::abs(y0) > maxJump && std::abs(y1) > maxJump && (y0 > 0) != (y1 > 0)) return;

    /*
     * Compares to a linear function
     */
    if (std::abs(y05 - yExpected) < tolerance)
    {
        segment.push_back({x1, y1});
        return;
    }

    subdivide(segment, x0, y0, x05, y05, depth + 1);
    subdivide(segment, x05, y05, x1, y1, depth + 1);
}

/*
 * V2 of the graphing function. I've read a bit on the topic and this here is a bit more complex
 * It checks for gaps in continuity itself instead of offloading that to the drawing function
 */
std::vector<std::vector<Point>> Grapher::GraphExpression(const double minX, const double maxX, const double step) const
{
    std::vector<std::vector<Point>> out;
    std::vector<Point> segment;

    double x0 = minX;
    double y0 = fn -> evaluate(minX);

    bool valid0 = isValid(x0);

    if (valid0) segment.push_back({x0, y0});

    double x1 = minX + step;

    while (x1 <= maxX)
    {
        const double y1 = fn -> evaluate(x1);

        bool valid1 = isValid(y1);

        // To see if there's been a sudden jump/basically if the function isn't continuous enough
        if (!valid0 || !valid1 || std::abs(y1 - y0) > maxJump)
        {
            if (segment.size() > 1) out.push_back(segment);
            segment.clear();

            if (valid1) segment.push_back({x1, y1});

            x0 = x1;
            y0 = y1;

            valid0 = valid1;

            continue;
        }

        subdivide(segment, x0, y0, x1, y1);

        x0 = x1;
        y0 = y1;

        valid0 = valid1;

        x1 += step;
    }
    if (segment.size() > 1) out.push_back(segment);

    return out;
}
