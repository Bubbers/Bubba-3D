#include <GameObject.h>
#include <Collider.h>
#include "Octree.h"

bool rayTriangle(float3 r_o, float3 r_d, float3 v1, float3 v2, float3 v3, float *ins);

Collider::Collider(Octree *tree) : tree(tree) {
}


Collider::~Collider() {
}

float Collider::rayIntersection(float3 rayOrigin, float3 rayVec) {
    std::vector<Triangle *> geometry;
    float3 rayVecInv = make_vector(1.0f / rayVec.x, 1.0f / rayVec.y, 1.0f / rayVec.z);
    if (rayVec.x < 0.001f) rayVecInv.x = 0.0f;
    if (rayVec.y < 0.001f) rayVecInv.y = 0.0f;
    if (rayVec.z < 0.001f) rayVecInv.z = 0.0f;


    tree->getGeometry(rayOrigin, rayVecInv, &geometry);

    float minIns = NULL;
    for (int i = 0; i < geometry.size(); i++) {
        Triangle t = *geometry[i];
        float ins;

        if (rayTriangle(rayOrigin, rayVec, t.p1, t.p2, t.p3, &ins)) {
            if ((minIns) > (ins) || minIns == NULL) {
                minIns = ins;
            }
        }
    }

    if (minIns == NULL || geometry.size() == 0) {
        return 0;
    }

    return -minIns;
}


void Collider::insertAll() {
    tree->insertAll(ts);
}


void Collider::addGameObject(GameObject *gameObject) {
    std::vector<Triangle *> gobTs = gameObject->getTriangles();
    ts.insert(ts.end(), gobTs.begin(), gobTs.end());
}


bool rayTriangle(float3 r_o, float3 r_d, float3 v1, float3 v2, float3 v3, float *ins) {
    float3 e2 = v3 - v1;       // second edge
    float3 e1 = v2 - v1;       // first edge


    float3 r = cross(r_d, e2);  // (d X e2) is used two times in the formula
    // so we store it in an appropriate vector
    float3 s = r_o - v1;       // translated ray origin
    float a = dot(e1, r);    // a=(d X e2)*e1
    float f = 1.0f / a;           // slow division*
    float3 q = cross(s, e1);
    float u = dot(s, r);
    bool frontfacing = true;
    float eps = 0.0001f;
    if (a > eps)            // eps is the machine fpu epsilon (precision),
        // or a very small number :)
    { // Front facing triangle...
        if ((u < 0) || (u > a)) return false;
        float v = dot(r_d, q);
        if ((v < 0) || (u + v > a)) return false;
    }
    else if (a < -eps) { // Back facing triangle...
        frontfacing = false;
        if ((u > 0) || (u < a)) return false;
        float v = dot(r_d, q);
        if ((v > 0) || (u + v < a)) return false;
    }
    else return false; // Ray parallel to triangle plane
    float t = f * dot(e2, q);
    float v = dot(r_d, q);
    u = u * f;
    v = v * f;

    *ins = t;

    return true;
}

float getPointDistanceToPlane(float3 point, float3 normalVector, float d) {
    return normalVector.x * point.x + normalVector.y * point.y + normalVector.z * point.z + d;
}


