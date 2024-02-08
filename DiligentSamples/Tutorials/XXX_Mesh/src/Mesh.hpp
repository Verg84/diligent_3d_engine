#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>

struct MeshLoader
{
    std::vector<float> Positions;
    std::vector<std::vector<unsigned long>> Polygons;
    unsigned long NV=0;
    unsigned long NI=0;
    unsigned long NF=0;
    unsigned long NE=0;

    bool ReadNextLine(std::ifstream& lstream, std::string& lstring);
    bool LoadMesh(const std::string& Path);

};
bool MeshLoader::ReadNextLine(std::ifstream& lstream, std::string& lstring)
{
    while (getline(lstream, lstring))
    {
        if (lstring.length()>1)
            return true;

    }
    return false;
}

bool MeshLoader::LoadMesh(const std::string& Path)
{
    std::ifstream lstrm;
    lstrm.open(Path);
    if (!lstrm.is_open())
        return false;
    std::string lstr;
    if (!ReadNextLine(lstrm, lstr))
        return false;
    if (lstr != "OFF")
        return false;
    if (!ReadNextLine(lstrm, lstr))
        return false;
    std::stringstream info_stream(lstr);
    info_stream >> NV >> NF >> NE;
    
    Positions.reserve(NV);
    for (auto i = 0;i<NV;++i)
    {
        if (!ReadNextLine(lstrm, lstr))
            return false;
        std::stringstream point_stream(lstr);
        float PosX,PosY,PosZ;
        PosX = PosY = PosZ = 0;
        point_stream >> PosX >> PosY >> PosZ;
        Positions.emplace_back(PosX);
        Positions.emplace_back(PosY);
        Positions.emplace_back(PosZ);
    }

    Polygons.reserve(NF);
    for (auto i = 0; i < NF; ++i)
    {
        if (!ReadNextLine(lstrm,lstr))
            return false;
        std::stringstream face_stream(lstr);
        unsigned long face_verts;
        unsigned long index_value;
        face_stream >> face_verts;
        Polygons.emplace_back(face_verts);
        NI+=face_verts;
        for (auto j = 0; j < face_verts; ++j)
        {
            face_stream >> index_value;
            Polygons.emplace_back(index_value);
        }
    }
    lstrm.close();
}