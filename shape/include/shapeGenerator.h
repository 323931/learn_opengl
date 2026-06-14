#pragma once
#include "shapedata.h"
#include <qshareddata.h>
class ShapeGenerator{
public:
    static ShapeData createTriangle();
    static ShapeData createCube();
    static ShapeData createArrow();

    static ShapeData createPlane(int dimension = 10);
    static void makePlaneIndex(ShapeData& data,int dimension);
    static void makePlaneVertex(ShapeData& data,int dimension);
};