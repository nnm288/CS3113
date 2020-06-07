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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, titleMatrix, starMatrix1, starMatrix2, projectionMatrix;

float triangle_translate = 0;
float triangle_scale = 1.0f;
float star_rotate = 0;
float star_scale = 1;

GLuint playerTextureID;
GLuint titleTextureID;
GLuint starTextureID;

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
	displayWindow = SDL_CreateWindow("Minjamin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	titleMatrix = glm::mat4(1.0f);
	starMatrix1 = glm::mat4(1.0f);
	starMatrix2 = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	

	glUseProgram(program.programID);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	playerTextureID = LoadTexture("ctg.png");
	titleTextureID = LoadTexture("title.png");
	starTextureID = LoadTexture("star.png");
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			gameIsRunning = false;
		}
	}
}


float lastTicks = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;


	
	titleMatrix = glm::mat4(1.0f);
	titleMatrix = glm::translate(titleMatrix, glm::vec3(0.0f, 0.8f, 0.0f));
	titleMatrix = glm::scale(titleMatrix, glm::vec3(6.0f, 3.0f, 1.0f));
	
	if (triangle_translate > -1.0f) {
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		triangle_translate -= 0.3f * deltaTime;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, triangle_translate, 0.0f));
		
	}

	float star_scale = triangle_translate;
	starMatrix1 = glm::mat4(1.0f);
	starMatrix2 = glm::mat4(1.0f);
	
	starMatrix1 = glm::translate(starMatrix1, glm::vec3(4.0f, 0.0f, 0.0f));
	starMatrix2 = glm::translate(starMatrix2, glm::vec3(-4.0f, 0.0f, 0.0f));

	
	star_scale += 0.1 * deltaTime;
	starMatrix1 = glm::scale(starMatrix1, glm::vec3(star_scale, star_scale, 0.0f));
	starMatrix2 = glm::scale(starMatrix2, glm::vec3(star_scale, star_scale, 0.0f));

	star_rotate += 480.0f * deltaTime;
	starMatrix1 = glm::rotate(starMatrix1, glm::radians(star_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	starMatrix2 = glm::rotate(starMatrix2, glm::radians(-star_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Render() {

	glClear(GL_COLOR_BUFFER_BIT);
	

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	program.SetModelMatrix(starMatrix1);
	glBindTexture(GL_TEXTURE_2D, starTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(starMatrix2);
	glBindTexture(GL_TEXTURE_2D, starTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(modelMatrix);
	glBindTexture(GL_TEXTURE_2D, playerTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	program.SetModelMatrix(titleMatrix);
	glBindTexture(GL_TEXTURE_2D, titleTextureID);
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