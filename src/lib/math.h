#pragma once
#include <diesel.h>

namespace math {

	float get_angle(const ds::vec2& v1, const ds::vec2& v2);

	float get_rotation(const ds::vec2& v1);

	float rad2deg(float angle);

	float deg2rad(float angle);
}