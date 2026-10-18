#include <algorithm>
#include <cmath>
#include "graham_scan.h"

ConvexHull GrahamScan(std::vector<Point>& points) {
    /* Find the point with the lowest y-coordinate, breaking ties by x-coordinate */
    auto it = std::min_element(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.y < b.y || (std::fabs(a.y - b.y) < 1e-12 && a.x < b.x);
    });
    std::swap(points[0], *it);
    Point p0 = points[0];

    /* Sort the points by polar angle with p0 */
    std::sort(points.begin() + 1, points.end(), [p0](const Point& a, const Point& b) {
        double dx1 = a.x - p0.x;
        double dy1 = a.y - p0.y;
        double dx2 = b.x - p0.x;
        double dy2 = b.y - p0.y;
        double crossProduct = dx1 * dy2 - dy1 * dx2;
        if (std::fabs(crossProduct) < 1e-12) {
            if (dx1*dx1 + dy1*dy1 >= dx2*dx2 + dy2*dy2) {
                return false;
            }
            else {
                return true;
            }
        }
        return (crossProduct > 0);
    });

    /* Remove aligned points */
    for (size_t i = 1; i < points.size() - 1; ++i) {
        double dx1 = points[i].x - p0.x;
        double dy1 = points[i].y - p0.y;
        double dx2 = points[i + 1].x - p0.x;
        double dy2 = points[i + 1].y - p0.y;
        if (std::fabs(dx1 * dy2 - dy1 * dx2) < 1e-12) {
            points.erase(points.begin() + i);
            --i;
        }
    }

    /* Build the convex hull using a stack */
    static ConvexHull hull;
    hull.reserve(128);
    hull.clear();
    hull.emplace_back(points[0]);
    hull.emplace_back(points[1]);

    for (size_t i = 2; i < points.size(); ++i) {
        while (hull.size() > 1) {
            Point p1 = hull[hull.size() - 2];
            Point p2 = hull[hull.size() - 1];
            Point p3 = points[i];

            double crossProduct = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
            if (crossProduct <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.emplace_back(points[i]);
    }

    return hull;
}
