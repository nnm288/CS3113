#include "Level4.h"
#define LEVEL4_WIDTH 11
#define LEVEL4_HEIGHT 92
#define ENEMY_COUNT_4 1
#define PUP_COUNT_4 7


unsigned int level4_data[] =
{
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 2, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level4::Initialize() {
	state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL4_WIDTH, LEVEL4_HEIGHT, level4_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.

	// Initialize Player
	GLuint playerTextureID = Util::LoadTexture("george_0.png");

	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->position = glm::vec3(7, -87, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0, -3.81f, 0);
	state.player->speed = 2.0f;
	state.player->textureID = playerTextureID;

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
	state.player->jumpPower = 8.5f;


	state.enemies = new Entity[ENEMY_COUNT_4];
	GLuint enemyTextureID = Util::LoadTexture("ctg.png");

	for (int i = 0; i < ENEMY_COUNT_4; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].textureID = enemyTextureID;
		state.enemies[i].position = glm::vec3(0, 0, 0);
		state.enemies[i].movement = glm::vec3(0);
		state.enemies[i].acceleration = glm::vec3(0, -4.1f, 0);
		state.enemies[i].speed = 1.13;

	}

	state.enemies[0].aiType = WALKER;
	state.enemies[0].aiState = LWALKING;
	state.enemies[0].position = glm::vec3(4, -85, 0);


	state.pup = new Entity[PUP_COUNT_4];
	GLuint pupTextureID = Util::LoadTexture("ctg1.png");

	for (int i = 0; i < PUP_COUNT_4; i++) {
		state.pup[i].entityType = PUP;
		state.pup[i].textureID = pupTextureID;
	}
	state.pup[0].position = glm::vec3(8, -84, 0);
	state.pup[1].position = glm::vec3(6, -32, 0);
	state.pup[2].position = glm::vec3(6, -74, 0);
	state.pup[3].position = glm::vec3(5, -60, 0);
	state.pup[4].position = glm::vec3(4, -45, 0);
	state.pup[5].position = glm::vec3(6, -22, 0);
	state.pup[6].position = glm::vec3(7, -12, 0);;


}
void Level4::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT_4, state.pup, PUP_COUNT_4, state.map);
	if (state.player->position.y >= -1.9) {
		state.player->win = true;
		state.nextScene = 5;
	}
	for (int i = 0; i < ENEMY_COUNT_4; i++) {
		state.enemies[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT_4, state.pup, PUP_COUNT_4, state.map);

	}
	for (int i = 0; i < PUP_COUNT_4; i++) {
		state.pup[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT_4, state.pup, PUP_COUNT_4, state.map);

	}



}
void Level4::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
	for (int i = 0; i < ENEMY_COUNT_4; i++) {
		state.enemies[i].Render(program);

	}
	for (int i = 0; i < PUP_COUNT_4; i++) {
		state.pup[i].Render(program);

	}
}