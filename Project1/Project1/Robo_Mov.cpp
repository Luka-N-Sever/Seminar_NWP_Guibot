#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <windows.h>
# define M_PI 3.14159265358979323846

class Leg {

public:
	int CurrAngle = 270; //straight down ... or rest postion (Unit Circle)
	int Length = 100; //px
	POINT Joint;
	POINT Foot;
	std::vector<POINT> Feet_Points;
	void Move(int& CurrAngle, int TargetAngle, int OrientatinFlag); // 0 == RightLeg, 1 == LeftLeg
};

void Leg::Move(int& CurrAngle, int TargetAngle, int OrientationFlag) {
	
	int AngleDiff = 0;

	if (!(Feet_Points.empty()))
		Feet_Points.clear();

	if (OrientationFlag == 0 && TargetAngle > CurrAngle)
		AngleDiff = 1;                                   //RightLeg Move OutWards
	if (OrientationFlag == 0 && TargetAngle < CurrAngle)
		AngleDiff = -1;                                  //RightLeg Move Inwards
	if (OrientationFlag == 1 && TargetAngle > CurrAngle)
		AngleDiff = 1;                                   //LeftLeg Move Inwards
	if (OrientationFlag == 1 && TargetAngle < CurrAngle)
		AngleDiff = -1;                                  //LeftLeg Move Outwards
			
	for (; CurrAngle != TargetAngle; CurrAngle += AngleDiff) {
		Foot.x = Joint.x + (Length * cos(CurrAngle*M_PI / 180));
		Foot.y = Joint.y - (Length * sin(CurrAngle*M_PI / 180));
		Feet_Points.push_back(Foot);
	}
	//SetFootPos(Foot);
}

struct Body {

	int length;
	POINT Center;
	int Delta_Step; // For the joint positions 
	Leg Right_L;
	Leg Left_L;

	Body(Leg Right_L, Leg Left_L) { 
		Center.x = (Right_L.Joint.x + Left_L.Joint.x) / 2;
		Center.y = (Right_L.Joint.y + Left_L.Joint.y) / 2;
	};

	void SetLength(int L) { length = L; };
};

class MathBot {
	
	Body B;
	
	void MoveLeft(Body B);
	void MoveRight(Body B);
};

//The movemnet of the joints is not descibed here due to the complexity behind their movemnet
// One day I hope to be able to have the time to map out their movemnet

void MathBot::MoveLeft(Body B) {
	B.Right_L.Move(B.Right_L.CurrAngle, 250, 0); //In by 20 Deg
	B.Left_L.Move(B.Left_L.CurrAngle, 250, 1); //Out by 20 Deg
	B.Right_L.Move(B.Right_L.CurrAngle, 270, 0); //Out by 20 Deg (Back to relative rest postion)
	B.Left_L.Move(B.Left_L.CurrAngle, 270, 1); //In by 20 Deg (Back to relative rest position)
}

void MathBot::MoveRight(Body B) {
	B.Left_L.Move(B.Left_L.CurrAngle, 290, 1); //In by 20 Deg
	B.Right_L.Move(B.Right_L.CurrAngle, 290, 0); //Out by 20 Deg
	B.Left_L.Move(B.Left_L.CurrAngle, 270, 1); //Out by 20 Deg (Back to relative rest postion)
	B.Right_L.Move(B.Right_L.CurrAngle, 270, 0); //In by 20 Deg (Back to relative rest position)
}

