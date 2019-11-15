#pragma once
#include <cstdint>
#include <iostream>
#include <array>

namespace bgraph
{
#define ostream std::ostream
#define istream std::istream
    template<typename T, typename U> 
    constexpr size_t offsetOf(U T::*member)
    {
        return (char*)&((T*)nullptr->*member) - (char*)nullptr;
    }

    namespace Topology
    {
        enum Topology
        {
            TOPO_UNKNOWN,
            TOPO_TRIANGLE_LIST,
            TOPO_TRIANGLE_STRIP,
        };

		ostream& operator << (ostream& os, Topology topo) 
		{
			static const char s_unknown[] = "unknown";
			static const char s_triangle_list[] = "triangle list";
			static const char s_triangle_strip[] = "triangle strip";
			using std::cout;
			switch (topo)
			{
			case bgraph::Topology::TOPO_UNKNOWN:
				cout << s_unknown;
				break;
			case bgraph::Topology::TOPO_TRIANGLE_LIST:
				cout << s_triangle_list;
				break;
			case bgraph::Topology::TOPO_TRIANGLE_STRIP:
				cout << s_triangle_strip;
				break;
			}
			return os;
		}
    };

    template<typename T>
    struct Point2D
    {
        T x;
        T y;
		
		friend ostream& operator <<(ostream& os, const Point2D<T>& pt) 
		{
			using std::cout;
			cout << "(" << pt.x << ", " << pt.y << ")";
			return os;
		}
    };

    template<typename T>
    struct Point3D : Point2D<T>
    {
        T z;
		friend ostream& operator <<(ostream& os, const Point3D<T>& pt)
		{
			using std::cout;
			cout << "(" << pt.x << ", " << pt.y <<", "<< pt.z << ")";
			return os;
		}
    };

    template<typename T>
    struct Graph2D
    {
        Point2D<T> pos;
    };

    template<typename T>
    struct Graph3D
    {
        Point2D<T> pos;
    };

#define ASSIGN2D(p,x,y) *p++ = x; *p++ = y
#define GAP_ASSIGN2D(p,x,y,gap) *p++ = x; *p++ = y; p = (decltype(p)*)((uint8_t*)p + gap)

    template<typename T> 
    struct Rectangle2D : Graph2D<T>
    {
        T width;
        T height;

        void getVertices(T* vertices, Topology::Topology topo)
        {
            switch(topo)
            {
                case Topology::TOPO_TRIANGLE_STRIP:
                getVertices<Topology::TOPO_TRAINGLE_STRIP>(vertices);
                return;
            }
            //error
        }

        template<Topology::Topology t_topo =  Topology::TOPO_TRIANGLE_STRIP>
        void getVertices(T* vertices);

        template<Topology::Topology t_topo =  Topology::TOPO_TRIANGLE_STRIP>
        void getVertices(T* vertices, unsigned int gapBytes);

		template<typename T_Attr, typename T_MemberAddr, Topology::Topology t_topo = Topology::TOPO_TRIANGLE_STRIP>
		void getVertices(T_Attr* attrs, T_MemberAddr T_Attr::* vertice) 
		{
			auto gap = sizeof(T_Attr) - sizeof(T) * 2;
			getVertices<t_topo>((T*) &(attrs->*vertice), gap);
		}

		template<Topology::Topology t_topo = Topology::TOPO_TRIANGLE_STRIP>
		void getVertices(Point2D<T>* vertices)
		{
			getVertices<t_topo>((T)*vertices);
		}

        template<> 
        void getVertices<Topology::TOPO_TRIANGLE_STRIP>(T* vertices)
        {
            T halfWidth = shape.width / 2, halfHeight = shape.height / 2;
			T ox = pos.x, oy = pos.y;
			T l = ox - halfWidth, r = ox + halfWidth, t = oy + halfHeight, b = oy - halfHeight;

            #define A(x,y) ASSIGN2D(vertices,x,y)
            A(l,t);A(r,t);A(l,b);A(r,b);
            #undef A
        }

        template<>
        void getVertices<Topology::TOPO_TRIANGLE_STRIP>(T* vertices, unsigned int gapBytes)
        {
            T halfWidth = shape.width / 2, halfHeight = shape.height / 2;
			T ox = pos.x, oy = pos.y;
			T l = ox - halfWidth, r = ox + halfWidth, t = oy + halfHeight, b = oy - halfHeight;

            #define A(x,y) GAP_ASSIGN2D(vertices,x,y,gapBytes)
            A(l,t);A(r,t);A(l,b);A(r,b);
            #undef A
        }
    };


	using Point2Di = Point2D<int>;
	using Point2Df = Point2D<float>;
	using Point2Dd = Point2D<double>;




#undef GAP_ASSIGN2D
#undef ASSIGN2D

#undef istream
#undef ostream
}