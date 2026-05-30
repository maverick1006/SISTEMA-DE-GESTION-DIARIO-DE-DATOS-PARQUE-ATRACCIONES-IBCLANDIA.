struct Parque *abrirParque(char *nombre, char *fecha, int maxCapacidad, int maxZonas){

  struct Parque *parqueCreado = NULL;
  int i;

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
  parqueCreado->cantZonas = 0;

  /* aseguramos que todo el arreglo se inicie en NULL, para hacerlo compacto*/
  for (i = 0; i < maxZonas; i++){
    parqueCreado->zonas[i] = NULL;
  }
  

  parqueCreado->raizVisitantes = NULL;
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
        raiz->izq = insertarVisitanteArbol(raiz->izq, nuevo);
    }else{
        raiz->der = insertarVisitanteArbol(raiz->der, nuevo);
    } 
    return raiz;
}

struct Zona *crearZona(int id, char *nombre, char *tematica, struct Horario apertura, struct Horario cierre, int cant_encargados, int maxVisitantes){

  int i;

  struct Zona *nueva = NULL;

  nueva = (struct Zona*)malloc(sizeof(struct Zona));

  nueva->nombre = (char*)malloc(strlen(nombre)+1);

  strcpy(nueva->nombre, nombre);

  nueva->tematica = (char*)malloc(strlen(tematica)+1);

  strcpy(nueva->tematica, tematica);

  nueva->horaApertura = apertura;
  nueva->horaCierre = cierre;
  
  nueva->idZona = id;
 
  nueva->numEncargados = cant_encargados;

  nueva->capacidadMaxima = maxVisitantes;
  
  /* variables para hacer los conteos*/
  
  nueva->visitantesActuales = 0;
  
  nueva->estadoAforo = 0;
  
  /* el plibre*/
  nueva->cantAtraccion = 0;
  /* aseguramos iniciar todo en NULL, para hacerlo de tipo compacto*/
  for (i = 0; i < MAX_ATRACCIONES; i++){
    nueva->arrAtracciones[i] = NULL;
  }
  
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
    
    /* si es que ya se llegó al máximo de zonas, nos salimos con un return vacío, ya que la función es void, lo mismo en el caso de arriba por si las entradas son NULL*/
    if(elParque->cantZonas >= MAX_ZONAS){
      /* quizás sería bueno agregar mensaje de error acá*/
        return;
    }
    
    /* en este punto se realiza el agregar y se aumenta el pLibre*/
    elParque->zonas[elParque->cantZonas] = nueva;
    
    elParque->cantZonas++;
}

struct Atraccion *crearAtraccion(int id, char *nombre, int estado, int capacidad, int duracion, float alturaMin, int edadMin){

  struct Atraccion *nueva = NULL;

  nueva = (struct Atraccion*)malloc(sizeof(struct Atraccion));

  nueva->idAtraccion = id;

  nueva->nombreJuego = (char*)malloc(strlen(nombre)+1);

  strcpy(nueva->nombreJuego, nombre);

  nueva->estado = estado;

  nueva->capacidadDentroDeAtraccion = capacidad;

