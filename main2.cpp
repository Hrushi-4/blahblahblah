#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <deque>
#include <limits>
#include <unordered_map>
#include "dcel.hpp"

using namespace std;

/** @file */

/**
 * @brief This function checks if a point is on the right side or left side or on the given line
 *
 * To determine which side the point lies we calculate its cross product value.
 * If the cross product is +ve means it lies on the right side.
 * If the cross product is -ve means it lies on the left side.
 * And if cross product is 0 then it lies on the line.
 *
 * @param A start co-ordinate of the line
 * @param B end co-ordinate of the line
 * @param P the point to be checked
 *
 * @return returns 1 if the point lies on the right side of the line
 * @return returns -1 if the point lies on the left side of the line
 * @return returns 0 if the point lies on the line
 */

int side(Vertex *A, Vertex *B, Vertex *P)
{
    const int RIGHT = 1, LEFT = -1, ZERO = 0;
    // subtracting co-ordinates of Vertex A from
    // B and P, to make A as origin
    int ax = A->x, bx = B->x, px = P->x, ay = A->y, by = B->y, py = P->y;
    bx -= ax;
    by -= ay;
    px -= ax;
    py -= ay;

    // Determining cross Product
    double cross_product = bx * py - by * px;

    // return RIGHT if cross product is positive
    if (cross_product > 0)
        return LEFT;

    // return LEFT if cross product is negative
    if (cross_product < 0)
        return RIGHT;

    // return ZERO if cross product is zero.
    return ZERO;
}

/**
 * @brief This function checks whether the given point lies inside the given polygon or not
 *
 * We use a for loop and for each side of the polygon we call #side2 function and check the given point.
 * If the point lies on the right side of all the sides then it lies inside the polygon
 * Else it lies outside or on the polygon
 *
 * @param p the point to be checked
 * @param polygon the given polygon
 *
 * @return true if the point p lies inside the given polygon
 * @return false if the point p lies outside or on the given polygon
 */
