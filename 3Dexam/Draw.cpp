#include "Draw.h"
#include "Resources/Shaders/shaderClass.h"
#include "glm/gtc/type_ptr.hpp"
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include "Resources/Shaders/shaderClass.h"
#include "Grid.h"
#include "Component.h"
#include <random> // For random seeds


Draw::Draw() : rotation(glm::quat(0.0, 0.0, 0.0, 0.0))
{
}

void Draw::DrawCube(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{

    position = pos;
    objSize = size;

    vertices.resize(8);
    glm::vec3 sizeXYZ = glm::vec3(1.f, 1.f, 1.f);

    // Front face vertices
    Vertex v0{ -sizeXYZ.x, -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 1.0f };
    Vertex v1{ sizeXYZ.x, -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 1.0f };
    Vertex v2{ sizeXYZ.x,  sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 0.0f };
    Vertex v3{ -sizeXYZ.x,  sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 0.0f };

    Vertex v4{ -sizeXYZ.x, -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 1.0f };
    Vertex v5{ sizeXYZ.x, -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 1.0f };
    Vertex v6{ sizeXYZ.x,  sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 0.0f };
    Vertex v7{ -sizeXYZ.x,  sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 0.0f };


    vertices = {
   v0, // Front bottom left
   v1, // Front bottom right
   v2, // Front top right
   v3, // Front top left
   v4, // Back bottom left
   v5, // Back bottom right
   v6, // Back top right
   v7  // Back top left
    };



    // Indices
    indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        4, 0, 3, 3, 7, 4,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Top face
        3, 2, 6, 6, 7, 3,
        // Bottom face
        4, 5, 1, 1, 0, 4
    };

    this->Initalize();
}

