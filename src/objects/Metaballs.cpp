#include "Metaballs.h"
#include <EnemySprites_VS_Main.h>
#include <EnemySprites_PS_Main.h>

Metaballs::Metaballs(RID textureID) {

	RID vertexShader = ds::createShader(ds::ShaderDesc()
		.Data(EnemySprites_VS_Main)
		.DataSize(sizeof(EnemySprites_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
	);

	RID pixelShader = ds::createShader(ds::ShaderDesc()
		.Data(EnemySprites_PS_Main)
		.DataSize(sizeof(EnemySprites_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
	);

	_metaBallConstantBuffer.screenCenter = { static_cast<float>(ds::getScreenWidth()) / 2.0f, static_cast<float>(ds::getScreenHeight()) / 2.0f, 1024.0f,1024.0f };
	_metaBallConstantBuffer.data = ds::vec4(0.0f);

	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::Camera camera = {
		orthoView,
		orthoProjection,
		orthoView * orthoProjection,
		ds::vec3(0,0,0),
		ds::vec3(0,0,0),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};

	_metaBallConstantBuffer.wvp = ds::matTranspose(camera.viewProjectionMatrix);
	RID constantBuffer = ds::createConstantBuffer(sizeof(MetaBallSpriteBatchConstantBuffer), &_metaBallConstantBuffer);

	RID blendState = ds::createBlendState(ds::BlendStateDesc()
		.SrcBlend(ds::BlendStates::SRC_ALPHA)
		.SrcAlphaBlend(ds::BlendStates::ZERO)
		.DestBlend(ds::BlendStates::ONE)
		.DestAlphaBlend(ds::BlendStates::ZERO)
		.AlphaEnabled(true)
	);

	_sprites = new SpriteBatchBuffer(SpriteBatchDesc()
		.MaxSprites(2048)
		.Texture(textureID)
		.BlendState(blendState)
		.PixelShader(pixelShader)
		.VertexShader(vertexShader)
		.ConstantBuffer(constantBuffer)
	);

	for (int i = 0; i < 64; ++i) {
		MetaBall& b = _balls[i];
		b.pos = ds::vec2(ds::random(100, 900), ds::random(100, 600));
		b.velocity = ds::vec2(cosf(ds::random(0, ds::TWO_PI)), sinf(ds::random(0, ds::TWO_PI))) * ds::random(20.0f, 80.0f);
		b.scale = ds::vec2(ds::random(1.0f, 2.0f));
	}
}

Metaballs::~Metaballs() {
}

void Metaballs::move(float dt) {

	_metaBallConstantBuffer.data.x += dt;

	for (int i = 0; i < 64; ++i) {
		MetaBall& b = _balls[i];
		b.pos += b.velocity * dt;
		if (b.pos.x < 10 || b.pos.x > 1000) {
			b.velocity.x *= -1.0f;
		}
		if (b.pos.y < 10 || b.pos.y > 700) {
			b.velocity.y *= -1.0f;
		}
	}
}

void Metaballs::render() {
	_sprites->begin();
	for (int i = 0; i < 64; ++i) {
		const MetaBall& b = _balls[i];
		float u = 100.0f * b.scale.x;
		float v = 100.0f * b.scale.y;
		_sprites->add(b.pos, ds::vec4(0.0f, 0.0f, u, v), ds::vec2(1.0f), 0.0f, ds::Color(255, 0, 0, 255));
	}
	_sprites->flush();
}
