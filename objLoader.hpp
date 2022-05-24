#define _CRT_SECURE_NO_WARNINGS
#include<vector>
#include<string>
#include<glm/glm.hpp>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

struct InfoOBJ {
    GLfloat* list;
    int size;
};

struct InfoOBJ loadOBJ(
    const char* path
    //std::vector < glm::vec3 >& out_vertices,
    //std::vector < glm::vec2 >& out_uvs,
    //std::vector < glm::vec3 >& out_normals
);