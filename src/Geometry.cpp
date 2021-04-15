#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SSE42 1
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#define GLM_FORCE_LEFT_HANDED

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// Initialize the teapot by reading in the file and populating its VAO
void initTeapot()
{
    FILE* fp;
    float x, y, z;
    int fx, fy, fz, ignore;
    int c1, c2;
    float minY = INFINITY, minZ = INFINITY;
    float maxY = -INFINITY, maxZ = -INFINITY;

    std::vector<vec3> teapotVertices;
    std::vector<vec3> teapotNormals;
    std::vector<unsigned int> teapotIndices;

    fp = fopen("assets\\teapot.obj", "rb");

    if (fp == NULL)
    {
        std::cerr << "Error loading obj file" << std::endl;
        exit(-1);
    }

    while (!feof(fp))
    {
        c1 = fgetc(fp);
        while (!(c1 == 'v' || c1 == 'f'))
        {
            c1 = fgetc(fp);
            if (feof(fp)) break;
        }
        c2 = fgetc(fp);

        if ((c1 == 'v') && (c2 == ' '))
        {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            teapotVertices.push_back(vec3(x, y, z));
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
        }
        else if ((c1 == 'v') && (c2 == 'n'))
        {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            // Ignore the normals in mytest2, as we use a solid color for the
            // teapot.
            teapotNormals.push_back(glm::normalize(vec3(x, y, z)));
        }
        else if ((c1 == 'f'))
        {
            fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz,
                   &ignore);
            teapotIndices.push_back(fx - 1);
            teapotIndices.push_back(fy - 1);
            teapotIndices.push_back(fz - 1);
        }
    }

    fclose(fp); // Finished parsing
                // Recenter the teapot
    float avgY = (minY + maxY) / 2.0f - 0.02f;
    float avgZ = (minZ + maxZ) / 2.0f;
    for (unsigned int i = 0; i < teapotVertices.size(); ++i)
    {
        vec3 shiftedVertex = (teapotVertices[i] - vec3(0.0f, avgY, avgZ)) *
                             vec3(1.58f, 1.58f, 1.58f);
        teapotVertices[i] = shiftedVertex;
    }
}
