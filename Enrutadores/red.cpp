#include "red.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <router.h>

using namespace std;

void red::SetEnrutador(router _enrutadores){
    enrutadores=_enrutadores;
}
void red::SetTabla_EN(){
    tabla_EN.push_back(enrutadores);
}
void red::SetClearTabla(string nombre){
    for(unsigned int i=0;i<tabla_EN.size();i++){
        if(tabla_EN[i].GetNombre()==nombre)tabla_EN.erase(tabla_EN.begin()+i);
    }
}
vector<router>red::getTabla_EN(){
    return tabla_EN;
}