void Draw::DrawPlane(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{

    position = pos;
    objSize = size;

    vertices.resize(4);
    glm::vec3 sizeXYZ = glm::vec3(1.f, 1.f, 1.f);

    // face vertices
    //Vertex v1{ sizeXYZ.x,  -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 0.0f };
    //Vertex v2{ -sizeXYZ.x,  -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 0.0f };
    //Vertex v3{ sizeXYZ.x,  -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 1.0f };
    //Vertex v0{ -sizeXYZ.x,  -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 1.0f };

    Vertex v1{ sizeXYZ.x,  -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 0.0f };
    Vertex v2{ -sizeXYZ.x,  sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 0.0f };
    Vertex v3{ sizeXYZ.x,  sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 1.0f };
    Vertex v0{ -sizeXYZ.x,  -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 1.0f };


    vertices = {
   v1, // Front top right
   v2, // Front top left
   v3, // Back top right
   v0  // Back top left
    };



    // Indices
    indices = {
    0,1,2,1,2,3
    };
    // Calculating the normal
    glm::vec3 A = glm::vec3(v1.x, v1.y, v1.z);
    glm::vec3 B = glm::vec3(v2.x, v2.y, v2.z);
    glm::vec3 C = glm::vec3(v0.x, v0.y, v0.z);

    // Creating vectors AB and AC
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;

    // Calculating the normal using the cross product
    glm::vec3 normal = glm::normalize(glm::cross(AB, AC));
    normalvector = normal;
    this->Initalize();
}

void Draw::DrawBoundingBox(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{
    position = pos;
    objSize = size;

    vertices.resize(8);
    glm::vec3 sizeXYZ = glm::vec3(1.f, 1.f, 1.f);

    // Front face vertices
    Vertex v0{ -sizeXYZ.x, -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 1.0f };
    Vertex v1{ sizeXYZ.x, -sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 1.0f };
    Vertex v2{ sizeXYZ.x,  sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 0.0f };
    Vertex v3{ -sizeXYZ.x,  sizeXYZ.y,  sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 0.0f };

    Vertex v4{ -sizeXYZ.x, -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 1.0f };
    Vertex v5{ sizeXYZ.x, -sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 1.0f };
    Vertex v6{ sizeXYZ.x,  sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 1.0f, 0.0f };
    Vertex v7{ -sizeXYZ.x,  sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z, 0.0f, 0.0f };


    vertices = {
   v0, // Front bottom left
   v1, // Front bottom right
   v2, // Front top right
   v3, // Front top left
   v4, // Back bottom left
   v5, // Back bottom right
   v6, // Back top right
   v7  // Back top left
    };



    // Indices
    indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        4, 0, 3, 3, 7, 4,
        // Right face
        1, 5, 6, 6, 2, 1
    };

    this->Initalize();
}

void Draw::DrawSphere(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{
    // Part of the source code is from this website
    // https://stackoverflow.com/questions/45482988/generating-spheres-vertices-indices-and-normals


    position = pos;
    objSize = size;
    int radius = 1.0f;
    int circumferenceTile = 18;
    int layerTile = 18;
    int slices = (int)(circumferenceTile + 0.5f);
    if (slices < 4) {
        slices = 4;
    }

    int half_slices = slices / 2;
    int layerCount = (int)(layerTile + 0.5f);
    if (layerCount < 2)
    {
        layerCount = 2;
    }
    float pi = 3.1415f;
    for (int layerIndex = 0; layerIndex <= layerCount; layerIndex++)
    {
        float v = (1.0 - (float)layerIndex / layerCount);
        float heightOffset = std::sin((1.0 - 2.0 * layerIndex / layerCount) * pi / 2.0);
        float cosUp = sqrt(1.0 - heightOffset * heightOffset);
        float z = heightOffset;
        for (int i = 0; i <= half_slices; i++)
        {
            float u = (float)i / (float)half_slices;
            float angle = 2 * pi * u; // pi * 2 to get full sphere
            float x = std::cos(angle) * cosUp;
            float y = std::sin(angle) * cosUp;
            Vertex V1 = Vertex{ x * radius, y * radius, z * radius, x, y, z, u, v };
            vertices.push_back(V1);
        }

    }
    for (int layer = 0; layer < layerCount; layer++)
    {
        for (int i = 0; i < half_slices; i++)
        {
            // Index for the current layer and the next layer
            int currentRow = layer * (half_slices + 1) * 2;
            int nextRow = (layer + 1) * (half_slices + 1) * 2;

            // Creating two triangles (quad) between each pair of vertices in adjacent layers
            indices.push_back(currentRow + i);        // 1st triangle: curRow, nextRow, nextRow+1
            indices.push_back(nextRow + i);
            indices.push_back(nextRow + i + 1);

            indices.push_back(currentRow + i);        // 2nd triangle: curRow, nextRow+1, curRow+1
            indices.push_back(nextRow + i + 1);
            indices.push_back(currentRow + i + 1);
        }
    }

    this->Initalize();
}

void Draw::DrawTerrain(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{
    position = pos;
    objSize = size;
    float waveAmplitude = 10.0f; // Adjust for more extreme or subtle "bumps"
    float waveFrequency = 1.0f;
    float terrainScale = 1.0f;
    int terrainDepth = 10;
    int terrainWidth = 10;

    // Generating vertices
    for (int z = 0; z <= terrainDepth; ++z) {
        for (int x = 0; x <= terrainWidth; ++x) {
            // Calculating vertex positions
            double xPos = static_cast<float>(x) * terrainScale;  // Scale based on x index
            double zPos = static_cast<float>(z) * terrainScale;  // Scale based on z index
            double yPos = sin(xPos * waveFrequency) * waveAmplitude + cos(zPos * waveFrequency) * waveAmplitude; // Height variation

            // Texture coordinates
            float u = static_cast<float>(x) / terrainWidth;
            float v = static_cast<float>(z) / terrainDepth;

            // Creating the vertex
            Vertex vertex;
            vertex.x = xPos - size.x / 2;
            vertex.y = yPos;
            vertex.z = zPos - size.z / 2;
            vertex.u = u;
            vertex.v = v;

            // Normal (placeholder for now)
            vertex.normalx = 0.0f;
            vertex.normaly = 1.0f;
            vertex.normalz = 0.0f;

            vertices.push_back(vertex);
        }
    }

    // Generating indices
    for (int z = 0; z < terrainDepth; ++z) {
        for (int x = 0; x < terrainWidth; ++x) {
            // Calculating indices
            int topLeftIndex = z * (terrainWidth + 1) + x;
            int topRightIndex = topLeftIndex + 1;
            int bottomLeftIndex = (z + 1) * (terrainWidth + 1) + x;
            int bottomRightIndex = bottomLeftIndex + 1;

            // First triangle (top-left, top-right, bottom-left)
            indices.push_back(topLeftIndex);
            indices.push_back(topRightIndex);
            indices.push_back(bottomLeftIndex);

            // Second triangle (top-right, bottom-right, bottom-left)
            indices.push_back(topRightIndex);
            indices.push_back(bottomRightIndex);
            indices.push_back(bottomLeftIndex);
        }
    }
    this->Initalize();
}

void Draw::DrawBSplineSurface(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{
   
    for (int i = 0; i <= d_u; ++i) {
        mu.push_back(0); // Start knots (degree + 1 times)
    }
    for (int i = 1; i <= n_u - d_u - 1; ++i) {
        mu.push_back(static_cast<float>(i)); // Interior knots
    }
    for (int i = 0; i <= d_u; ++i) {
        mu.push_back(static_cast<float>(n_u - d_u)); // End knots (degree + 1 times)
    }

    // Generate knot vector for v
    for (int i = 0; i <= d_v; ++i) {
        mv.push_back(0); // Start knots (degree + 1 times)
    }
    for (int i = 1; i <= n_v - d_v - 1; ++i) {
        mv.push_back(static_cast<float>(i)); // Interior knots
    }
    for (int i = 0; i <= d_v; ++i) {
        mv.push_back(static_cast<float>(n_v - d_v)); // End knots (degree + 1 times)
    }

    // Debugging: Print the knot vectors
    std::cout << "Knot vector mu: ";
    for (float u : mu) std::cout << u << " ";
    std::cout << std::endl;

    std::cout << "Knot vector mv: ";
    for (float v : mv) std::cout << v << " ";
    std::cout << std::endl;
    


    // Check if we have enough control points
   

    
    mc.push_back(glm::vec3(0, 0, 0)); 
    mc.push_back(glm::vec3(1, 0, 0));
    mc.push_back(glm::vec3(2, 0, 0));
    mc.push_back(glm::vec3(3, 0, 0));

    mc.push_back(glm::vec3(0, 0, 1)); 
    mc.push_back(glm::vec3(1, 2, 1)); 
    mc.push_back(glm::vec3(2, 2, 1));
    mc.push_back(glm::vec3(3, 0, 1)); 

    mc.push_back(glm::vec3(0, 0, 2)); 
    mc.push_back(glm::vec3(1, 0, 2)); 
    mc.push_back(glm::vec3(2, 0, 2));
    mc.push_back(glm::vec3(3, 0, 2)); 
    // Map the flat control point array mc to the 2D grid c
    std::vector<std::vector<glm::vec3>> c(n_u, std::vector<glm::vec3>(n_v));
    for (int i = 0; i < n_u; ++i) {
        for (int j = 0; j < n_v; ++j) {
            c[i][j] = mc[j * n_u + i]; // Correct mapping
        }
    }

    // Call the function to make the Biquadratic surface
    MakeBiquadraticSurface(n_u, n_v, d_u, d_v ,c);

    // Initialize any necessary OpenGL state
    this->Initalize();
}




void Draw::DrawPoints(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{
    position = pos;
    objSize = size;
    const char* file = "32-2-516-156-31.txt";
    std::vector<glm::vec3> pointCloud = Readfile(file);
    

    int numCols = sqrt(pointCloud.size());  // Assuming the grid is square
    int numRows = sqrt(pointCloud.size());
    float minHeight = std::numeric_limits<float>::max();
    float maxHeight = std::numeric_limits<float>::lowest();

    for (const auto& point : pointCloud) {
        minHeight = std::min(minHeight, point.y);
        maxHeight = std::max(maxHeight, point.y);
    }

    // Create vertices
    vertices.clear();
    int frictionOffset = 3;
    for (const auto& point : pointCloud) {
        frictionOffset += 3;
        
        Vertex vertex;
        vertex.x = point.x;
        vertex.y = point.y;
        vertex.z = point.z;
        vertex.u = point.x;
        vertex.v = point.y;

        float normalizedHeight = (point.y - minHeight) / (maxHeight - minHeight);
        vertex.r = normalizedHeight;
        vertex.g = 1.0f - normalizedHeight;
        vertex.b = 0.5f * (1.0f - normalizedHeight);
        if ((frictionOffset % 8 == 0) || (point.x > -50 && point.x < 50 && point.z > -50 && point.z < 50)) {
            vertex.r = 0.0f;
            vertex.g = 0.0f;
            vertex.b = 1.0f;
            vertex.friction = 0.9;
        }
        else {
            vertex.friction = 0.1;
        }
        vertex.normalx = 0.0f; // Initialize normal to zero
        vertex.normaly = 0.0f;
        vertex.normalz = 0.0f;

        vertices.push_back(vertex);
    }

    // Generate indices using Delaunay triangulation
    indices.clear();
    std::vector<Triangle> delaunayTriangles = delaunayTriangulation(pointCloud);
    std::cout << "TRIANGLES: " << delaunayTriangles.size() << std::endl;
    for (auto& tri : delaunayTriangles) {
        indices.push_back(tri.p1);
        indices.push_back(tri.p2);
        indices.push_back(tri.p3);

        // Calculate triangle normal
        glm::vec3 p1 = pointCloud[tri.p1];
        glm::vec3 p2 = pointCloud[tri.p2];
        glm::vec3 p3 = pointCloud[tri.p3];

        glm::vec3 edge1 = p2 - p1;
        glm::vec3 edge2 = p3 - p1;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        // Accumulate normals for smooth shading
        vertices[tri.p1].normalx += normal.x;
        vertices[tri.p1].normaly += normal.y;
        vertices[tri.p1].normalz += normal.z;

        vertices[tri.p2].normalx += normal.x;
        vertices[tri.p2].normaly += normal.y;
        vertices[tri.p2].normalz += normal.z;

        vertices[tri.p3].normalx += normal.x;
        vertices[tri.p3].normaly += normal.y;
        vertices[tri.p3].normalz += normal.z;
    }

    // Normalize the accumulated normals for each vertex
    for (auto& vertex : vertices) {
        glm::vec3 normal(vertex.normalx, vertex.normaly, vertex.normalz);
        normal = glm::normalize(normal);

        vertex.normalx = normal.x;
        vertex.normaly = normal.y;
        vertex.normalz = normal.z;
    }
    this->Initalize();
}

void Draw::DrawBspline()
{
    mc.clear();
    //mc.push_back(glm::vec3(0, 0, 0));
    mc = controllPoints;
    if (mc.size() <= 0) {
        return;
    }
   
    n_u = controllPoints.size();
    n_v = (controllPoints.size());
    if (n_u > controllPoints.size()) {
        return;
    }
    d_v = 0;
    d_u = 1;
    mu.clear();
    mv.clear();
    for (int i = 0; i <= d_u; ++i) {
        mu.push_back(0); // Start knots (degree + 1 times)
    }
    for (int i = 1; i <= n_u - d_u - 1; ++i) {
        mu.push_back(static_cast<float>(i)); // Interior knots
    }
    for (int i = 0; i <= d_u; ++i) {
        mu.push_back(static_cast<float>(n_u - d_u)); // End knots (degree + 1 times)
    }

  
   
    std::vector<std::vector<glm::vec3>> c1(n_u, std::vector<glm::vec3>(n_v));
   
    
    MakeBiquadraticLine(n_u, d_u, 0,c1);

    this->Initalize();
}

void Draw::DrawBSplinePointCloudSurface(glm::vec3 Color, glm::vec3 pos, glm::vec3 size)
{
    mc.clear();
    const char* file = "32-2-516-156-31.txt";
    std::vector<glm::vec3> pointCloud = Readfile(file);
   
    mc = pointCloud;
   
    n_u = sqrt(pointCloud.size());
    n_v = sqrt(pointCloud.size());
    d_v = 3;
    d_u = 3;
    mu.clear();
    mv.clear();
    for (int i = 0; i <= d_u; ++i) {
        mu.push_back(0); // Start knots (degree + 1 times)
    }
    for (int i = 1; i <= n_u - d_u - 1; ++i) {
        mu.push_back(static_cast<float>(i)); // Interior knots
    }
    for (int i = 0; i <= d_u; ++i) {
        mu.push_back(static_cast<float>(n_u - d_u)); // End knots (degree + 1 times)
    }

    // Generate knot vector for v
    for (int i = 0; i <= d_v; ++i) {
        mv.push_back(0); // Start knots (degree + 1 times)
    }
    for (int i = 1; i <= n_v - d_v - 1; ++i) {
        mv.push_back(static_cast<float>(i)); // Interior knots
    }
    for (int i = 0; i <= d_v; ++i) {
        mv.push_back(static_cast<float>(n_v - d_v)); // End knots (degree + 1 times)
    }

    std::cout << "Knot vector mu: ";
    for (float u : mu) std::cout << u << " ";
    std::cout << std::endl;

    std::cout << "Knot vector mv: ";
    for (float v : mv) std::cout << v << " ";
    std::cout << std::endl;
    std::vector<std::vector<glm::vec3>> c1(n_u, std::vector<glm::vec3>(n_v));
    
    for (int i = 0; i < n_u; ++i) {
        for (int j = 0; j < n_v; ++j) {
            c1[i][j] = mc[j * n_u + i]; // Correct mapping
        }
    }
    MakeBiquadraticLine(n_u, d_u, 0, c1);

    this->Initalize();
}






void Draw::Initalize()
{
    VAO.Bind(); // Binding the VAO
    VBO.Bind(); // Binding the VBO and upload vertex data

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Setting vertex attributes pointers
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, x)); // Position
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, r)); // Color
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u)); // TexCoords

    // Binding the EBO and upload index data
    EBO1.Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Unbinding VAO, VBO, EBO
    VAO.Unbind();
    VBO.Unbind();
    EBO1.Unbind();
}

