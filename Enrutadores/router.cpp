 #include "router.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

void router::SetNombre(string _nombre){
    nombre=_nombre;
}
void router::SetConexiones(map<string,int>conex){
    conexion=conex;
}
void router::SetCamino(map<string,vector<string>>cam){
    camino=cam;
}
string router::GetNombre(){
    return nombre;
}
map <string, int>router::GetConexiones(){
    return conexion;
}

map <string, vector<string>>router::GetCamino(){
    return camino;
}
