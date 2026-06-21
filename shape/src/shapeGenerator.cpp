#include "shapeGenerator.h"
#include "shapedata.h"
#include "vertex.h"
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "random.cpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using glm::vec3;

glm::vec3 normalFromTriangle(
    const glm::vec3& a,
    const glm::vec3& b,
    const glm::vec3& c
) {
    return glm::normalize(glm::cross(b - a, c - a));
}

glm::vec3 safeNormalFromTriangle(
    const glm::vec3& a,
    const glm::vec3& b,
    const glm::vec3& c
) {
    const glm::vec3 normal = glm::cross(b - a, c - a);
    const float lengthSquared = glm::dot(normal, normal);
    if (lengthSquared == 0.0f) {
        return glm::vec3(0.0f);
    }
    return glm::normalize(normal);
}

void calculateSmoothNormals(std::vector<Vertex>& vertices, const std::vector<GLushort>& indices)
{
    for (Vertex& vertex : vertices) {
        vertex.normal = glm::vec3(0.0f);
    }

    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        const GLushort a = indices[i];
        const GLushort b = indices[i + 1];
        const GLushort c = indices[i + 2];

        const glm::vec3 normal = safeNormalFromTriangle(
            vertices[a].position,
            vertices[b].position,
            vertices[c].position
        );

        vertices[a].normal += normal;
        vertices[b].normal += normal;
        vertices[c].normal += normal;
    }

    for (Vertex& vertex : vertices) {
        const float lengthSquared = glm::dot(vertex.normal, vertex.normal);
        if (lengthSquared == 0.0f) {
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        } else {
            vertex.normal = glm::normalize(vertex.normal);
        }
    }
}
// 设置一个面的法线，用法：
// 比如说在arrow中
// setFaceNormal(stackVerts, {8, 9, 10, 11}, 8, 10, 9);
// 给 8,9,10,11 这一整个面设置 normal；
// normal 用三角形 8,10,9 计算。
void setFaceNormal(Vertex* vertices, std::initializer_list<int> vertexIndices, int a, int b, int c)
{
    glm::vec3 normal = normalFromTriangle(
        vertices[a].position,
        vertices[b].position,
        vertices[c].position
    );

    for (int index : vertexIndices) {
        vertices[index].normal = normal;
    }
}


ShapeData ShapeGenerator::createTriangle(){
    ShapeData res;
    //默认创建 0，1     -1，-1    1，-1的三角形
    std::vector<Vertex> myTri{
        Vertex(vec3(+0.0f,+1.0f,+0.9f),vec3( +1.0f,+0.0f,+0.0f),vec3(0.0f,0.0f,1.0f)),
        Vertex(vec3(-1.0f,-1.0f,+0.9f),vec3( +0.0f,+1.0f,+0.0f),vec3(0.0f,0.0f,1.0f)),
        Vertex(vec3(+1.0f,-1.0f,+0.9f),vec3( +0.0f,+0.0f,+1.0f),vec3(0.0f,0.0f,1.0f)),
    };

    res.vertex_num = myTri.size();
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices,myTri.data(),res.vertex_num * sizeof(Vertex));

    GLushort indeices[] = {0,1,2};
    res.index_num = sizeof(indeices)/(sizeof(indeices[0]));
    res.indices = new GLushort[res.index_num];
    memcpy(res.indices, indeices, sizeof(indeices));

    return res;
}

