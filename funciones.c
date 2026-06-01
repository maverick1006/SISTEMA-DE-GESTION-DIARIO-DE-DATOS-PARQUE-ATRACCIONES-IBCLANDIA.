#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Horario {
    int hora;
    int minuto;
    char temporada; /* A = alta, B = baja, detalle únicamente para tener mayor info para la gestión y tener los 3 datos primitivos*/
};

struct Familia {
    char *rutResponsableFamiliar;
    char *apellidoFamilia;
    int cantidadIntegrantes;
};
/*nodo para lista simple de familias*/
struct NodoFamilia {
    struct Familia *familia;
    struct NodoFamilia *sig;
};

struct Entrada {
    int idEntrada;
    char *tipoEntrada;
    char *fechaVisita;
    float valor;
    int estado; /* 0: no utilizada, 1: utilizada */
};
/* nodo para lista simple de entradas */
struct NodoEntradas {
    struct Entrada *entrada;
    struct NodoEntradas *sig;
};

struct Visitante {
    char *rut;
    char *nombre;
    float altura;
    int edad;
    int dentroParque; /* 0: fuera del parque, 1: dentro del parque */
    struct NodoEntradas *listaEntrada; /* head a la lista de entradas */
    struct Familia *familia;
};
/* nodo para ABB de visitantes */
struct NodoVisitante {
    struct Visitante *visitante;
    struct NodoVisitante *izq; /* se utilizará el rut para manejarse*/
    struct NodoVisitante *der;
};

struct filaEspera {
    struct Visitante **cola;
    int maxCola;
    int cantidadEnFila;
    int estadoFila; /* 1 llena, 2 suspendida, 3 vacia, 4 aún con espacio*/
};

struct Atraccion {
    int idAtraccion;
    char *nombreJuego;
    int estado; /* 0 cerrada por horario, 1 abierta, 2 en mantenimiento, 3 fuera de servicio */
    int capacidadDentroDeAtraccion;
    int duracionCiclo;
    float alturaMinima;
    int edadMinima;
    int totalAtendidos;
    struct filaEspera *fila; /* puntero a la fila de espera de la atracción */
};

struct Zona {
    int idZona;
    char *nombre;
    char *tematica;
    struct Horario horaApertura;
    struct Horario horaCierre;
    int numEncargados;
    int capacidadMaxima;
    int visitantesActuales;
    int estadoAforo;
    struct Atraccion **arrAtracciones;
    int maxatracciones;
    int cantAtraccion; /* cantidad de atracciones en la zona (pLibre)*/
};

struct Parque {
    char *nombre;
    char *fechaOperacion;
    int capacidadMaxima;
    int visitantesActuales;
    int totalVisitantesHoy;
    int totalEntradasVendidas;
    int totalEntradasUtilizadas;
    float ingresosTotales;
    struct Zona **zonas; /* arreglo compacto de punteros a zonas con tamaño MAX_ZONAS */
    int maxZonas; /* capacidad del arreglo — nunca cambia (pLibre) */
    int cantZonas; /* cantidad de zonas en el parque*/
    struct NodoVisitante *raizVisitantes; /* raíz del ABB de visitantes */
    struct NodoFamilia *listaFamilias; /* head a la lista de familias */
};



/*========================== inicializacion del parque ==========================================*/
struct Parque *abrirParque(char *nombre, char *fecha, int maxCapacidad, int maxZonas){

  struct Parque *parqueCreado = NULL;

  parqueCreado = (struct Parque*)malloc(sizeof(struct Parque));

  /* asignación del nombre para el parque*/
  parqueCreado->nombre = (char*)malloc(strlen(nombre) + 1);
  strcpy(parqueCreado->nombre, nombre);

  /* asignación del día para el parque*/
  parqueCreado->fechaOperacion = (char*)malloc(strlen(fecha) + 1);
  strcpy(parqueCreado->fechaOperacion, fecha);

  /* todas las variables de conteos incizalizdas en 0*/

  parqueCreado->capacidadMaxima = maxCapacidad;
  parqueCreado->visitantesActuales = 0;
  parqueCreado->totalVisitantesHoy = 0;
  parqueCreado->totalEntradasVendidas = 0;
  parqueCreado->totalEntradasUtilizadas = 0;
  parqueCreado->ingresosTotales = 0.0;
  parqueCreado->maxZonas = maxZonas; /*fijo*/ 
  parqueCreado->cantZonas = 0; /*pLibre=0*/
  parqueCreado->zonas = (struct Zona**)malloc(maxZonas*sizeof(struct Zona*));
  parqueCreado->raizVisitantes = NULL;
  parqueCreado->listaFamilias = NULL;

  return parqueCreado;
}

/*==================================== visitantes ============================================================*/

struct NodoVisitante *crearVisitante(char *rut, char *nombre, float altura, int edad) {
    
    struct NodoVisitante *nuevo = NULL;
  
    nuevo = (struct NodoVisitante*)malloc(sizeof(struct NodoVisitante));
    
    nuevo->visitante = (struct Visitante*)malloc(sizeof(struct Visitante));
    /* asignación de memoria para el char rut y copia del dato ingresado*/
    nuevo->visitante->rut = (char*)malloc(strlen(rut) + 1);
    strcpy(nuevo->visitante->rut, rut);
     /* asignación de memoria para el char nombre y copia del dato ingresado*/
    nuevo->visitante->nombre = (char*)malloc(strlen(nombre) + 1);
    strcpy(nuevo->visitante->nombre, nombre);
    /* asignaciones de valores según los datos que se ikngresaron*/
    nuevo->visitante->altura = altura;
    nuevo->visitante->edad = edad;
    nuevo->visitante->dentroParque = 0;

    nuevo->visitante->listaEntrada = NULL;
    nuevo->visitante->familia = NULL;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    return nuevo;
}

