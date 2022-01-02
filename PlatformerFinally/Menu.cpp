#include "Menu.h"

#define MENU_WIDTH 14
#define MENU_HEIGHT 8
#define MENU_ENEMY_COUNT 1

unsigned int menu_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



void Menu::Initialize() {

    state.nextScene = -1;


    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -3.0f, 0);
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

    state.player->height = 0.8f; //doesnt matter
    state.player->width = 0.8f; //doesnt matter
    state.player->isActive = false;


    // Initialize Enemies
    state.enemies = new Entity[MENU_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(2, -2.25, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].move = WAITING;
    state.enemies[0].isActive = false;
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, MENU_ENEMY_COUNT, state.map);
}

void Menu::Render(ShaderProgram* program) {
    Util::DrawText(program, fontTextureID, "Young Biden", 0.8f, -0.25f, glm::vec3(3.0f, -3.5f, 0));
    Util::DrawText(program, fontTextureID, "Press any key to begin...", 0.5f, -0.2f, glm::vec3(2.5f, -5.0f, 0));
    state.map->Render(program);
    state.player->Render(program);
}