void Draw::InitalizePoints()
{
    VAO.Bind(); // Binding the VAO
    VBO.Bind(); // Binding the VBO and upload vertex data

    // Upload vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Setting vertex attributes pointers
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, x)); // Position
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, r)); // Color
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u)); // TexCoords
    glPointSize(1.5f);
    // If using an EBO, you need to bind the EBO and upload index data
    if (!indices.empty()) // Check if you have index data
    {
        EBO1.Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        EBO1.Unbind(); // Unbind EBO after uploading data
    }


    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glPointSize(100.5f);
    // Unbinding VAO and VBO after setup
    VAO.Unbind();
    VBO.Unbind();
    
   
}

void Draw::Render(const std::shared_ptr<Shader>& Shader, glm::mat4 viewproj, PositionComponent& pos)
{
    glm::mat4 model2 = glm::mat4(1.0f);
    
    model2 = glm::translate(model2, pos.position);
    model2 = glm::scale(model2, objSize);

    // Get the uniform location
    GLint camMatrixLocation = glGetUniformLocation(Shader->ID, "camMatrix");
    if (camMatrixLocation != -1) {
        glUniformMatrix4fv(camMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewproj * model2));
    }
    else {
        // Handle the error appropriately (e.g., log it)
        std::cerr << "Error: 'camMatrix' uniform not found in shader!" << std::endl;
    }

    VAO.Bind();
    VBO.Bind();
    EBO1.Bind();
    //glLineWidth(20.0f);
    // Adjust point size as needed
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0); // Use GL_POINTS instead of GL_POINT
    //glDrawArrays(GL_POINTS, 0, vertices.size());
   // glPointSize(1.0f);
    // Unbind
    VAO.Unbind();
    VBO.Unbind();
    EBO1.Unbind();


}

