#include "SampleBase.hpp"
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>

namespace Diligent
{
class MeshShader final : public SampleBase
{
public:
    virtual void        Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void        Update(double ctime, double etime) override final;
    virtual void        Render() override final;
    virtual const Char* GetSampleName() const override final
    {
        return "MESH SHADER LOAD OBJECT";
    }

private:
    std::vector<float3> Positions;
    std::vector<std::vector<unsigned long>> Polygons;
    
    
    unsigned long nFaces;
    unsigned long nVertices; 
    unsigned long nEdges;

    bool read_next_line(std::ifstream& lstrm, std::string& lstr);
    bool LoadOFF(const char *FilePath);
    

};
} // namespace Diligent