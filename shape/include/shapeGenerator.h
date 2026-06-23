#pragma once
#include "shapedata.h"
#include <qshareddata.h>
#include <string>
#include <vector>

class ShapeGenerator{
public:
    static ShapeData createTriangle();
    static ShapeData createCube();
    static ShapeData createArrow();

    static ShapeData createPlane(int dimension = 10);
    static void makePlaneIndex(ShapeData& data,int dimension);
    static void makePlaneVertex(ShapeData& data,int dimension);

    static ShapeData loadModel(const std::string& path,float scale = 1.0f);
    static std::vector<int> parseObjVertexIndex(const std::string& faceToken);

    static ShapeData createNormalLine(const ShapeData& shapeData);
};
