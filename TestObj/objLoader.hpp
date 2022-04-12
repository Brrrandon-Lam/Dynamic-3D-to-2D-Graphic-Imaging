#define _CRT_SECURE_NO_WARNINGS
#include<vector>
#include<string>
#include "glew.h"
#include "glut.h"

#include <GL/gl.h>
#include <GL/glu.h>

struct InfoOBJ {
    GLfloat* list;
    int size;
};

void loadOBJ(
    const char* path
    //std::vector < glm::vec3 >& out_vertices,
    //std::vector < glm::vec2 >& out_uvs,
    //std::vector < glm::vec3 >& out_normals
);