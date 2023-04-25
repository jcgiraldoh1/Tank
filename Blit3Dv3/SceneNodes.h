#pragma once
#include "SceneManager.h"
#include "Mesh.h"

class Mesh;

class TranslatorNode : public SceneNode
{
public:
	//need a vector to translate by
	glm::vec3 offset;
	//override Update() so that we can translate
	void Update(float seconds);
	TranslatorNode(glm::vec3 offset, SceneNode* parent);
};

//example node that "draws"
class PrintNode : public SceneNode
{
public:
	void Draw();
	PrintNode();
};

class MeshNode : public SceneNode
{
public:
	Mesh* mesh;
	MeshNode(SceneNode* parent, Mesh* mesh);
	void Draw();
	void Update(float seconds);
};

class RotatorNode : public SceneNode
{
public:
	float speed;
	float startingAngle;
	float angle;
	glm::vec3 axis;

	void Update(float seconds);
	RotatorNode(float speed, float startingAngle, glm::vec3 axis, float angle, SceneNode* parent);
};