#include"MeshShader.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new MeshShader();
}
bool MeshShader::read_next_line(std::ifstream &lstrm,std::string &lstr)
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

    std::ifstream lstrm(FilePath);
    if (lstrm.is_open())
        return false;  
    return true;
}
void MeshShader::Initialize(const SampleInitInfo& InitInfo)
{   
    VERIFY_EXPR(!LoadOFF("PS.psh"));

}
void MeshShader::Render()
{
}
void MeshShader::Update(double ctime, double etime)
{
}
}