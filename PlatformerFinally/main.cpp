#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include "SDL_MIXER.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Menu.h"

Mix_Music* looped; //change
Mix_Chunk* bounceFX; //change

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[4];

GLuint fontTextureID;

int lives = 3;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

int findIndex(Scene *arr[], Scene *s) {
    for (int i = 0; i < sizeof(arr); i++) {
        if (s == arr[i]) return i;
    }
    return -1;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    looped = Mix_LoadMUS("terrible_music_taste_jk_freesoundorg.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 10); //my eaaaars
    Mix_PlayMusic(looped, -1);

    bounceFX = Mix_LoadWAV("justinvoke__bounce_freesound.wav"); 

    fontTextureID = Util::LoadTexture("font1.png");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    sceneList[0] = new Menu(fontTextureID);
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    bool started = false;

    currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                //change key set up here

            case SDL_KEYDOWN: //Actually I can't change this. Space needs to be only activated on down not up or any press continiously 
                switch (event.key.keysym.sym) {
                case SDL_KEYDOWN: //pres any key //this did not work for some reason so forgive me for using the enter key like a noob
                case SDLK_RETURN:
                    if (currentScene == sceneList[0] && started == false) {
                        SwitchToScene(sceneList[1]);
                        started = true; //for some reason just a single conditional currentScene == sceneList[0] was not stopping the scene from reloading
                    }
                    break;
                
                case SDLK_SPACE:
                    currentScene->state.player->Jump();
                    Mix_PlayChannel(-1, bounceFX, 0);
                    break;
                }
                break; // for SDL_KEYDOWN This caused so much unneccessary confusion and errors with the brackets in my previous try at the platformer
                
        }
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        currentScene->state.player->movement.x = -1.0f;
    }

    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
        currentScene->state.player->movement.x = 1.0f;
    }
   

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
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
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

    // scroll
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }

    if (currentScene != sceneList[0]) {
        if ((currentScene->state.player->position.y < -6) || (currentScene->state.player->collidedEnemy)) { //collidedEnemy is also true when hit on the top
            //it seems extremely odd that velocity would be exactly zero at collision time and that hasn't come up in testing but better safe than sorry so I just check for collidedEnemy in general
            if (currentScene->state.player->collidedEnemy) currentScene->state.player->collidedEnemy = false; //for some odd reason if I fell through first on level 3, then the putin enemy would be unsquishable. Strange but I don't think that bug is there anymore.
            lives--;
            SwitchToScene(currentScene);
        }
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);

    if (currentScene->state.player->win) {
        Util::DrawText(&program, fontTextureID, "You Win!", 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x + 2.00, -3.0f, 0));
    }

    if (lives == 0) {
        Util::DrawText(&program, fontTextureID, "You Lose! Sorry!", 0.75f, -0.25f, glm::vec3(currentScene->state.player->position.x - .75, -3.0f, 0));
        Util::DrawText(&program, fontTextureID, "Lives:0", 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 3.5, -1.0f, 0));
        currentScene->state.player->lose = true;
    }

    else {
        if (currentScene != sceneList[0]) {
            if (currentScene->state.player->position.x > 4) {
                Util::DrawText(&program, fontTextureID, "Lives:" + std::to_string(lives), 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 3.5, -1.0f, 0)); //string concancantation with a conversion first solved the text display issue. How silly of me to not catch this last year
            }
            else {
                Util::DrawText(&program, fontTextureID, "Lives:" + std::to_string(lives), 0.5f, -0.25f, glm::vec3(1.5f, -1.0f, 0)); //string concancantation with a conversion first solved the text display issue 
            }
        }
    }

    currentScene->Render(&program);
    
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
