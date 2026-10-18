#ifndef _POINT_H
#define _POINT_H

struct Point {
    double x;
    double y;
    double vx;
    double vy;
    double body;

    Point()
        : x(0), y(0), vx(0), vy(0), body(0) {}
    Point(double px, double py, double pvx, double pvy, double pbody)
        : x(px), y(py), vx(pvx), vy(pvy), body(pbody) {}
};


#endif
