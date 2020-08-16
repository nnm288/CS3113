#include "End.h"

#define END_WIDTH 2
#define END_HEIGHT 2
#define ENEMY_COUNT_E 0

unsigned int end_data[] =
{
 0, 0,
 0, 0
};

void End::Initialize() {
	state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(END_WIDTH, END_HEIGHT, end_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.

	// Initialize Player
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->position = glm::vec3(0, 0, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0, -5.81f, 0);
	state.player->speed = 2.0f;
	state.player->textureID = Util::LoadTexture("george_0.png");

	state.player->animRight = new int[4]{ 3, 7, 11, 15 };
	state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
	state.player->animUp = new int[4]{ 2, 6, 10, 14 };
	state.player->animDown = new int[4]{ 0, 4, 8, 12 };

	state.player->animIndices = state.player->animRight;
	state.player->animFrames = 4;
	state.player->animIndex = 0;
	state.player->animTime = 0;
	state.player->animCols = 4;
	state.player->animRows = 4;

	state.player->height = 0.8;
	state.player->width = 0.3;
	state.player->jumpPower = 4.5f;


	/*
	state.enemies = new Entity[ENEMY_COUNT_M];
GLuint enemyTextureID = Util::LoadTexture("ctg.png");

	for (int i = 0; i < ENEMY_COUNT_M; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].textureID = enemyTextureID;
		state.enemies[i].position = glm::vec3(0, 0, 0);
		state.enemies[i].movement = glm::vec3(0);
		state.enemies[i].acceleration = glm::vec3(0, -5.1f, 0);
		state.enemies[i].speed = 1.13;

	}

	state.enemies[0].aiType = FOLLOWER;
	state.enemies[0].position = glm::vec3(2, -3.75, 0);
	*/



}
void End::Update(float deltaTime) {

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RETURN]) {
		state.nextScene = 1;
	}
}
void End::Render(ShaderProgram* program) {
	state.map->Render(program);
	//state.player->Render(program);
	//for (int i = 0; i < ENEMY_COUNT_M; i++) {
	//	state.enemies[i].Render(program);
	//
	//}
}