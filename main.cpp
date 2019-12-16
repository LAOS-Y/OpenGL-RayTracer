#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.h"
#include "utils.h"
#include "camera.h"
#include "mover.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const int N_SAMPLES = 2;

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

void initSceneTexture(){
    unsigned int specular, diffuse, ambient;

    unsigned int tex_units[3][2] = {GL_TEXTURE0, GL_TEXTURE1,
                                    GL_TEXTURE2, GL_TEXTURE3,
                                    GL_TEXTURE4, GL_TEXTURE5};

    for (int i = 0; i < N_SAMPLES; i++){
        specular = genTexture(SCR_WIDTH, SCR_HEIGHT, tex_units[0][i]);
        diffuse = genTexture(SCR_WIDTH, SCR_HEIGHT, tex_units[1][i]);
        ambient = genTexture(SCR_WIDTH, SCR_HEIGHT, tex_units[2][i]);

        glBindImageTexture(i, specular, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glBindImageTexture(i + N_SAMPLES, diffuse, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glBindImageTexture(i + N_SAMPLES * 2, ambient, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    }
}

void initObjectTexture(){
    genTexturefromPath("textures/sun.jpg", GL_TEXTURE6);
    genTexturefromPath("textures/mercury.jpg", GL_TEXTURE7);
    genTexturefromPath("textures/venus.jpg", GL_TEXTURE8);
    genTexturefromPath("textures/earth.jpg", GL_TEXTURE9);
    genTexturefromPath("textures/mars.jpg", GL_TEXTURE10);
    genTexturefromPath("textures/jupiter.jpg", GL_TEXTURE11);
    genTexturefromPath("textures/saturn.jpg", GL_TEXTURE12);
    genTexturefromPath("textures/uranus.jpg", GL_TEXTURE13);
    genTexturefromPath("textures/neptune.jpg", GL_TEXTURE14);
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

    // unsigned int specular, diffuse, ambient;
    initSceneTexture();
    std::cout << "DONE INIT RT SCENE TEXTURE" << std::endl;

    initObjectTexture();
    std::cout << "DONE INIT OBJECT TEXTURE" << std::endl;

    Mover mover;
    mover.addSphere("sphere0", 0, randomIn(0.04, 0.05), 0, 0);
    mover.addSphere("sphere1", 0.1, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    mover.addSphere("sphere2", 0.2, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    mover.addSphere("sphere3", 0.3, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    // mover.addSphere("sphere4", 0.4, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    // mover.addSphere("sphere5", 0.5, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    // mover.addSphere("sphere6", 0.6, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    // mover.addSphere("sphere7", 0.7, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));
    // mover.addSphere("sphere8", 0.8, randomIn(0.02, 0.04), randomIn(10, 15), randomIn(10, 20));

    for (int i = 0; i < 5; i++){
        std::cout << random01() << std::endl;
    }

    Camera camera(glm::vec3(0.0, 0.0, 0.5),
                  glm::vec3(0.0, 0.0, -0.5),
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

        for (int i = 0; i < mover.getNum(); i++){
            glm::vec4 vec;
            std::string name = mover.getSphereVec4(i, vec);

            computeShader.setUniVec4(name, vec);
        }

        mover.update();

        glDispatchCompute(SCR_WIDTH, SCR_HEIGHT, 1);

        // render container
        renderShader.use();
        renderShader.setUniInt("tex_specular[0]", 0);
        renderShader.setUniInt("tex_diffuse[0]", 2);
        renderShader.setUniInt("tex_ambient[0]", 4);

        renderShader.setUniInt("tex_specular[1]", 1);
        renderShader.setUniInt("tex_diffuse[1]", 3);
        renderShader.setUniInt("tex_ambient[1]", 5);


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
