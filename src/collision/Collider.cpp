#include <GameObject.h>
#include <Collider.h>
#include "Octree.h"

#define EPSILON 0.00001f
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


#define ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1) \
              isect0=VV0+(VV1-VV0)*D0/(D0-D1);    \
              isect1=VV0+(VV2-VV0)*D0/(D0-D2);


#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }


#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  float a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return 1;                 \
  }                                         \
}

int coplanar_tri_tri(float3 N,float3 V0,float3 V1,float3 V2,
                     float3 U0,float3 U1,float3 U2)
{
    float A[3];
    short i0,i1;
    /* first project onto an axis-aligned plane, that maximizes the area */
    /* of the triangles, compute indices: i0,i1. */
    A[0]=fabs(N[0]);
    A[1]=fabs(N[1]);
    A[2]=fabs(N[2]);
    if(A[0]>A[1])
    {
        if(A[0]>A[2])
        {
            i0=1;      /* A[0] is greatest */
            i1=2;
        }
        else
        {
            i0=0;      /* A[2] is greatest */
            i1=1;
        }
    }
    else   /* A[0]<=A[1] */
    {
        if(A[2]>A[1])
        {
            i0=0;      /* A[2] is greatest */
            i1=1;
        }
        else
        {
            i0=0;      /* A[1] is greatest */
            i1=2;
        }
    }

    /* test all edges of triangle 1 against the edges of triangle 2 */
    EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2);
    EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2);
    EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2);

    /* finally, test if tri1 is totally contained in tri2 or vice versa */
    POINT_IN_TRI(V0,U0,U1,U2);
    POINT_IN_TRI(U0,V0,V1,V2);

    return 0;
}

#define COMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,isect0,isect1) \
  if(D0D1>0.0f)                                         \
  {                                                     \
    /* here we know that D0D2<=0.0 */                   \
    /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
    ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
  }                                                     \
  else if(D0D2>0.0f)                                    \
  {                                                     \
    /* here we know that d0d1<=0.0 */                   \
    ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
  }                                                     \
  else if(D1*D2>0.0f || D0!=0.0f)                       \
  {                                                     \
    /* here we know that d0d1<=0.0 or that D0!=0.0 */   \
    ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1);          \
  }                                                     \
  else if(D1!=0.0f)                                     \
  {                                                     \
    ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
  }                                                     \
  else if(D2!=0.0f)                                     \
  {                                                     \
    ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
  }                                                     \
  else                                                  \
  {                                                     \
    /* triangles are coplanar */                        \
    return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2);      \
  }


/* sort so that a<=b */
#define SORT(a,b)       \
             if(a>b)    \
             {          \
               float c; \
               c=a;     \
               a=b;     \
               b=c;     \
             }


bool triangleTriangleIntersection(Triangle *t1, Triangle *t2)
{
    float3 V0 = t1->p1;
    float3 V1 = t1->p2;
    float3 V2 = t1->p3;
    float3 U0 = t2->p1;
    float3 U1 = t2->p2;
    float3 U2 = t2->p3;


    float3 E1,E2;
    float3 N1,N2;
    float d1,d2;
    float du0,du1,du2,dv0,dv1,dv2;
    float3 D;
    float isect1[2], isect2[2];
    float du0du1,du0du2,dv0dv1,dv0dv2;
    short index;
    float vp0,vp1,vp2;
    float up0,up1,up2;
    float b,c,max;

    /* compute plane equation of triangle(V0,V1,V2) */
    E1 = V1-V0;
    E2 = V2-V0;
    N1 = cross(E1,E2);
    d1 = -dot(N1,V0);
    /* plane equation 1: N1.X+d1=0 */

    /* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
    du0=dot(N1,U0)+d1;
    du1=dot(N1,U1)+d1;
    du2=dot(N1,U2)+d1;

    /* coplanarity robustness check */
#if USE_EPSILON_TEST==TRUE
    if(fabs(du0)<EPSILON) du0=0.0;
    if(fabs(du1)<EPSILON) du1=0.0;
    if(fabs(du2)<EPSILON) du2=0.0;
#endif
    du0du1=du0*du1;
    du0du2=du0*du2;

    if(du0du1>0.0f && du0du2>0.0f) /* same sign on all of them + not equal 0 ? */
        return 0;                    /* no intersection occurs */

    /* compute plane of triangle (U0,U1,U2) */
    E1 = U1 - U0;
    E2 = U2 - U0;
    N2 = cross(E1, E2);
    d2 = -dot(N2,U0);
    /* plane equation 2: N2.X+d2=0 */

    /* put V0,V1,V2 into plane equation 2 */
    dv0=dot(N2,V0)+d2;
    dv1=dot(N2,V1)+d2;
    dv2=dot(N2,V2)+d2;

#if USE_EPSILON_TEST==TRUE
    if(fabs(dv0)<EPSILON) dv0=0.0;
    if(fabs(dv1)<EPSILON) dv1=0.0;
    if(fabs(dv2)<EPSILON) dv2=0.0;
#endif

    dv0dv1=dv0*dv1;
    dv0dv2=dv0*dv2;

    if(dv0dv1>0.0f && dv0dv2>0.0f) /* same sign on all of them + not equal 0 ? */
        return false;                    /* no intersection occurs */

    /* compute direction of intersection line */
    D = cross(N1,N2);

    /* compute and index to the largest component of D */
    max=fabs(D[0]);
    index=0;
    b=fabs(D[1]);
    c=fabs(D[2]);
    if(b>max) max=b,index=1;
    if(c>max) max=c,index=2;

    /* this is the simplified projection onto L*/
    vp0=V0[index];
    vp1=V1[index];
    vp2=V2[index];

    up0=U0[index];
    up1=U1[index];
    up2=U2[index];

    /* compute interval for triangle 1 */
    COMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,isect1[0],isect1[1]);

    /* compute interval for triangle 2 */
    COMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,isect2[0],isect2[1]);

    SORT(isect1[0],isect1[1]);
    SORT(isect2[0],isect2[1]);

    if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return false;
    return true;
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

