#include "APC.h"

extern Blit3D* blit3D;


//Make an actual 3D tank by using nodes
APC::APC(SceneNode* parent, glm::vec3 startingPos, GLSLProgram* shader, float wheelTurnLimit, float gunAL, float moveSpeed)
{
	this->gunAngleLimit = gunAL;
	this->moveSpeed = moveSpeed;
	this->wheelTurnLimit = wheelTurnLimit;
	turnSpeed = std::pow(20, moveSpeed);									//Base the turning speed on how fast the tank is going

	//"Images" location
	hullMesh = new Mesh(blit3D->tManager, shader, "Data\\hull.s3d");
	turretMesh = new Mesh(blit3D->tManager, shader, "Data\\turret.s3d");
	gunMesh = new Mesh(blit3D->tManager, shader, "Data\\gun.s3d");
	wheelMesh = new Mesh(blit3D->tManager, shader, "Data\\wheel.s3d");

	//Main parts
	mainTranslatorNode = new TranslatorNode(startingPos, parent);
	addingRotatorNode = new RotatorNode(0.f, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f), 0.f, mainTranslatorNode);
	addingRotatorNode2 = new RotatorNode(0.f, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f), 0.f, addingRotatorNode);
	mainRotatorNode = new RotatorNode(1.f, 0.f, glm::vec3(0.f, 0.f, 1.f), 0.f, addingRotatorNode2);
	hullMeshNode = new MeshNode(mainRotatorNode, hullMesh);

	//Left front Wheel: Child of Hull
	lfFrontWTranslatorNode = new TranslatorNode(glm::vec3(-2.f, 1.75f, -1.6f), hullMeshNode);
	lfFtWSettingRotatorNode = new RotatorNode(0.f, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f), 0.f, lfFrontWTranslatorNode);
	lfFtWTurningRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f), 0.f, lfFtWSettingRotatorNode);
	lfFtWRollRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f), 0.f, lfFtWTurningRotatorNode);
	lfFtWMeshNode = new MeshNode(lfFtWRollRotatorNode, wheelMesh);
		
	//Left back Wheel: Child of Hull
	lfBkWTranslatorNode = new TranslatorNode(glm::vec3(-2.f, -2.25f, -1.6f), hullMeshNode);
	lfBkWSettingRotatorNode = new RotatorNode(0.f, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f), 0.f, lfBkWTranslatorNode);
	lfBkWRollRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f), 0.f, lfBkWSettingRotatorNode);
	lfBkWMeshNode = new MeshNode(lfBkWRollRotatorNode, wheelMesh);
		
	//Right front Wheel: Child of Hull
	rgFrontWTranslatorNode = new TranslatorNode(glm::vec3(2.f, 1.75f, -1.6f), hullMeshNode);
	rgFtWSettingRotatorNode = new RotatorNode(0.f, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f), 0.f, rgFrontWTranslatorNode);
	rgFtWTurningRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f), 0.f, rgFtWSettingRotatorNode);
	rgFtWRollRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f), 0.f, rgFtWTurningRotatorNode);
	rgFtWMeshNode = new MeshNode(rgFtWRollRotatorNode, wheelMesh);

	//Right back Wheel: Child of Hull
	rgBkWTranslatorNode = new TranslatorNode(glm::vec3(2.f, -2.25f, -1.6f), hullMeshNode);
	rgBkWSettingRotatorNode = new RotatorNode(0.f, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f), 0.f, rgBkWTranslatorNode);
	rgBkWRollRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f), 0.f, rgBkWSettingRotatorNode);
	rgBkWMeshNode = new MeshNode(rgBkWRollRotatorNode, wheelMesh);

	//Turret: Child of Hull 
	turretTranslatorNode = new TranslatorNode(glm::vec3(0.f, 0.f, 2.f), hullMeshNode);
	turrentRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f), 0.f, turretTranslatorNode);
	turretMeshNode = new MeshNode(turrentRotatorNode, turretMesh);

	//Gun: Child of Hull 
	gunTranslatorNode = new TranslatorNode(glm::vec3(0.f, 1.7f, 0.f), turretMeshNode);
	gunAddingRotatorNode = new RotatorNode(0.f, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f), 0.f, gunTranslatorNode);
	gunRotatorNode = new RotatorNode(1.f, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f), 0.f, gunAddingRotatorNode);
	gunMeshNode = new MeshNode(gunRotatorNode, gunMesh);
}

//Move the tank forward and backward. Also rotate stuff on it like; wheels, turret and gun
void APC::Update(float seconds, float rightBumper, float leftBumper, float leftJoystickLeftRight, float rightJoystickLeftRight, float rightJoystickUpDown)
{

	//moving forward
	if (rightBumper > 0.f)
	{
		moving = true;
		velocity.z += rightBumper * -1;
		wheelSpinAngle += rightBumper; 
		angle += leftJoystickLeftRight * -1; 
		movForOrBac = 1;
	}
	//moving backward
	else if (leftBumper > 0.f)
	{
		moving = true;
		velocity.z += leftBumper; 
		wheelSpinAngle += leftBumper * -1; 
		angle += leftJoystickLeftRight; 
		movForOrBac = -1;
	}
	else
		velocity.z = 0.f;

	//Rotate stuff
	wheelAngle = leftJoystickLeftRight; 
	gunAngle += rightJoystickUpDown; 
	turretAngle += rightJoystickLeftRight; 

	AngleUntil360(angle);
	AngleUntil360(wheelSpinAngle);
	AngleUntil360(turretAngle);

	if (moving)														
	{
		moving = false;												
		float radians = -(angle - 90.f) * (M_PI / 180.f);			
		facing.x = std::cos(radians);								
		facing.z = std::sin(radians);
		velocity += facing * 700.f;									
		velocity = glm::normalize(velocity) * moveSpeed;
		mainTranslatorNode->offset += velocity * (moveSpeed * movForOrBac) * seconds;
	}
	mainRotatorNode->angle = glm::radians(angle);				    

	//Rotate the wheel for rolling and turning
	lfFtWTurningRotatorNode->angle = -wheelAngle / wheelTurnLimit; 
	rgFtWTurningRotatorNode->angle = wheelAngle / wheelTurnLimit;  
	lfFtWRollRotatorNode->angle = glm::radians(wheelSpinAngle); 
	rgFtWRollRotatorNode->angle = glm::radians(-wheelSpinAngle);
	lfBkWRollRotatorNode->angle = glm::radians(wheelSpinAngle);  
	rgBkWRollRotatorNode->angle = glm::radians(-wheelSpinAngle); 


	if (gunAngle <= gunAngleLimit + 10.f && gunAngle >= -gunAngleLimit)	 
		prevGunAngle = gunRotatorNode->angle = glm::radians(-gunAngle);
	else
	{
		gunRotatorNode->angle = prevGunAngle;						
		if (gunAngle > 0)
			gunAngle = gunAngleLimit + 10.f;							  
		else
			gunAngle = -gunAngleLimit;							      
	}

	turrentRotatorNode->angle = glm::radians(turretAngle);			
}


//Resets the tank to a desired position
void APC::ResetPosition(glm::vec3 pos)
{
	mainTranslatorNode->offset = pos;//Translate the whole tank to this pos
}

//Checks and manage angle to stay between 0 and 360
void APC::AngleUntil360(float& angle)
{
	if (angle >= 360)
		angle = 0;
}