#include "Octree.h"
#include <cfloat>

Octree::Octree(float3 origin, float3 halfVector, int depth)
	:origin(origin), halfVector(halfVector), depth(depth)
{
	for (int i = 0; i < 8; i++) {
		children[i] = NULL;
	}
}

Octree::~Octree() {
	
}



void Octree::insertAll(std::vector<Triangle*> &triangles){
	if (hasChildren() || (triangles.size() + ts.size() > MAX_CHILDREN && depth < MAX_DEPTH)) {
		if (!hasChildren()){
			for (int i = 0; i<8; ++i) {
				// Compute new bounding box for this child
				float3 newOrigin = origin;
				newOrigin.x += halfVector.x * (i & 4 ? .5f : -.5f);
				newOrigin.y += halfVector.y * (i & 2 ? .5f : -.5f);
				newOrigin.z += halfVector.z * (i & 1 ? .5f : -.5f);
				children[i] = new Octree(newOrigin, halfVector*.5f, depth + 1);
			}
		}

		std::vector<Triangle*> newTris[8];

		for (unsigned int i = 0; i < triangles.size(); i++) {
			BoundingBox *b = triangles[i]->getBoundingBox();
			std::set<int> octs;
			for (int j = 0; j < 8; j++) {
				octs.insert(getOctantContainingPoint(b->points[j]));
			}

			for (std::set<int>::iterator it = octs.begin(); it != octs.end(); ++it)
			{
				newTris[*it].push_back(triangles[i]);
			}
		}

		for (int i = 0; i < 8; i++) {
			children[i]->insertAll(newTris[i]);
		}

		ts.clear();

	}
	else {
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			ts.push_back(triangles[i]);
		}
	}
}



void Octree::insert(Triangle* t) {
	if (isFull() && depth < MAX_DEPTH) {

		for (int i = 0; i<8; ++i) {
			// Compute new bounding box for this child
			float3 newOrigin = origin;
			newOrigin.x += halfVector.x * (i & 4 ? .5f : -.5f);
			newOrigin.y += halfVector.y * (i & 2 ? .5f : -.5f);
			newOrigin.z += halfVector.z * (i & 1 ? .5f : -.5f);
			children[i] = new Octree(newOrigin, halfVector*.5f, depth + 1);
		}

		addTriangle(t);
		for (unsigned int i = 0; i < ts.size(); i++) {
			addTriangle(ts[i]);
		}

		ts.clear();

	}
	else if (hasChildren()) {
		addTriangle(t);
	}
	else {
		ts.push_back(t);
	}
}

bool Octree::isFull() {
	return ts.size() >= MAX_CHILDREN;
}

bool Octree::hasChildren() {
	return *children != NULL;
}

void Octree::getChildren(std::vector<Octree>* octs) {
	if (hasChildren()) {
		for (int i = 0; i < 8; i++) {
			octs->push_back(*children[i]);
		}
	}
}

void Octree::addTriangle(Triangle* t) {
	BoundingBox *b = t->getBoundingBox();
	std::set<int> octs;
	for (int i = 0; i < 8; i++) {
		octs.insert(getOctantContainingPoint(b->points[i]));
	}

	for (std::set<int>::iterator it = octs.begin(); it != octs.end(); ++it)
	{
		children[*it]->insert(t);
	}

	
}

int Octree::getOctantContainingPoint(const float3& point) {
	int oct = 0;
	if (point.x >= origin.x) oct |= 4;
	if (point.y >= origin.y) oct |= 2;
	if (point.z >= origin.z) oct |= 1;
	return oct;
}

int Octree::getChildCount() {
	return ts.size();
}

int Octree::getCount() {
	std::vector<Octree> octs;
	int c = getChildCount();

	if (hasChildren()) {
		for (int i = 0; i < 8; i++) {
			c += children[i]->getCount();
		}
	}

	return c;
}

bool testSlab(float rayO, float rayD, float minV, float maxV, float* tNear, float* tFar) {
	if (rayD == 0.0f) {
		if (rayO <= maxV && rayO >= minV) {
			*tNear = 0;
			//*tFar = 0;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		float t1 = (minV - rayO) / rayD;
		float t2 = (maxV - rayO) / rayD;

		if (t1 > t2) {
			swap(t1, t2);
		}

		if (t1 > *tNear) {
			*tNear = t1;
		}
		if (t2 < *tFar) {
			*tFar = t2;
		}

		if (tNear < tFar) {
			return false;
		}

		if (tFar < 0) {
			return false;
		}
	}

	return true;
}


bool Octree::intersect(float3 rayOrigin, float3 rayVector) {
	float3 maxCorner = make_vector(origin.x + halfVector.x, origin.y + halfVector.y, origin.z + halfVector.z);
	float3 minCorner = make_vector(origin.x - halfVector.x, origin.y - halfVector.y, origin.z - halfVector.z);

	float3 dirFraction = make_vector(1.0f / rayVector.x, 1.0f / rayVector.y, 1.0f / rayVector.z);

	float tNear = -FLT_MAX, tFar = FLT_MAX;

	if (testSlab(rayOrigin.x, rayVector.x, minCorner.x, maxCorner.x, &tNear, &tFar)) {
		if (testSlab(rayOrigin.y, rayVector.y, minCorner.y, maxCorner.y, &tNear, &tFar)) {
			if (testSlab(rayOrigin.z, rayVector.z, minCorner.z, maxCorner.z, &tNear, &tFar)) {

				return true;
			}
		}
	}

	return false;
}

void Octree::getGeometry(float3 rayOrigin, float3 rayVector, std::vector<Triangle*> *tsp) {
	if (intersect(rayOrigin, rayVector)) {
		bool t = hasChildren();
		if (hasChildren()) {
			for (int i = 0; i < 8; i++) {
				children[i]->getGeometry(rayOrigin, rayVector, tsp);
			}
		}
		else {
			for (unsigned int i = 0; i < ts.size(); i++) {
				tsp->push_back(ts[i]);
			}
		}
	}
}
