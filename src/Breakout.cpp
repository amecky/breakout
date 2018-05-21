#include "Breakout.h"
#include <ds_imgui.h>
#include "lib\math.h"
#include "lib\AABBox.h"
#include <ds_intersection.h>

const ds::vec2 SECTIONS[] = {
	ds::vec2(math::deg2rad( 45.0f),math::deg2rad(135.0f)),
	ds::vec2(math::deg2rad(135.0f),math::deg2rad(225.0f)),
	ds::vec2(math::deg2rad(225.0f),math::deg2rad(315.0f)),
	ds::vec2(math::deg2rad(315.0f),math::deg2rad(360.0f)),
	ds::vec2(math::deg2rad(  0.0f),math::deg2rad( 45.0f))
};

const ds::vec2 PUSH_BACK[] = {
	ds::vec2(0.0f,1.2f),ds::vec2(1.2f,0.0f),ds::vec2(0.0f,1.2f),ds::vec2(1.2f,0.0f),ds::vec2(1.2f,0.0f)
};

ds::BaseApp *app = new Breakout(); 

Breakout::Breakout() : ds::BaseApp() {
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	_settings.windowTitle = "breakout";
	_settings.useIMGUI = true;
	_settings.clearColor = ds::Color(16, 16, 16, 255);
	_settings.guiToggleKey = 'O';
	_settings.synchedFrame = true;
	_buttonClicked[0] = false;
	_buttonClicked[1] = false;
	_buttonPressed[0] = false;
	_buttonPressed[1] = false;
	_dbgFollow = false;

	
}

Breakout::~Breakout() {
	delete _sprites;
}

