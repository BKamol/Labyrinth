#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Vertex
{
    int start, end, ring; //границы и кольцо
    Vertex(int _start=0, int _end=360, int _ring=-1) { start = _start; end = _end; ring = _ring; };
    Vertex(Vertex& other) { start = other.start, end = other.end, ring = other.ring; };
};
 
vector<string> customSplit(string str, char separator) 
{
    vector<string> strings;
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) 
    {
        if (str[i] == separator || i == str.size()) 
        {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
    return strings;
}

void labyrinth(string filename, map<int, vector<pair<int, int>> >& doors,  map<int, vector<int>>& walls)
{
    ifstream in(filename);
    string line;
    int number_of_circles;
    vector<string> string_vector;
    vector<string> coords_vector;
    pair<int, int> coords_pair;
    int wall;


    //number of circles
    getline(in, line);
    cout << line << endl;
    getline(in, line);
    number_of_circles = stoi(line);
    cout << number_of_circles << endl;

    //position of doors line
    getline(in, line);
    cout << line << endl;
    //reading doors
    for (int i = 0; i < number_of_circles; i++)
    {
        getline(in, line); //line about circle
        cout << line << endl;

        getline(in, line); //coordinates of doors
        //cout << line << endl;

        string_vector = customSplit(line, ' ');
        for (auto& door : string_vector)
        {
            coords_vector = customSplit(door, '-');
            coords_pair = { stoi(coords_vector[0]), stoi(coords_vector[1]) };
            cout << coords_pair.first << '-' << coords_pair.second << endl;
            doors[i].push_back(coords_pair); //adding door to map
        }
    }

    cout << endl;
    //Position of walls line
    getline(in, line);
    cout << line << endl;
    //reading walls
    for (int i = 0; i < number_of_circles - 1; i++)
    {
        getline(in, line); //line about ring
        cout << line << endl;

        getline(in, line); //coordinates of doors
        //cout << line << endl;

        string_vector = customSplit(line, ' ');
        for (auto& c : string_vector)
        {
            wall = stoi(c);
            cout << wall << ' ';
            walls[i].push_back(wall);
        }
        cout << endl;
    }

    in.close();
}

map<int, vector<pair<int, int>>> get_verticies(map<int, vector<int>>& walls)
{
    map<int, vector<pair<int, int>>> verts;
    pair<int, int> vert;
    for (auto& [ring, walls_vec] : walls)
    {
        for (int i = 0; i < walls_vec.size()-1; i++)
        {
            vert = { walls_vec[i], walls_vec[i + 1] };
            verts[ring].push_back(vert);
        }
        verts[ring].push_back({ walls_vec[walls_vec.size() - 1], walls_vec[0] });
    }
    verts[-1] = { { 0, 360 } }; //выход
    return verts;
}

pair<Vertex, Vertex> get_adj_verts(pair<int, int> door, map<int, vector<pair<int, int>>>& verts, int circle)
{
    int outer_ring = circle-1;
    int inner_ring = circle;
    Vertex v1;
    Vertex v2;
    for (auto& [start, end] : verts[outer_ring])
    {
        if (start <= door.first && door.second <= end)
            v1 = { start, end, outer_ring };
    }
    for (auto& [start, end] : verts[inner_ring])
    {
        if (start <= door.first && door.second <= end)
            v2 = { start, end, inner_ring };
    }
    return { v1, v2 };
}

//map<Vertex, vector<Vertex>> get_adj_lists(map<int, vector<pair<int, int>> >& doors, map<int, vector<pair<int, int>>>& verts)
//{
//    map<Vertex, vector<Vertex>> adj_lists;
//    for (auto& [circle, doors_vec] : doors)
//    {
//        for (auto& door : doors_vec)
//        {
//
//        }
//    }
//    return;
//}

int main()
{
    map<int, vector<pair<int, int>> > doors;        //у двери есть границы и номер окружности
    map<int, vector<int> > walls;                   //у стен есть координата и номер кольца
    map<int, vector<pair<int, int>>> verts;         //вершина - комната между двумя стенами, а ребра - двери

    labyrinth("lab.txt", doors, walls);

    /*cout << endl;
    for (auto& [circle, door] : doors)
    {
        cout << circle << ':';
        for (auto& c : door)
            cout << c.first << '-' << c.second << ' ';
        cout << endl;
    }
    cout << endl;
    for (auto& [ring, walls_vec] : walls)
    {
        cout << ring << ':';
        for (auto wall : walls_vec)
            cout << wall << ' ';
        cout << endl;
    }*/
    
    verts = get_verticies(walls);

    /*cout << endl;
    for (auto& [ring, walls_vec] : verts)
    {
        cout << ring << ':';
        for (auto& c : walls_vec)
            cout << c.first << '-' << c.second << ' ';
        cout << endl;
    }*/

    cout << endl;
    pair<Vertex, Vertex> adj = get_adj_verts({ 128, 133 }, verts, 1);
    cout << adj.first.start << ' ' << adj.first.end << ' ' << adj.first.ring << endl;
    cout << adj.second.start << ' ' << adj.second.end << ' ' << adj.second.ring << endl;
}
