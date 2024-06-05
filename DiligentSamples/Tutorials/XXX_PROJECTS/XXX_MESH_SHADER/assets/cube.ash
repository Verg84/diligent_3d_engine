#include"structures.fxh"

StructuredBuffer<DrawTask> DrawTasks;

cbuffer cbConstants
{
	Constants g_Constants;
};

cbuffer cdCubeData
{
	CubeData g_CubeData;
};

RWButeAddressBuffer Staticstics;

groupshared Payload s_Payload;

bool IsVisible(float3 cubeCenter,float radius)
{
	float4 center=(float4(cubeCenter,1.0));
	for(int i=0;i<6;++i)
	{
		if(dot(g_Constants.Frustum[i],center)< -radius)
			return false;
	}
	return true;
}

float CalcDetailLevel(float3 cubeCenter,float radius)
{
	float3 pos=mul(float4(cubeCenter,1.0),g_Constants.ViewMat).xyz;
	float2 dist=dot(pos,pos);
	float size=g_Constants.CoTanHalfFov*radius/sqrt(dist2-radius*radius);

	// detail level
	float level=clamp(1.0-size,0.0,1.0);
	return level;
}

groupshared uint s_TaskCount;

[numthreads(GROUP_SIZE,1,1)]
void main(
	in uint I:SV_GroupIndex,
	in uint wg:SV_GroupID)
{
	if(I==0)
	{
		s_TaskCount=0;
	}

	GroupMemoryBarrierWithGroupSync();

	// task arguments
	const uint gid=wg*GROUP_SIZE+I;
	DrawTask task=DrawTasks[gid];
	float3 pos=float3(task.BasePos,0.0).xyz;
	float scale=task.Scale;
	float timeOffset=task.TimeOffset;

	pos.y=sin(g_Constants.CurrTime+timeOffset);

	// Frustum Culling
	if(g_Constants.FrustumCulling==0 || IsVisible(pos,g_CubeData.SphereRadius.x*scale))
	{
		uint index=0;
		InterlockedAdd(s_TaskCount,1,index);

		s_Payload.PosX[index]=pos.x;
		s_Payload.PosY[index]=pos.y;
		s_Payload.PosZ[index]=pos.z;

		s_Payload.Scale[index]=scale;
		s_Payload.LODs[index]=CalcDetailLevel(pos,g_CubeData.SphereRadius.x*scale);
	}
	GroupMemoryBarrierWithGroupSync();
	if(I==0)
	{
		uint orig_value;
		Statistics.InterlockedAdd(0,s_TaskCount,orig_value);
	}
	DispatchMesh(s_TaskCount,1,1,s_Payload);
}
