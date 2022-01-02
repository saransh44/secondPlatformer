#include "Level1.h"

#define LEVEL1_WIDTH 15 //I wanted to point to something polical with these numbers but alas no numbers were a realistic combination. The only ones that come to mind a 50x50 to signify the senate split or like 7x70 to signify the only piece of major spending legislation to come out of the administration unfortunately. Funny how we couldnt have 1.75 trillion over 10 years in social spending but when it cmes to military defense for a single year, we don't ever have to worry about the debt or deficiets in that regard. It's truly maddeningly hypocritical and I hope that even republicans *sigh* even those who are anti-vax can agree with me on that. 
#define LEVEL1_HEIGHT 7 //for good luck
#define LEVEL1_ENEMY_COUNT 1

unsigned int level1_data[] =
{   
   
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 1,
	3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
	3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {

    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, -4, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -2.0f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("no_biden_spritesheet_available.png"); //I wanted to have a joe biden sprite sheet but evenly spacing things out and evenly drawing his legs walking in gamermaker studio's pixel editor proved to be time consuming


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
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("kim.png"); //fairly noncontraversial choice I hope. 

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO; //this is from the ai tutorials and whatnot
    state.enemies[0].move = WAITING;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(7, -4, 0);
    state.enemies[0].speed = 0.6;
    
}

void Level1::Update(float deltaTime) {
    if ((state.player->lose || state.player->win) == false) {

        state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);

        for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
            state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
        }

        if (state.player->position.x >= 14) {
            state.nextScene = 2;
        }
    }
}

void Level1::Render(ShaderProgram *program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}