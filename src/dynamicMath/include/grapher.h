#ifndef GRAPHERTOOLKIT_GRAPHER_H
#define GRAPHERTOOLKIT_GRAPHER_H

#include "dynamicMath.h"

namespace DynMath
{
    struct Point
    {
        double x;
        double y;
    };

    class Grapher
    {
    public:
        explicit Grapher(DynMath::Expression *expr);
        [[nodiscard]] std::vector<std::vector<Point>> GraphExpression(double minX, double maxX, double step) const;
    private:
        DynMath::Expression *fn;
        bool subdivide(std::vector<Point> &segment, double x0, double y0, double x1, double y1, int depth = 0) const;
    };
}

#endif
