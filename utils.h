#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

std::string double2str(double x, int precision=2){
    float tmp = 0.5;
    for (int i = 0; i < precision; i++){
        tmp /= 10;
    }
    
    x += tmp;

    int a = x;
    x -= a;

    std::string ans = "";

    while (a){
        ans = char('0' + a % 10) + ans;
        a /= 10;
    }

    if (ans == ""){
        ans = "0";
    }
    
    ans += '.';

    for (int i = 0; i < precision; i++){
        x *= 10;
        ans += char('0' + int(x) % 10);
    }

    return ans;
}

unsigned int genTexture(int width, int height, GLenum tex_unit, const char *ptr=NULL){
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(tex_unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, ptr);

    return texture;
}

#endif