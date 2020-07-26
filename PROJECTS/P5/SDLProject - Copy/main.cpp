#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>


#include <SDL_Mixer.h>

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"


Mix_Music* music;
Mix_Chunk* jump_sfx;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool game_won = false;
bool game_lost = false;
int player_health = 3;

Scene* currentScene;
Scene* sceneList[4];



void SwitchToScene(Scene* scene) {
	currentScene = scene;
	currentScene->Initialize();
}


ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;




void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("1song_3.mp3");
	jump_sfx = Mix_LoadWAV("jump.wav");
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	

	sceneList[0] = new Menu();
	sceneList[1] = new Level1();
	sceneList[2] = new Level2();
	sceneList[3] = new Level3();
	SwitchToScene(sceneList[0]);
	
	


}

void ProcessInput() {

	currentScene->state.player->movement = glm::vec3(0);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				// Move the player left
				break;

			case SDLK_RIGHT:
				// Move the player right
				break;

			case SDLK_SPACE:
				if (currentScene->state.player->collidedBot) {
					Mix_PlayChannel(-1, jump_sfx, 0);
					currentScene->state.player->jump = true;
				}
				break;
			}
			break; // SDL_KEYDOWN
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_LEFT]) {
		currentScene->state.player->movement.x = -1.0f;
		currentScene->state.player->animIndices = currentScene->state.player->animLeft;
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		currentScene->state.player->movement.x = 1.0f;
		currentScene->state.player->animIndices = currentScene->state.player->animRight;
	}

	if (keys[SDL_SCANCODE_RETURN]) {

	}


	if (glm::length(currentScene->state.player->movement) > 1.0f) {
		currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
	}

}



#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		currentScene->Update(FIXED_TIMESTEP);
		

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;


	
	viewMatrix = glm::mat4(1.0f);
	if (currentScene->state.player->position.x > 5 ) {
		viewMatrix = glm::translate(viewMatrix,
			glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
	}
	
	else {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
	}
	
}


void Render() {
	glClear(GL_COLOR_BUFFER_BIT);


	program.SetViewMatrix(viewMatrix);

	currentScene->Render(&program);

	GLuint font = Util::LoadTexture("font1.png");

	
	if (currentScene == sceneList[0]) {
		Util::DrawText(&program, font, "The FOLLOWER", 0.5, -0.25f, glm::vec3(4, -4, 0));
	}

	if (currentScene->state.player->health == 0 ||player_health ==0) {
		currentScene->state.player->isActive = false;
		game_lost = true;
	}

	if (game_lost) {
		Util::DrawText(&program, font, "You LOSE!", 0.5, -0.25f, glm::vec3(currentScene->state.player->position.x-0.5,-3,0));
	}
	if (currentScene->state.player->win == true) {
		currentScene->state.enemies[0].isActive = false;
		Util::DrawText(&program, font, "You WIN!", 0.5, -0.25f, glm::vec3(currentScene->state.player->position.x -0.5, -3,0));
	}

	if (currentScene->state.player->reset && !game_lost) {
		currentScene->state.player->reset = false;
		SwitchToScene(sceneList[currentScene->state.nextScene]);
		player_health--;
	}

	SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);

		Render();
	}

	Shutdown();
	return 0;
}