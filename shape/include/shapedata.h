#pragma once
#include <GL/gl.h>
#include <cstring>
#include "vertex.h"

class ShapeData{
public:
    ShapeData():vertices(nullptr),indices(nullptr),vertex_num(0),index_num(0){}

public:
    const int vertices_size() const{
        return vertex_num * sizeof(Vertex);
    }

    const int indices_size() const{
        return index_num * sizeof(GLshort);
    }

    void release(){
        delete [] vertices;
        delete [] indices;
        vertex_num = 0;
        index_num = 0;
    }
public:
    Vertex* vertices;
    GLshort* indices;
    int vertex_num;
    int index_num;
};