bool isInside(Vertex *p, vector<Vertex *> polygon)
{
    int n = polygon.size();
    for (int i = 0; i < n - 1; i++)
    {
        if (side(polygon[i], polygon[i + 1], p) == -1 || side(polygon[i], polygon[i + 1], p) == 0)
        {
            return false;
        }
    }
    if (side(polygon[n - 1], polygon[0], p) == -1 || side(polygon[n - 1], polygon[0], p) == 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief Checks if the angle between two line segments is acute or not
 *
 * Checks if the angle at point p2 formed by the line segments p1,p2 and p2,p3 is acute or not.
 *
 * @param p1 start co-ordinate of 1st line segment
 * @param p2 the  point at which angle is to be checked
 * @param p3 start co-ordinate of 2nd line segment
 *
 * @return true if the angle is acute
 * @return false otherwise
 */
bool isAcute(Vertex *p1, Vertex *p2, Vertex *p3)
{

    double x1 = p1->x;
    double y1 = p1->y;

    double x2 = p2->x;
    double y2 = p2->y;

    double x3 = p3->x;
    double y3 = p3->y;

    if (y1 == y2 && y2 == y3)
        return 1;
    if (x1 == x2 && x2 == x3)
        return 1;

    x1 = x1 - x2;
    y1 = y1 - y2;
    x3 = x3 - x2;
    y3 = y3 - y2;
    double dot = x1 * x3 + y1 * y3;
    double det = x1 * y3 - y1 * x3;

    double result = atan2(det, dot);
    // cout<<result<<endl;
    double res = ((result < 0) ? (result * 180 / 3.141592) + 360
                               : (result * 180 / 3.141592));

    // cout<<cos(result)<<" ";

    if (cos(result) == -1)
        return 1;

    if (res > 180)
        return 0;
    else
        return 1;
}

/**
 * @brief checks if any vertex in the given polygon is a notch
 *
 * We traverse all the sides of the given polygon,
 * for all two adjacent sides we call #isAcute2.
 * If it returns false then the given point is a notch
 * so we add it to the list of notches.
 *
 * @param inp the polygon to be checked
 *
 * @return List of all the vertices which are notch
 */
vector<Vertex *> notches(vector<Vertex *> inp)
{
    vector<Vertex *> notches;
    for (int i = 2; i < inp.size(); i++)
    {
        if (!isAcute(inp[i - 2], inp[i - 1], inp[i]))
            notches.push_back(inp[i - 1]);
    }

    if (!isAcute(inp[inp.size() - 2], inp[inp.size() - 1], inp[0]))
        notches.push_back(inp[inp.size() - 1]);
    if (!isAcute(inp[inp.size() - 1], inp[0], inp[1]))
        notches.push_back(inp[0]);
    return notches;
}

/**
 * @brief given an input polygon it returns a list of all the vertices which were a notch in the orignal polygon.
 *
 * @param inp input polygon whose vertices are to be checked
 * @param poly the original polygon
 *
 * @return List of vertices
 */
vector<Vertex *> checkNotch(vector<Vertex *> inp, vector<Vertex *> poly)
{
    vector<Vertex *> notch = notches(poly);
    vector<Vertex *> res;
    for (int i = 0; i < inp.size(); i++)
    {
        for (int j = 0; j < notch.size(); j++)
        {
            if (inp[i] == notch[j])
            {
                res.push_back(inp[i]);
                break;
            }
        }
    }
    return res;
}

/**
 * @brief checks if the given vertex is a notch in the given polygon
 *
 * For the given polygon we call the #notches2 function
 * which returns list of all the notches in the polygon
 * if the given vertex is part of that list we return 1 else 0
 *
 * @param p the point to be checked
 * @param poly the given polygon
 *
 * @return 1 if the given point is a notch
 * @return 0 otherwise
 */
int checkVertexNotch(Vertex *p, vector<Vertex *> poly)
{
    vector<Vertex *> notch = notches(poly);
    for (int j = 0; j < notch.size(); j++)
    {
        if (p == notch[j])
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief given a point and a polygon it returns the next point in the polygon in clockwise order
 *
 * @param v given point whose next point is to be found
 * @param poly the given polygon
 *
 * @return The next point
 */
Vertex *Next(Vertex *v, vector<Vertex *> poly)
{
    cout << "in next" << endl;
    int ind = -1;
    for (int i = 0; i < poly.size(); i++)
    {
        if (poly[i] == v)
        {
            ind = i;
            break;
        }
    }
    if (ind == poly.size() - 1)
    {
        return poly[0];
    }

    return poly[ind + 1];
}

/**
 * @brief Removes the vertices on one side of the line segment p and p1 of the polygon Lf
 *
 * @param p Point1 of the segment
 * @param L The polygon whose vertices are to be removed
 * @param p1 Point2 of the segment
 * @param Lf The polygon whose vertices are to be removed(same polygon L)
 */
void remove_side(Vertex *p, vector<Vertex *> L, Vertex *p1, vector<Vertex *> &Lf)
{
    int s = side(p, p1, L[L.size() - 1]);
    for (int i = 0; i < L.size(); i++)
    {
        if (side(p, p1, L[i]) == s && Lf.size() > i)
            Lf.erase(Lf.begin() + i);
    }
}

/**
 * @brief Gives us a rectangle that encloses the given polygon
 *
 * @param p the given polygon which is to be enclosed
 *
 * @return vector<double> (Points of the rectangle)
 */
vector<double> rectangle(vector<Vertex *> p)
{
    vector<double> a(4);
    double maxx = p[0]->x, minx = p[0]->x, maxy = p[0]->y, miny = p[0]->y;
    for (int i = 0; i < p.size(); i++)
    {

        maxx = max(maxx, p[i]->x);
        maxy = max(maxy, p[i]->y);

        minx = min(minx, p[i]->x);
        miny = min(miny, p[i]->y);
    }
    a[0] = maxx;
    a[1] = minx;
    a[2] = maxy;
    a[3] = miny;
    return a;
}

/**
 * @brief Checks if a given point is inside the rectangle or not
 *
 * @param p Given vertex
 * @param r given rectangle
 * @return true
 * @return false
 */
bool inSideRectangle(Vertex *p, vector<double> r)
{

    double x = p->x;
    double y = p->y;
    if (x <= r[0] && x >= r[1] && y <= r[2] && y >= r[3])
        return true;

    return false;
}

/**
 * @brief The algorithm for decomposition of the given polygon into convex polygons
 *
 * @param poly The original polygon
 * @param ans List of all the polygons after the partition process
 */
void fun(DCEL poly, vector<vector<Vertex *> > &ans)
{
    vector<Vertex *> nots = notches(poly.v);

    int s = poly.v.size();

    Vertex *first = poly.v[0];

    if (nots.size() == 0)
    {
        ans.push_back(poly.v);
        return;
    }

    int maxi = 0;

    vector<Vertex *> curr;
    vector<Vertex *> next;

    vector<vector<Vertex *> > L(3000);
    int m = 1;
    L[0].push_back(poly.v[0]);
    int count = 0;
    // L.push_back(poly[1]);
    while (poly.v.size() > 3)
    {

        if (m != 1)
        {
            if (poly.v.size() == s)
            {
                count++;
            }
            else
                count = 1;
            if (count == s)
            {
                ans.push_back(poly.v);
                return;
            }
        }
        s = poly.v.size();

        // can change initialisation
        vector<Vertex *> temp;
        // temp[0] = new Vertex();
        temp.push_back(poly.v[0]);
        temp.push_back(L[m - 1][L[m - 1].size() - 1]);
        temp.push_back(Next(temp[1], poly.v));
        L[m].push_back(temp[1]);
        L[m].push_back(temp[2]);
        int i = 2;
        temp.push_back(Next(temp[i], poly.v));

        while (isAcute(temp[i - 1], temp[i], temp[i + 1]) && isAcute(temp[i], temp[i + 1], temp[1]) && isAcute(temp[i + 1], temp[1], temp[2]) && L[m].size() < poly.v.size())
        {
            L[m].push_back(temp[i + 1]);
            i++;
            temp.push_back(Next(temp[i], poly.v));
            // seg fault possible, no pushes to temp[i];
        }
        bool t = isAcute(temp[i + 1], temp[1], temp[2]);

        // 3.4

        if (L[m].size() != poly.v.size())
        {

            // 3.4.1
            vector<Vertex *> cover; // P - L[m]
            for (auto i : poly.v)
                cover.push_back(i);
            for (int i = 0; i < L[m].size(); i++)
            {
                int s = cover.size();
                for (int j = 0; j < s; j++)
                {
                    if (cover[j] == L[m][i])
                        cover.erase(cover.begin() + j);
                }
            }
            vector<Vertex *> notch = checkNotch(cover, poly.v);

            // 3.4.2
            while (notch.size() > 0)
            {

                bool backward = false;
                vector<double> rect = rectangle(L[m]);
                Vertex *tem;
                while (!backward && notch.size() > 0)
                {
                    tem = notch[0];
                    if (inSideRectangle(tem, rect) == false && notch.size() > 0)
                        notch.erase(notch.begin());
                    else
                        break;
                }

                if (notch.size() > 0)
                {
                    if (isInside(tem, L[m]) == true && notch.size() > 0)
                    {
                        remove_side(tem, L[m], temp[1], L[m]);
                        backward = true;
                        if (notch.size() > 0)
                            notch.erase(notch.begin());
                    }
                    else if (notch.size() > 0)
                        notch.erase(notch.begin());
                }
            }
        }

        else
        {
            ans.push_back(L[m]);
            cout << "Equal\n";
            return;
        }

        // 3.5

        if (L[m][L[m].size() - 1] != temp[2])
        {
            ans.push_back(L[m]);

            for (int i = 1; i < L[m].size() - 1; i++)
            {
                int s = poly.v.size();
                for (int j = 0; j < s; j++)
                {
                    if (L[m][i] == poly.v[j])
                        poly.v.erase(poly.v.begin() + j);
                }
            }
        }
        else
        {
            Vertex *temp = poly.v[0];
            if (poly.v.size() > 0)
                poly.v.erase(poly.v.begin());
            poly.v.push_back(temp);
        }
        int ti = L[m].size();
        if (ti >= maxi)
            maxi = t;
        cout << "The maximum size is" << maxi << "\n";
        m += 1;
    }
}

/**
 * @brief given a point and a polygon it returns the previous point in the polygon in clockwise order
 *
 * @param v given point whose previous point is to be found
 * @param poly the given polygon
 *
 * @return The prev point
 */

Vertex *Prev(Vertex *v, vector<Vertex *> poly)
{
    int ind = -1;
    for (int i = 0; i < poly.size(); i++)
    {
        if (poly[i] == v)
        {
            ind = i;
            break;
        }
    }
    if (ind == 0)
        return poly[poly.size() - 1];
    return poly[ind - 1];
}

/**
 * @brief Every diagonal of the partition is checked whether it can be removed
 *
 * A diagonal d is said to be essential if removal of d creates a piece that is non convex.
 * The above partition process may sometimes produce partitions that contain inessential diagonals.
 * To prevent this after the partition process we call this merging funcion
 * which checks everyone of the diagonal in order whether it can be removed.
 *
 * @param ans List of all the polygons after the partition process
 * @param poly The original polygon
 */

void merge(vector<vector<Vertex*> > &ans, DCEL poly){
     
            
                vector<pair<Vertex *, Vertex *> > LLE, temp; 
                for(int i=0; i<ans.size(); i++){
                    for(int j=0; j<ans[i].size(); j++){
                        if(Next(ans[i][j], ans[i]) != Next(ans[i][j], poly.v)){
                            temp.push_back(make_pair(ans[i][j], Next(ans[i][j], ans[i]))); 
                        }
                    }
                }
     
     
                for(int i=0; i<temp.size(); i++){
                    if(LLE.size() == 0) {LLE.push_back(temp[i]);}
                    else {
                        bool found = false; 
                        for(int j=0; j<LLE.size(); j++){
                            if(temp[i].second == LLE[j].first && temp[i].first == LLE[j].second) {found = true; break;  }
                        }
                        if(found) continue; 
                        else {
                            LLE.push_back(temp[i]); 
                        }
                    }
                }
     
                for(int i=0; i<LLE.size(); i++){
                    int i1 = -1, i2 = -1;
                    int ss = ans.size(); 
                    for(int j=0; j<ss; j++){
                        for(int k=0; k<ans[j].size(); k++){
                            if(((ans[j][k] == LLE[i].first && Next(ans[j][k], ans[j]) == LLE[i].second) )|| ((ans[j][k] == LLE[i].second && Next(ans[j][k], ans[j]) == LLE[i].first))){
                                if(i1 == -1) {i1 = j; }
                                else{
                                    if(j == i1){break; }
                                    else {
                                        i2 = j; 
                                    }
                                    
                                }
                            }
                        }
                    }



                     unordered_map<Vertex*, int> m; 
                   for(int l=0; l<ans[i1].size(); l++){
                        m[ans[i1][l]]++;
                   }

                   for(int l=0; l<ans[i2].size(); l++){
                        m[ans[i2][l]]++;
                   }

                   vector<Vertex*> inter;

                   for(int i = 0; i < poly.v.size(); i++){
                        if(m[poly.v[i]] != 0)
                            inter.push_back(poly.v[i]);
                   }

                   vector<Vertex*> ac = notches(inter);

                   if(ac.size() == 0) {
                        if(i1>i2) {
                            ans.erase(ans.begin() + i1); 
                            ans.erase(ans.begin() + i2); 
                            ans.push_back(inter); 
                        }
                        else {
                             ans.erase(ans.begin() + i2); 
                            ans.erase(ans.begin() + i1); 
                            ans.push_back(inter); 
                        }
                    }


     
                    
                    
                  


     
                }
        
     
     
     
                ofstream myfile;
                myfile.open("Points.txt");
                for(int i = 0; i < ans.size(); i++){
                    cout<<"vector aft merge"<<endl;
                    for(int j = 0; j < ans[i].size(); j++){
                        cout<<ans[i][j]->x<<" "<<ans[i][j]->y<<endl;
                    }
                }
             
                for(int i = 0; i < ans.size(); i++){
                    for(int j = 0; j < ans[i].size(); j++){
                        myfile<<ans[i][j]->x<<" "<<ans[i][j]->y<<" "<<ans[i][(j+1)%ans[i].size()]->x<<" "<<ans[i][(j+1)%ans[i].size()]->y<<endl;
                    }
                }
                myfile.close();
             
            }
     
     


int main()
{

    fstream file("inp.txt");

    int n;
    file >> n;
    DCEL poly;
    vector<Vertex *> inp;
    for (int i = 0; i < n; i++)
    {
        double x, y;
        file >> x >> y;
        // cout<<x<<" "<<y<<endl;
        // poly->addVertex
        Vertex *p = new Vertex(x, y);
        // p->x = x;
        // p->y = y;
        poly.addVertex(x, y);
        inp.push_back(p);
    }

    for (int i = 1; i < n; i++)
    {
        poly.addEdge(poly.v[i - 1]->x, poly.v[i - 1]->y, poly.v[i]->x, poly.v[i]->y);
    }
    poly.addEdge(poly.v[n - 1]->x, poly.v[n - 1]->y, poly.v[0]->x, poly.v[0]->y);

    file.close();
    vector<vector<Vertex *> > ans;

    fun(poly, ans);
    ofstream myfile;
    myfile.open("Vertexs.txt");
    for (int i = 0; i < ans.size(); i++)
    {
        cout << "vector" << endl;
        for (int j = 0; j < ans[i].size(); j++)
        {
            cout << ans[i][j]->x << " " << ans[i][j]->y << endl;
        }
    }

    for (int i = 0; i < ans.size(); i++)
    {
        for (int j = 0; j < ans[i].size(); j++)
        {
            myfile << ans[i][j]->x << " " << ans[i][j]->y << " " << ans[i][(j + 1) % ans[i].size()]->x << " " << ans[i][(j + 1) % ans[i].size()]->y << endl;
        }
    }

    // set<pair<Vertex*, Vertex*>> added;

     merge(ans, poly);
    return 0;
}
