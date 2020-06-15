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
#include "time.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;


ShaderProgram program;
glm::mat4 viewMatrix, player2Matrix, player1Matrix, ballMatrix, projectionMatrix;

bool ball_moving = false;



glm::vec3 ball_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_movement = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 player1_position = glm::vec3(9.5f, 0.0f, 0.0f);
glm::vec3 player1_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 player2_position = glm::vec3(-9.5f, 0.0f, 0.0f);
glm::vec3 player2_movement = glm::vec3(0.0f, 0.0f, 0.0f);

float player_speed = 2.7f;
float ball_speed = 20.0f;



void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 1280, 720);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	viewMatrix = glm::mat4(1.0f);

	ballMatrix = glm::mat4(1.0f);
	player2Matrix = glm::mat4(1.0f);
	player1Matrix = glm::mat4(1.0f);
	
	
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(0.0f, 1.0f, 1.0f, 1.0f);
	

	glUseProgram(program.programID);

	glClearColor(0.57f, 0.4f, 0.931f, 1.0f);

}

void ProcessInput() {

	player1_movement = glm::vec3(0);
	player2_movement = glm::vec3(0);


	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;


		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				ball_moving = true;
				int random = time(NULL) % 4;
				if (random == 0){
					ball_movement.x = 1;
					ball_movement.y = 1;
				}
				else if (random == 3) {
					ball_movement.x = -1;
					ball_movement.y = 1;
				}
				else if (random == 1) {
					ball_movement.x = 1;
					ball_movement.y = -1;
				}
				else {
					ball_movement.x = -1;
					ball_movement.y = -1;
				}
				
				
				break;
			}
			break;
		}
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	if (keys[SDL_SCANCODE_UP] && !(keys[SDL_SCANCODE_DOWN])) {
		if (player1_position.y < 1.4f) {
			player1_movement.y = 1.0f;
		}
	}
	if (keys[SDL_SCANCODE_DOWN] && !(keys[SDL_SCANCODE_UP])) {
		if (player1_position.y > -1.4f) {
			player1_movement.y = -1.0f;
		}
	}
	if (keys[SDL_SCANCODE_W] && !(keys[SDL_SCANCODE_S])) {
		if (player2_position.y < 1.4f) {
			player2_movement.y = 1.0f;
		}
	}
	if (keys[SDL_SCANCODE_S] && !(keys[SDL_SCANCODE_W])) {
		if (player2_position.y > -1.4f) {
			player2_movement.y = -1.0f;
		}
	}


	if (glm::length(player1_movement) > 1.0f) {
		player1_movement = glm::normalize(player1_movement);
	}

	if (glm::length(player2_movement) > 1.0f) {
		player2_movement = glm::normalize(player2_movement);
	}
}

float lastTicks = 0.0f;

void UpdateBall() {

	ballMatrix = glm::scale(ballMatrix, glm::vec3(0.19f, 0.24f, 1.0f));
	
	if (ball_moving) {
		if (ball_position.y >= 15) {
			ball_movement.y = -1;
		}
		else if (ball_position.y <= -15) {
			ball_movement.y = +1;
		}
		
		
		
		bool x_hit = (fabs(ball_position.x) > 24);
		bool p1_hit = (((ball_position.y*0.24) >= (player1_position.y*2.0f) - 1.12) && ((ball_position.y*0.24) <= (player1_position.y*2) +1.12));
		bool p2_hit = (((ball_position.y*0.24) >= (player2_position.y*2.0f) - 1.12) && ((ball_position.y*0.24) <= (player2_position.y*2) + 1.12));

		if (x_hit && p1_hit ) {
			ball_movement.x = -1;
		}
		else if (ball_position.x > 26) {
			gameIsRunning = false;
		}
		
		
		if (x_hit && p2_hit) {
			ball_movement.x = 1;
		}
		else   if (ball_position.x < -26) {
			gameIsRunning = false;
		}
		
		
	}

	
	
}


void Update() {
	
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	player1_position += player1_movement * player_speed * deltaTime;
	player2_position += player2_movement * player_speed * deltaTime;
	ball_position += ball_movement * ball_speed * deltaTime;

	player1Matrix = glm::mat4(1.0f);
	player1Matrix = glm::scale(player1Matrix, glm::vec3(0.5f, 2.0f, 1.0f));
	player1Matrix = glm::translate(player1Matrix, player1_position);


	player2Matrix = glm::mat4(1.0f);
	player2Matrix = glm::scale(player2Matrix, glm::vec3(0.5f, 2.0f, 1.0f));
	player2Matrix = glm::translate(player2Matrix, player2_position);


	ballMatrix = glm::mat4(1.0f);
	UpdateBall();
	ballMatrix = glm::translate(ballMatrix, ball_position);

}

void Render() {

	glClear(GL_COLOR_BUFFER_BIT);
	

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	

	program.SetModelMatrix(player2Matrix);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(player1Matrix);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(ballMatrix);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

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
		Render();
	}

	Shutdown();

	return 0;
}