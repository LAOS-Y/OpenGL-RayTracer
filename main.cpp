#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.h"
#include "utils.h"
#include "camera.h"

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

float vertices[] = {
    // positions          // texture coords
    1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
    1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

unsigned int VBO, VAO, EBO;

void initGLDataObject(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err = glewInit();
    std::cout << "ERR:" << err << std::endl;

    Shader renderShader("./raytracer.vert", "./raytracer.frag"); 
    Shader computeShader("./raytracer.comp");

    initGLDataObject();


    // load and create a texture 
    // -------------------------

    unsigned int specular = genTexture(SCR_WIDTH, SCR_HEIGHT, GL_TEXTURE0);
    unsigned int diffuse = genTexture(SCR_WIDTH, SCR_HEIGHT, GL_TEXTURE1);
    unsigned int ambient = genTexture(SCR_WIDTH, SCR_HEIGHT, GL_TEXTURE2);

    glBindImageTexture(0, specular, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindImageTexture(1, diffuse, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindImageTexture(2, ambient, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    std::cout << "DONE INIT RT SCENE TEXTURE" << std::endl;

    unsigned int texture1 = genTexturefromPath("plant2.jpg", GL_TEXTURE3);
    unsigned int texture2 = genTexturefromPath("plant3.jpg", GL_TEXTURE4);

    std::cout << "DONE INIT planet TEXTURE" << std::endl;

    Camera camera(glm::vec3(0.0, 0.0, 1.0),
                  glm::vec3(0.0, 0.0, -1.0),
                  glm::vec3(0.0, 1.0, 0.0),
                  window=window,
                  0.2, 20);

    std::cout << "Start rendering" << std::endl;

    double tic = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double toc = glfwGetTime();
        std::string title = std::string("FPS: ") + double2str(1 / (toc - tic));
        glfwSetWindowTitle(window, title.c_str());
        tic = toc;

        // input
        // -----
        processInput(window);
        camera.update();

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        computeShader.use();

        computeShader.setUniVec3("cam_origin", camera.origin());
        computeShader.setUniVec3("cam_lookat", camera.lookat());
        computeShader.setUniVec3("cam_lookup", camera.lookup());


        // glActiveTexture(GL_TEXTURE3);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        glDispatchCompute(SCR_WIDTH, SCR_HEIGHT * 3, 1);

        // render container
        renderShader.use();
        renderShader.setUniInt("tex_specular", 0);
        renderShader.setUniInt("tex_diffuse", 1);
        renderShader.setUniInt("tex_ambient", 2);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
