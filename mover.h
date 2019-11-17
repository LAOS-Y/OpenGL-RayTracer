#ifndef MOVER_H
#define MOVER_H

#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Sphere{
    std::string name;
    float orbit_r;
    float self_r;

    float orbit_a;
    float rotate_a;

    float cur_orbit_deg;
    float cur_rotate_deg;
};

class Mover{
public:
    Mover(){
        tic = glfwGetTime();
    }

    int getNum(){
        return list.size();
    }

    void addSphere(const std::string &name, float orbit_r, float self_r, float orbit_a, float rotate_a){
        Sphere sp = {name, orbit_r, self_r, orbit_a, rotate_a, 0.0, 0.0};
        list.push_back(sp);
    }

    void update(){
        float toc = glfwGetTime();
        float delta_t = toc - tic;
        tic = toc;

        for (auto i = list.begin(); i != list.end(); i++){
            i->cur_orbit_deg += delta_t * i->orbit_a;
            i->cur_rotate_deg += delta_t * i->rotate_a;
        }
    }

    std::string getSphereVec4(int i, glm::vec4 &vec){
        Sphere sp=list[i];

        vec =  glm::vec4(sp.orbit_r, sp.self_r, sp.cur_orbit_deg, sp.cur_rotate_deg);

        return sp.name;
    }

private:
    float tic;
    std::vector<Sphere> list;
};

#endif