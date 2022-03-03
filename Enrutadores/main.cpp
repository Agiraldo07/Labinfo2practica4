#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <router.h>
#include <red.h>

string direccion="../Enrutadores/red.txt";// variable global con la direccion del archivo

using namespace std;

red Primer_camino (red);
map <int, vector<string>> Camino_corto (string, string, vector<router>, vector<string>, int*, map<int, vector<string>>);//recorre los caminos
map <int, vector<string>> Calcular_camino (map<int, vector<string>>);// escoge el menor costo
bool No_conexion(map<int, vector<string>>);// determina si el router tiene conexiones

int main()
{
    int costo=0;
    string nombre,nombre_router,linea,temp="";
    map<string,int>tabla_temp;
    router enruta_temp;
    red topologia;
    ifstream archivo;
    archivo.open(direccion.c_str(),ios::in);// se abre un archivo en modo lectura
    if(!archivo.is_open()){// se verifica si el archivo abre exitosamente
        cout<<"Error abriendo el archivo"<<endl;
        exit(1);
    }
    while(!archivo.eof()){// lee mientras no sea el final del archivo
        getline(archivo,linea);
        unsigned int pos=linea.find(";");//toma la posicion entre el inicio y el punto y coma como el nombre del router
        nombre=linea.substr(0,pos);// obtenemos el nombre del enrutador
        enruta_temp.SetNombre(nombre);//asignamos el nombre a un router temporal
        for(unsigned int i=pos+1;i<linea.size();i++){
            if(linea.at(i)!=',')temp=temp+linea.at(i);// tomamos el nombre de las conexiones al encontrar una coma
            else {
                unsigned int pos=temp.find(":");// nombre de y cada uno de los costos de conexiones
                nombre_router=temp.substr(0,pos);// nombre del router de conexion
                costo=stoi(temp.substr(pos+1));//costo de conexiones
                tabla_temp.insert(make_pair(nombre_router,costo));// creamos tabla de conexion con sus costos temporal
                temp="";
            }
            fflush(stdin);
        }
        enruta_temp.SetConexiones(tabla_temp);//obtiene la tabla de enrutamiento
        topologia.SetEnrutador(enruta_temp);//asignamos el enrutador a la red
        topologia.SetTabla_EN();// crea un vector con cada una de las conexiones
        tabla_temp.clear();// limpiamos la variable auxiliar
    }
    archivo.close();// cerrar archivo
    red topologia_final;
    topologia_final=Primer_camino(topologia);// define la red con costos optimos
    map<string,int>::iterator it;
    vector<router> tabla_e=topologia_final.getTabla_EN();//obtiene la tabla de enrutamiento
    router temp2;//guardar enrutadores de tabla_e
    map<string,int>conex2;//conexiones de cada enrutador
    string opcion="0";

    while(opcion!="9"){
        cout<<"Sistema de red"<<endl<<endl<<"Ingrese:"<<endl<<endl;
        cout<<"1.Agregar enrutador"<<endl<<"2.Eliminar enrutador"<<endl<<"3.Mostrar red"<<endl;
        cout<<"4.Mostrar tabla"<<endl<<"5.Modificar costos de enlace"<<endl<<"6.Calcular ruta y menor costo de envio de un router a otro"<<endl;
        cout<<"7.Agregar un enlace a la red"<<endl<<"8.Eliminar un enlace de la red"<<endl<<"9.Salir"<<endl<<endl;
        cout<<"Escriba su opcion:";
        cin>>opcion;
        cout<<endl;
        if(opcion=="1"){//agregar un enrutador
            map<string,int>maptemp;
            fflush(stdin);
            cout<<"Escriba el nombre del router que va a agregar:";
            getline(cin,nombre);
            enruta_temp.SetNombre(nombre);
            cout<<"El router "<<nombre<<" solo puede tener un enlace"<<endl;
            cout<<"Conexiones posibles:"<<endl;
            for(unsigned int i=0;i<tabla_e.size();i++){// routers de la red
                temp2=tabla_e[i];
                cout<<"*"<<temp2.GetNombre()<<endl;
            }
            fflush(stdin);
            cout<<"Ingrese el router de conexion:";
            getline(cin,nombre_router);
            cout<<"Ingrese el costo:";
            cin>>costo;
            maptemp.insert(make_pair(nombre_router,costo));//router nuevo con su conexion
            enruta_temp.SetConexiones(maptemp);
            topologia.SetEnrutador(enruta_temp);//ingresamos el enrutador a la red
            topologia.SetTabla_EN();// ingresa la tabla de conexiones a la red
            tabla_e=topologia.getTabla_EN();
            for(unsigned int i=0;i<tabla_e.size();i++){//conectar con el router destino
                router enruta_temp2=tabla_e[i];
                if(enruta_temp2.GetNombre()==nombre_router){
                    topologia.SetClearTabla(nombre_router);// borra el router de llegada a la red
                    maptemp=enruta_temp2.GetConexiones();// obtenemos las conexiones del router de llegada
                    maptemp.insert(make_pair(nombre,costo));// se agrega la nueva conexion
                    enruta_temp2.SetConexiones(maptemp);// se agrega el router
                    topologia.SetEnrutador(enruta_temp2);// se actualiza la red original
                    topologia.SetTabla_EN();
                }
            }
            cout<<"La red se ha actualizado"<<endl<<endl;
            topologia_final=Primer_camino(topologia);
        }
        else if(opcion=="2"){//eliminar un enrutador
            string eliminar;
            tabla_e=topologia.getTabla_EN();
            cout<<"Routers de la red"<<endl;
            for(unsigned int i=0;i<tabla_e.size();i++){
                temp2=tabla_e[i];
                cout<<"*"<<temp2.GetNombre()<<endl;
            }
            fflush(stdin);
            cout<<"Ingrese el nombre del router a eliminar:";
            getline(cin,eliminar);
            for(unsigned int i=0;i<tabla_e.size();i++){
                router enruta_temp2;
                enruta_temp2=tabla_e[i];
                if(enruta_temp2.GetNombre()==eliminar)topologia.SetClearTabla(eliminar);
                conex2=enruta_temp2.GetConexiones();
                conex2.erase(eliminar);
                topologia.SetClearTabla(enruta_temp2.GetNombre());//elimina el router de llegada
                enruta_temp2.SetConexiones(conex2);//se agregan las conexiones al router
                topologia.SetEnrutador(enruta_temp2);//se actualiza en la red original
                topologia.SetTabla_EN();
            }
            for(unsigned int i=0;i<tabla_e.size();i++){
                if(tabla_e[i].GetNombre()==eliminar)
                    tabla_e.erase(tabla_e.begin()+i);// eliminamos el router
            }
            cout<<"La red se ha actualizado"<<endl<<endl;
            topologia_final=Primer_camino(topologia);
        }
        else if(opcion=="3"){
            tabla_e=topologia.getTabla_EN();
            for(unsigned int i=0;i<tabla_e.size();i++){
                temp2=tabla_e[i];
                conex2=temp2.GetConexiones();
                for(it=conex2.begin();it!=conex2.end();it++){//imprime el arreglo original con sus conexiones y costos|
                    cout<<temp2.GetNombre()<<"->"<<it->first<<" Costo:"<<it->second<<endl;
                }
                cout<<endl;
            }
        }
        else if(opcion=="4"){
            tabla_e=topologia_final.getTabla_EN();
            for(unsigned int i=0;i<tabla_e.size();i++){
                temp2=tabla_e[i];
                conex2=temp2.GetConexiones();
                for(it=conex2.begin();it!=conex2.end();it++){//imprime la tabla de conexiones
                    cout<<temp2.GetNombre()<<"->"<<it->first<<" Costo:"<<it->second<<endl;
                }
                cout<<endl;
            }
        }
        else if(opcion=="5"){
            string inicial,final;
            int costo;
            tabla_e=topologia.getTabla_EN();// asignamos tabla de enrutadores con sus costos
            cout<<"Routers de la red:"<<endl;
            for(unsigned int i=0;i<tabla_e.size();i++){// muestra los routers disponibles
                temp2=tabla_e[i];
                cout<<'*'<<temp2.GetNombre()<<endl;
            }
            fflush(stdin);
            cout<<"Ingrese el nombre del router de inicio:";
            getline(cin,inicial);
            cout<<"Ingrese el nombre del router de llegada:";
            getline(cin,final);
            for(unsigned int i=0;i<tabla_e.size();i++){//buscamos router de partida
                enruta_temp=tabla_e[i];
                if(enruta_temp.GetNombre()==inicial){
                    conex2=enruta_temp.GetConexiones();
                    map<string,int>::iterator iter=conex2.find(final);// busca el router de llegada en las conexiones
                    if(iter!=conex2.end()){
                        cout<<"El costo del enlace actual desde "<<inicial<< " hasta "<<final<<" es:"<<iter->second<<endl;
                        cout<<"Ingrese el nuevo costo:";
                        cin>>costo;
                        iter->second=costo;
                    }
                    //actualizamos la red
                    topologia.SetClearTabla(inicial);
                    enruta_temp.SetConexiones(conex2);
                    topologia.SetEnrutador(enruta_temp);
                    topologia.SetTabla_EN();
                 }
                if(enruta_temp.GetNombre()==final){
                    conex2=enruta_temp.GetConexiones();
                    map<string,int>::iterator iter=conex2.find(inicial);// busca el router de llegada en las conexiones
                    if(iter!=conex2.end()){
                        iter->second=costo;
                    }
                    //actualizamos la red
                    topologia.SetClearTabla(final);
                    enruta_temp.SetConexiones(conex2);
                    topologia.SetEnrutador(enruta_temp);
                    topologia.SetTabla_EN();
                }
            }
            topologia_final=Primer_camino(topologia);
            cout<<endl;
        }
        else if(opcion=="6"){
            string inicial,final;
            tabla_e=topologia_final.getTabla_EN();// asignamos tabla de enrutadores con sus costos
            cout<<"Routers de la red:"<<endl;
            for(unsigned int i=0;i<tabla_e.size();i++){// muestra los routers disponibles
                temp2=tabla_e[i];
                cout<<'*'<<temp2.GetNombre()<<endl;
            }
            fflush(stdin);
            cout<<"Ingrese el nombre del router de inicio para el envio de paquetes:";
            getline(cin,inicial);
            cout<<"Ingrese el nombre del router de llegada:";
            getline(cin,final);
            for(unsigned int i=0;i<tabla_e.size();i++){//buscamos router de partida
                enruta_temp=tabla_e[i];
                if(enruta_temp.GetNombre()==inicial){
                    conex2=enruta_temp.GetConexiones();
                    map<string,int>::iterator iter=conex2.find(final);// busca el router de llegada en las conexiones
                    if(iter!=conex2.end()){
                        cout<<"El costo del paquete por el camino mas corto desde "<<inicial<< " hasta "<<final<<" es:"<<iter->second<<endl;
                    }
                    map<string,vector<string>>camino;
                    camino=enruta_temp.GetCamino();
                    vector<string>camino2;
                    map<string,vector<string>>::iterator iter2=camino.find(final);
                    if(iter2!=camino.end())camino2=iter2->second;// toma el vector de la ruta correspondiente
                    cout<<"La ruta fue:";
                    for(unsigned int i=0;i<camino2.size();i++)cout<<"->"<<camino2[i];//imprime la ruta
                    cout<<"->"<<final;
                }
            }
            cout<<endl<<endl;
        }
        else if(opcion=="7"){
            string inicial,final;
            int costo;
            tabla_e=topologia.getTabla_EN();// asignamos tabla de enrutadores con sus costos
            cout<<"Routers de la red:"<<endl;
            for(unsigned int i=0;i<tabla_e.size();i++){// muestra los routers disponibles
                temp2=tabla_e[i];
                cout<<'*'<<temp2.GetNombre()<<endl;
            }
            fflush(stdin);
            cout<<"Ingrese el nombre del router de inicio del enlace:";
            getline(cin,inicial);
            cout<<"Ingrese el nombre del router de llegada:";
            getline(cin,final);
            cout<<"Ingrese el costo:";
            cin>>costo;
            for(unsigned int i=0;i<tabla_e.size();i++){//buscamos router de partida
                enruta_temp=tabla_e[i];
                if(enruta_temp.GetNombre()==inicial){
                    conex2=enruta_temp.GetConexiones();// obtenemos las coexiones de dicho router
                    conex2.insert(make_pair(final,costo));// agregamos la conexion
                    topologia.SetClearTabla(inicial);// eliminamos el router de partida
                    enruta_temp.SetConexiones(conex2);// asignamos la nueva conexion
                    topologia.SetEnrutador(enruta_temp);//asignamos el router con las conexiones asignadas
                    topologia.SetTabla_EN();
                }
                if(enruta_temp.GetNombre()==final){
                    conex2=enruta_temp.GetConexiones();// obtenemos las coexiones de dicho router
                    conex2.insert(make_pair(inicial,costo));// agregamos la conexion
                    topologia.SetClearTabla(final);// eliminamos el router de partida
                    enruta_temp.SetConexiones(conex2);// asignamos la nueva conexion
                    topologia.SetEnrutador(enruta_temp);//asignamos el router con las conexiones asignadas
                    topologia.SetTabla_EN();
                }
            }
            cout<<"La red se ha actualizado"<<endl<<endl;
            topologia_final=Primer_camino(topologia);// actualizamos la red final
        }
        else if (opcion == "8") {
                    string inicial,final;
                    tabla_e=topologia.getTabla_EN();// asignamos tabla de enrutadores con sus costos
                    cout<<"Routers de la red:"<<endl;
                    for(unsigned int i=0;i<tabla_e.size();i++){// muestra los routers disponibles
                        temp2=tabla_e[i];
                        cout<<'*'<<temp2.GetNombre()<<endl;
                    }
                    fflush(stdin);
                    cout<<"Ingrese el nombre del router de inicio del enlace:";
                    getline(cin,inicial);
                    for(unsigned int i=0;i<tabla_e.size();i++){//buscamos router de partida
                        enruta_temp=tabla_e[i];
                        if(enruta_temp.GetNombre()==inicial){
                            conex2=enruta_temp.GetConexiones();
                            map<string,int>::iterator it;
                            cout << "Conexiones del router seleccionado: "<<endl;
                            for(it=conex2.begin(); it != conex2.end();it++){
                                cout<<"->" <<it->first<<" costo: "<<it->second<<endl;
                            }
                            cout<<"Ingrese el nombre del router de la conexion a eliminar: "; getline(cin,final);
                            conex2.erase(final);//eliminamos el router
                            topologia.SetClearTabla(inicial);
                            enruta_temp.SetConexiones(conex2);
                            topologia.SetEnrutador(enruta_temp);
                            topologia.SetTabla_EN();
                        }
                        if(enruta_temp.GetNombre()==final){
                            conex2=enruta_temp.GetConexiones();
                            conex2.erase(inicial);//eliminamos el router
                            topologia.SetClearTabla(final);
                            enruta_temp.SetConexiones(conex2);
                            topologia.SetEnrutador(enruta_temp);
                            topologia.SetTabla_EN();
                        }
                    }
                    topologia_final=Primer_camino(topologia);
                    cout<<endl;
                }
    }
    return 0;
}
map <int, vector<string>> camino_corto (string inicial, string final, vector<router>tabla, vector<string>ruta, int*costo, map<int, vector<string>>calcular_costo)
{
    router temp;
    map<string, int> conexiones;
    int costo_ida=0,vuelta=1;
    string ruta_ida="";
    for (int unsigned i=0;i<tabla.size();i++){//Buscar las conexiones del inicial
        temp=tabla[i];
        if (temp.GetNombre()==inicial){
            break;
        }
    }
    conexiones=temp.GetConexiones();
    map<string,int>::iterator iter;
    bool aux=true;
    for (iter=conexiones.begin();iter!=conexiones.end();iter++){//Iteración sobre las conexiones
        costo_ida=iter->second;//Resta para devolverse en el costo.
        ruta_ida=iter->first;//Resta para devolverse en el camino.
        for(int unsigned m=0;m<ruta.size();m++){//Revisa si ya pasó por ese nodo, para no hacerlo nuevamente
            if (iter->first==ruta[m]){
                aux=false;
                vuelta=0;
                break;
            }
        }
        if(aux==true){
            *costo+=iter->second;//Suma el costo de la ruta que va hacer.
            if (iter->first==final){//Indica si ya encontró el nodo de llegada.
                calcular_costo.insert(make_pair(*costo,ruta));//Cuanto costó llegar y la ruta a llegar.
                vuelta=1;//Indica que ya se puede devolver para la resta mas abajo.
            }
            else{
                ruta.push_back(iter->first);//Indica que el nodo elegido no es el final para ir a buscar en los nodos conectados a el.
                calcular_costo=camino_corto(iter->first,final,tabla,ruta,costo,calcular_costo);
            }
        }
        if(vuelta==1){//Indica que ya se va devolver.
            *costo=0;
            for(int unsigned i=0;i<ruta.size();i++){//Resta la ruta que eligió para llegar.
                if(ruta[i]==ruta_ida){
                    ruta.erase(ruta.begin()+i);
                }
            }
        }
        vuelta=1;//Indica que ya se puede devolver.
        aux=true;//Indica que el nodo a elegir puede ser un posible nodo.
    }
    return calcular_costo;
}
map <int, vector<string>> Calcular_camino (map<int, vector<string>>costo)
{
    map<int,vector<string>>::iterator iter;
    map<int,vector<string>>caminos;
    vector<string>ruta;
    int cont=0,temp=0;
    for(iter=costo.begin();iter!=costo.end();iter++){//recorre las rutas asignadas
        if(cont==0){
            temp=iter->first;
            ruta=iter->second;
            cont++;
        }
        else{
            if(temp>iter->first){//indica si el costo elegido es menor que el anterior
                temp=iter->first;
                ruta=iter->second;
            }
        }
    }
    caminos.insert(make_pair(temp,ruta));//guarda el menor costo con la ruta asignada
    return caminos;
}
bool No_conexion(map<int, vector<string>>tabla)
{
    map<int,vector<string>>::iterator iter;
    for(iter=tabla.begin();iter!=tabla.end();iter++){
        if(iter->first==0)return true;// si alguno es cero es porque no hay conexion con ese router
    }
    return false;
}
red Primer_camino (red topologia)
{
    router enruta_temp;
    int costo=0;
    vector<router> tabla_e = topologia.getTabla_EN();//Obtener tabla de enrutamiento.
    router temp2;//Guardar enrutadores guardados en tabla_E.
    red topologia_final;
    vector<string> ruta;
    vector<string> nom_enrutadores;//Para conectar todos los routers.
    map<string, int> conexiones;
    map<int, vector<string>>calcular_cam;
    map<int, vector<string>>calcular_costo;
    map<string, vector<string>>caminos;//Caminos a todos los enrutadores.
    for (int unsigned i=0; i<tabla_e.size();i++){//Capturar todos los routers en la red original.
        temp2=tabla_e[i];
        nom_enrutadores.push_back(temp2.GetNombre());
    }
    bool conex;
    for (int unsigned i=0; i<nom_enrutadores.size();i++){//Recorre los nodos.
        enruta_temp.SetNombre(nom_enrutadores[i]);//Asigana la conexión con el mismo.
        conexiones.insert(make_pair(nom_enrutadores[i],0));//Agrega su propia conexión.
        for(int unsigned y = 0; y < nom_enrutadores.size(); y++){//Conecta el router con los demás.
            if (nom_enrutadores[i]==nom_enrutadores[y]){//Si el mismo ya no se escoge.
            }
            else
            {
                ruta.push_back(nom_enrutadores[i]);//Se asigna el punto de partida.
                //Calcula todos los caminos posibles
                calcular_costo = camino_corto(nom_enrutadores[i], nom_enrutadores[y], tabla_e, ruta, &costo, calcular_costo);
                ruta.clear();//Reinicia los valores
            }
            calcular_cam =Calcular_camino(calcular_costo);//Calcular camino mas corto.
            conex = No_conexion(calcular_cam);//Saber si tiene conexión con el nodo buscado.
            costo=0;
            calcular_costo.clear();
            map<int, vector<string>>::iterator iter=calcular_cam.begin();
            while (iter!=calcular_cam.end()) {
                if (conex){
                    iter++;
                }
                else{
                    conexiones.insert(make_pair(nom_enrutadores[y],iter->first));
                    caminos.insert(make_pair(nom_enrutadores[y],iter->second));
                    iter ++;
                }
            }
        }
        //actualizamos la red
        enruta_temp.SetCamino(caminos);
        enruta_temp.SetConexiones(conexiones);
        conexiones.clear();
        topologia_final.SetEnrutador(enruta_temp);
        topologia_final.SetTabla_EN();
        caminos.clear();
    }
    return topologia_final;
}