bool triangleTriangleIntersection(Triangle *t1, Triangle *t2) {

    //T1
    float3 normalVectorT1 = cross(t1->p2 - t1->p1, t1->p3 - t1->p1);
    float d1 = -normalVectorT1.x * t1->p1.x + -normalVectorT1.y * t1->p1.y + -normalVectorT1.z * t1->p1.z;

    float3 normalVectorT2 = cross(t2->p2 - t2->p1, t2->p3 - t2->p1);
    float d2 = -normalVectorT2.x * t2->p1.x + -normalVectorT2.y * t2->p1.y + -normalVectorT2.z * t2->p1.z;

    float t2DistToT1V1 = getPointDistanceToPlane(t1->p1, normalVectorT2, d2);
    float t2DistToT1V2 = getPointDistanceToPlane(t1->p2, normalVectorT2, d2);
    float t2DistToT1V3 = getPointDistanceToPlane(t1->p3, normalVectorT2, d2);

    if (t2DistToT1V1 != 0 && t2DistToT1V2 != 0 && t2DistToT1V3 != 0) {
        if (t2DistToT1V1 > 0 && t2DistToT1V2 > 0 && t2DistToT1V3 > 0) { return false; }
        if (t2DistToT1V1 < 0 && t2DistToT1V2 < 0 && t2DistToT1V3 < 0) { return false; }
    }

    //T2
    float t1DistToT1V1 = getPointDistanceToPlane(t2->p1, normalVectorT1, d1);
    float t1DistToT1V2 = getPointDistanceToPlane(t2->p2, normalVectorT1, d1);
    float t1DistToT1V3 = getPointDistanceToPlane(t2->p3, normalVectorT1, d1);

    if (t1DistToT1V1 != 0 && t1DistToT1V2 != 0 && t1DistToT1V3 != 0) {
        if (t1DistToT1V1 > 0 && t1DistToT1V2 > 0 && t1DistToT1V3 > 0) { return false; }
        if (t1DistToT1V1 < 0 && t1DistToT1V2 < 0 && t1DistToT1V3 < 0) { return false; }
    }

    if (t2DistToT1V1 != 0 && t2DistToT1V2 != 0 && t2DistToT1V3 != 0) {
        if (t2DistToT1V1 > 0 && t2DistToT1V2 > 0 && t2DistToT1V3 > 0) { return false; }
        if (t2DistToT1V1 < 0 && t2DistToT1V2 < 0 && t2DistToT1V3 < 0) { return false; }
    }


    float3 lineDirection = cross(normalVectorT1, normalVectorT2);
    float3 projectionT1V1 = lineDirection * t1->p1;
    float3 projectionT1V2 = lineDirection * t1->p2;
    float3 projectionT1V3 = lineDirection * t1->p3;
    float3 projectedLinePoint1T1 =
            projectionT1V1 + (projectionT1V2 - projectionT1V1) * (t1DistToT1V1 / (t1DistToT1V1 - t1DistToT1V2));
    float3 projectedLinePoint2T1 =
            projectionT1V2 + (projectionT1V3 - projectionT1V2) * (t1DistToT1V2 / (t1DistToT1V2 - t1DistToT1V3));
    float3 t1Interval = projectedLinePoint1T1 - projectedLinePoint2T1;

    float3 projectionT2V1 = lineDirection * t2->p1;
    float3 projectionT2V2 = lineDirection * t2->p2;
    float3 projectionT2V3 = lineDirection * t2->p3;
    float3 projectedLinePoint1T2 =
            projectionT2V1 + (projectionT2V2 - projectionT2V1) * (t2DistToT1V1 / (t2DistToT1V1 - t2DistToT1V2));
    float3 projectedLinePoint2T2 =
            projectionT2V2 + (projectionT2V3 - projectionT2V2) * (t2DistToT1V2 / (t2DistToT1V2 - t2DistToT1V3));
    float3 t2Interval = projectedLinePoint1T2 - projectedLinePoint2T2;

    float3 perpendicularLine = cross(t1Interval, t2Interval);

    if (t2DistToT1V1 == 0 && t2DistToT1V2 == 0 && t2DistToT1V3 == 0) {
        float3 lineSegment = projectedLinePoint2T2 - projectedLinePoint2T1;

        if (fabs(dot(lineSegment, perpendicularLine)) > 0.01) { //Lines are not coplanar
            return false;
        }

        //TOFDO
    }


    if (t1Interval.x * perpendicularLine.x + t1Interval.y * perpendicularLine.y +
        t1Interval.z * perpendicularLine.z ==
        t2Interval.x * perpendicularLine.x + t2Interval.y * perpendicularLine.y +
        t2Interval.z * perpendicularLine.z) {
        return true;
    }

    return false;

}

bool AabbAabbintersection(AABB *aabb1, AABB *aabb2) {
    if (aabb1->maxV.x < aabb2->minV.x) { return false; }
    if (aabb1->maxV.y < aabb2->minV.y) { return false; }
    if (aabb1->maxV.z < aabb2->minV.z) { return false; }

    if (aabb2->maxV.x < aabb1->minV.x) { return false; }
    if (aabb2->maxV.y < aabb1->minV.y) { return false; }
    if (aabb2->maxV.z < aabb1->minV.z) { return false; }

    return true;
}

bool lineSegmentAabbIntersection(AABB *aabb, float3 linesegment) {
    return true; //TODO
}

bool triangleAabbIntersection(AABB *aabb, Triangle *triangle) {
    return lineSegmentAabbIntersection(aabb, triangle->p2 - triangle->p1) ||
           lineSegmentAabbIntersection(aabb, triangle->p2 - triangle->p3) ||
           lineSegmentAabbIntersection(aabb, triangle->p3 - triangle->p1);
}

