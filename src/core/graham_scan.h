#ifndef _GRAHAM_SCAN_H_
#define _GRAHAM_SCAN_H_

#include <vector>
#include "point.h"

typedef std::vector<Point> ConvexHull;
typedef std::vector<ConvexHull> ConvexHullVector;

ConvexHull GrahamScan(std::vector<Point>& points);

#endif
