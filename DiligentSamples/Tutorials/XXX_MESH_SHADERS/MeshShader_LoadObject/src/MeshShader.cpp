#include"MeshShader.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new MeshShader();
}
bool MeshShader::read_next_line(std::fstream &lstrm,std::string &lstr)
{
    while (getline(lstrm, lstr))
    {
        if (lstr.length() > 1 && lstr[0] != '#')
        {
            return true;
        }
    }
    return false;
}
bool MeshShader::LoadOFF(const char* FilePath)
{

    std::fstream lstrm(FilePath);
    if (!lstrm.is_open())
        return false;  
    return true;
}
void MeshShader::Initialize(const SampleInitInfo& InitInfo)
{   
    VERIFY_EXPR(!LoadOFF("cube.off"));

}
void MeshShader::Render()
{
}
void MeshShader::Update(double ctime, double etime)
{
}
}