Triangle *multiplyTriangleWithModelMatrix(Triangle *triangle, float4x4 *modelMatrix) {
    float4 p1 = make_vector(triangle->p1.x, triangle->p1.y, triangle->p1.z, 1.0f);
    float4 p2 = make_vector(triangle->p2.x, triangle->p2.y, triangle->p2.z, 1.0f);
    float4 p3 = make_vector(triangle->p3.x, triangle->p3.y, triangle->p3.z, 1.0f);

    float4 convertedP1 = *modelMatrix * p1;
    float4 convertedP2 = *modelMatrix * p2;
    float4 convertedP3 = *modelMatrix * p3;

    float3 p1f3 = make_vector(convertedP1.x, convertedP1.y, convertedP1.z);
    float3 p2f3 = make_vector(convertedP2.x, convertedP2.y, convertedP2.z);
    float3 p3f3 = make_vector(convertedP3.x, convertedP3.y, convertedP3.z);

    return new Triangle(p1f3, p2f3, p3f3);
}

AABB *multiplyAABBWithModelMatrix(AABB *aabb, float4x4 *modelMatrix) {
    float4 maxVB4 = make_vector(aabb->maxV.x, aabb->maxV.y, aabb->maxV.z, 1.0f);
    float4 minVB4 = make_vector(aabb->minV.x, aabb->minV.y, aabb->minV.z, 1.0f);

    float4 convertedP1 = *modelMatrix * maxVB4;
    float4 convertedP2 = *modelMatrix * minVB4;

    float3 maxV = make_vector(convertedP1.x, convertedP1.y, convertedP1.z);
    float3 minV = make_vector(convertedP2.x, convertedP2.y, convertedP2.z);

    maxV.x = maxV.x > minV.x ? maxV.x : minV.x;
    maxV.y = maxV.y > minV.y ? maxV.y : minV.y;
    maxV.z = maxV.z > minV.z ? maxV.z : minV.z;

    minV.x = minV.x < maxV.x ? minV.x : maxV.x;
    minV.y = minV.y < maxV.y ? minV.y : maxV.y;
    minV.z = minV.z < maxV.z ? minV.z : maxV.z;

    AABB *convertedAabb = new AABB();
    convertedAabb->maxV = maxV;
    convertedAabb->minV = minV;
    return convertedAabb;

}

bool octreeOctreeIntersection(Octree *object1Octree, float4x4 *object1ModelMatrix, Octree *object2Octree,
                              float4x4 *object2ModelMatrix) {

    AABB *object1Aabb = multiplyAABBWithModelMatrix(object1Octree->getAABB(), object1ModelMatrix);
    AABB *object2Aabb = multiplyAABBWithModelMatrix(object2Octree->getAABB(), object2ModelMatrix);

    if (!AabbAabbintersection(object1Aabb, object2Aabb)) {
        return false;
    }

    if (!object1Octree->hasChildren() && !object2Octree->hasChildren()) {
        std::vector<Triangle *> *triangles1 = object1Octree->getTriangles();
        std::vector<Triangle *> *triangles2 = object2Octree->getTriangles();

        for (auto t1 = triangles1->begin(); t1 != triangles1->end(); t1++) {
            Triangle *triangle1 = multiplyTriangleWithModelMatrix(*t1, object1ModelMatrix);
            for (auto t2 = triangles2->begin(); t2 != triangles2->end(); t2++) {
                Triangle *triangle2 = multiplyTriangleWithModelMatrix(*t2, object2ModelMatrix);

                if (triangleTriangleIntersection(triangle1, triangle2)) {
                    return true;
                }

            }
        }
    } else if (object1Octree->hasChildren() && object2Octree->hasChildren()) {
                if (object1Aabb->getSize() > object2Aabb->getSize()) {
                    std::vector<Octree *> object1Children;

                    object1Octree->getChildren(&object1Children);
                    for (auto it = object1Children.begin(); it != object1Children.end(); it++) {
                        if (octreeOctreeIntersection(*it, object1ModelMatrix, object2Octree, object2ModelMatrix)) {
                            return true;
                        }
                    }
                } else {
                    std::vector<Octree *> object2Children;

                    object2Octree->getChildren(&object2Children);
                    for (auto it = object2Children.begin(); it != object2Children.end(); it++) {
                        if (octreeOctreeIntersection(object1Octree, object1ModelMatrix, *it, object2ModelMatrix)) {
                            return true;
                        }
                    }

            }
        } else if (!object1Octree->hasChildren() && object2Octree->hasChildren()) {

                std::vector<Octree *> object2Children;
                object2Octree->getChildren(&object2Children);

                for (auto it = object2Children.begin(); it != object2Children.end(); it++) {
                    if(octreeOctreeIntersection(object1Octree, object1ModelMatrix, *it, object2ModelMatrix)) {
                        return true;
                    }

                }

        } else {

                std::vector<Octree *> object1Children;
                object1Octree->getChildren(&object1Children);

                for (auto it = object1Children.begin(); it != object1Children.end(); it++) {
                    if(octreeOctreeIntersection(*it, object1ModelMatrix, object2Octree, object2ModelMatrix)) {
                        return true;
                    }
                }

        }

    return false;
}






























