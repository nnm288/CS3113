#include "level3.h"
#define LEVEL3_WIDTH 22
#define LEVEL3_HEIGHT 8
#define ENEMY_COUNT_3 1


unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,3,
 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3,3,3,3,3,3,3,3,3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,3,3,3,3,3,3,3,3
};

void Level3::Initialize() {
	state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.

	// Initialize Player
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->position = glm::vec3(5, 0, 0);
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


	state.enemies = new Entity[ENEMY_COUNT_3];
	GLuint enemyTextureID = Util::LoadTexture("ctg.png");

	for (int i = 0; i < ENEMY_COUNT_3; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].textureID = enemyTextureID;
		state.enemies[i].position = glm::vec3(0, 0, 0);
		state.enemies[i].movement = glm::vec3(0);
		state.enemies[i].acceleration = glm::vec3(0, -5.1f, 0);
		state.enemies[i].speed = 1.6;

	}

	state.enemies[0].aiType = FOLLOWER;
	state.enemies[0].position = glm::vec3(2, -3.75, 0);


}
void Level3::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT_3, state.map);
	if (state.player->position.x >= 20) {
		state.player->win = true;
	}
	for (int i = 0; i < ENEMY_COUNT_3; i++) {
		state.enemies[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT_3, state.map);

	}
}
void Level3::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
	for (int i = 0; i < ENEMY_COUNT_3; i++) {
		state.enemies[i].Render(program);

	}
}