#include "utils.h"
#include <core\math\math.h>

struct NumberDefinition {
	int top;
	int left;
	int width;
	int height;
	ds::Texture texture;

	NumberDefinition(int t, int l, int w, int h) : top(t), left(l), width(w), height(h) {
		texture = math::buildTexture(top, left, width, height);
	}
};
/*
static NumberDefinition NUMBER[]{
	{ 440, 220, 63, 50} ,
	{ 440, 285, 37, 50 },
	{ 440, 325, 58, 50 },
	{ 440, 386, 55, 50 },
	{ 440, 444, 59, 50 },
	{ 440, 508, 56, 50 },
	{ 440, 568, 55, 50 },
	{ 440, 628, 56, 50 },
	{ 440, 688, 57, 50 },
	{ 440, 759, 57, 50 },
};
*/
static NumberDefinition NUMBER[]{
	{ 520,   0, 40, 50 } ,
	{ 520,  40, 40, 50 },
	{ 520,  80, 40, 50 },
	{ 520, 120, 40, 50 },
	{ 520, 160, 40, 50 },
	{ 520, 200, 40, 50 },
	{ 520, 240, 40, 50 },
	{ 520, 280, 40, 50 },
	{ 520, 320, 40, 50 },
	{ 520, 360, 40, 50 },
};
namespace utils {

	int convertNumber(int value, int length, ds::Texture* textures) {
		int index = 0;
		int tmp = value;
		int div = 1;
		for (int i = 0; i < length; ++i) {
			if (i > 0) {
				div *= 10;
			}
		}
		for (int i = 0; i < length; ++i) {
			int r = tmp / div;
			textures[index++] = NUMBER[r].texture;
			tmp = tmp - r * div;
			div /= 10;
		}
		return index;
	}

	bool convertNumber(int value, ds::Texture* texture) {
		if (value >= 0 && value < 10) {
			*texture = NUMBER[value].texture;
			return true;
		}
		return false;
	}
}