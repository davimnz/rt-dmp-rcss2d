#include <cmath>

#include "constants.h"
#include "estimate.h"

std::vector<Point> NaiveEstimate(Point target, int angularDiv, int maxIterations) {

    std::vector<Point> allPoints;
    Point initialPoint;

    size_t beginIdx = 0;
    size_t endIdx = 0;

    allPoints.emplace_back(target);
    for (int i = 0; i < maxIterations; ++i) {
        endIdx = allPoints.size();
        for (size_t j = beginIdx; j < endIdx; ++j) {
            initialPoint = allPoints[j];
            for (int k = 0; k < angularDiv; ++k) {
                Point novelPoint;
                double angle = -M_PI + k * (2 * M_PI / angularDiv);
                double accelerationX = MAX_ACCELERATION * cos(angle);
                double accelerationY = MAX_ACCELERATION * sin(angle);
                novelPoint.vx = initialPoint.vx + accelerationX;
                novelPoint.vy = initialPoint.vy + accelerationY;
                novelPoint.x = initialPoint.x + novelPoint.vx;
                novelPoint.y = initialPoint.y + novelPoint.vy;
                novelPoint.vx *= MIN_VELOCITY_DECAY;
                novelPoint.vy *= MIN_VELOCITY_DECAY;
                allPoints.emplace_back(novelPoint);
            }
        }
        beginIdx = allPoints.size() - angularDiv * (endIdx - beginIdx);
    }

    return allPoints;
};

ConvexHullVector ConvexHullEstimate(Point target, int angularDiv, int maxIterations) {

    static std::vector<Point> runPoints;
    static ConvexHullVector chVec;
    static ConvexHull ch;
    static Point initialPoint;

    chVec.reserve(maxIterations);
    chVec.clear();
    ch.reserve(1024);
    ch.clear();

    ch.emplace_back(target);
    for (int i = 0; i < maxIterations; ++i) {
        runPoints.clear();
        for (size_t j = 0; j < ch.size(); ++j) {
            initialPoint = ch[j];
            for (int k = 0; k < angularDiv; ++k) {
                Point novelPoint;
                double angle = -M_PI + k * (2 * M_PI / angularDiv);
                double accelerationX = MAX_ACCELERATION * cos(angle);
                double accelerationY = MAX_ACCELERATION * sin(angle);
                novelPoint.vx = initialPoint.vx + accelerationX;
                novelPoint.vy = initialPoint.vy + accelerationY;
                novelPoint.x = initialPoint.x + novelPoint.vx;
                novelPoint.y = initialPoint.y + novelPoint.vy;
                novelPoint.vx *= MIN_VELOCITY_DECAY;
                novelPoint.vy *= MIN_VELOCITY_DECAY;
                runPoints.emplace_back(novelPoint);
            }
        }
        ch = GrahamScan(runPoints);
        chVec.emplace_back(ch);
    }
    return chVec;
};

ConvexHullVector ConvexHullAdaptiveRateEstimate(Point target, int angularDiv, int maxIterations) {

    static std::vector<Point> runPoints;
    static ConvexHullVector chVec;
    static ConvexHull ch;
    static Point initialPoint;

    chVec.reserve(maxIterations);
    chVec.clear();
    ch.reserve(1024);
    ch.clear();

    ch.emplace_back(target);
    for (int i = 0; i < maxIterations; ++i) {
        runPoints.clear();
        for (size_t j = 0; j < ch.size(); ++j) {
            initialPoint = ch[j];
            for (int k = 0; k < angularDiv; ++k) {
                Point novelPoint;
                double angle = -M_PI + k * (2 * M_PI / angularDiv);
                double angleDiff = std::fabs(std::atan2(std::sin((target.body * (M_PI / 180.0)) - angle), std::cos((target.body * (M_PI / 180.0)) - angle)));
                double dirRate = 0;
                if (std::fabs(angleDiff) > M_PI / 2) {
                    dirRate = DASH_POWER_RATE_BACK - (DASH_POWER_RATE_BACK - DASH_POWER_RATE_SIDE) * (1 - (((angleDiff * 180.0 / M_PI) - 90) / 90));
                } else {
                    dirRate = DASH_POWER_RATE_SIDE + (1 - DASH_POWER_RATE_SIDE) * (1 - ((angleDiff * 180.0 / M_PI) / 90));
                }
                double accelerationX = MAX_ACCELERATION * dirRate * cos(angle);
                double accelerationY = MAX_ACCELERATION * dirRate * sin(angle);
                novelPoint.vx = initialPoint.vx + accelerationX;
                novelPoint.vy = initialPoint.vy + accelerationY;
                novelPoint.x = initialPoint.x + novelPoint.vx;
                novelPoint.y = initialPoint.y + novelPoint.vy;
                novelPoint.vx *= MIN_VELOCITY_DECAY;
                novelPoint.vy *= MIN_VELOCITY_DECAY;
                runPoints.emplace_back(novelPoint);
            }
        }
        ch = GrahamScan(runPoints);
        chVec.emplace_back(ch);
    }
    return chVec;
}