void Draw::RenderPoints(const std::shared_ptr<Shader>& shader, glm::mat4 viewproj, PositionComponent& pos)
{
    glm::mat4 model2 = glm::mat4(1.0f);
    if (bHasBsplineFollow) {
        //std::cout << pos.GetPosition().x << std::endl;
        
        

    }
    model2 = glm::translate(model2, pos.position);
    model2 = glm::scale(model2, objSize);

    // Get the uniform location
    GLint camMatrixLocation = glGetUniformLocation(shader->ID, "camMatrix");
    if (camMatrixLocation != -1) {
        glUniformMatrix4fv(camMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewproj * model2));
    }
    else {
        // Handle the error appropriately (e.g., log it)
        std::cerr << "Error: 'camMatrix' uniform not found in shader!" << std::endl;
    }

    VAO.Bind();
    VBO.Bind();
    EBO1.Bind();
    // Adjust point size as needed
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Use GL_POINTS instead of GL_POINT
    //glDrawArrays(GL_POINTS, 0, vertices.size());
   // glPointSize(1.0f);
    // Unbind
    VAO.Unbind();
    VBO.Unbind();
    EBO1.Unbind();
}



void Draw::MakeBiquadraticSurface(const int n_u,const int n_v,int d_u,int d_v, std::vector<std::vector<glm::vec3>> c)
{
    float h = 0.1f; // Spacing
    int nu = static_cast<int>((mu[n_u] - mu[d_u]) / h);  // Calculate the number of steps in u
    int nv = static_cast<int>((mv[n_v] - mv[d_v]) / h);  // Calculate the number of steps in v
    indices.clear();
    vertices.clear();
    // Iterate through v and u to generate surface points
    for (int i = 0; i < nv; ++i)
    {
        for (int j = 0; j < nu; ++j)
        {
            float u = j * h;
            float v = i * h;

            // Evaluate the biquadratic surface at the current u and v
            glm::vec3 surfacePoint = deBoorSurface(d_u, d_v, mu, mv, mc, u, v);

            Vertex vertex;

            // Assign the position values from the evaluated surface point
            vertex.x = surfacePoint.x;
            vertex.y = surfacePoint.y;
            vertex.z = surfacePoint.z;

            // Assign color and texture coordinates
            vertex.r = 1.0f;
            vertex.g = 1.0f;
            vertex.b = 1.0f;

            vertex.u = static_cast<float>(j) / (nu - 1); // Column index normalized
            vertex.v = static_cast<float>(i) / (nv - 1); // Row index normalized

            vertex.normalx = 0.0f;
            vertex.normaly = 1.0f;
            vertex.normalz = 0.0f;

            // Push the computed surface point into the vertices array
            vertices.push_back(vertex);
        }
    }

    // Generate indices for the triangle mesh
    for (int i = 0; i < nv - 1; ++i) {
        for (int j = 0; j < nu - 1; ++j) {
            int idx1 = i * nu + j;
            int idx2 = idx1 + 1;
            int idx3 = idx1 + nu;
            int idx4 = idx3 + 1;

            // First triangle (idx1, idx2, idx3)
            indices.push_back(idx1);
            indices.push_back(idx2);
            indices.push_back(idx3);

            // Second triangle (idx2, idx4, idx3)
            indices.push_back(idx2);
            indices.push_back(idx4);
            indices.push_back(idx3);
        }
    }
}
void Draw::MakeBiquadraticLine(const int n_u, int d_u, float v, std::vector<std::vector<glm::vec3>> c) {
    float h = 0.1f; // Spacing between points
    vertices.clear();
    indices.clear();

    float u_min = mu[d_u];
    float u_max = mu[n_u];
    
    int nu = static_cast<int>((u_max - u_min) / h); // Number of steps in u

    // Iterate through u to generate line points
    for (int j = 0; j < nu; ++j) {
        float u = u_min + j * h;

        // Find the corresponding knot interval for u
        //int my_u = FindKnotInterval(mu, d_u, n_u, u);

        // Fixed v interval
        //int my_v = FindKnotInterval(mv, d_u, n_u, v);

        // Evaluate the biquadratic curve at the current u (for the fixed v)
        glm::vec3 linePoint = deBoor(d_u, d_u,mu,controllPoints,u);

        Vertex vertex;

        // Assign position from the evaluated point
        vertex.x = linePoint.x;
        vertex.y = linePoint.y;
        vertex.z = linePoint.z;

        // Assign color and texture coordinates
        vertex.r = 1.0f;
        vertex.g = 1.0f;
        vertex.b = 1.0f;

        vertex.u = static_cast<float>(j) / (nu - 1); // Normalize along u
        vertex.v = 0.0f; // Single value since this is a line

        vertex.normalx = 0.0f;
        vertex.normaly = 1.0f;
        vertex.normalz = 0.0f;

        // Push the computed line point into the vertices array
        vertices.push_back(vertex);
    }

    // Generate indices for the line
    for (int j = 0; j < nu - 1; ++j) {
        indices.push_back(j);
        indices.push_back(j + 1);
        
    }
}

