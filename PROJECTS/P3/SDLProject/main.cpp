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
bool game_over = false;
bool won = false;


ShaderProgram program;
glm::mat4 viewMatrix, landerMatrix, wallMatrix, winMatrix, loseMatrix, msgMatrix, projectionMatrix;

bool lander_moving = false;
float gravity = 0.001;
float lander_speed = 20.0f;

glm::vec3 wall_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 win_position = glm::vec3(0.0f, -4.0f, 0.0f);
glm::vec3 lose_position = glm::vec3(0.0f, -4.1f, 0.0);
glm::vec3 lander_position = glm::vec3(0.0f, 15.0f, 0.0f);
glm::vec3 lander_movement = glm::vec3(0.0f, 0.0f, 0.0f);

GLuint winID;
GLuint loseID;
GLuint defID;

GLuint LoadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}

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

	landerMatrix = glm::mat4(1.0f);
	wallMatrix = glm::mat4(1.0f);	
	winMatrix = glm::mat4(1.0f);
	loseMatrix = glm::mat4(1.0f);
	msgMatrix = glm::mat4(1.0f);

	
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(0.0f, 1.0f, 1.0f, 1.0f);
	

	glUseProgram(program.programID);

	glClearColor(0.47f, 0.5f, 0.631f, 1.0f);

	winID = LoadTexture("win.png");
	loseID = LoadTexture("lose.png");
	//defID = LoadTexture("def.png");
}

void ProcessInput() {

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
				lander_moving = true;
			
				
				break;
			}
			break;
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (lander_moving == true) {
		if (keys[SDL_SCANCODE_RIGHT]) {
			if (lander_position.x < 24.0f) {
				lander_movement.x += 0.007f;
			}

		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			if (lander_position.x > -24.0f) {
				lander_movement.x += -0.007f;

			}
		}
		else if (lander_movement.x > 0.0f) {

			lander_movement.x -= 0.005f;
		}
		else if (lander_movement.x < 0.0f) {
			lander_movement.x += 0.005f;

		}
		else if (-0.2f < lander_movement.x < 0.2f) {
			lander_movement.x == 0.0f;
		}
		if (lander_position.x > 24.0f) {
			lander_movement.x = 0.0f;
			lander_position.x -= 0.05f;
		}
		if (lander_position.x < -24.0f) {
			lander_movement.x = 0.0f;
			lander_position.x += 0.05f;
		}

	}

	if (glm::length(lander_movement) > 1.0f) {
		lander_movement = glm::normalize(lander_movement);
	}

}


void Updatelander() {

	landerMatrix = glm::scale(landerMatrix, glm::vec3(0.2f, 0.24f, 1.0f));

	if (lander_moving) {
		gravity += 0.0005  ;
		lander_position.y -= gravity;
	}

	float xwalldist = fabs(wall_position.x - lander_position.x) - ((10 ) / 2);
	float ywalldist = fabs(wall_position.y - lander_position.y) -(7.4 /2);
	if (xwalldist < 0 && ywalldist < 0) {
		lander_movement = glm::vec3(0.0f, 0.0f, 0.0f);
		gravity = 0;
		lander_moving = false;
		game_over = true;
	}

	float xwindist = fabs(win_position.x - lander_position.x) - ((7) / 2);
	float ywindist = fabs((win_position.y-13 ) - lander_position.y) - ((6) / 2);
	if (xwindist < 0 && ywindist < 0) {
		lander_movement = glm::vec3(0.0f, 0.0f,0.0f);
		gravity = 0;
		lander_moving = false;
		game_over = true;
		won = true;
	}

	float xlosedist = fabs(win_position.x - lander_position.x) - ((40) / 2);
	float ylosedist = fabs((win_position.y - 14) - lander_position.y) - ((6) / 2);
	if (xlosedist < 0 && ylosedist < 0) {
		lander_movement = glm::vec3(0.0f, 0.0f, 0.0f);
		gravity = 0;
		lander_moving = false;
		game_over = true;
	}

	
	
}


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void Update() {
	
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;
	/*
	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		// Update. Notice it's FIXED_TIMESTEP. Not deltaTime
		

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;
	*/
	lander_position += lander_movement * lander_speed * deltaTime;

	winMatrix = glm::mat4(1.0f);
	loseMatrix = glm::mat4(1.0f);
	wallMatrix = glm::mat4(1.0f);

	wallMatrix = glm::scale(wallMatrix, glm::vec3(1.6f, 1.5f, 1.0f));
	loseMatrix = glm::scale(loseMatrix, glm::vec3(20.0f, 1.0f, 1.0f));

	winMatrix = glm::translate(winMatrix, win_position);
	loseMatrix = glm::translate(loseMatrix, lose_position);
	wallMatrix = glm::translate(wallMatrix, wall_position);

	landerMatrix = glm::mat4(1.0f);
	Updatelander();
	landerMatrix = glm::translate(landerMatrix, lander_position);
	

}

void Render() {

	glClear(GL_COLOR_BUFFER_BIT);
	

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);


	
	glBindTexture(GL_TEXTURE_2D, defID);
	program.SetModelMatrix(winMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(loseMatrix);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(wallMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	program.SetModelMatrix(landerMatrix);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);


	program.SetModelMatrix(msgMatrix);
	if (game_over) {
		if (won == true) {
			glBindTexture(GL_TEXTURE_2D, winID);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, loseID);
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	

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