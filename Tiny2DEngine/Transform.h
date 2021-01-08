#pragma once

#include <Eigen/Dense>

using Eigen::Matrix4f;
using Eigen::Vector3f;

struct Transform {

    Vector3f translation;
    Matrix4f rotation;

    Transform() {
        translation << 0, 0, 0;
        rotation = Matrix4f::Identity();
    }

    Transform translate(Vector3f v) {
        Transform t = *this;
        t.translation += v;
        return t;
    }

    Transform rotate(Matrix4f rot) {
        Transform t = *this;
        t.rotation = rot * t.rotation;
        return t;
    }
    
    Matrix4f getMatrix() {
        Matrix4f mat = Matrix4f::Identity();
        Matrix4f trans;
        trans << 1, 0, 0, translation[0],
                 0, 1, 0, translation[1],
                 0, 0, 1, translation[2],
                 0, 0, 0,              1;
        return trans * rotation * mat;
    }
};