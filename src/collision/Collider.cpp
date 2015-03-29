#include "Collider.h"

bool rayTriangle(float3 r_o, float3 r_d, float3 v1, float3 v2, float3 v3, float *ins);

Collider::Collider(Octree* tree) : tree(tree)
{
}


Collider::~Collider()
{
}

float Collider::rayIntersection(float3 rayOrigin, float3 rayVec) {
	std::vector<Triangle*> geometry;

	tree->getGeometry(rayOrigin, rayVec, &geometry);

	float minIns = NULL;
	for (int i = 0; i < geometry.size(); i++) {
		Triangle t = *geometry[i];
		float ins;

		if (rayTriangle(rayOrigin, rayVec, t.p1, t.p2, t.p3, &ins)) {
			if ((minIns) >(ins) || minIns == NULL) {
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


void Collider::addMesh(Mesh* model) {
	float4x4 modelMatrix = model->m_modelMatrix;
	for (int i = 0; i < model->m_chunks.size(); i++) {

		for (int j = 0; j < model->m_chunks[i].m_positions.size(); j += 3) {


			float4 p1 = modelMatrix * make_vector(model->m_chunks[i].m_positions[j + 0].x,
				model->m_chunks[i].m_positions[j + 0].y,
				model->m_chunks[i].m_positions[j + 0].z, 1.0f);

			float4 p2 = modelMatrix * make_vector(model->m_chunks[i].m_positions[j + 1].x,
				model->m_chunks[i].m_positions[j + 1].y,
				model->m_chunks[i].m_positions[j + 1].z, 1.0f);

			float4 p3 = modelMatrix * make_vector(model->m_chunks[i].m_positions[j + 2].x,
				model->m_chunks[i].m_positions[j + 2].y,
				model->m_chunks[i].m_positions[j + 2].z, 1.0f);

			Triangle* t = new Triangle(make_vector(p1.x, p1.y, p1.z), make_vector(p2.x, p2.y, p2.z), make_vector(p3.x, p3.y, p3.z));

			ts.push_back(t);
		}
	}
	checkMinMax(model->m_aabb.maxV.x, model->m_aabb.maxV.y, model->m_aabb.maxV.z, &aabb_coll.minV, &aabb_coll.maxV);
	checkMinMax(model->m_aabb.minV.x, model->m_aabb.minV.y, model->m_aabb.minV.z, &aabb_coll.minV, &aabb_coll.maxV);
}


bool rayTriangle(float3 r_o, float3 r_d, float3 v1, float3 v2, float3 v3, float *ins)
{
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
		if ((u<0) || (u>a)) return false;
		float v = dot(r_d, q);
		if ((v<0) || (u + v>a)) return false;
	}
	else if (a<-eps)
	{ // Back facing triangle...
		frontfacing = false;
		if ((u > 0) || (u < a)) return false;
		float v = dot(r_d, q);
		if ((v>0) || (u + v<a)) return false;
	}
	else return false; // Ray parallel to triangle plane
	float t = f*dot(e2, q);
	float v = dot(r_d, q);
	u = u*f;
	v = v*f;

	*ins = t;

	return true;
}
