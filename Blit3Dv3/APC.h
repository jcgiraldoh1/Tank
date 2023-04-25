#pragma once
#include "Blit3D.h"
#include "SceneNodes.h"
class APC
{
private:
	//Translate node parts
	TranslatorNode* mainTranslatorNode;
	TranslatorNode* turretTranslatorNode;
	TranslatorNode* gunTranslatorNode;
	TranslatorNode* lfFrontWTranslatorNode;
	TranslatorNode* rgFrontWTranslatorNode;
	TranslatorNode* lfBkWTranslatorNode;
	TranslatorNode* rgBkWTranslatorNode;

	//Rotation nodes
	RotatorNode* addingRotatorNode;
	RotatorNode* addingRotatorNode2;
	RotatorNode* mainRotatorNode;
	RotatorNode* turrentRotatorNode;
	RotatorNode* gunAddingRotatorNode;
	RotatorNode* gunRotatorNode;
	RotatorNode* lfFtWSettingRotatorNode;
	RotatorNode* lfFtWTurningRotatorNode;
	RotatorNode* lfFtWRollRotatorNode;
	RotatorNode* rgFtWSettingRotatorNode;
	RotatorNode* rgFtWTurningRotatorNode;
	RotatorNode* rgFtWRollRotatorNode;
	RotatorNode* rgBkWSettingRotatorNode;
	RotatorNode* rgBkWRollRotatorNode;
	RotatorNode* lfBkWSettingRotatorNode;
	RotatorNode* lfBkWRollRotatorNode;

	//mesh nodes
	MeshNode* hullMeshNode;
	MeshNode* turretMeshNode;
	MeshNode* gunMeshNode;
	MeshNode* lfFtWMeshNode;
	MeshNode* rgFtWMeshNode;
	MeshNode* lfBkWMeshNode; 
	MeshNode* rgBkWMeshNode; 

	Mesh* hullMesh;
	Mesh* turretMesh;
	Mesh* gunMesh;
	Mesh* wheelMesh;

	float wheelTurnLimit = 2.5f;	
	float gunAngleLimit = 20.f;			
	float moveSpeed = 2.f;			
	float turnSpeed = 700.f;		
	float prevGunAngle = 0.f;		

	glm::vec3 facing;
public:

	glm::vec3 velocity;
	int movForOrBac = 0;			
	float wheelAngle = 0.f;			
	float gunAngle = 0.f;			
	float turretAngle = 0.f;		
	float wheelSpinAngle = 0.f;		
	float angle = 0.f;				
	bool moving = false;			

	APC(SceneNode* parent, glm::vec3 startingPos, GLSLProgram* shader, float wheelTurnLimit, float gunLimit, float moveSpeed);
	void Update(float seconds, float driveA, float reverseB, float leftJoystickX, float rightJoystickX, float rightJoystickY);
	void ResetPosition(glm::vec3 pos);//Reset the position of the tank when pressed 'R' when running the program
	void AngleUntil360(float& angle);
};
