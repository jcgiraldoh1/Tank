#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "TextureManager.h"
#include "SceneNodes.h"
#include "glslprogram.h"
#include <string>

//this macro helps calculate offsets for VBO stuff
//Pass i as the number of bytes for the offset, so be sure to use sizeof() 
//to help calculate bytes accurately.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class Mesh
{
public:
	int numberOfVertices;
	std::vector<GLfloat> Vertices;  // possibly make a struct for 8 at a time...
	int numberOfIndices;
	std::vector<GLushort> Indices;
	std::string textureFilename;

	GLuint vaoID, vboID, eboID, texID;
	TextureManager* txManager;
	GLSLProgram* shaders;

	Mesh(TextureManager* tManager, GLSLProgram* shaders, std::string filename);	//Constructor
	void LoadMeshFromFile(std::string filename);
	void Draw(glm::mat4* modelMatrix);
	~Mesh();

};
