#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <windows.h>
# define M_PI 3.14159265358979323846

struct Leg {

	int Length;
	POINT Joint;
	POINT Foot;

	std::vector<POINT> Feet_Points;

	void SetPoint(POINT p) { Joint = p; };
	void SetLength(int length) { Length = length; };
	void SetFootPos(POINT p) { Foot = p; };
	void Move_Inwards_By_20_Deg(Leg leg, POINT joint, POINT foot);
	void Move_Outwards_By_20_Deg(Leg leg, POINT joint, POINT foot);
	void Move_From_Outwards_To_Inwards(Leg leg, POINT joint, POINT foot);
	void Move_From_Inwards_To_Outwards(Leg leg, POINT joint, POINT foot);
};

void Leg::Move_Inwards_By_20_Deg(Leg leg, POINT joint, POINT foot) {
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 270; angle >= 250; angle--) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

void Leg::Move_Outwards_By_20_Deg(Leg leg, POINT joint, POINT foot) {
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 270; angle <= 290; angle++) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

void Leg::Move_From_Outwards_To_Inwards(Leg leg, POINT joint, POINT foot) {
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 290; angle >= 250; angle--) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

void Leg::Move_From_Inwards_To_Outwards(Leg leg, POINT joint, POINT foot) {
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 250; angle <= 290; angle++) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

struct RightLeg : Leg {};

struct LeftLeg : Leg {

	//These Functions must be overwritten for the left leg because of the unit crcle!!
	void Move_Inwards_By_20_Deg(Leg leg, POINT joint, POINT foot);
	void Move_Outwards_By_20_Deg(Leg leg, POINT joint, POINT foot);
	void Move_From_Outwards_To_Inwards(Leg leg, POINT joint, POINT foot);
	void Move_From_Inwards_To_Outwards(Leg leg, POINT joint, POINT foot);
};

void LeftLeg::Move_Inwards_By_20_Deg(Leg leg, POINT joint, POINT foot){
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 270; angle <= 290; angle++) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

void LeftLeg::Move_Outwards_By_20_Deg(Leg leg, POINT joint, POINT foot){
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 270; angle >= 250; angle--) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

void LeftLeg::Move_From_Outwards_To_Inwards(Leg leg, POINT joint, POINT foot){
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 250; angle <= 290; angle++) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

void LeftLeg::Move_From_Inwards_To_Outwards(Leg leg, POINT joint, POINT foot){
	if (!(Feet_Points.empty()))
		Feet_Points.clear();
	for (int angle = 290; angle >= 250; angle--) {
		foot.x = joint.x + (leg.Length * cos(angle*M_PI / 180));
		foot.y = joint.y - (leg.Length * sin(angle*M_PI / 180));
		Feet_Points.push_back(foot);
	}
	SetFootPos(foot);
}

struct Body {

	int length;
	POINT Center;
	int Delta_Step;
	RightLeg Right_L;
	LeftLeg Left_L;

	Body(RightLeg Right_L, LeftLeg Left_L) { 
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

void MathBot::MoveLeft(Body B) {
	B.Right_L.Move_Inwards_By_20_Deg(B.Right_L, B.Right_L.Joint, B.Right_L.Foot);
	//B.Right_L.Feet_Points.front();
	//or use iterator to get all of the feet points in order and do something with them while they're being spat out one by one
	B.Left_L.Move_Outwards_By_20_Deg(B.Left_L, B.Left_L.Joint, B.Left_L.Foot);
	B.Right_L.Move_From_Inwards_To_Outwards(B.Right_L, B.Right_L.Joint, B.Right_L.Foot);
	B.Right_L.Joint.x + B.Delta_Step; B.Left_L.Joint.x + B.Delta_Step; B.Center.x + B.Delta_Step;
	B.Left_L.Move_Inwards_By_20_Deg(B.Left_L, B.Left_L.Joint, B.Left_L.Foot);
	B.Right_L.Move_Inwards_By_20_Deg(B.Right_L, B.Right_L.Joint, B.Right_L.Foot);
}

void MathBot::MoveRight(Body B) {
	B.Left_L.Move_Inwards_By_20_Deg(B.Left_L, B.Left_L.Joint, B.Left_L.Foot);
	B.Right_L.Move_Outwards_By_20_Deg(B.Right_L, B.Right_L.Joint, B.Right_L.Foot);
	B.Left_L.Move_From_Inwards_To_Outwards(B.Left_L, B.Left_L.Joint, B.Left_L.Foot);
	B.Right_L.Joint.x + B.Delta_Step; B.Left_L.Joint.x + B.Delta_Step; B.Center.x + B.Delta_Step;
	B.Right_L.Move_Inwards_By_20_Deg(B.Right_L, B.Right_L.Joint, B.Right_L.Foot);
	B.Left_L.Move_Inwards_By_20_Deg(B.Left_L, B.Left_L.Joint, B.Left_L.Foot);
}