std::pair<glm::vec3, glm::vec3> Draw::B2(float tu, float tv, int my_u, int my_v)
{
    glm::vec3 Bu, Bv;

    // Assuming tu and tv are in the [0, 1] range for the basis functions:
    Bu.x = (1 - tu) * (1 - tu);
    Bu.y = 2 * tu * (1 - tu);
    Bu.z = tu * tu;

    Bv.x = (1 - tv) * (1 - tv);
    Bv.y = 2 * tv * (1 - tv);
    Bv.z = tv * tv;
    float sumBu = Bu.x + Bu.y + Bu.z;
    float sumBv = Bv.x + Bv.y + Bv.z;

    /*Bu /= sumBu;
    Bv /= sumBv;*/
   /* std::cout << "Bu: (" << Bu.x << ", " << Bu.y << ", " << Bu.z << ")\n";
    std::cout << "Bv: (" << Bv.x << ", " << Bv.y << ", " << Bv.z << ")\n";*/
    return std::make_pair(Bu, Bv);
}

int Draw::FindKnotInterval(const std::vector<float>& knots, int degree, int n, float t)
{
    for (int i = degree; i < n; ++i) {
        if (t >= knots[i] && t < knots[i + 1]) {
            return i;
        }
    }
    std::cout << "could not find knot" << std::endl;
    return -1;
}
glm::vec3 Draw::deBoorSurface(int du, int dv, const std::vector<float>& knotsU, const std::vector<float>& knotsV, std::vector<glm::vec3> controlPoints, float u, float v)
{
    // Step 1: Apply de Boor along the u-direction
    std::vector<glm::vec3> tempPoints;
    for (int i = 0; i < n_v; ++i) {
        // Extract the i-th row of control points (nu points per row)
        std::vector<glm::vec3> row;
        for (int j = 0; j < n_u; ++j) {
            row.push_back(controlPoints[i * n_u + j]);
        }

        // Apply de Boor along the u-direction for this row
        glm::vec3 t = deBoor(du, du, knotsU, row, u);
        tempPoints.push_back(t); // Store the result for v-direction interpolation
    }

    // Now apply de Boor along the v-direction on the result from the u-direction
    glm::vec3 f = deBoor(dv, dv, knotsV, tempPoints, v);
    return f; // Interpolate along v with the new points
}


