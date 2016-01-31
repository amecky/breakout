#pragma once
#include <utils\Color.h>

const ds::Color CL_BLUE = ds::Color(0.0f,0.5f,1.0f,1.0f);
const ds::Color CL_ORANGE = ds::Color(1.0f,0.5f,0.0f,1.0f);

const float starsParallaxPeriod = 32.0f;
const float starsParallaxAmplitude = 4096.0f;

const float FADING_TTL = 2.0f;
const float FADE_TTL = 0.3f;

const int ST_DLG_X = 282;
const int ST_DLG_Y = 360;

const int QUIT_DLG_X = 282;
const int QUIT_DLG_Y = 314;

const int END_DLG_X = 282;
const int END_DLG_Y = 314;

const int STARTX = 110;
const int STARTY = 150;
const int MAX_X = 20;
const int MAX_Y = 12;

enum EVENTS {
	EVN_GAME_OVER,
	EVN_START_GAME,
	EVN_ADD_SCORE,
	EVN_RESET_GAME,
	EVN_QUIT_GAME,
	EVN_CONTINUE_GAME,
	EVN_SETTINGS_CHANGED,
	EVN_LEVEL_READY,
	EVN_START_FADING
};

struct Score {
	int piecesLeft;
	int highestCombo;
	int points;
	int highscore;
	int totalPieces;
	int minutes;
	int seconds;
};

struct GameSettings {
	int level;
	bool rushMode;
	int ttl;
};