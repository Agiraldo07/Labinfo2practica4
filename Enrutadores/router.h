
#ifndef ROUTER_H
#define ROUTER_H
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class router
{
    private:
        string nombre;
        map <string, int> conexion;
        map <string, vector<string>> camino;

    public:
        void SetNombre(string);
        void SetConexiones(map<string,int>);
        void SetCamino(map<string,vector<string>>);
        string GetNombre();
        map<string,int>GetConexiones();
        map <string,vector<string>>GetCamino();
    };
#endif // ROUTER_H
