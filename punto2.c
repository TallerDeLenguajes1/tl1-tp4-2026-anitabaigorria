#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 50

char *estados[] = { "Pendiente", "Realizada" };

typedef struct Tarea{ 
    int TareaID; //Numérico autoincremental comenzando en 1000 
    char *Descripcion;      
    int Duracion; // entre 10 – 100 
    char *estado; 
} Tarea;  

typedef struct Nodo{ 
    Tarea T;  
    struct Nodo *Siguiente;  
} Nodo; 

Nodo *crearNodo(Tarea cargar);
void insertarNodo(Nodo **cabecera, Nodo *nuevo);
void listarTareas(Nodo *listaT);
bool valido(Nodo *verificar, int nro);
void transfPendR(Nodo **listaR, int indice, Nodo **listaP);
void mostrarTareaxID(Nodo *mostrar, int nro);
void mostrarTareaxPalabra(Nodo *mostrar, char *buffer);
void liberarMemoria(Nodo *liberar);

int main(){
    
    char buffer[MAX];
    int eleccion;
    Nodo *TareasPendientes = NULL, *TareasRealizadas = NULL;
    Tarea cargar;
    cargar.TareaID = 999;
    
    // interfaz de carga de tareas para solicitar tareas pendientes
    do
    {

        printf("--------------------------- CARGA DE TAREA (PENDIENTE) --------------------------- \n");
        cargar.estado = estados[0];
        cargar.TareaID++;
        printf("ID DE TAREA: %d\n",cargar.TareaID);

        printf("Descripcion: ");
        scanf("%s",&buffer);
        fflush(stdin);

        cargar.Descripcion = (char *)malloc((strlen(buffer)+1)*sizeof(char));
        strcpy(cargar.Descripcion,buffer);

        do
        {
            printf("Duracion (entre 10 y 100): ");
            scanf("%d",&cargar.Duracion);
            
            if (cargar.Duracion > 100 || cargar.Duracion < 10)
            {
                printf("Por favor, escriba una duracion entre 10 y 100.\n");
            }
            
        } while (cargar.Duracion > 100 || cargar.Duracion < 10);
        
        insertarNodo(&TareasPendientes,crearNodo(cargar));
        printf("-- Carga finalizada! --\n");

        do
        {
            printf(" Desea cargar otra tarea o continuar?\n 1 = HACER OTRA CARGA | 0 = CONTINUAR\n INGRESE: ");
            scanf(" %d",&eleccion);
            
            if (eleccion != 0 && eleccion != 1)
            {
                printf("-- SOLO SE ADMITEN LAS OPCIONES 1 = HACER OTRA CARGA | 0 = CONTINUAR. INGRESE NUEVAMENTE \n");
            }
        } while (eleccion != 0 && eleccion != 1);
        
    } while (eleccion == 1);
    
    printf("*TareasPendientes");
    listarTareas(TareasPendientes);

    // interfaz para elegir qué tareas de la lista de pendientes deben ser transferidas a la lista de tareas realizadas -> listar (funcion) y dejar elegir + funcion para mover
    do
    {
        printf("\nSeleccione una tarea para marcarla como realizada (identificada por ID): ");
        scanf(" %d",&eleccion);
        
        if (valido(TareasPendientes, eleccion) == false){
            printf("Por favor, ingrese un ID valido");
        }

    } while (valido(TareasPendientes, eleccion) != true);

    transfPendR(&TareasRealizadas,eleccion,&TareasPendientes);
    printf("-- Transferencia realizada! --\n");
    
    printf("*TareasRealizadas");
    listarTareas(TareasRealizadas);
    printf("\n -------------------------------------------\n");
    printf("*TareasPendientes");
    listarTareas(TareasPendientes);

    // implemente una funcionalidad que permita consultar tareas por id o palabra clave y mostrarlas por pantalla, indicando si corresponde a una tarea pendiente o realizada. 
    do
    {
       printf("-- CONSULTA DE TAREAS --\n 1 = POR ID | 0 = POR PALABRA CLAVE\n INGRESE: ");
       scanf(" %d",&eleccion);
           
       if (eleccion != 0 && eleccion != 1)
       {
          printf("-- SOLO SE ADMITEN LAS OPCIONES 1 = POR ID | 0 = POR PALABRA CLAVE. INGRESE NUEVAMENTE \n");
       }
    } while (eleccion != 0 && eleccion != 1);

    if (eleccion == 1)
    {
        // POR ID
        do
        {
            printf("\nIngrese ID: ");
            scanf(" %d",&eleccion);
            
            if (valido(TareasPendientes, eleccion) == false || valido(TareasRealizadas, eleccion) == false){
                printf("Por favor, ingrese un ID valido");
            }
    
        } while (valido(TareasPendientes, eleccion) != true && valido(TareasRealizadas, eleccion) == false);
    
        mostrarTareaxID(TareasPendientes,eleccion);
        mostrarTareaxID(TareasRealizadas,eleccion);
    } else
    {
        // POR PALABRA CLAVE
        printf("\nIngrese la palabra: ");
        scanf("%s",&buffer);
        fflush(stdin);

        mostrarTareaxPalabra(TareasPendientes,buffer);        
    }
    
    
    return 0;
}

