    #include <iostream>
    #include <math.h>
    #include <vector>
    #include <fstream>
    #include <set>
    #include <map>
    using namespace std;

    /** @file */

    class Edge;
    class Face;
    /**
     * @brief  A 2d representation of a point - has x, y coordinates and outgoing edge
     * 
     */
    class Vertex
    {
    public:
        double x, y;
        Edge * outgoingEdge; // outgoing edges
        Vertex(double a, double b)
        {
            x = a;
            y = b;
        }
    };
    /**
     * @brief represents a edge in DCEL - has an origin vertex, twin, nextEdge and previousEdge
     * 
     */
    class Edge
    {
     
    public:
        Vertex *origin; // e.destination = e.twin.origin
        Edge *twin;
        Face *left;
        vector<Edge *> nextEdges, previousEdges; // in counter clockwise wrt to face
    };
     
    /**
     * @brief represents a side or a closed polygon in DCEL, has the edge
     * 
     */
    class Face
    {
     
    public:
        Edge *incidet; // anti-clock wise
    };
     

    /**
    * @brief implementation of Doubly Connected Edge List (DCEL) as a data structure to store the polygon decomposition
    * 
    */ 
    class DCEL
    {
    public:
        vector<Vertex *> v; // vertices
     
        vector<Edge *> e; // edges
     
        vector<Face *> f; // faces
     
        /**
             * @brief adds vertex to the DCEL vertex list
             * 
             * @param x x co-ordinate of the vertex
             * @param y y co-ordinate of the vertex
        */
        void addVertex(double x, double y);
        /**
             * @brief adds an edge to the DCEL edge list 
             * 
             * @param x1 - The x coordinate of the first point
             * @param y1 - The y coordinate of the first point
             * @param x2 - The x coordinate of the second point
             * @param y2 - The y coordinate of the second point
        */
        void addEdge(double x1, double y1, double x2, double y2);
        void addFace();
        
    };
     
    void DCEL ::addVertex(double x, double y)
    {
        Vertex *vnew = new Vertex(x, y);
        v.push_back(vnew);
    }
    void DCEL :: addFace(){
        Face * x = new Face();
        x->incidet = e[e.size()-1];
        f.push_back(x);
    }
    void DCEL::addEdge(double x1, double y1, double x2, double y2)
    {
        Edge *e1 = new Edge(); 
        Edge *e2 = new Edge();
     
        e.push_back(e1);
        e.push_back(e2); 
     
        Vertex *v1, *v2;
     
        for (int i = 0; i < v.size(); i++)
        {
            if (v[i]->x == x1 && v[i]->y == y1)
            {
                v1 = v[i];
            }
            if (v[i]->x == x2 && v[i]->y == y2)
            {
                v2 = v[i];
            }
        }
     
        e1->twin = e2;
        e2->twin = e1;
     
        e1->origin = v1;
        e2->origin = v2;
     
        //e1->nextEdges = v2->outgoingEdges;
        //e2->nextEdges = v1->outgoingEdges;
     
        //e1->previousEdges = v1->outgoingEdges;
     
        // cout << (int )(e1->previousEdges.size())-1 <<"  "<< i<<endl;
        for (int i = 0; i < (int)(e1->previousEdges.size()) - 1; i++)
        {
            e1->previousEdges[i] = e1->previousEdges[i]->twin;
        }
     
        //e2->previousEdges = v2->outgoingEdges;
        for (int i = 0; i < (int)(e2->previousEdges.size()) - 1; i++)
        {
            e2->previousEdges[i] = e2->previousEdges[i]->twin;
        }
     
        // adding outgoing edges at the end so that next edges and previous edges won't get affected
        //v1->outgoingEdges.push_back(e1);
        //v2->outgoingEdges.push_back(e2);
    }

     /**
     * @brief has x, y coordinates - representation of a point
     * 
     */ 
    class Point{
        public:
            double x, y;
    };