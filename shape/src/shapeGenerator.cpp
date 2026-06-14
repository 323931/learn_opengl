#include "shapeGenerator.h"
#include "shapedata.h"
#include "vertex.h"
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "random.cpp"
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

    GLushort indeices[] = {0,1,2};
    res.index_num = sizeof(indeices)/(sizeof(indeices[0]));
    res.indices = new GLushort[res.index_num];
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

    GLushort myIndices[] = {
        0,1,2,0,2,3,       // top
        4,5,6,4,6,7,       // front
        8,9,10,8,10,11,    // right
        12,13,14,12,14,15, // left
        16,17,18,16,18,19, // back
        20,22,21,20,23,22, // bottom（教程原版顺序）
    };

    res.index_num = sizeof(myIndices)/sizeof(GLushort);
    res.indices = new GLushort[res.index_num];
    memcpy(res.indices, myIndices, sizeof(myIndices));

    return res;
}

ShapeData ShapeGenerator::createArrow(){
    ShapeData res;
    Vertex stackVerts[] =
    {
        // Top side of arrow head
        Vertex(glm::vec3( 0.00f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 0
        Vertex(glm::vec3( 0.50f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 1
        Vertex(glm::vec3( 0.00f,  0.25f, -1.00f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 2
        Vertex(glm::vec3(-0.50f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 3
        // Bottom side of arrow head
        Vertex(glm::vec3( 0.00f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 4
        Vertex(glm::vec3( 0.50f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 5
        Vertex(glm::vec3( 0.00f, -0.25f, -1.00f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 6
        Vertex(glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 7
        // Right side of arrow tip
        Vertex(glm::vec3( 0.50f,  0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f)),// 8
        Vertex(glm::vec3( 0.00f,  0.25f, -1.00f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 9
        Vertex(glm::vec3( 0.00f, -0.25f, -1.00f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 10
        Vertex(glm::vec3( 0.50f, -0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 11
        // Left side of arrow tip
        Vertex(glm::vec3( 0.00f,  0.25f, -1.00f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 12
        Vertex(glm::vec3(-0.50f,  0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 13
        Vertex(glm::vec3( 0.00f, -0.25f, -1.00f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 14
        Vertex(glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 15
        // Back side of arrow tip
        Vertex(glm::vec3(-0.50f,  0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 16
        Vertex(glm::vec3( 0.50f,  0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 17
        Vertex(glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 18
        Vertex(glm::vec3( 0.50f, -0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 19
        // Top side of back of arrow  
        Vertex(glm::vec3( 0.25f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 20
        Vertex(glm::vec3( 0.25f,  0.25f,  1.00f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 21
        Vertex(glm::vec3(-0.25f,  0.25f,  1.00f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 22
        Vertex(glm::vec3(-0.25f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f)), // 23
        // Bottom side of back of arrow
        Vertex(glm::vec3( 0.25f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 24
        Vertex(glm::vec3( 0.25f, -0.25f,  1.00f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 25
        Vertex(glm::vec3(-0.25f, -0.25f,  1.00f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 26
        Vertex(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f)), // 27
        // Right side of back of arrow
        Vertex(glm::vec3( 0.25f,  0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 28
        Vertex(glm::vec3( 0.25f, -0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 29
        Vertex(glm::vec3( 0.25f, -0.25f,  1.00f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 30
        Vertex(glm::vec3( 0.25f,  0.25f,  1.00f), glm::vec3( 0.60f,  1.00f,  0.00f)), // 31
        // Left side of back of arrow
        Vertex(glm::vec3(-0.25f,  0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 32
        Vertex(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 33
        Vertex(glm::vec3(-0.25f, -0.25f,  1.00f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 34
        Vertex(glm::vec3(-0.25f,  0.25f,  1.00f), glm::vec3( 0.00f,  1.00f,  0.00f)), // 35
        // Back side of back of arrow
        Vertex(glm::vec3(-0.25f,  0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 36
        Vertex(glm::vec3( 0.25f,  0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 37
        Vertex(glm::vec3(-0.25f, -0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 38
        Vertex(glm::vec3( 0.25f, -0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f)), // 39
    };

    res.vertex_num = sizeof(stackVerts)/sizeof(Vertex);
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices, stackVerts, sizeof(stackVerts));

    GLushort stackIndices[] =
    {
        0, 1, 2, // Top
        0, 2, 3,
        4, 6, 5, // Bottom
        4, 7, 6,
        8, 10, 9, // Right side of arrow tip
        8, 11, 10,
        12, 15, 13, // Left side of arrow tip
        12, 14, 15,
        16, 19, 17, // Back side of arrow tip
        16, 18, 19,
        20, 22, 21, // Top side of back of arrow
        20, 23, 22,
        24, 25, 26, // Bottom side of back of arrow
        24, 26, 27,
        28, 30, 29, // Right side of back of arrow
        28, 31, 30,
        32, 33, 34, // Left side of back of arrow
        32, 34, 35,
        36, 39, 37, // Back side of back of arrow
        36, 38, 39,
    };

    res.index_num = sizeof(stackIndices)/sizeof(GLushort);
    res.indices = new GLushort[res.index_num];
    memcpy(res.indices, stackIndices, sizeof(stackIndices));
    return res;
}


ShapeData ShapeGenerator::createPlane(int dimension){
    ShapeData res;
    makePlaneVertex(res,dimension);
    makePlaneIndex(res,dimension);
    return res;
}


void ShapeGenerator::makePlaneVertex(ShapeData& data,int dimension){
    data.vertices = new Vertex[dimension * dimension];
    data.vertex_num = dimension * dimension;
    int half = dimension/2;
    //假如是10 * 10的平面，那就是 100个顶点，依次循环给其赋值即可。
    for(int i = 0;i<dimension;++i){
        for(int j = 0;j<dimension;++j){
            Vertex &v = data.vertices[i * dimension + j];
            v.position.x = j-half;//-5 -4 -3 -2 -1 0 1 2 3 4 5
            v.position.z = i-half;//-5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5
            v.position.y = 0.0f;
            v.color = randomColor();
        }
    }
}

void ShapeGenerator::makePlaneIndex(ShapeData& data,int dimension){
    //创建索引，可以以3*3的来看，其实就是2*2的矩阵，一个矩形是两个三角形，一个三角形三个顶点，所以就是 dimension-1 * dimension-1 * 2 * 3
    data.indices = new GLushort[(dimension-1) * (dimension-1) * 2 * 3];
    data.index_num = (dimension-1) * (dimension-1) * 2 * 3;
    int index = 0;
    for(int i = 0;i<dimension-1;++i){
        for(int j = 0;j<dimension-1;++j){
            data.indices[index++] = i * dimension + j;//0
            data.indices[index++] = i * dimension + j + dimension; //3
            data.indices[index++] = i * dimension + j + dimension +1;//4


            data.indices[index++] = i * dimension + j;//0
            data.indices[index++] = i * dimension + j + dimension +1;//4
            data.indices[index++] = i * dimension + j+1;//1
        }
    }
}