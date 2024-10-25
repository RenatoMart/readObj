#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/freeglut.h>

GLfloat camX = 20.0f, camY = 20.0f, camZ = 20.0f; // Posición inicial de la cámara

struct Vertex {
    float x, y, z;
};

struct Normal {
    float nx, ny, nz;
};

struct TextureCoord {
    float u, v;
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> textureIndices;
    std::vector<int> normalIndices;
};

class OBJLoader {
public:
    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                Vertex vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            } else if (prefix == "vn") {
                Normal normal;
                iss >> normal.nx >> normal.ny >> normal.nz;
                normals.push_back(normal);
            } else if (prefix == "vt") {
                TextureCoord texCoord;
                iss >> texCoord.u >> texCoord.v;
                textureCoords.push_back(texCoord);
            } else if (prefix == "f") {
                Face face;
                std::string vertexData;
                while (iss >> vertexData) {
                    std::istringstream viss(vertexData);
                    std::string index;
                    int vertexIndex, textureIndex, normalIndex;

                    std::getline(viss, index, '/');
                    vertexIndex = std::stoi(index);
                    face.vertexIndices.push_back(vertexIndex);

                    if (std::getline(viss, index, '/')) {
                        if (!index.empty()) {
                            textureIndex = std::stoi(index);
                            face.textureIndices.push_back(textureIndex);
                        }
                        if (std::getline(viss, index, '/')) {
                            normalIndex = std::stoi(index);
                            face.normalIndices.push_back(normalIndex);
                        }
                    }
                }
                faces.push_back(face);
            }
        }

        file.close();
        return true;
    }

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<Normal>& getNormals() const { return normals; }
    const std::vector<TextureCoord>& getTextureCoords() const { return textureCoords; }
    const std::vector<Face>& getFaces() const { return faces; }

private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<TextureCoord> textureCoords;
    std::vector<Face> faces;
};

void moverCamara(unsigned char key, int x, int y)
{
    switch (key) {
        case 'z':  //camara
            camZ -= 1.0f;
        break;
        case 'x':
            camZ += 1.0f;
        break;
        case 'c':
            camY += 1.0f;
        break;
        case 'v':
            camY -= 1.0f;
        break;
        case 'w':
            camX -= 1.0f;
        break;
        case 'q':   //camara
            camX += 1.0f;
        break;
        default:
            break;
    }
    glutPostRedisplay();  // Actualiza la pantalla
}

void drawFace(const Face& face, const std::vector<Vertex>& vertices, const std::vector<Normal>& normals, const std::vector<TextureCoord>& textureCoords) {
    glBegin(face.vertexIndices.size() == 3 ? GL_TRIANGLES : GL_QUADS);  // Si son 3 vértices, dibuja triángulos, si son 4, dibuja cuadriláteros
    for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
        if (!face.normalIndices.empty()) {
            const Normal& normal = normals[face.normalIndices[i] - 1];
            glNormal3f(normal.nx, normal.ny, normal.nz);
        }
        if (!face.textureIndices.empty()) {
            const TextureCoord& texCoord = textureCoords[face.textureIndices[i] - 1];
            glTexCoord2f(texCoord.u, texCoord.v);
        }
        const Vertex& vertex = vertices[face.vertexIndices[i] - 1];
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Configura la cámara usando las variables de posición de la cámara
    gluLookAt(camX, camY, camZ,  // posición de la cámara (ojo)
              0.0, 0.0, 0.0,    // hacia donde mira
              0.0, 1.0, 0.0);   // vector de "arriba"

    // Dibuja el modelo OBJ cargado
    OBJLoader loader;
    if (loader.load("/home/renatomr/CLionProjects/readObj/cmake-build-debug/centaurwarriorzzz.obj")) {
        const auto& vertices = loader.getVertices();
        const auto& normals = loader.getNormals();
        const auto& textureCoords = loader.getTextureCoords();
        const auto& faces = loader.getFaces();

        for (const auto& face : faces) {
            drawFace(face, vertices, normals, textureCoords);
        }
    }

    glutSwapBuffers();
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OBJ Loader");

    initOpenGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(moverCamara);

    glutMainLoop();

    return 0;
}


