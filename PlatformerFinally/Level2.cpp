#include "Level2.h"

#define LEVEL2_WIDTH 15
#define LEVEL2_HEIGHT 7 //good luck!
#define LEVEL2_ENEMY_COUNT 1

unsigned int level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0,
    3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0,
    3, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3, 3, 0,
    3, 2, 2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3, 3, 3
};

void Level2::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);

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
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("putin.png"); //fairly noncontraversial choice I hope. 

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(7, -4, 0);
    state.enemies[0].speed = 0.6;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].move = WAITING;
}

void Level2::Update(float deltaTime) {
    if ((state.player->lose || state.player->win) == false) {

        state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);

        for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
            state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
        }

        if (state.player->position.x >= 14) {
            state.nextScene = 3;
        }
    }
}

void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}