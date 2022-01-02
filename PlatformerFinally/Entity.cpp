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
void Entity::Jump()
{
    if (collidedBottom)
    {
        velocity.y += 2.0f;
    }
}
bool Entity::CheckCollision(Entity *other) {
    if (other == this) return false;

    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width/2.0f + other->width/2.0f)/2.0f); //This is actually so weird but if you take out the last 2.0f, you'll find that essentially collisions happen just a couple of frames before because you aren't stopping in time towards the enemy. You can put a debug here and see it which is nice but visually speaking I didn't like it that it really does not "look" like the collision actually occured unless I add the last /2.0f to let things close the distance a bit more
    float ydist = fabs(position.y - other->position.y) - ((height/2.0f + other->height/2.0f));

    if (xdist < 0 && ydist < 0) 
        return true;

    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float ydist = fabs(position.y - object->position.y);

            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if ((object->entityType == ENEMY || object->entityType == ENEMYBOUNCE) && (xdist < 0.3)) {
                    win = false;
                    collidedEnemy = true; //The enemy has somehow gotten on top of me and now I lose a life!
                }
            }

            else if (velocity.y < 0) {
                if ((object->entityType == ENEMY || object->entityType == ENEMYBOUNCE) && (xdist < 0.3)) {
                    object->isActive = false; // the enemy is squished
                }
                

                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true; 

                if ((object->entityType == ENEMYBOUNCE)) {
                    this->Jump(); //that good ol' platformer feel ahhhhhhh

                    object->isActive = false;
                }
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {

            if (object->entityType == ENEMY || object->entityType == ENEMYBOUNCE) { //Horizonontal Enemy Collision
                collidedEnemy = true;
                win = false;
            }

            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}

void Entity::CheckCollisionsY(Map *map)
{
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z = 0);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z = 0); //either this or the below two lines needed to be commented out and well I choose simplicity
    
    //This is actually a better way to check for collisions then I was doing in RiseoftheAI that I found as I was going through the videos and whatnot on platformers. It accounts for corners which is interesting and honestly I wish I had more time to debug the code because I feel like the glitch below can be solved through other means honestly than commenting out nice code.  
    //However, I feel like it is still glitchy esp at a corner. You can unncomment this and see that the player will sort of get stuck in a side block even when the jump button is pressed. I believe if I were to have like a spiderman style wall jumping from side to side, or even want to have more complex AI paths like in  rise of the Ai with good colision, this code actually needs to be cleaned up take into account the velocity . Right now simply having a piece stuck on the side feels illogical
    //glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z = 0);
    //glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z = 0);

    //glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z = 0); 
    //glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z = 0);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    /*else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) { //NO MORE STICKING TO THE SIDE AND NOT JUMPING GLITCH WOOHOO!
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }*/
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    /*else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }*/
}

void Entity::CheckCollisionsX(Map *map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z = 0);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z = 0);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::AIIdle() {}

void Entity::AIWaitAndGo(Entity *player) {  
    bool fly = true;
    switch (move) {
        case WAITING:
            if (glm::distance(position, player->position) < 3.0f) {

                if (this->entityType == ENEMYBOUNCE) {
                    move = WALKING; 
                }
                else
                    move = FLOATING;
            }
            break;
        case FLOATING:
            if (fly == true) {
                movement = glm::vec3(0, 1, 0);
            }
            if (position.y < -2)
                fly = true;
            else if (position.y >= -2) 
                fly = false;   

            if (fly == false) 
                movement = glm::vec3(0, -1, 0);
            
            break;

        case WALKING:
            if (player->position.x < position.x) {
                movement = glm::vec3(-1, 0, 0);
            }
            else {
                movement = glm::vec3(1, 0, 0);
            }
            break;

    }
}


void Entity::AI(Entity *player) {
    switch (aiType) {
        case IDLE:
            AIIdle();
            break;
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
    }
}


void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map)
{
    if (isActive == false) return;
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (entityType == ENEMY || entityType == ENEMYBOUNCE) {
        AI(player);
        //velocity.y = movement.y * speed; // velocity is truely a calculation that has both a north and south as well as east and west direction 
        
    }

    //this is absolutely neat and great code from the Youtube series for executing small animations of walking. Thank you so much professor. 
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


    
    // velocity is truely a calculation that has both a north and south as well as east and west direction 
    velocity.x += movement.x * speed * deltaTime;  
    velocity.y += movement.y * speed * deltaTime;

    //if the player likes a more old school platformer feel, I can comment out the 238 and 239th lines and uncomment the below line
    //velocity.x = movement.x * speed; //Because we aren't constantly adding to velocity, it essentially flickers between a few consistent values based on my keyboard press like 0 or speed which is always two and movement.x which is either simply -1 or 1, this always yields either 0, -2, or 2 making it just feel consistent but forgoing acceleration always feels slightly illogical of me. If I have to run, I don't get a immediate stop and so I choose to the above two lines.

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed

    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed

    if (entityType == PLAYER) {

        CheckCollisionsY(objects, objectCount);
        CheckCollisionsX(objects, objectCount);

    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
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
