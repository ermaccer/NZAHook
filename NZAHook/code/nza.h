#pragma once

#define NZAHOOK_VERSION "0.1"


struct CVector {
	float X, Y, Z;
};


int _addr(int addr);



class Camera {
public:
	CVector pos;
	// matrix ?
	void SetPosition(CVector* pos);
	// definitely not vector, some big struct, probably matrix?
	void SetRotation(CVector* rot);
};