Triangle multiplyTriangleWithModelMatrix(Triangle *triangle, float4x4 *modelMatrix) {
    float4 p1 = make_vector(triangle->p1.x, triangle->p1.y, triangle->p1.z, 1.0f);
    float4 p2 = make_vector(triangle->p2.x, triangle->p2.y, triangle->p2.z, 1.0f);
    float4 p3 = make_vector(triangle->p3.x, triangle->p3.y, triangle->p3.z, 1.0f);

    float4 convertedP1 = *modelMatrix * p1;
    float4 convertedP2 = *modelMatrix * p2;
    float4 convertedP3 = *modelMatrix * p3;

    float3 p1f3 = make_vector(convertedP1.x, convertedP1.y, convertedP1.z);
    float3 p2f3 = make_vector(convertedP2.x, convertedP2.y, convertedP2.z);
    float3 p3f3 = make_vector(convertedP3.x, convertedP3.y, convertedP3.z);

    return Triangle(p1f3, p2f3, p3f3);
}

void calculateAndUpdateMinMax(float3 point, float4x4* modelMatrix, float3 *minV, float3 *maxV) {
    float4 convertedValue = *modelMatrix * make_vector(point.x, point.y, point.z, 1.0f);

    if( maxV->x < convertedValue.x) { maxV->x = convertedValue.x;}
    if( maxV->y < convertedValue.y) { maxV->y = convertedValue.y;}
    if( maxV->z < convertedValue.z) { maxV->z = convertedValue.z;}

    if( minV->x > convertedValue.x) { minV->x = convertedValue.x;}
    if( minV->y > convertedValue.y) { minV->y = convertedValue.y;}
    if( minV->z > convertedValue.z) { minV->z = convertedValue.z;}
}


AABB multiplyAABBWithModelMatrix(AABB *aabb, float4x4 *modelMatrix) {
    AABB convertedAabb = AABB();

    calculateAndUpdateMinMax(make_vector(aabb->maxV.x,aabb->maxV.y,aabb->maxV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);
    calculateAndUpdateMinMax(make_vector(aabb->maxV.x,aabb->maxV.y,aabb->minV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);
    calculateAndUpdateMinMax(make_vector(aabb->maxV.x,aabb->minV.y,aabb->maxV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);
    calculateAndUpdateMinMax(make_vector(aabb->maxV.x,aabb->minV.y,aabb->minV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);

    calculateAndUpdateMinMax(make_vector(aabb->minV.x,aabb->maxV.y,aabb->maxV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);
    calculateAndUpdateMinMax(make_vector(aabb->minV.x,aabb->maxV.y,aabb->minV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);
    calculateAndUpdateMinMax(make_vector(aabb->minV.x,aabb->minV.y,aabb->maxV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);
    calculateAndUpdateMinMax(make_vector(aabb->minV.x,aabb->minV.y,aabb->minV.z), modelMatrix, &convertedAabb.minV, &convertedAabb.maxV);

    return convertedAabb;
}

bool octreeOctreeIntersection(Octree *object1Octree, float4x4 *object1ModelMatrix, Octree *object2Octree,
                              float4x4 *object2ModelMatrix) {
    AABB object1Aabb = multiplyAABBWithModelMatrix(object1Octree->getAABB(), object1ModelMatrix);
    AABB object2Aabb = multiplyAABBWithModelMatrix(object2Octree->getAABB(), object2ModelMatrix);

    if (!AabbAabbintersection(&object1Aabb, &object2Aabb)) {
        return false;
    }

    if (!object1Octree->hasChildren() && !object2Octree->hasChildren()) {
        std::vector<Triangle *> *triangles1 = object1Octree->getTriangles();
        std::vector<Triangle *> *triangles2 = object2Octree->getTriangles();

        for (auto t1 = triangles1->begin(); t1 != triangles1->end(); t1++) {
            Triangle triangle1 = multiplyTriangleWithModelMatrix(*t1, object1ModelMatrix);
            for (auto t2 = triangles2->begin(); t2 != triangles2->end(); t2++) {
                Triangle triangle2 = multiplyTriangleWithModelMatrix(*t2, object2ModelMatrix);

                if (triangleTriangleIntersection(&triangle1, &triangle2)) {
                    return true;
                }

            }
        }
    } else if (object1Octree->hasChildren() && object2Octree->hasChildren()) {
                if (object1Aabb.getSize() > object2Aabb.getSize()) {
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











