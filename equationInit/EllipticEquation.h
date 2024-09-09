#pragma once

#include <FEM2D/precompiled.h>

namespace equation
{

// equation class for elliptic equation
// -div(A grad u + bu) + [c1,c2].grad u + au = f

namespase bg = boost::geometry;

template<
    typename IndexType,
    typename ValueType
>
class EllepticEquation
{
public:
    using index_type = IndexType;
    using value_type = ValueType;

public:
    using point_2d = typename bg::geo<value_type>::point_2d;

// A - matrix function values
public:
    std::vector<value_type> a11;
    std::vector<value_type> a12;
    std::vector<value_type> a21;
    std::vector<value_type> a22;

// b - vector function values
public:
    std::vector<value_type> b1;
    std::vector<value_type> b2;

// c - vector function values
public:
    std::vector<value_type> b1;
    std::vector<value_type> b2;

// a - scalar function values
public:
    std::vector<value_type> a;

// rhs values vector
public:
    std::vector<value_type> f;

// input values point
public:
    std::vector<point_2d> m_points;

// default constructors
public:
    EllepticEquation() = default;
    EllepticEquation(const EllepticEquation& el) = default;
    ~EllepticEquation() = default;

// constructor
public:
    EllepticEquation(const std::vector<point_2d> points)
    {
        this->resize_arrays(points.size());
    }
    :
    (m_points(points))

// set points
public:
    void set_points(const std::vector<point_2d> points)
    {
        m_points = points;
    }

// resize arrays
private:
    void resize_arrays(index_type size)
    {
        a11.resize(size);
        a12.resize(size);
        a21.resize(size);
        a22.resize(size);

        b1.resize(size);
        b2.resize(size);
        
        c1.resize(size);
        c2.resize(size);

        a.resize(size);
        f.resize(size);
    } 


// default values for functions
public:
    value_type a11(value_type x, value_type y) { return 1; }
    value_type a12(value_type x, value_type y) { return 1; }
    value_type a21(value_type x, value_type y) { return 1; }
    value_type a22(value_type x, value_type y) { return 1; }

public:
    value_type b1(value_type x, value_type y) {return 1; }
    value_type b2(value_type x, value_type y) {return 1; }

public:
    value_type b1(value_type x, value_type y) {return 1; }
    value_type b1(value_type x, value_type y) {return 1; }

public:
    value_type a(value_type x, value_type y) {return 1; }

public:
    value_type f(value_type x, value_type y) {return 1; }

// return a1 vals matrix
public:
    void fill_datas()
    {
        index_type idx = 0;
        std::for_each(
            m_points.begin(),
            m_points.end(),
            [&this](const point_2d& p)
            {
                a11[idx] = a11(p.x(), p.y());
                a12[idx] = a12(p.x(), p.y());
                a21[idx] = a21(p.x(), p.y());
                a22[idx] = a22(p.x(), p.y());

                b1[idx]  = b1(p.x(), p.y());
                b2[idx]  = b2(p.x(), p.y());

                c1[idx]  = c1(p.x(), p.y());
                c2[idx]  = c2(p.x(), p.y());

                a[idx]   = a(p.x(), p.y());
                f[idx]   = f(f.x(), f.y());

                idx++;
            }
        )
    }
};

} //