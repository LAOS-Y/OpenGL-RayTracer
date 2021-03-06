#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        id = glCreateProgram();

        int success;
        char log[512];

        int v_shader = glCreateShader(GL_VERTEX_SHADER);
        _createShader(vertexPath, "VERTEX", v_shader);

        int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
        _createShader(fragmentPath, "FRAGMENT", f_shader);

        glLinkProgram(id);

        glGetShaderiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
        }

        glDeleteShader(v_shader);
        glDeleteShader(f_shader);
    }

    Shader(const char* computePath)
    {
        id = glCreateProgram();

        int success;
        char log[512];

        int c_shader = glCreateShader(GL_COMPUTE_SHADER);
        _createShader(computePath, "COMPUTE", c_shader);

        glLinkProgram(id);

        glGetShaderiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
        }

        glDeleteShader(c_shader);
    }

    void use(){
        glUseProgram(id);
    }

    void setUniBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
    }

    void setUniInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
    }

    void setUniFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
    }

    void setUniVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); 
    }

    void setUniVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); 
    }

    void setUniVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); 
    }

    void setUniMat4(const std::string &name, const glm::mat4 &value){
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    int id;
    
    void _createShader(const char *path, const char *shader_type, int shader_id){
        std::string code;
        std::ifstream file;

        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            file.open(path);
            std::stringstream stream;

            stream << file.rdbuf();
            file.close();

            code = stream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::" << shader_type << "::SOURCE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* code_char = code.c_str();

        // std::cout << code_char << std::endl;

        glShaderSource(shader_id, 1, &code_char, NULL);
        glCompileShader(shader_id);

        int success;
        char log[512];

        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_id, 512, NULL, log);
            std::cout << "ERROR::SHADER::" << shader_type << "::COMPILATION_FAILED\n" << log << std::endl;
        }
        glAttachShader(id, shader_id);
    }
};

#endif
