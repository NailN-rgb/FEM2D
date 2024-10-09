
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
#include <FEM2D/mesh/boost_datatypes/types.h>



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

namespace bg = boost::geometry;
  
class TRIANGLEMESH_DLL_API TriangleMesh
{
public:
	using point_2d = typename bg::geo<double>::point_2d;
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

public:
	std::vector<point_2d> get_points_list(const triangulateio &io)
	{
		std::vector<point_2d> points_list;

		for(auto pt = io.pointlist; pt != io.pointlist + 2 * io.numberofpoints; pt+=2)
		{
			points_list.push_back(point_2d(*pt, *(pt + 1)));
		}

		return points_list;
	}


public:
	std::vector<int> get_boundary_attributes(const triangulateio &io)
	{
		std::vector<int> result;

		for(auto pt_m = io.pointmarkerlist; pt_m != io.pointmarkerlist + io.numberofpoints; pt_m++)
		{
			result.push_back(*pt_m);
		}

		return result;
	}

public:
	std::vector<std::vector<int>> get_segments_list(const triangulateio &io)
	{
		std::vector<std::vector<int>> segments_list;

		for(auto pt = io.edgelist; pt != io.edgelist + 2 * io.numberofedges; pt+=2)
		{
			std::vector<int> segment;
			segment.push_back(*pt);
			segment.push_back(*(pt + 1));

			segments_list.push_back(segment);
		}

		return segments_list;
	}

public:
	std::vector<std::vector<int>> get_triangle_list(const triangulateio &io)
	{
		std::vector<std::vector<int>> tri_list;

		for(auto pt = io.trianglelist; pt < io.trianglelist + io.numberofcorners * io.numberoftriangles; pt+=3)
		{
			std::vector<int> tri;
			tri.push_back(*pt);
			tri.push_back(*(pt + 1));
			tri.push_back(*(pt + 2));

			tri_list.push_back(tri);
		}

		return tri_list;
	}
    
};//class TriangleMesh 


} //namespace trianglemesh

} //namespace mesh

} //namespace FEM2D

#include<FEM2D/mesh/trianglemesh/detail/trianglemesh.inl>

#endif // __TRIANGLEMESH__
