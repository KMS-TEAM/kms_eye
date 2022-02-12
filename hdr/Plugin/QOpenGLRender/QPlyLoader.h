#ifndef QPLYLOADER_H
#define QPLYLOADER_H

#include <stdio.h>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

#include <QString>

class QPlyLoader
{
public:
    QPlyLoader();

    int load(char* filename);
    void draw();
    float* calculateNormal(float *coord1, float *coord2, float *coord3);

    float* Faces_Triangles;
    float* Faces_Quads;
    float* Vertex_Buffer;
    float* Normals;

    int TotalConnectedTriangles;
    int TotalConnectedQuads;
    int TotalConnectedPoints;
    int TotalFaces;
};

#endif // QPLYLOADER_H
