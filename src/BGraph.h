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
			static const char* s_nameTable[]{
				s_unknown,
				s_triangle_list,
				s_triangle_strip
			};
			if (topo >= sizeof(s_nameTable)) {
				os << s_unknown;
			}
			else {
				os << s_nameTable[topo];
			}
			return os;
		}
    };

    template<typename T>
    struct Point2
    {
        T x;
        T y;

		T* data() { return (T*)this; }
		
		friend ostream& operator <<(ostream& os, const Point2<T>& pt) 
		{
			os << "(" << pt.x << ", " << pt.y << ")";
			return os;
		}
    };

    template<typename T>
    struct Point3 : Point2<T>
    {
        T z;

		T* data() { return (T*)this; }

		friend ostream& operator <<(ostream& os, const Point3<T>& pt)
		{
			os << "(" << pt.x << ", " << pt.y <<", "<< pt.z << ")";
			return os;
		}

    };

	template<typename T>
	struct Point4 : Point3<T>
	{
		T w;

		T* data() { return (T*)this; }

		friend ostream& operator <<(ostream& os, const Point4<T>& pt)
		{
			os << "(" << pt.x << ", " << pt.y << ", " << pt.z <<", "<<pt.w<<", "<< ")";
			return os;
		}

	};

#define ASSIGN2D(p,x,y) *p++ = x; *p++ = y
#define GAP_ASSIGN2D(p,x,y,gap) *p++ = x; *p++ = y; p = (decltype(p))((uint8_t*)p + gap)

    template<typename T> 
    struct Rectangle
    {
        T width;
        T height;

		T* data() { return (T*)this; }

        void getVertices(T* vertices, Topology::Topology topo) const
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
        void getVertices(T* vertices) const;

        template<Topology::Topology t_topo =  Topology::TOPO_TRIANGLE_STRIP>
        void getVertices(T* vertices, unsigned int gapBytes) const;

		template<typename T_Attrs, typename T_MemVerticeAddr, Topology::Topology t_topo = Topology::TOPO_TRIANGLE_STRIP>
		void getVertices(T_Attrs* attrs, T_MemVerticeAddr T_Attrs::* vertice) const
		{
			auto gap = sizeof(T_Attrs) - sizeof(T) * 2;
			getVertices<t_topo>((T*) &(attrs->*vertice), (unsigned int)gap);
		}

		template<Topology::Topology t_topo = Topology::TOPO_TRIANGLE_STRIP>
		void getVertices(Point2<T>* vertices) const
		{
			getVertices<t_topo>((T*) vertices);
		}

        template<> 
        void getVertices<Topology::TOPO_TRIANGLE_STRIP>(T* vertices) const
        {
            T halfWidth = width / 2, halfHeight = height / 2;
			T l = - halfWidth, r =  halfWidth, t = halfHeight, b = - halfHeight;

            #define A(x,y) ASSIGN2D(vertices,x,y)
            A(l,t);A(r,t);A(l,b);A(r,b);
            #undef A
        }

        template<>
        void getVertices<Topology::TOPO_TRIANGLE_STRIP>(T* vertices, unsigned int gapBytes) const
        {
			T halfWidth = width / 2, halfHeight = height / 2;
			T l = -halfWidth, r = halfWidth, t = halfHeight, b = -halfHeight;

            #define A(x,y) GAP_ASSIGN2D(vertices,x,y,gapBytes)
            A(l,t);A(r,t);A(l,b);A(r,b);
            #undef A
        }

		friend ostream& operator <<(ostream& os, const Rectangle& rect) 
		{
			os << "width: " << rect.width << ", height: " << rect.height;
			return os;
		}
    };

	using Point2i = Point2<int>;
	using Point2f = Point2<float>;
	using Point2d = Point2<double>;
	using Point3i = Point3<int>;
	using Point3f = Point3<float>;
	using Point3d = Point3<double>;



#undef GAP_ASSIGN2D
#undef ASSIGN2D

#undef istream
#undef ostream
}