#pragma once

#include <Eigen/Dense>
#include <iostream>

using Eigen::Matrix4f;
using Eigen::Vector3f;
using Eigen::Vector2f;

class Trackball {
private:
    float radius = 1.f;
    const float PI = acosf(-1.f);
    Vector2f prev;
    Vector2f cur;

public:
    Matrix4f getRotation() {
        Matrix4f mat = Matrix4f::Identity();
        if (prev == cur) {
            return mat;
        }
        Vector3f prev3 = toVectorOnBall(prev);
        Vector3f cur3 = toVectorOnBall(cur);
        float angle = acosf(prev3.dot(cur3) / (prev3.norm() * cur3.norm()));
        // std::cout << "angle: " << angle << std::endl;
        Vector3f dir = prev3.cross(cur3).normalized();
        float c = cos(angle);
        float s = sin(angle);
        float x = dir[0];
        float y = dir[1];
        float z = dir[2];
        mat <<  (1-c)*x*x+c,    (1-c)*x*y-s*z,  (1-c)*x*z+s*y,  0,
                (1-c)*x*y+s*z,  (1-c)*y*y+c,    (1-c)*y*z-s*x,  0,
                (1-c)*x*z-s*y,  (1-c)*y*z+s*x,  (1-c)*z*z+c,    0,
                0,              0,              0,              1;
        return mat;
    }

    Vector3f toVectorOnBall(Vector2f v) {
        Vector3f retVector;
        float arc = v.norm();
        float a = arc / radius;     // angle between retVector and vertical direction(the direction that points outward through the screen)
        float b = atan2f(v[1], v[0]);
        retVector <<    radius * sinf(a) * cosf(b),
                        radius * sinf(a) * sinf(b),
                        radius * cosf(a);
        return retVector;
    }

    // Update mouse position, x & y should be normalized to (-1,1)
    void update(float x, float y) {
        cur = Vector2f(x, y);
    }
    void set() {
        prev = cur;
    }
    
};