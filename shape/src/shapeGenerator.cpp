#include "shapeGenerator.h"
#include "shapedata.h"
#include "vertex.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

using glm::vec3;

ShapeData ShapeGenerator::createTriangle(){
    ShapeData res;
    //默认创建 0，1     -1，-1    1，-1的三角形
    GLfloat myTri[]={
        +0.0f,+1.0f,+0.9f,
        +1.0f,+0.0f,+0.0f,//1

        -1.0f,-1.0f,+0.9f,
        +0.0f,+1.0f,+0.0f,//2

        +1.0f,-1.0f,+0.9f,
        +0.0f,+0.0f,+1.0f,//3
    };
    res.vertex_num = sizeof(myTri)/(sizeof(float)*6);
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices,myTri,sizeof(myTri));

    GLshort indeices[] = {0,1,2};
    res.index_num = sizeof(indeices)/(sizeof(indeices[0]));
    res.indices = new GLshort[res.index_num];
    memcpy(res.indices, indeices, sizeof(indeices));

    return res;
}

ShapeData ShapeGenerator::createCube(){
    ShapeData res;

    Vertex myVertices[] = {
        // Top (0-3)
        Vertex(vec3(-1.0f,+1.0f,+1.0f),vec3(+1.0f,+0.0f,+0.0f)),
        Vertex(vec3(+1.0f,+1.0f,+1.0f),vec3(+0.0f,+1.0f,+0.0f)),
        Vertex(vec3(+1.0f,+1.0f,-1.0f),vec3(+0.0f,+0.0f,+1.0f)),
        Vertex(vec3(-1.0f,+1.0f,-1.0f),vec3(+1.0f,+1.0f,+1.0f)),

        // Front (4-7)
        Vertex(vec3(-1.0f,+1.0f,-1.0f),vec3(+1.0f,+0.0f,+1.0f)),
        Vertex(vec3(+1.0f,+1.0f,-1.0f),vec3(+0.0f,+0.5f,+0.2f)),
        Vertex(vec3(+1.0f,-1.0f,-1.0f),vec3(+0.8f,+0.6f,+0.4f)),
        Vertex(vec3(-1.0f,-1.0f,-1.0f),vec3(+0.3f,+1.0f,+0.5f)),

        // Right (8-11)
        Vertex(vec3(+1.0f,+1.0f,-1.0f),vec3(+0.2f,+0.5f,+0.2f)),
        Vertex(vec3(+1.0f,+1.0f,+1.0f),vec3(+0.9f,+0.3f,+0.7f)),
        Vertex(vec3(+1.0f,-1.0f,+1.0f),vec3(+0.3f,+0.7f,+0.5f)),
        Vertex(vec3(+1.0f,-1.0f,-1.0f),vec3(+0.5f,+0.7f,+0.5f)),

        // Left / -X (12-15)
        Vertex(vec3(-1.0f,+1.0f,+1.0f),vec3(+0.7f,+0.8f,+0.2f)),
        Vertex(vec3(-1.0f,+1.0f,-1.0f),vec3(+0.5f,+0.7f,+0.3f)),
        Vertex(vec3(-1.0f,-1.0f,-1.0f),vec3(+0.4f,+0.7f,+0.7f)),
        Vertex(vec3(-1.0f,-1.0f,+1.0f),vec3(+0.2f,+0.5f,+1.0f)),

        // Back / +Z (16-19)
        Vertex(vec3(+1.0f,+1.0f,+1.0f),vec3(+0.6f,+1.0f,+0.7f)),
        Vertex(vec3(-1.0f,+1.0f,+1.0f),vec3(+0.6f,+0.4f,+0.8f)),
        Vertex(vec3(-1.0f,-1.0f,+1.0f),vec3(+0.2f,+0.8f,+0.7f)),
        Vertex(vec3(+1.0f,-1.0f,+1.0f),vec3(+0.2f,+0.7f,+1.0f)),

        // Bottom (20-23)
        Vertex(vec3(+1.0f,-1.0f,-1.0f),vec3(+0.8f,+0.3f,+0.7f)),
        Vertex(vec3(-1.0f,-1.0f,-1.0f),vec3(+0.8f,+0.9f,+0.5f)),
        Vertex(vec3(-1.0f,-1.0f,+1.0f),vec3(+0.5f,+0.8f,+0.5f)),
        Vertex(vec3(+1.0f,-1.0f,+1.0f),vec3(+0.9f,+1.0f,+0.2f)),
    };

    res.vertex_num = sizeof(myVertices)/sizeof(Vertex);
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices, myVertices, sizeof(myVertices));

    GLshort myIndices[] = {
        0,1,2,0,2,3,       // top
        4,5,6,4,6,7,       // front
        8,9,10,8,10,11,    // right
        12,13,14,12,14,15, // left
        16,17,18,16,18,19, // back
        20,22,21,20,23,22, // bottom（教程原版顺序）
    };

    res.index_num = sizeof(myIndices)/sizeof(GLshort);
    res.indices = new GLshort[res.index_num];
    memcpy(res.indices, myIndices, sizeof(myIndices));

    return res;
}
