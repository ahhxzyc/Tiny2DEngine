#pragma once

////////////////////////////////////////////////////////////////////////
//
//  Axis aligned bounding box implementation.
//
////////////////////////////////////////////////////////////////////////

#include <Eigen/Dense>
#include <algorithm>

using Eigen::Vector3f;
using Eigen::Vector2i;
using Eigen::Vector2f;


struct AABB3D {
    Vector3f minp, maxp;

    AABB3D(): minp(), maxp() {}
    AABB3D(Vector3f a, Vector3f b) {
        minp[0] = (std::min)(a[0], b[0]);
        minp[1] = (std::min)(a[1], b[1]);
        minp[2] = (std::min)(a[2], b[2]);
        maxp[0] = (std::max)(a[0], b[0]);
        maxp[1] = (std::max)(a[1], b[1]);
        maxp[1] = (std::max)(a[2], b[2]);
    }
};


struct AABB2D {
    Vector2i minp, maxp;

    AABB2D(): minp(), maxp() {}
    AABB2D(Vector2i a, Vector2i b) {
        minp[0] = (std::min)(a[0], b[0]);
        minp[1] = (std::min)(a[1], b[1]);
        maxp[0] = (std::max)(a[0], b[0]);
        maxp[1] = (std::max)(a[1], b[1]);
    }

    Vector2i vmin(const Vector2i &a, const Vector2i &b) {
        Vector2i v;
        v[0] = (std::min)(a[0], b[0]);
        v[1] = (std::min)(a[1], b[1]);
        return v;
    }

    Vector2i vmax(const Vector2i &a, const Vector2i &b) {
        Vector2i v;
        v[0] = (std::max)(a[0], b[0]);
        v[1] = (std::max)(a[1], b[1]);
        return v;
    }

    bool contain(Vector2i p)        {return p[0] >= minp[0] && p[0] <= maxp[0] && p[1] >= minp[1] && p[1] <= maxp[1];}
    bool contain(AABB2D box)        {return contain(box.minp) && contain(box.maxp);}

    AABB2D intersect(AABB2D box)    {AABB2D b; b.minp = vmax(box.minp, minp); b.maxp = vmin(box.maxp, maxp); return b;}
};

struct AABB2Df {
    Vector2f minp, maxp;

    AABB2Df(Vector2f a, Vector2f b) {
        minp[0] = (std::min)(a[0], b[0]);
        minp[1] = (std::min)(a[1], b[1]);
        maxp[0] = (std::max)(a[0], b[0]);
        maxp[1] = (std::max)(a[1], b[1]);
    }

    Vector2f vmin(const Vector2f &a, const Vector2f &b) {
        Vector2f v;
        v[0] = (std::min)(a[0], b[0]);
        v[1] = (std::min)(a[1], b[1]);
        return v;
    }

    Vector2f vmax(const Vector2f &a, const Vector2f &b) {
        Vector2f v;
        v[0] = (std::max)(a[0], b[0]);
        v[1] = (std::max)(a[1], b[1]);
        return v;
    }


    bool contain(Vector2f p)        {return p[0] >= minp[0] && p[0] <= maxp[0] && p[1] >= minp[1] && p[1] <= maxp[1];}
    bool contain(AABB2Df box)       {return contain(box.minp) && contain(box.maxp);}
    void add(Vector2f p)            {minp = vmin(minp, p); maxp = vmax(maxp, p);}
};

