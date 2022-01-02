#include "SimpleMenu.h"


void SimpleMenu::renderText(ShaderProgram* program) {
    Util::DrawText(program, fontTextureID, "Young Biden", 0.8f, -0.25f, glm::vec3(3.0f, -3.5f, 0));
    Util::DrawText(program, fontTextureID, "Press any key to begin", 0.5f, -0.2f, glm::vec3(2.5f, -5.0f, 0));
}