void Breakout::initialize() {
	RID textureID = loadImageFromFile("content\\textures\\TextureArray.png");
	_sprites = new SpriteBatchBuffer(SpriteBatchDesc()
		.MaxSprites(2048)
		.Texture(textureID)
	);
	_paddle.position = ds::vec2(512, 80);
	_paddle.texture = ds::vec4(80, 200, 102, 30);

	_ball.reset();
	_indicator.reset();
	_mousePressed = false;
	/*
	//ds::vec2 v(cosf(math::deg2rad(45.0f)), sinf(math::deg2rad(45.0f)));
	ds::vec2 v(0, 1);
	v *= 100.0f;
	ds::vec2 n(0, 1);
	float dt = dot(v, n);
	ds::vec2 nn = dt * n;
	ds::vec2 nv = 2.0f * dot(v, n) * n - v;
	*/
	//phCircle circle = { ds::vec2(110.0f),20.0f };
	// 0 0.8 u1 12.5
	//ball x : 475 y : 385 previous 468 385
	phAABBox box1 = { ds::vec2(512,384),ds::vec2(60,30) };
	phAABBox box2 = { ds::vec2(475,385),ds::vec2(18,18) };

	//phManifold m;

	//phCircletoAABBManifold(circle, box, &m);
	float u0 = 0.0f;
	float u1 = 0.0f;

	bool ret = sweepTest(box1, box1.pos, box2, ds::vec2(468, 385), &u0, &u1);
	if (ret) {
		ds::vec2 d = box2.pos - ds::vec2(468, 385);
		d *= u0;
		ds::vec2 hit = box2.pos + d;
		DBG_LOG("HIT %1.1f %1.1f", u0, u1);
	}
	else {
		DBG_LOG("NO HIT");
	}

	//DBG_LOG("m - depth %3.0f", m.depths[0]);

	/*
	AABBox first(ds::vec2(100, 100), ds::vec2(20, 20));
	AABBox second(ds::vec2(110, 100), ds::vec2(20, 20));
	if (first.intersects(second)) {
		ds::vec2 diff = second.position - first.position;
		DBG_LOG("diff x %3.0f y %3.0f", diff.x, diff.y);
	}
	*/
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Breakout::update(float dt) {

	handleButtons();

	// move paddle
	ds::vec2 vel = ds::vec2(0.0f);
	if (ds::isKeyPressed('A')) {
		vel.x -= 1.0f;
	}
	if (ds::isKeyPressed('D')) {
		vel.x += 1.0f;
	}
	_paddle.position += vel * 350.0f * dt;
	if (_paddle.position.x < 150.0f) {
		_paddle.position.x = 150.0f;
	}
	if (_paddle.position.x > 880.0f) {
		_paddle.position.x = 880.0f;
	}

	// move sticky ball
	if (_ball.isSticky()) {
		if (_dbgFollow) {
			ds::vec2 bp = _paddle.position + ds::vec2(0.0f, 30.0f);
			_ball.setPosition(bp);
			_indicator.setPosition(bp);
		}
		else {
			_indicator.setPosition(_ball.getPosition());
		}
		if ( _buttonClicked[0]) {
			_buttonClicked[0] = false;
			_ball.startMoving(_indicator.getRotation());
		}
		_indicator.tick(dt);
	}	
	else {
		if (_ball.isMoving()) {
			//ID id = _bricks.checkIntersection(_ball.getPosition(), ds::vec2(9.0f));
			float u0 = 0.0f;
			float u1 = 0.0f;
			ID id = _bricks.checkIntersection(_ball.getBox(), _ball.getPrevious(), &u0, &u1);
			if (id != NO_RID && u0 < 1.0f) {
				DBG_LOG("u0 %1.1f u1 %1.1f", u0, u1);
				const Brick& brick = _bricks.getBrick(id);
				ds::vec2 d = _ball.getPosition() - brick.position;
				float da = math::get_rotation(d);
				int idx = -1;
				for (int i = 0; i < 5; ++i) {
					if (da >= SECTIONS[i].x && da < SECTIONS[i].y) {
						idx = i;
					}
				}
				//if (idx == 4) {
					//--idx;
				//}
				ds::vec2 dd = d - ds::vec2(9.0f) - ds::vec2(30.0f, 15.0f);
				dd.x = abs(d.x) - 9.0f - 30.0f;
				dd.y = abs(d.x) - 9.0f - 15.0f;
				ds::vec2 pushBack = dd;
				pushBack.x *= PUSH_BACK[idx].x;
				pushBack.y *= PUSH_BACK[idx].y;

				pushBack = u0 * _ball.getPosition();

				ds::vec2 vd = _ball.getPosition() - _ball.getPrevious();
				vd *= u0;
				ds::vec2 hit = _ball.getPosition() + vd;
				DBG_LOG("==> hit x: %3.0f y: %3.0f", hit.x, hit.y);

				float angle = ds::PI * 0.5f - _ball.getDirection();// math::get_rotation(d);
				DBG_LOG("ball x: %3.0f y: %3.0f previous %3.0f %3.0f", _ball.getPosition().x, _ball.getPosition().y, _ball.getPrevious().x, _ball.getPrevious().y);
				DBG_LOG("brick x: %3.0f y: %3.0f", brick.position.x, brick.position.y);
				DBG_LOG("push_back x: %3.0f y: %3.0f", pushBack.x, pushBack.y);
				DBG_LOG("=> d x: %3.0f y: %3.0f", d.x, d.y);
				DBG_LOG("dd x: %3.0f y: %3.0f", dd.x, dd.y);
				DBG_LOG("section: %d d: %3.0f %3.0f dir %3.0f angle %3.0f da %3.0f", idx, d.x, d.y, math::rad2deg(_ball.getDirection()), math::rad2deg(angle),math::rad2deg(da));
				//_ball.setPosition(_ball.getPosition() + pushBack);
				_ball.setPosition(hit);

				id = _bricks.checkIntersection(_ball.getPosition(), ds::vec2(9.0f));
				if (id != NO_RID) {
					DBG_LOG("Still intersecting");
				}
				//_ball.setDirection(_ball.getDirection() - angle);
				_ball.stopMoving();
			}
		}
	}

	if (_buttonClicked[1]) {
		_buttonClicked[1] = false;
		_ball.reset(ds::getMousePosition());
	}

	_ball.tick(dt);
	ds::vec2 p = _ball.getPosition();
	if (p.y < 40.0f) {
		_ball.reset();
	}
}

void Breakout::render() {
	_sprites->begin();
	_sprites->add(_paddle.position, _paddle.texture);
	_bricks.render(_sprites);
	_sprites->add(_ball.getPosition(), ds::vec4(160, 60, 18, 18));
	if (_ball.isSticky()) {
		_sprites->add(_indicator.getPosition(), ds::vec4(280, 300, 120, 120), ds::vec2(1.0f), _indicator.getRotation());
	}	
	_sprites->flush();
}

void Breakout::drawLeftPanel() {
	gui::Value("Angle", (_indicator.getRotation() * 360.0f / ds::TWO_PI));
	gui::Checkbox("Follow", &_dbgFollow);
	if (gui::Button("Reset ball")) {
		_ball.reset();
	}
}

void Breakout::handleButtons() {
	for (int i = 0; i < 2; ++i) {
		if (ds::isMouseButtonPressed(i)) {
			_buttonPressed[i] = true;
		}
		else if (_buttonPressed[i]) {
			_buttonPressed[i] = false;
			_buttonClicked[i] = true;
		}
	}
}