ShapeData ShapeGenerator::createCube(){
    ShapeData res;

    const vec3 topNormal = vec3(0.0f,1.0f,0.0f);
    const vec3 frontNormal = vec3(0.0f,0.0f,-1.0f);
    const vec3 rightNormal = vec3(1.0f,0.0f,0.0f);
    const vec3 leftNormal = vec3(-1.0f,0.0f,0.0f);
    const vec3 backNormal = vec3(0.0f,0.0f,+1.0f);
    const vec3 bottomNormal = vec3(0.0f,-1.0f,0.0f);

    std::vector<Vertex> myVertices = {
        // Top (0-3)
        Vertex(vec3(-1.0f,+1.0f,+1.0f),vec3(+1.0f,+0.0f,+0.0f),topNormal),
        Vertex(vec3(+1.0f,+1.0f,+1.0f),vec3(+0.0f,+1.0f,+0.0f),topNormal),
        Vertex(vec3(+1.0f,+1.0f,-1.0f),vec3(+0.0f,+0.0f,+1.0f),topNormal),
        Vertex(vec3(-1.0f,+1.0f,-1.0f),vec3(+1.0f,+1.0f,+1.0f),topNormal),

        // Front (4-7)
        Vertex(vec3(-1.0f,+1.0f,-1.0f),vec3(+1.0f,+0.0f,+1.0f),frontNormal),
        Vertex(vec3(+1.0f,+1.0f,-1.0f),vec3(+0.0f,+0.5f,+0.2f),frontNormal),
        Vertex(vec3(+1.0f,-1.0f,-1.0f),vec3(+0.8f,+0.6f,+0.4f),frontNormal),
        Vertex(vec3(-1.0f,-1.0f,-1.0f),vec3(+0.3f,+1.0f,+0.5f),frontNormal),

        // Right (8-11)
        Vertex(vec3(+1.0f,+1.0f,-1.0f),vec3(+0.2f,+0.5f,+0.2f),rightNormal),
        Vertex(vec3(+1.0f,+1.0f,+1.0f),vec3(+0.9f,+0.3f,+0.7f),rightNormal),
        Vertex(vec3(+1.0f,-1.0f,+1.0f),vec3(+0.3f,+0.7f,+0.5f),rightNormal),
        Vertex(vec3(+1.0f,-1.0f,-1.0f),vec3(+0.5f,+0.7f,+0.5f),rightNormal),

        // Left / -X (12-15)
        Vertex(vec3(-1.0f,+1.0f,+1.0f),vec3(+0.7f,+0.8f,+0.2f),leftNormal),
        Vertex(vec3(-1.0f,+1.0f,-1.0f),vec3(+0.5f,+0.7f,+0.3f),leftNormal),
        Vertex(vec3(-1.0f,-1.0f,-1.0f),vec3(+0.4f,+0.7f,+0.7f),leftNormal),
        Vertex(vec3(-1.0f,-1.0f,+1.0f),vec3(+0.2f,+0.5f,+1.0f),leftNormal),

        // Back / +Z (16-19)
        Vertex(vec3(+1.0f,+1.0f,+1.0f),vec3(+0.6f,+1.0f,+0.7f),backNormal),
        Vertex(vec3(-1.0f,+1.0f,+1.0f),vec3(+0.6f,+0.4f,+0.8f),backNormal),
        Vertex(vec3(-1.0f,-1.0f,+1.0f),vec3(+0.2f,+0.8f,+0.7f),backNormal),
        Vertex(vec3(+1.0f,-1.0f,+1.0f),vec3(+0.2f,+0.7f,+1.0f),backNormal),

        // Bottom (20-23)
        Vertex(vec3(+1.0f,-1.0f,-1.0f),vec3(+0.8f,+0.3f,+0.7f),bottomNormal),
        Vertex(vec3(-1.0f,-1.0f,-1.0f),vec3(+0.8f,+0.9f,+0.5f),bottomNormal),
        Vertex(vec3(-1.0f,-1.0f,+1.0f),vec3(+0.5f,+0.8f,+0.5f),bottomNormal),
        Vertex(vec3(+1.0f,-1.0f,+1.0f),vec3(+0.9f,+1.0f,+0.2f),bottomNormal),
    };

    res.vertex_num = myVertices.size();
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices, myVertices.data(), res.vertex_num * sizeof(Vertex));

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
    const vec3 topNormal(0.0f, 1.0f, 0.0f);
    const vec3 bottomNormal(0.0f, -1.0f, 0.0f);
    const vec3 arrowTipRightNormal(0.832f, 0.0f, -0.555f);
    const vec3 arrowTipLeftNormal(-0.832f, 0.0f, -0.555f);
    const vec3 arrowTipBackNormal(0.0f, 0.0f, 1.0f);
    const vec3 shaftRightNormal(1.0f, 0.0f, 0.0f);
    const vec3 shaftLeftNormal(-1.0f, 0.0f, 0.0f);
    const vec3 shaftBackNormal(0.0f, 0.0f, 1.0f);
    std::vector<Vertex> stackVerts =
    {
        // Top side of arrow head
        Vertex(glm::vec3( 0.00f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f,1.0f,0.0f)), // 0
        Vertex(glm::vec3( 0.50f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f,1.0f,0.0f)), // 1
        Vertex(glm::vec3( 0.00f,  0.25f, -1.00f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f,1.0f,0.0f)), // 2
        Vertex(glm::vec3(-0.50f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f,1.0f,0.0f)), // 3
        // Bottom side of arrow head
        Vertex(glm::vec3( 0.00f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f,-1.0f,0.0f)), // 4
        Vertex(glm::vec3( 0.50f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f,-1.0f,0.0f)), // 5
        Vertex(glm::vec3( 0.00f, -0.25f, -1.00f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f,-1.0f,0.0f)), // 6
        Vertex(glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f,-1.0f,0.0f)), // 7
        // Right side of arrow tip
        Vertex(glm::vec3( 0.50f,  0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(0.832f, 0.0f, -0.555f)),// 8
        Vertex(glm::vec3( 0.00f,  0.25f, -1.00f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(0.832f, 0.0f, -0.555f)), // 9
        Vertex(glm::vec3( 0.00f, -0.25f, -1.00f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(0.832f, 0.0f, -0.555f)), // 10
        Vertex(glm::vec3( 0.50f, -0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(0.832f, 0.0f, -0.555f)), // 11
        // Left side of arrow tip
        Vertex(glm::vec3( 0.00f,  0.25f, -1.00f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-0.832f, 0.0f, -0.555f)), // 12
        Vertex(glm::vec3(-0.50f,  0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-0.832f, 0.0f, -0.555f)), // 13
        Vertex(glm::vec3( 0.00f, -0.25f, -1.00f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-0.832f, 0.0f, -0.555f)), // 14
        Vertex(glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-0.832f, 0.0f, -0.555f)), // 15
        // Back side of arrow tip
        Vertex(glm::vec3(-0.50f,  0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 16
        Vertex(glm::vec3( 0.50f,  0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 17
        Vertex(glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 18
        Vertex(glm::vec3( 0.50f, -0.25f, -0.25f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 19
        // Top side of back of arrow  
        Vertex(glm::vec3( 0.25f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f, 1.0f, 0.0f)), // 20
        Vertex(glm::vec3( 0.25f,  0.25f,  1.00f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f, 1.0f, 0.0f)), // 21
        Vertex(glm::vec3(-0.25f,  0.25f,  1.00f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f, 1.0f, 0.0f)), // 22
        Vertex(glm::vec3(-0.25f,  0.25f, -0.25f), glm::vec3( 1.00f,  0.00f,  0.00f),vec3(0.0f, 1.0f, 0.0f)), // 23
        // Bottom side of back of arrow
        Vertex(glm::vec3( 0.25f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f, -1.0f, 0.0f)), // 24
        Vertex(glm::vec3( 0.25f, -0.25f,  1.00f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f, -1.0f, 0.0f)), // 25
        Vertex(glm::vec3(-0.25f, -0.25f,  1.00f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f, -1.0f, 0.0f)), // 26
        Vertex(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3( 0.00f,  0.00f,  1.00f),vec3(0.0f, -1.0f, 0.0f)), // 27
        // Right side of back of arrow
        Vertex(glm::vec3( 0.25f,  0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(1.0f, 0.0f, 0.0f)), // 28
        Vertex(glm::vec3( 0.25f, -0.25f, -0.25f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(1.0f, 0.0f, 0.0f)), // 29
        Vertex(glm::vec3( 0.25f, -0.25f,  1.00f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(1.0f, 0.0f, 0.0f)), // 30
        Vertex(glm::vec3( 0.25f,  0.25f,  1.00f), glm::vec3( 0.60f,  1.00f,  0.00f),vec3(1.0f, 0.0f, 0.0f)), // 31
        // Left side of back of arrow
        Vertex(glm::vec3(-0.25f,  0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-1.0f, 0.0f, 0.0f)), // 32
        Vertex(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-1.0f, 0.0f, 0.0f)), // 33
        Vertex(glm::vec3(-0.25f, -0.25f,  1.00f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-1.0f, 0.0f, 0.0f)), // 34
        Vertex(glm::vec3(-0.25f,  0.25f,  1.00f), glm::vec3( 0.00f,  1.00f,  0.00f),vec3(-1.0f, 0.0f, 0.0f)), // 35
        // Back side of back of arrow
        Vertex(glm::vec3(-0.25f,  0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 36
        Vertex(glm::vec3( 0.25f,  0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 37
        Vertex(glm::vec3(-0.25f, -0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 38
        Vertex(glm::vec3( 0.25f, -0.25f,  1.00f), glm::vec3( 0.50f,  0.50f,  0.50f),vec3(0.0f, 0.0f, 1.0f)), // 39
    };

    res.vertex_num = stackVerts.size();
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices, stackVerts.data(), res.vertex_num * sizeof(Vertex));

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
            const bool alternateTile = ((i + j) % 2) == 0;
            v.color = alternateTile ? vec3(0.34f, 0.36f, 0.34f) : vec3(0.42f, 0.43f, 0.40f);
            v.normal = vec3(0.0f, 1.0f, 0.0f);
            v.texCoord = glm::vec2(
                static_cast<float>(j) / static_cast<float>(dimension - 1) * 8.0f,
                static_cast<float>(i) / static_cast<float>(dimension - 1) * 8.0f
            );
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

ShapeData ShapeGenerator::loadModel(const std::string &path,float scale){
    std::ifstream file(path);
    ShapeData res;

    if(!file.is_open()){
        std::cerr << "Failed to open file: " << path << std::endl;
        return res;
    }

    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    std::string line;
    while(std::getline(file,line)){
        std::istringstream lineStream(line);

        std::string type;
        lineStream >> type;

        if(type == "v"){
            float x,y,z;
            lineStream >>x >>y >>z;
            Vertex vertex;
            vertex.position = glm::vec3(x,y,z) * scale;
            vertex.color = randomColor();
            vertices.push_back(vertex);
        }

        if(type == "f"){
            std::string v1,v2,v3;
            lineStream >>v1 >>v2 >>v3;
            indices.push_back(parseObjVertexIndex(v1));
            indices.push_back(parseObjVertexIndex(v2));
            indices.push_back(parseObjVertexIndex(v3));
        }
    }

    calculateSmoothNormals(vertices, indices);

    res.vertex_num= vertices.size();
    res.vertices = new Vertex[res.vertex_num];
    memcpy(res.vertices, vertices.data(), res.vertex_num * sizeof(Vertex));

    res.index_num = indices.size();
    res.indices = new GLushort[res.index_num];
    memcpy(res.indices, indices.data(), res.index_num * sizeof(GLushort));

    return res;
}

//parseObjVertexIndex("12/8/3") -> 11
//parseObjVertexIndex("12//3") -> 11
GLushort ShapeGenerator::parseObjVertexIndex(const std::string &faceToken){
    const size_t slashPosition = faceToken.find('/');
    const std::string vertexIndexText = faceToken.substr(0, slashPosition);
    const int objIndex = std::stoi(vertexIndexText);
    return static_cast<GLushort>(objIndex - 1);
}

ShapeData ShapeGenerator::createNormalLine(const ShapeData& shapeData){
    ShapeData res;

    res.vertex_num = shapeData.vertex_num * 2;
    res.vertices = new Vertex[res.vertex_num];

    for(int i = 0;i<shapeData.vertex_num;++i){
        Vertex& sourceVertex = shapeData.vertices[i];

        Vertex& start = res.vertices[i*2];
        Vertex& end = res.vertices[i*2+1];

        start.position = sourceVertex.position;
        end.position = start.position + sourceVertex.normal;

        //白色表示法线
        start.color = glm::vec3(1.0f, 1.0f, 1.0f);
        end.color = glm::vec3(1.0f, 1.0f, 1.0f);

        start.normal = sourceVertex.normal;
        end.normal = sourceVertex.normal;

        start.normal = glm::normalize(start.normal);
        end.normal = glm::normalize(end.normal);
    }

    res.index_num = res.vertex_num;
    res.indices = new GLushort[res.index_num];
    for(int i = 0 ; i<res.index_num;++i){
        res.indices[i] = i;
    }
    return res;
}
