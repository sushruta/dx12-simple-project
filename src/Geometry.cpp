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

/*
	// Done loading teapot file, now bind it
	glBindVertexArray(VAOs[TEAPOT]);

	// Bind vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[TEAPOT]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * teapotVertices.size(), &teapotVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind normals to layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, NBOs[TEAPOT]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * teapotNormals.size(), &teapotNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// The indices array tells OpenGL what order to iterate through the buffers in when the shaders execute
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[TEAPOT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * teapotIndices.size(), &teapotIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Draws a solid teapot
void solidTeapot(float size) {
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview * glm::scale(mat4(1.0f), vec3(size, size, size)))[0][0]);
	glBindVertexArray(VAOs[TEAPOT]);
	glDrawElements(GL_TRIANGLES, teapotIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
*/