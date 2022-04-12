#include<vector>
#include<string>
#include "glew.h"
#include "glut.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "glm/glm.hpp"
#include "glm/fwd.hpp"


std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;

void loadOBJ(const char* path)
{
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        /*struct InfoOBJ returnMe;
        returnMe.list = NULL;
        returnMe.size = 0;
        return returnMe;*/
    }

    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            //printf("added %2.6f %2.6f %2.6f\n", vertex.x, vertex.y, vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            /*if (matches != 9) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return NULL;
            }*/
            //printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", vertexIndex[0], uvIndex[0], normalIndex[0], vertexIndex[1], uvIndex[1], normalIndex[1], vertexIndex[2], uvIndex[2], normalIndex[2]);
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

            // For each vertex of each triangle
            for (unsigned int i = 0; i < vertexIndices.size(); i++) {

                unsigned int vertexIndex = vertexIndices[i];

                glm::vec3 vertex = temp_vertices[vertexIndex - 1];

                //out_vertices.push_back(vertex);
            }

        }
    }

    int arrayLen = temp_vertices.size() * 11;
    printf("arrayLen: %d \n", arrayLen);
    printf("ARRAY SIZES: %d %d %d\n", temp_vertices.size(), temp_normals.size(), temp_uvs.size());
    GLfloat* toReturnInfo = new GLfloat[arrayLen];
    //printf("SIZE?? %d\n", sizeof(toReturnInfo));
    int spotInVertex = 0;
    int spotInNormals = 0;
    int spotInTex = 0;

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < arrayLen; i += 11)
    {
        toReturnInfo[i] = temp_vertices.at(spotInVertex).x;
        toReturnInfo[i + 1] = temp_vertices.at(spotInVertex).y;
        toReturnInfo[i + 2] = temp_vertices.at(spotInVertex).z;

        glVertex3f(temp_vertices.at(spotInVertex).x, temp_vertices.at(spotInVertex).y, temp_vertices.at(spotInVertex).z);

        toReturnInfo[i + 3] = 1.f;
        toReturnInfo[i + 4] = 1.f;
        toReturnInfo[i + 5] = 1.f;

        if (spotInTex >= temp_uvs.size()) {
            toReturnInfo[i + 6] = 0;
            toReturnInfo[i + 7] = 0;
        }
        else {
            toReturnInfo[i + 6] = temp_uvs.at(spotInTex).s;
            toReturnInfo[i + 7] = temp_uvs.at(spotInTex).t;

        }

        if (spotInNormals >= temp_normals.size()) {
            toReturnInfo[i + 8] = 0;
            toReturnInfo[i + 9] = 0;
            toReturnInfo[i + 10] = 0;
        }
        else {
            toReturnInfo[i + 8] = temp_normals.at(spotInNormals).x;
            toReturnInfo[i + 9] = temp_normals.at(spotInNormals).y;
            toReturnInfo[i + 10] = temp_normals.at(spotInNormals).z;
        }


        spotInVertex++;
        spotInNormals++;
        spotInTex++;
        /*fprintf(stderr, "%d check obj: %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f\n",
            i,
            toReturnInfo[i],
            toReturnInfo[i + 1],
            toReturnInfo[i + 2],

            toReturnInfo[i + 3],
            toReturnInfo[i + 4],
            toReturnInfo[i + 5],

            toReturnInfo[i + 6],
            toReturnInfo[i + 7],

            toReturnInfo[i + 8],
            toReturnInfo[i + 9],
            toReturnInfo[i + 10]
        );
        printf("arrayLen: %d \n", arrayLen - (i));//*/
    }

    glEnd();

    /*struct InfoOBJ returnMe;
    returnMe.list = toReturnInfo;
    returnMe.size = arrayLen;*/

    return;
}