struct NodoVisitante *insertarVisitanteArbol(struct NodoVisitante *raiz, struct NodoVisitante *nuevo) {
    /* creación de variable para realizar las comparaciones y ver si x rut esa mayor o menor para guardarlo correctament*/
    int aux;

    if (raiz == NULL) {
        return nuevo;
    }

    aux = strcmp(nuevo->visitante->rut, raiz->visitante->rut);
    /* recursividad para buscar la posición donde debe ir ese visitante*/
    if (aux < 0) {
        raiz->izq = insertarVisitanteArbol(raiz->izq, nuevo);
    }else{
        raiz->der = insertarVisitanteArbol(raiz->der, nuevo);
    } 
    return raiz;
}

void listarVisitantes(struct NodoVisitante *raiz){
    if (raiz != NULL){

        /* utilizamos in-orden, osea recorremos primero por ls izq */
        listarVisitantes(raiz->izq);

        /* Imprime los datos del visitante */
        printf("\nRUT: %s Nombre: %s Edad: %d Dentro del parque: ",
               raiz->visitante->rut,
               raiz->visitante->nombre,
               raiz->visitante->edad);

        /* validación si visitante está dentro del parque */
        if (raiz->visitante->dentroParque == 1){
            printf("SI");
        }else{
            printf("NO");
        }

        /* Recorre por la derecha */
        listarVisitantes(raiz->der);
    }
}

void mostrarVisitante(struct NodoVisitante *nodo) {
    if (nodo == NULL) return;
    printf("\n========== VISITANTE ==========\n");
    printf("RUT : %s\n", nodo->visitante->rut);
    printf("Nombre : %s\n", nodo->visitante->nombre);
    printf("Edad : %d anios\n", nodo->visitante->edad);
    printf("Altura : %.2f mts\n", nodo->visitante->altura);
    printf("En parque : %s\n", nodo->visitante->dentroParque ? "SI" : "NO");
    if (nodo->visitante->familia != NULL)
        printf("Familia   : %s\n", nodo->visitante->familia->apellidoFamilia);
}

/* función recursiva que nos ayuda a realizar la búsqueda binaria en el árbol, ya que era necesario agregar un buscar para cada estructura existente */
struct NodoVisitante *buscarVisitante(struct NodoVisitante *raiz, char *rut){
    int cmp; /* variable auxiliar para hacer ls distintas comparaciones*/
    
    if (raiz == NULL) return NULL; /* si la raíz es NULL, retornamos NULL*/

    /* comparación del rut a buscar con el nodo actual*/
    cmp = strcmp(rut, raiz->visitante->rut);

    if (cmp == 0) return raiz; /* si es igual a 0, significa que lo hemos encontrado y retornamos la raíz*/

    if (cmp < 0)  return buscarVisitante(raiz->izq, rut); /* si es menor a 0 el resultado, seguirmos buscando por la izq*/
    
    return buscarVisitante(raiz->der, rut); /* en el caso contrario de ser mayor a 0, seguimos con la búsqueda pero por la derecha*/
}

void registrarVisitante(struct Parque *parque, char *rut, char *nombre, float altura, int edad) {
    struct NodoVisitante *nuevo = NULL;
    /* variable utilizada como aux para la validación de si ese rut ya está registrado*/
    struct NodoVisitante *existe = NULL;
    if (parque == NULL) return;

    existe = buscarVisitante(parque->raizVisitantes, rut);
    /* validación por si, ya existe un visitante con ese rut, llamando a nuestra función buscarVisitante y si retorna algo que no sea NULL, significa que ya existía*/
    if(existe != NULL){
        printf("\nYa existe un visitante con el rut: %s.\n", rut);
        return;
    }
    nuevo = crearVisitante(rut, nombre, altura, edad);
    parque->raizVisitantes = insertarVisitanteArbol(parque->raizVisitantes, nuevo);
    parque->totalVisitantesHoy++;

    printf("\nVisitante registrado correctamente\n");
}


void agregarEntradaVisitante(struct Parque *parque, struct Visitante *visitante, int id, char *tipo, char *fecha, float valor, int estado){

    struct Entrada *nuevaEntrada = NULL;
    struct NodoEntradas *nuevoNodo = NULL;
    /* no sé si queda mejor agregar un mensaje de error*/
    if (parque == NULL || visitante == NULL) return;

    /* creación de mallocs y asignación de datos */
    nuevaEntrada = (struct Entrada*)malloc(sizeof(struct Entrada));
    nuevaEntrada->idEntrada = id;

    nuevaEntrada->tipoEntrada = (char*)malloc(strlen(tipo) + 1);
    strcpy(nuevaEntrada->tipoEntrada, tipo);

    nuevaEntrada->fechaVisita = (char*)malloc(strlen(fecha) + 1);
    strcpy(nuevaEntrada->fechaVisita, fecha);

    nuevaEntrada->valor = valor;
    nuevaEntrada->estado = estado;

    /* creación del nodo para la lista simple de entradas */
    nuevoNodo = (struct NodoEntradas*)malloc(sizeof(struct NodoEntradas));
    nuevoNodo->entrada = nuevaEntrada;
    nuevoNodo->sig = NULL;

    /* añadir el nodo al principio de la lista del visitante */
    nuevoNodo->sig = visitante->listaEntrada;
    visitante->listaEntrada = nuevoNodo;

    /* acá se actualizan los conteos para cálculos internos del parque*/
    parque->totalEntradasVendidas++;
    parque->ingresosTotales += valor;

    printf("\nEntrada %d asignada con exito al visitante %s (RUT: %s).\n", id, visitante->nombre, visitante->rut);
}

/* función utilizada cuando eliminamos a un nodo de tipo padre, que tiene hijos a su izq y derecha, para encontrarle un reemplazo*/
struct NodoVisitante *minimoABB(struct NodoVisitante *raiz){
    
