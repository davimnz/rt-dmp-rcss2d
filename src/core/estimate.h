#ifndef _ESTIMATE_H_
#define _ESTIMATE_H_

#include "graham_scan.h"

std::vector<Point> NaiveEstimate(Point target, int angularDiv, int maxIterations);

ConvexHullVector ConvexHullEstimate(Point target, int angularDiv, int maxIterations);

ConvexHullVector ConvexHullAdaptiveRateEstimate(Point target, int angularDiv, int maxIterations);

#endif
