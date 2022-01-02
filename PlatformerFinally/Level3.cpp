#include "Level3.h"

#define LEVEL3_WIDTH 16
#define LEVEL3_HEIGHT 7 //good luck 
#define LEVEL3_ENEMY_COUNT 1

unsigned int level3_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3,
    3, 0, 2, 0, 1, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 3, 0, 2, 0, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3
};

//This level design is slightly cheap and illogical of me but meant to increase difficulty by the player thinking the AI can't walk over gaps because the player can't without falling through. I really need to make Cat Mario after this

void Level3::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -2.0f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("no_biden_spritesheet_available.png");

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

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    // Initialize Enemies
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("putin.png");

    state.enemies[0].entityType = ENEMYBOUNCE;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].move = WALKING;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(5, -4, 0);
    state.enemies[0].speed = 0.6f;
}

void Level3::Update(float deltaTime) {
    if ((state.player->lose || state.player->win) == false) {

        state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);

        for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
            state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
        }

        if (state.player->position.x >= 14) {
            state.player->win = true;
        }
    }
}

void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}