    while(raiz != NULL && raiz->izq != NULL){
        raiz = raiz->izq;
    }
    return raiz;
}
/* búsqueda de un visitante por su rut, eliminandolo sin desarmar el árbol*/
struct NodoVisitante *eliminarVisitante(struct NodoVisitante *raiz, char *rut){
    
    struct NodoVisitante *temp = NULL; /* creación de variable temporal*/

    /* validación por si raíz es NULL*/
    if(raiz == NULL){
        return NULL;
    }
    /* sI el rut a buscar es menor, seguimos buscando por la izq*/
    if(strcmp(rut, raiz->visitante->rut) < 0){
        raiz->izq = eliminarVisitante(raiz->izq, rut);
    }/*en caso de ser mayor, lo hacemos al revés, buscando por la derecha*/
    else if(strcmp(rut, raiz->visitante->rut) > 0){
        raiz->der = eliminarVisitante(raiz->der, rut);
    }/*y cómo ultimo caso de ya encontrarse el rut solicitado procedemos con la eliminación de x visitante del árbol*/
    else{
        /* Caso 1: sin hijos */
        if(raiz->izq == NULL && raiz->der == NULL){
            free(raiz);
            return NULL;
        }
        /* Caso 2: un hijo */
        if(raiz->izq == NULL){
            temp = raiz->der;
            free(raiz);
            return temp;
        }
        if(raiz->der == NULL){
            temp = raiz->izq;
            free(raiz);
            return temp;
        }
        /* Caso 3: dos hijos, acá llamamos a minimoABB, buscando un reemplazo para ese nodo padre que se desea eliminar*/
        temp = minimoABB(raiz->der);
        raiz->visitante = temp->visitante;
        raiz->der = eliminarVisitante(raiz->der, temp->visitante->rut);
    }
    /* finalmente retornamos nuevo árbol ocon los cambios solicitados*/
    return raiz;
}

/*============================= ZONAS ==================================================*/

struct Zona *crearZona(int id, char *nombre, char *tematica, struct Horario apertura, struct Horario cierre, int cant_encargados, int maxVisitantes, int maxAtracciones){
    struct Zona *nueva;
    
    nueva = (struct Zona*)malloc(sizeof(struct Zona));
    nueva->idZona = id;
    nueva->nombre = (char*)malloc(strlen(nombre)+1);
    strcpy(nueva->nombre,nombre);
    nueva->tematica = (char*)malloc(strlen(tematica)+1);
    strcpy(nueva->tematica,tematica);
    nueva->horaApertura = apertura;
    nueva->horaCierre = cierre;
    nueva->numEncargados = cant_encargados;
    nueva->capacidadMaxima = maxVisitantes;
    nueva->visitantesActuales = 0;
    nueva->estadoAforo = 0;
    nueva->maxatracciones = maxAtracciones;
    nueva->cantAtraccion = 0;

    nueva->arrAtracciones =(struct Atraccion**)malloc(maxAtracciones * sizeof(struct Atraccion*));
    return nueva;
}

struct Zona *buscarZona(struct Parque *elParque, int idBuscar){
    int i;
    if (elParque == NULL){
        return NULL;
    }
    /* recorrido para el arreglo de zonas, utilizando por el id*/
    for (i = 0; i < elParque->cantZonas; i++){
        if (elParque->zonas[i]->idZona == idBuscar){
            return elParque->zonas[i];
        }
    }
    
    return NULL;
}

void agregarZona(struct Parque *elParque, struct Zona *nueva){
    
    if(elParque == NULL || nueva == NULL){
      /* quizás sería bueno agregar mensaje de error acá*/
        return;
    }
    if (elParque->cantZonas >= elParque->maxZonas) {
        printf("\nEl parque ya alcanzo el maximo de zonas.\n");
        return;
    }

    /* en este punto se realiza el agregar y se aumenta el pLibre*/
    elParque->zonas[elParque->cantZonas] = nueva;
    elParque->cantZonas++;
}

void cambiarEstadoZona(struct Zona *zona) {
    if (zona ==NULL) return;

    if (zona->visitantesActuales >= zona->capacidadMaxima) {
        zona->estadoAforo = 1;
        printf("\nESTA ZONA ESTA EN AFORO MAXIMO\n");
    }else {
        zona->estadoAforo = 0;
        printf("\nESTA ZONA AUN CUENTA CON AFORO\n");
    }
}

void mostrarZona(struct Zona *zona) {
    printf("\nID Zonas: %d", zona->idZona);
    printf("\nNombre: %s", zona->nombre);
    printf("\nTematica: %s", zona->tematica);
    printf("\nTemporada: %c", zona->horaApertura.temporada);
    printf("\nNumero de encargados: %d", zona->numEncargados);
    printf("\nVisitantes actuales: %d", zona->visitantesActuales);
    printf("\nCapacidad maxima: %d", zona->capacidadMaxima);

    if (zona->estadoAforo == 1) {
        printf("\nEstado Aforo: COMPLETO");
    }else {
        printf("\nEstado Aforo: DISPONIBLE");
    }
    printf("\nCantidad atracciones: %d", zona->cantAtraccion);
}

void listarZonas(struct Parque *parque) {
    int i;
    struct Zona *zona;

    if (parque == NULL) return;
    printf("\n========== ZONAS DEL PARQUE ==========\n");
    for (i = 0; i < parque->cantZonas; i++) {
        zona = parque->zonas[i];
        if (zona != NULL) mostrarZona(zona);
        printf("\n-----------------------------------\n");
    }
}