Nodo *crearNodo(Tarea cargar){
    Nodo *crear = (Nodo *)malloc(sizeof(Nodo));
    crear->T = cargar;
    crear->Siguiente = NULL;

    return crear;
}

void insertarNodo(Nodo **cabecera, Nodo *nuevo){
    // apuntamos el nodo nuevo a lo que estaba apuntando la cabecera
    nuevo->Siguiente = *cabecera; 
    
    // actualizamos la cabecera original para que ahora apunte al nuevo nodo
    *cabecera = nuevo;

}

void listarTareas(Nodo *listaT){
    Nodo *actual = listaT;
    
    printf("\n      NULL      \n");
    while (actual != NULL)
    {
        printf("      ^       \n");
        printf("      |       \n");

        printf("\nTAREA %d (%s) \n", actual->T.TareaID, actual->T.estado);
        printf("DESCRIPCION: %s \n", actual->T.Descripcion);
        printf("DURACION: %d \n", actual->T.Duracion);
        

        actual = actual->Siguiente;
    }    
}

bool valido(Nodo *verificar, int nro){
    Nodo *actual = verificar;
    while (actual != NULL)
    {
        if (actual->T.TareaID == nro)
        {
           return true;
        }        
        // avanzo al siguiente nodo
        actual = actual->Siguiente;
    }
    return false;
}

void transfPendR(Nodo **listaR, int indice, Nodo **listaP){
    // recorro la lista hasta encontrar el nodo seleccionado a traves de un nodo auxiliar
    Nodo *actual = *listaP, *anterior = NULL;
    while (actual != NULL)
    {
        // si esto se cumple, 'actual' es el nodo a transferir
        if (actual->T.TareaID == indice)
        {
            actual->T.estado = estados[1];
            // dependiendo de si estoy en el primer elemento o en uno del medio, actuo
            if (anterior != NULL) { // medio
                // le doy la direccion de ese actual al anterior (para que deje de apuntar a actual y apunte a lo que apuntaba actual), es decir, "Puenteo"
                anterior->Siguiente = actual->Siguiente; 
            } else { // inicio
                *listaP = actual->Siguiente; // avanzo al siguiente, movimos el primer elemento
            }
            
            // cambio la direccion de actual a NULL o a otro elemento de TareasRealizadas
            insertarNodo(listaR,actual);
            break;
        }
        anterior = actual;
        // avanzo al siguiente nodo
        actual = actual->Siguiente;
    }   
}

void mostrarTareaxID(Nodo *mostrar, int nro){
    Nodo *actual = mostrar;
    while (actual != NULL)
    {
        if (actual->T.TareaID == nro)
        {
            printf("\nTAREA %d (%s) \n", actual->T.TareaID, actual->T.estado);
            printf("DESCRIPCION: %s \n", actual->T.Descripcion);
            printf("DURACION: %d \n", actual->T.Duracion);
        }        
        // avanzo al siguiente nodo
        actual = actual->Siguiente;
    }
}

void mostrarTareaxPalabra(Nodo *mostrar, char *buffer){
    Nodo *actual = mostrar;
    while (actual != NULL)
    {
        if (strstr(actual->T.Descripcion,buffer) != NULL)
        {
            printf("\nTAREA %d (%s) \n", actual->T.TareaID, actual->T.estado);
            printf("DESCRIPCION: %s \n", actual->T.Descripcion);
            printf("DURACION: %d \n", actual->T.Duracion);
        }
        // avanzo al siguiente nodo
        actual = actual->Siguiente;
    }
}

void liberarMemoria(Nodo *liberar){
    Nodo *actual = liberar;
    Nodo *siguiente; // var temporal para no perder el que sigue
    
    while (actual != NULL)
    {
        siguiente = actual->Siguiente; // guardamos para saber donde ir despues
        
        free(actual->T.Descripcion); 
        free(actual);                        

        actual = siguiente;     // avanzamos al siguiente que habíamos guardado
    }
}