struct Parque *abrirParque(char *nombre, char *fecha, int maxCapacidad, int maxZonas){

  struct Parque *parqueCreado = NULL;
  int i;

  parqueCreado = (struct Parque*)malloc(sizeof(struct Parque));

  /* asignación del nombre para el parque*/
  parqueCreado->nombre = (char*)malloc(strlen(nombre) + 1);
  strcpy(ParqueCreado->nombre, nombre);

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
  parqueCreado->cantZonas = 0;

  /* creación del arreglo para las zonas*/
  parqueCreado->zonas = (struct Zona**)malloc(maxZonas * sizeof(struct Zona*));

  parqueCreado->RaizVisitantes = NULL;
  parqueCreado->listaFamilias = NULL;

  return parqueCreado;
  
}


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
        raiz->izq = insertarVisitanteABB(raiz->izq, nuevo);
    }else{
        raiz->der = insertarVisitanteABB(raiz->der, nuevo);
    } 
    return raiz;
}