void eliminarZona(struct Parque *elParque, int idBuscar) {
    int i, j;
    /* aux booleano para cambiar su estado al encontrar x zona buscada*/
    int aux = 0;

    if (elParque == NULL) return;

    /* buscamos zona la zona por su id en su respecitvo arreglo*/
    for (i = 0; i < elParque->cantZonas; i++) {
        if (elParque->zonas[i] != NULL && elParque->zonas[i]->idZona == idBuscar) {
            aux = 1;

            /* al detectarla hacemos el compactar*/
            for (j = i; j < elParque->cantZonas - 1; j++) {
                elParque->zonas[j] = elParque->zonas[j + 1];
            }

            /* dejamos NULL ese último espacio y restamos - 1 al pLibre*/
            elParque->zonas[elParque->cantZonas - 1] = NULL;
            elParque->cantZonas--;

            printf("\nzona de id %d eliminada del parque.\n", idBuscar);
            break;
        }
    }
    if (aux == 0) {
        printf("\nzona de id %d no encontrada dentro del parque.\n", idBuscar);
    }
}
void registrarZona(struct Parque *parque, int id, char *nombre, char *tematica, struct Horario apertura, struct Horario cierre, int encargados, int capacidad, int maxAtracciones) {
    struct Zona *nueva;

    if (parque == NULL) return;
    nueva = crearZona(id, nombre, tematica, apertura, cierre, encargados, capacidad, maxAtracciones);
    agregarZona(parque,nueva);
}

/*=========================== REPORTES =========================================*/

void mostrarAtraccionMasVisitada(struct Parque *parque) {
    int i, j;
    struct Atraccion *masVisit = NULL;
    struct Zona *zonaAct = NULL;
    struct Atraccion *actual = NULL;

    if (parque == NULL) return;
    for (i = 0; i < parque->cantZonas; i++) {
        zonaAct = parque->zonas[i];
        for (j = 0; j < zonaAct->cantAtraccion; j++) {
            actual = zonaAct->arrAtracciones[j];
            if (masVisit == NULL || actual->totalAtendidos > masVisit->totalAtendidos) {
                masVisit = actual;
            }
        }
    }
    if (masVisit != NULL) {
        printf("\nAtraccion mas visitada:\n");
        printf("Nombre: %s\n", masVisit->nombreJuego);
        printf("Total atendidos: %d\n", masVisit->totalAtendidos);
    }else {
        printf("\nNo hay atracciones registradas\n");
    }
}

void mostrarZonaLlenas(struct Parque *parque) {
    int i, encontradas = 0;
    if (parque == NULL) return;
    for (i = 0; i < parque->cantZonas; i++) {
        if (parque->zonas[i]->visitantesActuales >= parque->zonas[i]->capacidadMaxima || parque->zonas[i]->estadoAforo == 1) {
            printf("\nID Zona: %d\n", parque->zonas[i]->idZona);
            printf("\nNombre: %s\n", parque->zonas[i]->nombre);
            printf("\nVisitantes: %d\n", parque->zonas[i]->visitantesActuales);
            printf("Capacidad maxima: %d\n", parque->zonas[i]->capacidadMaxima);
            encontradas += 1;
        }
    }
    if (encontradas == 0) {
        printf("\nNo hay zonas llenas\n");
    }
}

/*================================ FAMILIAS =================================================*/

struct NodoFamilia *crearFamilia(char *rut, char *apellido, int integrantes) {
    struct NodoFamilia *nuevo;
    nuevo = (struct NodoFamilia *) malloc (sizeof(struct NodoFamilia));
    nuevo->familia = (struct Familia *) malloc (sizeof(struct Familia));

    nuevo->familia->rutResponsableFamiliar = (char*) malloc ((strlen(rut) + 1) * sizeof(char));
    strcpy(nuevo->familia->rutResponsableFamiliar, rut);

    nuevo->familia->apellidoFamilia = (char*) malloc ((strlen(apellido) + 1) * sizeof(char));
    strcpy(nuevo->familia->apellidoFamilia, apellido);

    nuevo->familia->cantidadIntegrantes = integrantes;

    nuevo->sig = NULL;
    return nuevo;
}

void registrarFamilia(struct Parque *parque, struct NodoFamilia *nueva) {
    struct NodoFamilia *rec;
    if (parque == NULL || nueva == NULL) return;
    if (parque->listaFamilias == NULL) {
        parque->listaFamilias = nueva;
        return;
    }
    rec = parque->listaFamilias;

    while (rec->sig != NULL) {
        rec = rec->sig;
    }
    rec->sig = nueva;
}

struct NodoFamilia *buscarFamilia(struct Parque *parque, char *rutBuscado) {
    struct NodoFamilia *rec;
    if (parque == NULL ) return NULL;
    rec = parque->listaFamilias;

    while (rec != NULL) {
        if (strcmp(rec->familia->rutResponsableFamiliar, rutBuscado) == 0) {
            return rec;
        }
        rec = rec->sig;
    }
    return NULL;
}

void mostrarFamilia(struct NodoFamilia *listFam) {
    if (listFam == NULL) return;

    printf("\n========== FAMILIA ==========\n");
    printf("Rut Responsable: %s\n", listFam->familia->rutResponsableFamiliar);
    printf("Cantidad integrantes : %d\n", listFam->familia->cantidadIntegrantes);
}

void eliminarFamilia(struct Parque *parque, char *rutBuscado) {
    struct NodoFamilia *actual;
    struct NodoFamilia *anterior;
    if (parque == NULL) return;

    actual = parque->listaFamilias;
    anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->familia->rutResponsableFamiliar, rutBuscado) == 0) {
            if (anterior == NULL) {
                parque->listaFamilias = actual->sig;
            }else {
                anterior->sig = actual->sig;
            }
            printf("\nFamilia eliminada\n");
            return;
        }
        anterior = actual;
        actual = actual->sig;
    }

    printf("\nFamilia no encontrada\n");
}

void listarFamilias(struct Parque *parque){
    struct NodoFamilia *actual;

    if (parque == NULL) return;

    printf("\nFamilias registradas\n");

    /* si es NULL retornamos este mensaje*/
    if (parque->listaFamilias == NULL){
        printf("No hay familias registradas en el parque actualmente.\n");
        return;
    }

    /* de lo contrario al actual lo dejamos de head*/
    actual = parque->listaFamilias;

    /* recorrido hasta encontrar nodo NULL */
    while(actual != NULL){
        printf("Familia: %s | RUT Responsable: %s | Integrantes: %d\n",actual->familia->apellidoFamilia, actual->familia->rutResponsableFamiliar, actual->familia->cantidadIntegrantes);
        
        /* pasamos al siguiente nodo */
        actual = actual->sig;
    }
}

