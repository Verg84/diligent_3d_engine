struct ParticleAttribs
{
	float2 f2Pos;
	float2 f2NewPos;
	float2 f2Speed;
	flat2 f2NewSpeed;

	float fSize;
	float fTemperature;
	int iNumCollisions;
	float fPadding0;
};

struct GlobalConstants
{
	uint uiNumParticles;
	float fDeltaTime;
	float dummy0;
	float dummy1;

	float2 f2Scale;
	int2 i2ParticleGridSize;
};