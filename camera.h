#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
    Camera(const glm::vec3 &origin,
           const glm::vec3 &lookat,
           const glm::vec3 &lookup,
           GLFWwindow *window,
           float v=0, float a=0){
        _origin = origin;
        _lookat = glm::normalize(lookat);
        _lookup = glm::normalize(lookup);
        _window = window;
        
        tic = glfwGetTime();

        _v = v;
        _a = a;
    }

    glm::vec3 origin(){
        return _origin;
    }
    
    glm::vec3 lookat(){
        return _lookat;
    }
    
    glm::vec3 lookr(){
        return glm::normalize(glm::cross(_lookat, _lookup));
    }

    glm::vec3 lookup(){
        return _lookup;
    }

    void update(){
        float toc = glfwGetTime();
        float dt = toc - tic;
        tic = toc;

        _translate(dt);
        _rotate(dt);
    }

private:
    glm::vec3 _origin;
    glm::vec3 _lookat;
    glm::vec3 _lookup;

    GLFWwindow *_window;
    
    float tic;
    float _v;
    float _a;

    glm::vec3 _rotateVec3(const glm::vec3 &vec, const glm::vec3 &axis, float angel){
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angel), glm::normalize(axis));
        glm::vec4 vec_ = glm::vec4(vec, 1);

        vec_ = rotate * vec_;

        return glm::vec3(vec_.x, vec_.y, vec_.z);
    }

    void _translate(float dt){
        float delta = _v * dt;

        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS){
            _origin += lookat() * delta;
        }

        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS){
            _origin -= lookat() * delta;
        }

        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS){
            _origin += lookr() * delta;
        }

        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS){
            _origin -= lookr() * delta;
        }

        if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS){
            _origin += lookup() * delta;
        }

        if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
            _origin -= lookup() * delta;
        }
    }

    void _rotate(float dt){
        float delta = _a * dt;

        if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS){
            _lookat = _rotateVec3(_lookat, lookr(), delta);
            _lookup = _rotateVec3(_lookup, lookr(), delta);
        }

        if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS){
            _lookat = _rotateVec3(_lookat, lookr(), -delta);
            _lookup = _rotateVec3(_lookup, lookr(), -delta);
        }

        if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS){
            _lookat = _rotateVec3(_lookat, lookup(), delta);
        }

        if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            _lookat = _rotateVec3(_lookat, lookup(), -delta);
        }
    }
};

#endif