/*====================================== Atracciones ==============================================================*/

struct Atraccion *crearAtraccion(int id, char *nombre, int estado, int capacidad, int duracion, float alturaMin, int edadMin){
  struct Atraccion *nueva = NULL;
  nueva = (struct Atraccion*)malloc(sizeof(struct Atraccion));

  nueva->idAtraccion = id;

  nueva->nombreJuego = (char*)malloc(strlen(nombre)+1);
  strcpy(nueva->nombreJuego, nombre);

  nueva->estado = estado;
  nueva->capacidadDentroDeAtraccion = capacidad;
  nueva->duracionCiclo = duracion;
  nueva->alturaMinima = alturaMin;
  nueva->edadMinima = edadMin;
  nueva->totalAtendidos = 0; /* por defecto al iniciarse ese día*/
  
  /* creación de la fila de espera para esa atracción y que esté vacía*/
  nueva->fila = (struct filaEspera*)malloc(sizeof(struct filaEspera));
  
  /* inicialización de la fila de espera*/
  nueva->fila->maxCola = 100; /* es un ejemplo, para tener una referencia*/
  nueva->fila->cantidadEnFila = 0;
  nueva->fila->estadoFila = 4; /* por dfecto con espacio disponible*/
  
  /* creación del arreglo para la fila de espera*/
  nueva->fila->cola = (struct Visitante**)malloc(nueva->fila->maxCola * sizeof(struct Visitante*));
  
  return nueva;
}

void agregarAtraccionAZona(struct Zona *zonaSeleccionada, struct Atraccion *nuevaAtraccion){
    if(zonaSeleccionada == NULL || nuevaAtraccion == NULL){
        return;
    }
    if(zonaSeleccionada->cantAtraccion >= zonaSeleccionada->maxatracciones){
        printf("\nNo hay espacio para mas atracciones\n");
        return;
       }

    zonaSeleccionada->arrAtracciones[zonaSeleccionada->cantAtraccion] = nuevaAtraccion;
    zonaSeleccionada->cantAtraccion++;
}

/* funciona tal cuál como un compactar que vimos en el módulo 1*/
void eliminarAtraccionDeZona(struct Zona *zona, int idBuscar){
    int i, j;
    /* funciona como una variable booleana para ver si ya encontramos x atracción*/
    int encontrada = 0;

    if (zona == NULL) return;

    /* buscamos la atracción por su id */
    for (i = 0; i < zona->cantAtraccion; i++) {
        if (zona->arrAtracciones[i] != NULL && zona->arrAtracciones[i]->idAtraccion == idBuscar){
            encontrada = 1;

            /* desplazamos todos los elementos que le siguen hacia la izq*/
            for (j = i; j < zona->cantAtraccion - 1; j++) {
                zona->arrAtracciones[j] = zona->arrAtracciones[j + 1];
            }

            /* dejamos en NULL el último espacio y reducimos el pLibre = cantAtraccion*/
            zona->arrAtracciones[zona->cantAtraccion - 1] = NULL;
            zona->cantAtraccion--;

            printf("\nAtraccion con id %d eliminada correctamente de la zona: '%s'.\n", idBuscar, zona->nombre);
            break;
        }
    }

    if (encontrada == 0) {
        printf("\nAtracción con id %d no encontrada en la zona: '%s'.\n", idBuscar, zona->nombre);
    }
}

void modificarEstadoDeAtraccion(struct Atraccion *atraccion, int nuevoValor){
    if (atraccion == NULL) return;

    /* validación de valor ingresado*/
    if (nuevoValor >= 0 && nuevoValor <= 3){
        atraccion->estado = nuevoValor;
        printf("\nEl estado de la atraccion '%s' se modifico a: ", atraccion->nombreJuego);

        /* validación del nuevoValor = Estado*/
        if (nuevoValor == 0) {
            printf("Cerrada por horario\n");
        }else if (nuevoValor == 1){
            printf("Abierta\n");
        }else if (nuevoValor == 2){
            printf("En mantenimiento\n");
        }else if (nuevoValor == 3){
            printf("Fuera de servicio\n");
        }

    }else{
        printf("\nValor ingresado invalido, ingrese un valor entre 0 y 3.\n");
    }
}

void listarAtraccionesZona(struct Zona *zona){
    int i;
    if (zona == NULL) return;

    printf("\nAtracciones de la zona: %s\n", zona->nombre);

    if (zona->cantAtraccion == 0) {
        printf("No hay atracciones en esta zona..\n");
        return;
    }

    for (i = 0; i < zona->cantAtraccion; i++){
        if (zona->arrAtracciones[i] != NULL){
            /* muestra info básica pero relevantw*/
            printf("\nAtraccion %d\n", i + 1);
            printf("ID: %d\n", zona->arrAtracciones[i]->idAtraccion);
            printf("Nombre: %s\n", zona->arrAtracciones[i]->nombreJuego);

            /* mostrar estado*/
            printf("Estado: ");
            if (zona->arrAtracciones[i]->estado == 0){
                printf("Cerrada por horario\n");
            } else if (zona->arrAtracciones[i]->estado == 1){
                printf("Abierta\n");
            } else if (zona->arrAtracciones[i]->estado == 2){
                printf("En mantenimiento\n");
            } else if (zona->arrAtracciones[i]->estado == 3){
                printf("Fuera de servicio\n");
            }

            /* impresión además de cosas ma´s técnicas*/
            printf("Capacidad: %d personas por ciclo\n", zona->arrAtracciones[i]->capacidadDentroDeAtraccion);
            printf("Duracion del ciclo: %d minutos\n", zona->arrAtracciones[i]->duracionCiclo);
            printf("Altura minima: %.2f mts\n", zona->arrAtracciones[i]->alturaMinima);
            printf("Edad minima: %d anios\n", zona->arrAtracciones[i]->edadMinima);
            printf("Total atendidos hoy: %d\n", zona->arrAtracciones[i]->totalAtendidos);

            /* Mostramos el estado de la fila si existe */
            if (zona->arrAtracciones[i]->fila != NULL){
                printf("Personas en fila actual: %d / %d\n", zona->arrAtracciones[i]->fila->cantidadEnFila, zona->arrAtracciones[i]->fila->maxCola);
            }
        }
    }
}
struct Atraccion *buscarAtraccion(struct Parque *parque, int idBuscar) {
    int i, j;

