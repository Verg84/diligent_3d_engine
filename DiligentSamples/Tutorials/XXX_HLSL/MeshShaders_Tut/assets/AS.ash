#inlude"structures.fxh"

// draw tasks
StructuredBuffer<DrawTask> DrawTasks;

// constants buffer
cbuffer cbConstants
{
	Constants g_Constants;
};
// object geometry
cbuffer cbCubeData
{
	CubeData g_CubeData;
}

// payload for the mesh shader
groupshared Payload s_Payload;

// visibility
bool IsVisible(float3 cubeCenter,float radius)
{
	float4 center=float4(cubeCenter,1.0);
	// compute distance from each plane
	// visible if distance is smaller than object's radius
	for(int i=0;i<6;++i)
	{
		if(dot(g_Constants.Frustum[i],center)<-radius)
		return false;
	}
	return true;
}

// LOD compute
float CalcLevelOfDetail(float3 cubeCenter,float radius)
{
	// cube center - center of sphere
	// radius	-	radius of circumscribed sphere

	// Position in View Space
	float3 pos=mul(float4(cubeCenter,1.0),g_Constants.ViewMat).xyz;
	// Square distance from camera to circ.sphere
	float dist2=dot(pos,pos);
	// sphere size in screen space
	float size=g_Constants.CoTanHalfFov*radius/sqrt(dist2-radius*radius);
	// Calculate level of detail
	float level=clammp(1.0-size,0.0,1.0);
	return level;
}

// number of visible objects
groupshared uint s_TaskCount;

[numthreads(GROUP_SIZE,1,1)]
void main(
	in uint I:SV_GroupIndex,
	in uint wg:SV_GroupID)
{
	// Reset counter from the 1st thread in the group
	if(I==0)
	{
		s_TaskCount=0;
	}
	// synchronize cache
	GroupMemoryBarrierWithGroupSync();

	// Read Task arguments
	cost uint gid=wg*GROUP_SIZE+I;
	DrawTask task=drawTasks[gid];
	float3 scale=task.Scale;
	float3 pos=float3(task.BasePos,0.0).xyz;
	// Frustum Culling
	if(g_Constants.FrustumCulling==0||IsVisible(pos,g_Constants.SphereRadius.x*scale))
	{
		uint index=0;
		InterlockedAdds(s_TaskCount,1,index);
		s_Payload.PosX[index]=pos.x;
		s_Payload.PosY[index]=pos.y;
		s_Payload.PosZ[index]=pos.z;
		s_Payload.Scale[index]=scale;
		s_Payload.LODs[index]=CalcLevelOfDetail(pos,g_CubeData.SphereRadius.x*scale);
	}
	// wait until all threads are finished
	// before reading s_TaskCount
	GroupMemoryBarrierWithGroupSync();
	DispatchMesh(s_TaskCount,1,1,s_Payload);
}