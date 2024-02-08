#include"SampleBase.hpp"
#include<vector>
#include<fstream>
#include<ostream>
#include<string>
#include<iostream>

namespace Diligent
{
	class Mesh final :public SampleBase
	{
    public:
       
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName() const override final
        {
            return "XXX::MESH";
        }

    private:
        const std::vector<float> Positions;
        const std::vector<Uint32> Indices;
        static Uint32 NV;
        static Uint32 NI;
        

       
            

        

	};
    class MeshLoader
    {
    private:
        std::vector<float> positions;
        std::vector<unsigned long>       faces;
        std::vector<unsigned long>       indices;
        std::vector<std::vector<unsigned long>> polygons;
        
        unsigned long NV=0;
        unsigned long NE = 0;
        unsigned long NF = 0;
        unsigned long NI = 0;

    public:
        MeshLoader(const char* Path) :
            NV(0), NE(0), NI(0), NF(0)
        {
            if (readMeshData(Path))
                std::cout << "MESH LOAD";
            else
                std::cout << "...error reading mesh";

        }
        bool read_next_line(std::fstream &lstream, std::string &lstring)
        {
            while (getline(lstream, lstring))
                if (lstring.length() > 1)
                    return true;
            return false;
        }
        bool readMeshData(const char* Path)
        {
            std::fstream lstrm;
            lstrm.open(Path);
            if (!lstrm.is_open())
                return false;
            std::string lstr;
            if (!read_next_line(lstrm, lstr))
                return false;
            std::stringstream info_stream(lstr);
            info_stream >> NV >> NF >> NE;
            positions.reserve(NV);    
            for(auto i = 0; i < NV; ++i)
            {
                if (!read_next_line(lstrm, lstr))
                    return false;
                float point;
                std::stringstream point_stream(lstr);
                point_stream >> point;
                positions.emplace_back(point);
            }
            polygons.reserve(NF);
            faces.reserve(NF);
            for (auto i=0;i<NF;++i)
            {
                if (!read_next_line(lstrm, lstr))
                    return false;
                std::stringstream face_info(lstr);
                Uint32 index_value;
                Uint32 face_verts;
                face_info >> face_verts;
                polygons.emplace_back(face_verts);
                faces.emplace_back();
                NI += face_verts;
                for (auto j = 0; j < face_verts; ++j)
                {
                    face_info >> index_value;
                    polygons[i][j] = index_value;
                }
            }
            lstrm.close();
            return true;
        }

    };
}