     if (parque == NULL) return NULL;

    for (i = 0; i < parque ->cantZonas; i++) {
        for (j = 0; j < parque->zonas[i]->cantAtraccion; j++) {
            if(parque->zonas[i]->arrAtracciones[j] != NULL &&
              parque->zonas[i]->arrAtracciones[j]->idAtraccion == idBuscar){

                return parque->zonas[i]->arrAtracciones[j];
              }
        }
    }
    return NULL;

}

void registrarAtraccion(struct Zona *zona, int id, char *nombre, int estado, int capacidad, int duracion, float alturaMin, int edadMin) {
    struct Atraccion *nueva;
    if(zona == NULL) return;

    nueva = crearAtraccion(id, nombre, estado, capacidad, duracion, alturaMin, edadMin);

    agregarAtraccionAZona(zona, nueva);
    printf("\nAtraccion registrada correctamente.\n");
}

void agregarVisitanteAFila(struct Atraccion *atraccion, struct Visitante *visitante){
    int pos;
    if(atraccion == NULL || visitante == NULL) return;

    if(visitante->altura < atraccion->alturaMinima || visitante->edad < atraccion->edadMinima) {
        printf("\nVisitante no puede hacer fila, ya que no cumple con requisitos de altura y/o edad.\n");
        return;
    }
    if(atraccion->fila->cantidadEnFila >= atraccion->fila->maxCola){
        printf("\nFila llena.\n");
        return;
    }

    pos = atraccion->fila->cantidadEnFila;

    atraccion->fila->cola[pos] = visitante;

    atraccion->fila->cantidadEnFila++;

    printf("\nVisitante agregado a la fila.\n");
}

struct Visitante *atenderVisitante(struct Atraccion *atraccion){
    int i;
    struct Visitante *atendido;
    if(atraccion == NULL) return NULL;

    if(atraccion->fila->cantidadEnFila == 0){
        printf("\nFila vacia.\n");
        return NULL;
    }

    atendido = atraccion->fila->cola[0];
    for(i = 0; i < atraccion->fila->cantidadEnFila - 1; i++){
        atraccion->fila->cola[i] =
        atraccion->fila->cola[i+1];
    }
    atraccion->fila->cantidadEnFila--;
    atraccion->totalAtendidos++;

    return atendido;
}

void mostrarFila(struct Atraccion *atraccion){
    int i;
    if(atraccion == NULL) return;

    printf("\n===== FILA =====\n");
    if(atraccion->fila->cantidadEnFila == 0){
        printf("Fila vacia\n");
        return;
    }
    for(i = 0; i < atraccion->fila->cantidadEnFila; i++){
        printf("%d) %s\n", i+1, atraccion->fila->cola[i]->nombre);
    }
}
/*====================================== MENU =======================================================*/

void pausa() {
    printf("\n\nPresione ENTER...");
    getchar();
    getchar();
}

