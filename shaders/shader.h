#ifndef SHADER_H
#define SHADER_H

#include "../include/glad/gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int ID;

} Shader;

Shader createShader(const char* vertexPath, const char* fragmentPath); 
void use(Shader* shader);

void setInt(Shader* shader, const char* name, int value);
void setFloat(Shader* shader, const char* name, float value);
void deleteShader(Shader* shader);
#endif
