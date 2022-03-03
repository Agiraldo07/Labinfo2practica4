#ifndef RED_H
#define RED_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <router.h>

using namespace std;

class red
{
    private:
        router enrutadores;
        vector<router> tabla_EN;
    public:
        void SetEnrutador(router);
        void SetTabla_EN();
        void SetClearTabla(string);
        vector<router>getTabla_EN();
};

#endif // RED_H