glm::vec3 Draw::deBoor(int k, int degree, const std::vector<float>& knots, std::vector<glm::vec3> controlPoints, float t)
{
    // Find the knot span index
    int span = -1;
    for (int i = degree; i < knots.size() - 1; ++i) {
        if (t >= knots[i] && t < knots[i + 1]) {
            span = i;
            break;
        }
    }

    if (span == -1) {
        std::cout << "Could not find knot span" << std::endl;
        return glm::vec3(0.0f); // Return a default value if not found
    }

    // Initialize d as a copy of control points for the relevant knot span
    std::vector<glm::vec3> d(degree + 1);
    for (int i = 0; i <= degree; ++i) {
        d[i] = controlPoints[span - degree + i];
    }

    // Perform de Boor recursion
    for (int r = 1; r <= degree; ++r) {
        for (int j = degree; j >= r; --j) {
            float alpha = (t - knots[span - degree + j]) / (knots[span + 1 + j - r] - knots[span - degree + j]);
            d[j] = (1.0f - alpha) * d[j - 1] + alpha * d[j];
        }
    }

    // The evaluated point is now stored in d[degree]
    return d[degree];
}

std::vector<glm::vec3> Draw::ReadLazFile(const std::string& filePath)
{
    std::vector<glm::vec3> points;

   

    return points; // Return the vector of points
}


glm::vec3 Draw::GetPosition()
{
    return position;
}

glm::vec3 Draw::GetSize()
{
    return objSize;
}


void Draw::SetPosition(glm::vec3 newPos)
{
    position = newPos;
}

float Draw::GetMass()
{
    return mass;
}

void Draw::SetMass(float newMass)
{
    mass = newMass;
}

void Draw::SetVelocity(glm::vec3 newVelocity)
{
    velocity = newVelocity;
}

glm::vec3 Draw::GetVelocity()
{
    return velocity;
}

void Draw::ApplyForce(glm::vec3 force)
{
    Acceleration += force / mass;
}

void Draw::Update(float deltaTime, Grid* grid)
{
    velocity += Acceleration * deltaTime;
    position += velocity * deltaTime;
    Acceleration = glm::vec3(0.0f, gravity, 0.0f);
}

void Draw::MoveXdir()
{
   // position.x += speed;
}




void Draw::Delete()
{
    VAO.Delete();
    VBO.Delete();
    EBO1.Delete();
}

void Draw::SetNormalVector(glm::vec3 normal)
{
    normalvector = normal;
}

void Draw::SetAngularVelocity(glm::vec3 angularVelocity)
{
    AngularVelocity = angularVelocity;
}

glm::vec3 Draw::GetAngularVelocity()
{
    return AngularVelocity;
}

void Draw::RotateCube(float deltaTime)
{
    glm::vec3 velocityDirection;
    if (AngularVelocity.x != 0 || AngularVelocity.z != 0) {
        glm::vec3 velocityDirection = glm::normalize(AngularVelocity);
        // std::cout << "V Dir " << velocityDirection.x << ", " << velocityDirection.z << std::endl;
    }

    float speed = glm::length(AngularVelocity);

    glm::quat AngularRotation = glm::angleAxis(glm::radians(1.0f), glm::vec3(-AngularVelocity.z, 0.0f, -AngularVelocity.x));
    Quaternion = Quaternion * AngularRotation * deltaTime;  // Updating rotation (quaternion math)
    Quaternion = glm::normalize(Quaternion);

    glm::mat4 newRotationMatrix = glm::mat4_cast(Quaternion);
    rotation = newRotationMatrix;
    //position += AngularVelocity * deltaTime;
}

glm::vec3 Draw::GetNormal()
{
    return normalvector;
}

void Draw::CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal) {
    float gravity = 9.81f;
    
    
    // Downward gravity force
    glm::vec3 gravityForce(0.0f, -gravity, 0.0f);

    // Calculating normal force (perpendicular to the slope)
    float normalForceMagnitude = glm::dot(gravityForce, normal); // Gravity along the normal
    glm::vec3 normalForce = normal * normalForceMagnitude;

    // Calculating gravitational force acting parallel to the slope (slope vector)
    glm::vec3 gravityParallel = gravityForce - normalForce; // Parallel force along the slope

    // Projecting this parallel gravity onto the slope's horizontal direction (slopeVector)
    glm::vec3 gravityAlongSlope = glm::dot(gravityParallel, slopeVector) * slopeVector;

    // Applying the force along the slope
    ApplyForce(gravityAlongSlope);
}

