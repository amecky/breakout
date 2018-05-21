#pragma once
#include <diesel.h>

//#define PH_INTERSECTION_IMPLEMENTATION

typedef struct {

	ds::vec2 pos;
	ds::vec2 dim;
} phAABBox;

typedef struct  {
	ds::vec2 pos;
	float radius;
} phCircle;

typedef struct {
	int count;
	float depths[2];
	ds::vec2 contact_points[2];
	// always points from shape A to shape B (first and second shapes passed into
	// any of the c2***to***Manifold functions)
	ds::vec2 n;
} phManifold;

bool phOverlaps(phAABBox first, phAABBox second);

void phCircletoAABBManifold(phCircle A, phAABBox B, phManifold* m);

bool sweepTest(phAABBox b1, const ds::vec2& prev1, phAABBox b2, const ds::vec2& prev2, float* u0, float *u1);

#ifdef PH_INTERSECTION_IMPLEMENTATION

bool phOverlaps(phAABBox first, phAABBox second) {
	float top = first.pos.y + first.dim.y * 0.5f;
	float left = first.pos.x - first.dim.x * 0.5f;
	float right = first.pos.x + first.dim.x * 0.5f;
	float bottom = first.pos.y - first.dim.y * 0.5f;

	float otherTop = second.pos.y + second.dim.y * 0.5f;
	float otherLeft = second.pos.x - second.dim.x * 0.5f;
	float otherRight = second.pos.x + second.dim.x * 0.5f;
	float otherBottom = second.pos.y - second.dim.y * 0.5f;

	if (right < otherLeft || left > otherRight) return false;
	if (bottom > otherTop || top < otherBottom) return false;
	return true;
}

void phCircletoAABBManifold(phCircle A, phAABBox B, phManifold* m) {

	ds::vec2 boxMin = ds::vec_min(B.pos - B.dim * 0.5f, B.pos + B.dim * 0.5f);
	ds::vec2 boxMax = ds::vec_max(B.pos - B.dim * 0.5f, B.pos + B.dim * 0.5f);

	ds::vec2 L = ds::vec_max(A.pos, ds::vec_min(boxMin, boxMax));

	ds::vec2 ab = L - A.pos;

	float d2 = dot(ab, ab);

	float r2 = A.radius * A.radius;
	if (d2 < r2) {
		if (d2 != 0) {
			float d = sqrtf(d2);
			ds::vec2 n = normalize(ab);
			m->count = 1;
			m->depths[0] = A.radius - d;
			m->contact_points[0] = A.pos + n * d;
			m->n = n;
		}
		else {
			ds::vec2 mid = (boxMin + boxMax) * 0.5f;
			ds::vec2 e = (boxMax - boxMin) * 0.5f;
			ds::vec2 d = A.pos - mid;
			ds::vec2 abs_d = ds::vec2(abs(d.x), abs(d.y));

			float x_overlap = e.x - abs_d.x;
			float y_overlap = e.y - abs_d.y;

			float depth;
			ds::vec2 n;

			if (x_overlap < y_overlap)
			{
				depth = x_overlap;
				n = ds::vec2(1.0f, 0.0f);
				if (d.x >= 0.0f) {
					n *= -1.0f;
				}
				//n = c2Mulvs(n, d.x < 0 ? 1.0f : -1.0f);
			}

			else
			{
				depth = y_overlap;
				n = ds::vec2(0.0f, 1.0f);
				//n = c2Mulvs(n, d.y < 0 ? 1.0f : -1.0f);
				if (d.y >= 0.0f) {
					n *= -1.0f;
				}
			}

			m->count = 1;
			m->depths[0] = A.radius + depth;
			m->contact_points[0] = A.pos - n * depth;
			m->n = n;
		}
	}
}

// http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=3
bool sweepTest(phAABBox b1, const ds::vec2& prev1, phAABBox b2, const ds::vec2& prev2, float* u0, float *u1) {
	const phAABBox A = { prev1, b1.dim };//previous state of AABB A
	const phAABBox B = { prev2, b2.dim };//previous state of AABB B
	const ds::vec2 va = b1.pos - prev1;//displacement of A
	const ds::vec2 vb = b2.pos - prev2;//displacement of B
											//the problem is solved in A's frame of reference
	ds::vec2 v = vb - va;
	//relative velocity (in normalized time)
	ds::vec2 u_0(0, 0);
	//first times of overlap along each axis
	ds::vec2 u_1(1, 1);
	//last times of overlap along each axis

	//check if they were overlapping 
	// on the previous frame
	if (phOverlaps(A,B)) {
		*u0 = *u1 = 0.0f;
		return true;
	}

	ds::vec2 AboxMin = ds::vec_min(A.pos - A.dim * 0.5f, A.pos + A.dim * 0.5f);
	ds::vec2 AboxMax = ds::vec_max(A.pos - A.dim * 0.5f, A.pos + A.dim * 0.5f);

	ds::vec2 BboxMin = ds::vec_min(B.pos - B.dim * 0.5f, B.pos + B.dim * 0.5f);
	ds::vec2 BboxMax = ds::vec_max(B.pos - B.dim * 0.5f, B.pos + B.dim * 0.5f);

	//find the possible first and last times
	//of overlap along each axis
	for (long i = 0; i < 2; ++i) {
		if (AboxMax.data[i] < BboxMin.data[i] && v.data[i]<0) {
			u_0.data[i] = (AboxMax.data[i] - BboxMin.data[i]) / v.data[i];
		}
		else if (BboxMax.data[i] <AboxMin.data[i] && v.data[i]>0) {
			u_0.data[i] = (AboxMin.data[i] - BboxMax.data[i]) / v.data[i];
		}
		if (BboxMax.data[i]>AboxMin.data[i] && v.data[i]<0) {
			u_1.data[i] = (AboxMin.data[i] - BboxMax.data[i]) / v.data[i];
		}
		else if (AboxMax.data[i] > BboxMin.data[i] && v.data[i] > 0 ) {
			u_1.data[i] = (AboxMax.data[i] - BboxMin.data[i]) / v.data[i];
		}
	}
	//possible first time of overlap
	*u0 = max(u_0.x, u_0.y);
	//possible last time of overlap
	*u1 = min(u_1.x, u_1.y);
	//they could have only collided if
	//the first time of overlap occurred
	//before the last time of overlap
	return *u0 <= *u1;
}
#endif