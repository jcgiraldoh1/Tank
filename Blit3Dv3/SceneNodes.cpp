#define _USE_MATH_DEFINES
#include <math.h>
#include "SceneNodes.h"
#include <glm/gtc/matrix_transform.hpp>

//for the PrintNode
#include <iostream>

TranslatorNode::TranslatorNode(glm::vec3 offset, SceneNode* parent)
{
	typeId = sceneNodeType::TRANSLATE;
	this->offset = offset;
	this->parent = parent;
	this->parent->children.push_back(this);
}

void TranslatorNode::Update(float seconds)
{
	assert(parent != NULL && "parent pointer is NULL");
	//translate
	snodeMutex.lock();
	parent->snodeMutex.lock();
	modelMatrix = glm::translate(parent->modelMatrix, offset);
	parent->snodeMutex.unlock();
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

PrintNode::PrintNode()
{
	typeId = sceneNodeType::PRINT;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

void PrintNode::Draw()
{
	//we would normally use our modelMatrix to draw, so let's calculate where we 
	//would be in the world based on it
	glm::vec4 position = glm::vec4(0, 0, 0, 1);

	snodeMutex.lock();
	position = modelMatrix * position;
	snodeMutex.unlock();

	std::cout << "Drawing at: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	//draw all of our children
	for (auto n : children)
	{
		n->Draw();
	}
}

MeshNode::MeshNode(SceneNode* parent, Mesh* mesh)
{
	typeId = sceneNodeType::MESH;
	this->parent = parent;
	this->parent->children.push_back(this);
	this->mesh = mesh;
	this->modelMatrix = parent->modelMatrix;
}

void MeshNode::Draw()
{
	mesh->Draw(&this->modelMatrix);

	//draw all of our children
	for (auto n : children)
	{
		n->Draw();
	}
}

void MeshNode::Update(float seconds)
{
	snodeMutex.lock();
	parent->snodeMutex.lock();
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

//Rotating a node on an axis
RotatorNode::RotatorNode(float speed, float startingAngel, glm::vec3 axis, float angle, SceneNode* parent)
{
	typeId = sceneNodeType::ROTATE;

	this->speed = glm::radians(speed);
	this->startingAngle = startingAngel;
	this->axis = axis;
	this->angle = angle;
	this->parent = parent;
	this->parent->children.push_back(this);
	this->modelMatrix = parent->modelMatrix;
}

//Rotate a node on an axis
void RotatorNode::Update(float seconds)
{
	if (speed != 0)
	{
		angle += speed * seconds;
		if (angle > M_PI * 2)
			angle -= M_PI * 2;
		if (angle < -M_PI * 2)
			angle += M_PI * 2;

		//calculate model matrix
		snodeMutex.lock();
		parent->snodeMutex.lock();
		modelMatrix = glm::rotate(parent->modelMatrix, angle, axis);//apply modelMatrix from our parent	
		parent->snodeMutex.unlock();
		snodeMutex.unlock();

	}
	else
	{
		//calculate model matrix
		snodeMutex.lock();
		parent->snodeMutex.lock();
		modelMatrix = glm::rotate(parent->modelMatrix, startingAngle, axis);//apply modelMatrix from our parent	
		parent->snodeMutex.unlock();
		snodeMutex.unlock();
	}
	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}