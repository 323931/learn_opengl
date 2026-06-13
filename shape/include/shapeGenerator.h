#pragma once
#include "shapedata.h"
#include <qshareddata.h>
class ShapeGenerator{
public:
    static ShapeData createTriangle();
    static ShapeData createCube();
    static ShapeData createArrow();
};