void menuReportes(struct Parque *parque) {
    int opcion;

    if (parque == NULL) return;
    do {
        printf("\n===== MENU REPORTES =====\n\n");

        printf("1. Total visitantes hoy\n");
        printf("2. Total entradas vendidas hoy\n");
        printf("3. Total entradas utilizadas\n");
        printf("4. Ingresos totales\n");
        printf("5. Atraccion mas visitada\n");
        printf("6. Zonas mas ocupadas\n");
        printf("7. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                printf("\nTotal visitantes hoy: %d\n", parque->totalVisitantesHoy);
                pausa();
                break;
            case 2:
                /*Funcion ver Total entradas vendidas hoy*/
                printf("\nTotal entradas vendidas: %d\n", parque->totalEntradasVendidas);
                pausa();
                break;
            case 3:
                /*Funcion ver Total entradas utilizadas*/
                printf("\nTotal Entradas Utilizadas: %d\n", parque->totalEntradasUtilizadas);
                pausa();
                break;
            case 4:
                /*funcion para ver ingresos totales*/
                printf("\nIngresos totales: %.2f\n", parque ->ingresosTotales);
                pausa();
                break;
            case 5:
                /*Funcion mostrar atraccion mas visitada*/
                mostrarAtraccionMasVisitada(parque);
                pausa();
                break;
            case 6:
                /*mostrar zona mas ocupada*/
                mostrarZonaLlenas(parque);
                pausa();
                break;
            case 7:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 7);
}

void menuAtraccion(struct Parque *parque) {
    int opcion;
    int id;
    int idAtr;
    int estado;
    int idZona;
    int capacidad;
    int duracion;
    int edadMin;
    float alturaMin;
    char nombre[50];
    char rut[20];

    struct Atraccion *atr;
    struct NodoVisitante *vis;
    struct Visitante *atendido;
    struct Zona *zona;

    do {
        printf("\n===== MENU ATRACCIONES =====\n\n");

        printf("1. Buscar atraccion\n");
        printf("2. Cambiar estado\n");
        printf("3. Agregar visitante fila\n");
        printf("4. Atender visitante fila\n");
        printf("5. Mostrar Fila\n");
        printf("6. Eliminar atraccion\n");
        printf("7. Registrar atraccion\n");
        printf("8. Listar atracciones de una zona con su ID\n");
        printf("9. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion buscar atraccion*/
                printf("ID atraccion: ");
                scanf("%d",&id);

                atr = buscarAtraccion(parque,id);
                if(atr != NULL) {
                    printf("\nAtraccion encontrada: %s\n",atr->nombreJuego);
                }else {
                    printf("\nNo encontrada\n");
                }
                pausa();
                break;
            case 2:
                /*Fincion cambiar estado*/
                printf("ID atraccion: ");
                scanf("%d",&id);

                atr = buscarAtraccion(parque,id);
                if(atr != NULL){
                    printf("Nuevo estado: ");
                    scanf("%d",&estado);
                    modificarEstadoDeAtraccion(atr,estado);
                }else {
                    printf("\nNo encontrada\n");
                }
                pausa();
                break;
            case 3:
                /*Funcion agregar visitante a fila*/
                printf("ID atraccion: ");
                scanf("%d",&idAtr);
                printf("Rut visitante: ");
                scanf("%s",rut);

                atr = buscarAtraccion(parque,idAtr);
                vis = buscarVisitante(parque->raizVisitantes,rut);
                if(atr != NULL && vis != NULL) {
                    agregarVisitanteAFila(atr,vis->visitante);
                }
                pausa();
                break;
            case 4:
                /*Funcion atender visitante (esta quita de la fila al atender. No se si acerla realmente)*/
                printf("ID atraccion: ");
                scanf("%d",&idAtr);

                atr = buscarAtraccion(parque,idAtr);
                if(atr != NULL){
                    atendido = atenderVisitante(atr);
                    if(atendido != NULL) {
                        printf("\nAtendido: %s\n", atendido->nombre);
                    }
                }
                pausa();
                break;
            case 5:
                /*funcion Mostrar fila*/
                printf("ID atraccion: ");
                scanf("%d",&idAtr);

                atr = buscarAtraccion(parque,idAtr);
                if(atr != NULL) {
                    mostrarFila(atr);
                }
                pausa();
                break;
            case 6:
                /*funcion Eliminar atraccion*/
                printf("ID zona: ");
                scanf("%d",&idZona);

                zona = buscarZona(parque,idZona);
                if(zona != NULL){
                    printf("ID atraccion: ");
                    scanf("%d",&idAtr);
                    eliminarAtraccionDeZona(zona,idAtr);
                }
                pausa();
                break;
            case 7:
                /*funcion registrar atraccion*/
                printf("ID zona: ");
                scanf("%d",&idZona);

                zona = buscarZona(parque,idZona);
                if(zona != NULL){
                    printf("ID atraccion: ");
                    scanf("%d",&id);
                    printf("Nombre: ");
                    scanf(" %[^\n]", nombre);
                    printf("Estado: ");
                    scanf("%d",&estado);
                    printf("Capacidad: ");
                    scanf("%d",&capacidad);
                    printf("Duracion: ");
                    scanf("%d",&duracion);
                    printf("Altura minima: ");
                    scanf("%f",&alturaMin);
                    printf("Edad minima: ");
                    scanf("%d",&edadMin);

                    registrarAtraccion(zona, id, nombre, estado, capacidad, duracion, alturaMin, edadMin);
                }
                pausa();
                break;
            case 8:
                /*funcion Eliminar atraccion*/
                printf("Ingrese el ID de la zona: ");
                scanf("%d",&idZona);

                zona = buscarZona(parque,idZona);
                if(zona != NULL){
                    listarAtraccionesZona(zona);
                }else{
                    printf("\nLa zona no existe\n");
                }
                pausa();
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 8);
}

void menuZonas(struct Parque *parque) {
    int opcion;
    int id;
    int encargados;
    int capacidad;
    int cantAtracciones;
    char nombre[50];
    char tematica[50];

    struct Zona *zona;
    struct Horario apertura;
    struct Horario cierre;
    do {
        printf("\n===== MENU ZONAS =====\n\n");

        printf("1. Buscar zona\n");
        printf("2. Cambiar estado\n");
        printf("3. Registrar zona\n");
        printf("4. Eliminar zona\n");
        printf("5. Listar zonas\n");
        printf("6. Volver\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                /*Funcion buscar zona*/
                printf("ID zona: ");
                scanf("%d",&id);

                zona = buscarZona(parque,id);
                if(zona != NULL) {
                    mostrarZona(zona);
                }else{
                printf("\nZona no encontrada\n");
                }
                pausa();
                break;
            case 2:
                /*Funcion cambiar estado*/
                printf("ID zona: ");
                scanf("%d",&id);

                zona = buscarZona(parque,id);
                if(zona != NULL) {
                    cambiarEstadoZona(zona);
                }else {
                    printf("\nZona no encontrada\n");
                }
                pausa();
                break;
            case 3:
                /*funcion agregar zona*/
                printf("ID: ");
                scanf("%d",&id);
                printf("Nombre: ");
                scanf(" %[^\n]", nombre);
                printf("Tematica: ");
                scanf(" %[^\n]", tematica);
                printf("Hora apertura: ");
                scanf("%d",&apertura.hora);
                printf("Hora cierre: ");
                scanf("%d",&cierre.hora);

                apertura.minuto = 0;
                cierre.minuto = 0;

                printf("Encargados: ");
                scanf("%d",&encargados);
                printf("Capacidad: ");
                scanf("%d",&capacidad);
                printf("Maximo de atracciones: ");
                scanf("%d",&cantAtracciones);
                
                registrarZona(parque, id, nombre, tematica, apertura, cierre, encargados, capacidad,cantAtracciones);
                pausa();
                break;
            case 4:
                /*funcion eliminar zona*/
                printf("ID zona: ");
                scanf("%d",&id);
                eliminarZona(parque,id);
                pausa();
                break;
            case 5:
                listarZonas(parque);
                pausa();
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 5);
}

void menuFamilias(struct Parque *parque) {
    int opcion;
    char rut[20];
    char apellido[20];
    int integrantes;
    struct NodoFamilia *nuevo;
    struct NodoFamilia *encontrado;
    do {
        printf("\n===== MENU FAMILIAS =====\n\n");

        printf("1. Registrar familia\n");
        printf("2. Buscar familia\n");
        printf("3. Eliminar familia\n");
        printf("4. Listar familias\n");
        printf("5. volver\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion agregar familia*/
                printf("Rut responsable: ");
                scanf("%s", rut);
                printf("Apellido familia: ");
                scanf(" %[^\n]", apellido);
                printf("Cantidad integrantes: ");
                scanf("%d", &integrantes);

                nuevo = crearFamilia(rut, apellido, integrantes);
                registrarFamilia(parque, nuevo);
                printf("\nFamilia registrada\n");
                pausa();
                break;
            case 2:
                /*Funcion buscar familia*/
                printf("Rut a buscar: ");
                scanf("%s", rut);
                encontrado = buscarFamilia(parque, rut);
                if (encontrado != NULL) {
                    mostrarFamilia(encontrado);
                }else {
                    printf("\nFamilia no encontrada\n");
                }
                pausa();
                break;
            case 3:
                /*Funcion quitar familia*/
                printf("Rut familia a eliminar: ");
                scanf("%s", rut);

                eliminarFamilia(parque, rut);
                pausa();
                break;
            case 4:
                listarFamilias(parque);
                pausa();
                break;
            case 5:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 5);
}

void menuVisitantes(struct Parque *parque) {
    int opcion;
    int opcionTipo;
    int edad;
    float altura;
    float valor;
    char rut[20];
    char nombre[50];
    char tipo[20];

    struct NodoVisitante *vis;

    do {
        printf("\n===== MENU VISITANTES =====\n\n");

        printf("1. Registrar visitante\n");
        printf("2. Buscar visitante\n");
        printf("3. Registrar Entrada visitante\n");
        printf("4. Eliminar Visitante\n");
        printf("5. Listar Vistantes\n");
        printf("6. Volver\n");

        printf("\nseleccione opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                /*funcion Registrar */
                printf("Rut: ");
                scanf("%s",rut);
                printf("Nombre: ");
                scanf(" %[^\n]", nombre);
                printf("Altura: ");
                scanf("%f",&altura);
                printf("Edad: ");
                scanf("%d",&edad);

                registrarVisitante(parque, rut, nombre, altura, edad);
                pausa();
                break;
            case 2:
                /*funcion Buscar*/
                printf("Rut: ");
                scanf("%s",rut);

                vis = buscarVisitante(parque->raizVisitantes,rut);
                if(vis != NULL) {
                    mostrarVisitante(vis);
                }else {
                    printf("\nVisitante no encontrado\n");
                }
                pausa();
                break;
            case 3:
                /*Funcion agregar entrada*/
                printf("Rut visitante: ");
                scanf("%s",rut);
                vis = buscarVisitante(parque->raizVisitantes,rut);

                if(vis != NULL){
                    printf("\n===== TIPO DE ENTRADA =====\n");
                    printf("1. General\n");
                    printf("2. Rapida\n");
                    printf("3. Familiar\n");
                    printf("Seleccione opcion: ");
                    scanf("%d",&opcionTipo);
                    switch(opcionTipo){
                        case 1:
                            strcpy(tipo,"GENERAL");
                            valor = 10000;
                            break;
                        case 2:
                            strcpy(tipo,"RAPIDA");
                            valor = 15000;
                            break;
                        case 3:
                            strcpy(tipo,"FAMILIAR");
                            valor = 30000;
                            break;
                        default:
                            printf("\nTipo invalido.\n");
                            pausa();
                            break;
                    }
                    if(opcionTipo >= 1 && opcionTipo <= 3){
                        agregarEntradaVisitante( parque, vis->visitante, rand()%10000, tipo, parque->fechaOperacion, valor, 0);
                        printf("\nEntrada registrada correctamente.\n");
                    }
                }
                else{
                    printf("\nVisitante no encontrado.\n");
                }
                pausa();
                break;
            case 4:
                /*Funcion eliminar visitante*/
                printf("Rut visitante a eliminar: ");
                scanf("%s", rut);

                if(buscarVisitante(parque->raizVisitantes, rut) != NULL){
                    parque->raizVisitantes = eliminarVisitante(parque->raizVisitantes, rut);
                    printf("\nVisitante eliminado\n");
                }else{
                    printf("\nVisitante no encontrado\n");
                }
                pausa();
                break;
            case 5:
                listarVisitantes(parque->raizVisitantes);
                pausa();
                break;
            case 6:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 6);
}

void menuPrincipal(struct Parque *parque) {
    int opcion;
    do {
        printf("\n===== IBCLANDIA =====\n\n");

        printf("1. Gestion Visitantes\n");
        printf("2. Gestion Familias\n");
        printf("3. Gestion Zonas\n");
        printf("4. Gestion Atracciones\n");
        printf("5. Reportes\n");
        printf("6. Salir\n");

        printf("Seleccionar accion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuVisitantes(parque);
                break;
            case 2:
                menuFamilias(parque);
                break;
            case 3:
                menuZonas(parque);
                break;
            case 4:
                menuAtraccion(parque);
                break;
            case 5:
                menuReportes(parque);
                break;
            case 6:
                printf("\nSaliendo...");
                break;
            default :
                printf("\nOpcion Invalida");
                pausa();
        }
    }while(opcion != 6);
}

int main(void) {
    struct Parque *parque = NULL;
    int cantZonas;
    if (parque == NULL) {
        printf("Indique maximo de zonas del parque: ");
        scanf("%d", &cantZonas);
        parque = abrirParque("IBCLANDIA", "29/05/2026", 1000, cantZonas);
    }
    printf("\n");
    menuPrincipal(parque);
}
