#include <cstdint>

namespace bgraph
{
    namespace Topology
    {
        enum Topology
        {
            TOPO_UNKNOWN,
            TOPO_TRIANGLE_LIST,
            TOPO_TRIANGLE_STRIP
        };
    };

    template<typename T>
    struct Point2D
    {
        T x;
        T y;
    };

    template<typename T>
    struct Point3D : Point2D<T>
    {
        T z;
    };

    template<typename T>
    struct Graph2D
    {
        Point2D pos;
    };

    template<typename T>
    struct Graph3D
    {
        Point2D pos;
    };
#define ASSIGN2D(p,x,y) *p++ = x; *p++ = y
#define GAP_ASSIGN2D(p,x,y,gap) *p++ = x; *p++ = y; p = (decltype(p)*)((uint8_t*)p + gap)

    template<typename T> 
    struct Rectangle2D : Graph2D
    {
        T width;
        T height;

        void getVertices(T* vertices, Topology topo = Topology::TOPO_TRIANGLE_STRIP)
        {
            switch(topo)
            {
                case Topology::TOPO_TRIANGLE_STRIP:
                getVertices<Topology::TOPO_TRAINGLE_STRIP>(vertices);
                return;
            }
            //error
        }
        
        template<typename T, T::*

        template<Topology::Topology t_topo =  Topology::TOPO_TRIANGLE_STRIP>
        void getVertices(T* vertices);

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

    };
#undef GAP_ASSIGN2D
#undef ASSIGN2D
}