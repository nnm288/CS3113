#include "Entity.h"

Entity::Entity()
{
	position = glm::vec3(0);
	movement = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);
	speed = 0;

modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
	if (isActive == false || other->isActive == false) { return false; }

	float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
	float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
	if (xdist < 0 && ydist < 0) { return true; }
	return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount) {

	for (int i = 0; i < objectCount; i++) {
		Entity* object = &objects[i];

		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object->position.y);
			float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
			if (velocity.y > 0) {
				collidedTop = true;
				position.y -= penetrationY;
				velocity.y = 0;

			}
			else if (velocity.y < 0) {
				collidedBot = true;
				if (object->entityType == ENEMY) {
					object->isActive = false;
				}
				position.y += penetrationY;
				velocity.y = 0;

			}

		}
	}
}
void Entity::CheckCollisionsX(Entity* objects, int objectCount) {

	for (int i = 0; i < objectCount; i++) {
		Entity* object = &objects[i];

		if (CheckCollision(object))
		{
			float xdist = fabs(position.x - object->position.x);
			float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
			if (velocity.x > 0) {
				collidedRight = true;
				position.x -= penetrationX;
				velocity.x = 0;

			}
			else if (velocity.x < 0) {
				collidedLeft = true;
				position.x += penetrationX;
				velocity.x = 0;

			}

		}
	}
}

void Entity::AIWalker(Entity* player) {
	switch (aiState)
	{
	case IDLE:
		movement = glm::vec3(0, 0, 0);
		if (glm::distance(position, player->position) < 1.5f) {
			aiState = RWALKING;
		}
	case LWALKING:
		movement = glm::vec3(-1, 0, 0);
		if (glm::distance(position, player->position) < 1.5f) {
			aiState = RWALKING;
		}
		else if (position.x < -2.5f) {
			aiState = RWALKING;
		}
		break;

	case RWALKING:
		movement = glm::vec3(1, 0, 0);
		if (position.x > 1.9f) { aiState = LWALKING; }
		break;
	}
}

void Entity::AIJumper(Entity* player) {

	switch (aiState)
	{
	case IDLE:
		aiState = JUMPING;

	case JUMPING:
		if (position.y < -2.73) {
			velocity.y += 4.4;
		}

		if (position.y > -1.3){
			velocity.y -= 0.7;
}
		if (collidedBot) {
			aiState = IDLE;
		}
	
	}
	
}

void Entity::AIFollower(Entity* player) {

	movement.x = player->movement.x;

}

void Entity::AI(Entity* player) {
	switch (aiType)
	{
	case WALKER:
		AIWalker(player);
		break;

	case JUMPER:
		AIJumper(player);
		break;


	case FOLLOWER:
		AIFollower(player);
		break;
	}
}


void Entity::Update(float deltaTime, Entity* player, Entity* enemy, int enemyCount, Entity* object, int objectCount)
{
	if (isActive == false) { return; }

	collidedTop = false;
	collidedBot = false;
	collidedLeft = false;
	collidedRight = false;

	if (entityType == PLAYER) {
		CheckCollisionsY(enemy, enemyCount);
		CheckCollisionsX(enemy, enemyCount);
		if (collidedLeft || collidedRight || collidedTop) { isActive = false; }

	}
	if (entityType == ENEMY) {
		AI(player);
		CheckCollisionsY(player, 1);
		if (collidedTop) { isActive = false; }
		if (collidedBot) { player->isActive = false; }
		
		
	}

	

	if (animIndices != NULL) {
		if (glm::length(movement) != 0) {
			animTime += deltaTime;

			if (animTime >= 0.25f)
			{
				animTime = 0.0f;
				animIndex++;
				if (animIndex >= animFrames)
				{
					animIndex = 0;
				}
			}
		}
		else {
			animIndex = 0;
		}
	}

	if (jump) {
		jump = false;
		velocity.y += jumpPower;
	}

	velocity.x = movement.x * speed;
	velocity += acceleration * deltaTime;
	
	position.y += velocity.y * deltaTime;
	CheckCollisionsY(object, objectCount);
	
	position.x += velocity.x * deltaTime;
	CheckCollisionsX(object, objectCount);


	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
	float u = (float)(index % animCols) / (float)animCols;
	float v = (float)(index / animCols) / (float)animRows;

	float width = 1.0f / (float)animCols;
	float height = 1.0f / (float)animRows;

	float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
		u, v + height, u + width, v, u, v };

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
	
	if (isActive == false) { return; }

	program->SetModelMatrix(modelMatrix);

	if (animIndices != NULL) {
		DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
		return;
	}

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}