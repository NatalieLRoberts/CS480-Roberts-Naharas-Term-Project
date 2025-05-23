#include "object.h"
#include "Texture.h"

class Sphere :
    public Object
{
public:
    Sphere();

    void SetLighting(glm::vec3 lightPosition, glm::vec3 lightCol, bool emissive);

    void Render(GLint positionAttribLoc, GLint colorAttribLoc);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex, GLint lightPosLoc, GLint lightColorLoc, GLint isEmissiveLoc);

    glm::mat4 GetModel() { return model; }
    void Update(glm::mat4 matModel);

    Sphere(int prec);
    Sphere(int prec, const char* fname);

    // Factory methods for celestial bodies
    static Sphere* createSun(int prec = 48);
    static Sphere* createPlanet(int prec = 48, float orbitDistance = 5.0f);
    static Sphere* createMoon(int prec = 48, glm::vec3 planetPosition = glm::vec3(5.0f, 0.0f, 0.0f),float orbitDistance = 1.5f);

    // Orbital motion methods
    void updatePlanetOrbit(float time, float orbitRadius, float orbitSpeed);
    void updateMoonOrbit(glm::vec3 planetPosition, float time, float orbitRadius, float orbitSpeed);

    int getNumVertices();
    int getNumIndices();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();

    GLuint getTextureID() { return m_texture->getTextureID(); }
    
    bool hasTex;

    void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);


private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    glm::vec3 lightPos; 
    glm::vec3 lightColor; 
    bool isEmissive;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    Texture* m_texture;


    GLuint vao;

    float angle;

    void setupVertices();
    void setupBuffers();

    int numVertices;
    int numIndices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    void init(int);
    float toRadians(float degrees);

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

 

};