void Draw::FollowPlayer(Draw& ball, float speed)
{
    glm::vec3 direction = (ball.GetPosition() - GetPosition());

    // Computing the distance between the balls
    float distance = glm::length(direction);

    // Normalizing the direction vector (prevent zero-length vector)
    if (distance > 0.0001f) {
        glm::vec3 dirvec = glm::normalize(direction);

        // Applying force to move the follower ball towards the player ball
        // With speed controls for how fast the follower moves
        glm::vec3 force = dirvec * speed;

        ApplyForce(force); // Applying the calculated force to move the follower
    }
}


std::vector<glm::vec3> Draw::Readfile(const char* fileName)
{
  
    std::ifstream inputFile(fileName);
    std::istringstream iss;
    std::vector<glm::vec3> pointCloud;
    pointCloud.reserve(2600000);
    if (inputFile.is_open()) {

        std::string line;
        std::getline(inputFile, line);
        Vertex vertex;
        char comma; // to capture the commas in the file
        glm::vec3 point;

        int pointSkip = 0;
        while (inputFile >> point.x >> comma >> point.z >> comma >> point.y) {
            pointSkip++;
            if (pointSkip % 5000 == 0) {
                point.x -= 608016.02;
                point.y -= 336.8007;
                point.z -= 6750620.771;
                pointCloud.push_back(point);
            }
           
 
         
        }

        inputFile.close();
    }
    else {
        std::cerr << "Unable to open the input file for reading." << std::endl;
    }
    std::cout << "point Cloud " << pointCloud.size() << std::endl;
    return pointCloud;

}
bool isCounterClockwise(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    // Calculate the signed area (twice) of the triangle
    double area2 = (p2.x - p1.x) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.x - p1.x);
    return area2 > 0;
}

bool Draw::isPointInCircumcircle(glm::vec3& p, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3)
{
  
    // Translating points relative to `p`
    double ax = p1.x - p.x;
    double az = p1.z - p.z;
    double bx = p2.x - p.x;
    double bz = p2.z - p.z;
    double cx = p3.x - p.x;
    double cz = p3.z - p.z;

    // Squared lengths from p to p1, p2, and p3
    double A = ax * ax + az * az;
    double B = bx * bx + bz * bz;
    double C = cx * cx + cz * cz;

    // Determinant for circumcircle test
    double det = ax * (bz * C - B * cz) - az * (bx * C - B * cx) + A * (bx * cz - bz * cx);
   
    // If determinant is positive, point `p` is inside the circumcircle of p1, p2, p3
    return det > 0.0;

  
   

}

