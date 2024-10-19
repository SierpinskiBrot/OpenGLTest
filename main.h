#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void initCharacters(FT_Face face);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);