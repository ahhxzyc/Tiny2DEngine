#pragma once

#include <Eigen/Dense>

using Eigen::Vector3f;

struct Triangle3D {
    Vector3f v[3] = {};
    Vector3f a() const      {return v[0];}
    Vector3f b() const      {return v[1];}
    Vector3f c() const      {return v[2];}
    Vector3f normal() const {return (v[1] - v[0]).cross(v[2] - v[0]).normalized();}
    Vector3f positive_normal() const {
        Vector3f n = normal();
        for (int i = 0; i < 3; i ++ )   n[i] = fabs(n[i]);
        return n;
    }
};

struct Triangle2D {
    Vector2f v[3] = {};
    Vector2f a() const {return v[0];}
    Vector2f b() const {return v[1];}
    Vector2f c() const {return v[2];}
};