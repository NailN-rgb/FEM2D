
#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#if defined(_MSC_VER)
//Microsoft
#if defined(TRIANGLEMESH_EXPORTS)
#define TRIANGLEMESH_DLL_API __declspec(dllexport) 
#else
#define TRIANGLEMESH_DLL_API __declspec(dllimport) 
#endif
#elif defined(__GNUC__)
//GCC
#if defined(TRIANGLEMESH_EXPORTS)
#define TRIANGLEMESH_DLL_API __attribute__((visibility("default")))
#else
#define TRIANGLEMESH_DLL_API
#endif
#else

#define TRIANGLEMESH_DLL_API
#pragma warning Unknown dynamic link import/export semantics.
#endif



#include <FEM2D/precompiled.h>


namespace FEM2D
{

namespace mesh
{

namespace trianglemesh
{

extern "C" {

#ifndef REAL
#define REAL double
#endif

//#define	ANSI_DECLARATORS
//#define VOID int

#include <FEM2D/mesh/trianglemesh/detail/triangle/triangle.h>

}
  
class TRIANGLEMESH_DLL_API TriangleMesh
{
protected:
	enum IO_Type 
	{
		TINPUT = 0,
		TOUTPUT = 1,
		TBOTH = 2
	};

public:
	triangulateio in_;
	triangulateio out_;
	triangulateio vorout_;
    
public:
	TriangleMesh()
	{
		init(in_);
		init(out_);
		init(vorout_);
	}

public:
	virtual ~TriangleMesh()
	{
		destroy(in_, TINPUT);
		destroy(out_, TOUTPUT);
		destroy(vorout_, TOUTPUT);
	}

public:
	bool read_poly(const std::string& filename);
	bool read_poly(std::stringstream& PolyFile);
	
public:
	bool build_mesh(
		const std::string& path,
		const std::string& delimiter,
		const std::string& polyfilename,
		const std::string& triswitches
	);

public:
	bool build_mesh(
		const std::string& switches,
		std::stringstream& poly_file
	);

public:
	bool refine_mesh(const std::string& triswitches);
	
protected:
    void init(triangulateio& t);
    void destroy(triangulateio& t, IO_Type);
    bool replace(triangulateio& in, triangulateio& out);

protected:    
    bool read_nodes(const std::string& filename);
    bool read_edges(const std::string& filename);
    bool read_ele(const std::string& filename); 
    
};//class TriangleMesh 


} //namespace trianglemesh

} //namespace mesh

} //namespace FEM2D

#include<FEM2D/mesh/trianglemesh/detail/trianglemesh.inl>

#endif // __TRIANGLEMESH__