  nueva->duracionCiclo = duracion;
void pausa() {
    printf("\n\nPresione ENTER...");
    getchar();
    getchar();
}

void menuReportes() {
    int opcion;
    do {
        printf("\n===== MENU REPORTES =====\n\n");

        printf("1. Total visitantes hoy\n");
        printf("2. Total entradas vendidas\n");
        printf("3. Total entradas utilizadas\n");
        printf("4. Ingresos totales\n");
        printf("5. Atraccion mas visitada\n");
        printf("6. Zona mas ocupada\n");
        printf("7. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion agregar familia*/
                break;
            case 2:
                /*Funcion buscar familia*/\
                break;
            case 3:
                /*Funcion quitar familia*/
                break;
            case 4:
                /*funcion para ver ingresos totales*/
                break;
            case 5:
                /*Funcion mostrar atraccion mas visitada*/
                break;
            case 6:
                /*mostrar zona mas ocupada*/
                break;
            case 7:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 7);
}


void menuAtraccion() {
    int opcion;
    do {
        printf("\n===== MENU ATRACCIONES =====\n\n");

        printf("1. Buscar atraccion\n");
        printf("2. Cambiar estado\n");
        printf("3. Agregar visitante fila\n");
        printf("4. Atender visitante fila\n");
        printf("5. Mostrar Fila\n");
        printf("6. Eliminar atraccion\n");
        printf("7. Registrar atraccion\n");
        printf("8. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion buscar atraccion*/
                break;
            case 2:
                /*Fincion cambiar estado*/
                break;
            case 3:
                /*Funcion agregar visitante a fila*/
                break;
            case 4:
                /*Funcion atender visitante (esta quita de la fila al atender. No se si acerla realmente)*/
                break;
            case 5:
                /*funcion Mostrar fila*/
                break;
            case 6:
                /*funcion Eliminar atraccion*/
                break;
            case 7:
                /*funcion registrar atraccion*/
                break;
            case 8:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 81);
}

void menuZonas() {
    int opcion;
    do {
        printf("\n===== MENU ZONAS =====\n\n");

        printf("1. Buscar zona\n");
        printf("2. Cambiar estado\n");
        printf("3. Registrar zona\n");
        printf("4. Eliminar zona\n");
        printf("5. Volver\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion buscar zona*/
                break;
            case 2:
                /*Funcion cambiar estado*/\
                break;
            case 3:
                /*funcion agregar zona*/
                break;
            case 4:
                /*funcion eliminar zona*/
                break;
            case 5:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 5);
}

void menuFamilias() {
    int opcion;
    do {
        printf("\n===== MENU FAMILIAS =====\n\n");

        printf("1. Registrar familia\n");
        printf("2. Buscar familia\n");
        printf("3. Eliminar familia\n");
        printf("4. volver\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion agregar familia*/
                break;
            case 2:
                /*Funcion buscar familia*/\
                break;
            case 3:
                /*Funcion quitar familia*/
                break;
            case 4:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 4);
}

void menuVisitantes() {
    int opcion;
    do {
        printf("\n===== MENU VISITANTES =====\n\n");

        printf("1. Registrar visitante\n");
        printf("2. Buscar visitante\n");
        printf("3. Registrar Entrada visitante\n");
        printf("4. Eliminar Visitante\n");
        printf("5. Volver\n");

        printf("\nseleccione opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                /*funcion Registrar */
                break;
            case 2:
                /*funcion Buscar*/
                break;
            case 3:
                /*Funcion agregar entrada*/
                break;
            case 4:
                /*Funcion eliminar visitante*/\
                break;
            case 5:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 5);
}

void menuPrincipal() {
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
                menuVisitantes();
                break;
            case 2:
                menuFamilias();
                break;
            case 3:
                menuZonas();
                break;
            case 4:
                menuAtraccion();
                break;
            case 5:
                menuReportes();
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

int main() {
    menuPrincipal();
}

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

void agregarAtraccionAZona(struct Zona *zonaSeleccionada, struct Atraccion *nuevaAtraccion) {
    /* validación de espacio */
    if (zonaSeleccionada->cantAtraccion >= MAX_ATRACCIONES) {
        /* quizás sería bueno agregar mensaje de error acá*/
        return;
    }

    /* Guardamos la atracción en base al pLibre actual y lo incrementamos */
    zonaSeleccionada->arrAtracciones[zonaSeleccionada->cantAtraccion] = nuevaAtraccion;
    zonaSeleccionada->cantAtraccion++;
    
    /* quizás sería bueno agregar mensaje de que sepudo acá */
}




void pausa() {
    printf("\n\nPresione ENTER...");
    getchar();
    getchar();
}

void menuReportes() {
    int opcion;
    do {
        printf("\n===== MENU REPORTES =====\n\n");

        printf("1. Total visitantes hoy\n");
        printf("2. Total entradas vendidas\n");
        printf("3. Total entradas utilizadas\n");
        printf("4. Ingresos totales\n");
        printf("5. Atraccion mas visitada\n");
        printf("6. Zona mas ocupada\n");
        printf("7. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion agregar familia*/
                break;
            case 2:
                /*Funcion buscar familia*/\
                break;
            case 3:
                /*Funcion quitar familia*/
                break;
            case 4:
                /*funcion para ver ingresos totales*/
                break;
            case 5:
                /*Funcion mostrar atraccion mas visitada*/
                break;
            case 6:
                /*mostrar zona mas ocupada*/
                break;
            case 7:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 7);
}


void menuAtraccion() {
    int opcion;
    do {
        printf("\n===== MENU ATRACCIONES =====\n\n");

        printf("1. Buscar atraccion\n");
        printf("2. Cambiar estado\n");
        printf("3. Agregar visitante fila\n");
        printf("4. Atender visitante fila\n");
        printf("5. Mostrar Fila\n");
        printf("6. Eliminar atraccion\n");
        printf("7. Registrar atraccion\n");
        printf("8. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion buscar atraccion*/
                break;
            case 2:
                /*Fincion cambiar estado*/
                break;
            case 3:
                /*Funcion agregar visitante a fila*/
                break;
            case 4:
                /*Funcion atender visitante (esta quita de la fila al atender. No se si acerla realmente)*/
                break;
            case 5:
                /*funcion Mostrar fila*/
                break;
            case 6:
                /*funcion Eliminar atraccion*/
                break;
            case 7:
                /*funcion registrar atraccion*/
                break;
            case 8:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 81);
}

void menuZonas() {
    int opcion;
    do {
        printf("\n===== MENU ZONAS =====\n\n");

        printf("1. Buscar zona\n");
        printf("2. Cambiar estado\n");
        printf("3. Registrar zona\n");
        printf("4. Eliminar zona\n");
        printf("5. Volver\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion buscar zona*/
                break;
            case 2:
                /*Funcion cambiar estado*/\
                break;
            case 3:
                /*funcion agregar zona*/
                break;
            case 4:
                /*funcion eliminar zona*/
                break;
            case 5:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 5);
}

void menuFamilias() {
    int opcion;
    do {
        printf("\n===== MENU FAMILIAS =====\n\n");

        printf("1. Registrar familia\n");
        printf("2. Buscar familia\n");
        printf("3. Eliminar familia\n");
        printf("4. volver\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                /*Funcion agregar familia*/
                break;
            case 2:
                /*Funcion buscar familia*/\
                break;
            case 3:
                /*Funcion quitar familia*/
                break;
            case 4:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 4);
}

void menuVisitantes() {
    int opcion;
    do {
        printf("\n===== MENU VISITANTES =====\n\n");

        printf("1. Registrar visitante\n");
        printf("2. Buscar visitante\n");
        printf("3. Registrar Entrada visitante\n");
        printf("4. Eliminar Visitante\n");
        printf("5. Volver\n");

        printf("\nseleccione opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                /*funcion Registrar */
                break;
            case 2:
                /*funcion Buscar*/
                break;
            case 3:
                /*Funcion agregar entrada*/
                break;
            case 4:
                /*Funcion eliminar visitante*/\
                break;
            case 5:
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 5);
}

void menuPrincipal() {
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
                menuVisitantes();
                break;
            case 2:
                menuFamilias();
                break;
            case 3:
                menuZonas();
                break;
            case 4:
                menuAtraccion();
                break;
            case 5:
                menuReportes();
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

int main() {
    menuPrincipal();
}