std::vector<Triangle> Draw::delaunayTriangulation(std::vector<glm::vec3>& points)
{
    std::vector<Triangle> triangles;
    if (points.size() < 3) return triangles; // Need at least 3 points for triangulation
    triangles.reserve(2600000);

    // Step 1: Calculate bounds of the point cloud in the xz plane
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (auto& point : points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minZ = std::min(minZ, point.z);
        maxZ = std::max(maxZ, point.z);
    }

    // Step 2: Define a super-triangle large enough to encompass all points
    float dx = maxX - minX;
    float dz = maxZ - minZ;
    float deltaMax = std::max(dx, dz);
    float midX = (minX + maxX) / 2.0f;
    float midZ = (minZ + maxZ) / 2.0f;

    float offsetMultiplier = 20.0f; // Experiment with a multiplier (e.g., 10, 20)
    glm::vec3 superP1(midX - offsetMultiplier * deltaMax, 0.0f, midZ - deltaMax);
    glm::vec3 superP2(midX + offsetMultiplier * deltaMax, 0.0f, midZ - deltaMax);
    glm::vec3 superP3(midX, 0.0f, midZ + deltaMax);

    // Add super-triangle to points
    points.push_back(superP1);
    points.push_back(superP2);
    points.push_back(superP3);

    // Step 3: Initialize the triangles with the super-triangle indices
    triangles.push_back({ points.size() - 3, points.size() - 2, points.size() - 1 });

    // Step 4: Incrementally add each point in the point cloud to the triangulation
    std::unordered_map<glm::vec3, size_t, std::hash<glm::vec3>> pointIndexMap;
    for (size_t i = 0; i < points.size(); ++i) {
        pointIndexMap[points[i]] = i;
    }
    int barWidth = 50;
    int processedLines = 0;
    int totalLines = points.size();
    std::cout << "DELAUNEY TRIANGULATING.." << std::endl;
    for (size_t i = 0; i < points.size() - 3; ++i) {
        glm::vec3& p = points[i];
        std::vector<Edge> polygon;

        // Mark triangles whose circumcircles contain the point
        for (auto it = triangles.begin(); it != triangles.end(); /* no increment */) {
            Triangle& tri = *it;
            glm::vec3& p1 = points[tri.p1];
            glm::vec3& p2 = points[tri.p2];
            glm::vec3& p3 = points[tri.p3];

            if (isPointInCircumcircle(p, p1, p2, p3)) {
                polygon.push_back({ p1, p2 });
                polygon.push_back({ p2, p3 });
                polygon.push_back({ p3, p1 });
                it = triangles.erase(it);  // Remove the invalidated triangle
            }
            else {
                ++it;
            }
        }

      /*  if (!polygon.empty()) {
            std::cout << "Polygon edges added for point " << i << ": " << polygon.size() << std::endl;
        }*/

        // Remove duplicate edges from polygon
        for (auto e1 = polygon.begin(); e1 != polygon.end(); ++e1) {
            for (auto e2 = e1 + 1; e2 != polygon.end(); ++e2) {
                if ((e1->v1 == e2->v2 && e1->v2 == e2->v1) || (e1->v1 == e2->v1 && e1->v2 == e2->v2)) {
                    e1->isInvalid = e2->isInvalid = true;
                }
            }
        }

        polygon.erase(std::remove_if(polygon.begin(), polygon.end(), [](Edge& e) { return e.isInvalid; }), polygon.end());

        for (auto& edge : polygon) {
            auto it_v1 = pointIndexMap.find(edge.v1);
            auto it_v2 = pointIndexMap.find(edge.v2);

            if (it_v1 != pointIndexMap.end() && it_v2 != pointIndexMap.end()) {
                size_t index_v1 = it_v1->second;
                size_t index_v2 = it_v2->second;

                // Create the triangle using these indices
                triangles.push_back({ index_v1, index_v2, i });
            }
           
           
            
            // Iterate through v and u to generate surface points
      
        }
       
    }

    // Step 5: Remove triangles connected to super-triangle vertices
    size_t superIdx1 = points.size() - 3, superIdx2 = points.size() - 2, superIdx3 = points.size() - 1;
    triangles.erase(
        std::remove_if(triangles.begin(), triangles.end(), [superIdx1, superIdx2, superIdx3](const Triangle& tri) {
            return tri.p1 == superIdx1 || tri.p2 == superIdx1 || tri.p3 == superIdx1 ||
                tri.p1 == superIdx2 || tri.p2 == superIdx2 || tri.p3 == superIdx2 ||
                tri.p1 == superIdx3 || tri.p2 == superIdx3 || tri.p3 == superIdx3;
            }),
        triangles.end()
    );

    // Remove super-triangle points from points vector
    points.erase(points.end() - 3, points.end());

    return triangles;

       
}

std::vector<glm::vec3> Draw::CalculateBaryCentricCoordinates(std::vector<unsigned int> indices, std::vector<Vertex> vertecies)
{
    std::vector<glm::vec3> results;
    for (int i = 0; i < indices.size(); i += 3) {

        int index0 = indices[i];
        int index1 = indices[i + 1];
        int index2 = indices[i + 2];

        glm::vec3 v0 = glm::vec3(
            (vertecies[index0].x),
            (vertecies[index0].y),
            (vertecies[index0].z));

        glm::vec3 v1 = glm::vec3(
            (vertecies[index1].x,
            vertecies[index1].y,
            vertecies[index1].z));

        glm::vec3 v2 = glm::vec3(
            (vertecies[index2].x),
            (vertecies[index2].y),
            (vertecies[index2].z));

        glm::vec3 centerPoint = (v0 + v1 + v2)/3.0f;
        glm::vec3 v0v1 = v1 - v0;
        glm::vec3 v0v2 = v2 - v0;
        glm::vec3 v0p = centerPoint - v0;

        // Computing dot products
        double dot00 = glm::dot(v0v1, v0v1);
        double dot01 = glm::dot(v0v1, v0v2);
        double dot02 = glm::dot(v0v1, v0p);
        double dot11 = glm::dot(v0v2, v0v2);
        double dot12 = glm::dot(v0v2, v0p);

        // Computing barycentric coordinates
        double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
        double v = (dot11 * dot02 - dot01 * dot12) * invDenom;
        double w = (dot00 * dot12 - dot01 * dot02) * invDenom;
        double u = 1 - v - w;

        glm::vec3 result = glm::vec3(u, v, w);
        results.push_back(result);
    }
    
    
    return results;
}

std::vector<float> Draw::CreateClampedKnotVector(int numControlPoints, int degree)
{
    int knotVectorSize = numControlPoints + degree + 1;
    std::vector<float> knotVector(knotVectorSize);

    // First degree + 1 knots are clamped to 0
    for (int i = 0; i <= degree; ++i) {
        knotVector[i] = 0.0f;
    }

    // Interior knots are evenly spaced
    int numInteriorKnots = knotVectorSize - 2 * (degree + 1);
    for (int i = 1; i <= numInteriorKnots; ++i) {
        knotVector[degree + i] = static_cast<float>(i);
    }

    // Last degree + 1 knots are clamped to numInteriorKnots
    float maxKnotValue = static_cast<float>(numInteriorKnots);
    for (int i = knotVectorSize - degree - 1; i < knotVectorSize; ++i) {
        knotVector[i] = maxKnotValue;
    }

    return knotVector;
}

void Draw::UpdateBSpline(glm::vec3 pos, glm::vec3 velocity)
{
    
    pos += velocity;
    
    controllPoints.push_back(pos);

    DrawBspline();
}



