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
            if (masVisit != NULL || actual->totalAtendidos > masVisit->totalAtendidos) {
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

    while (rec-sig != NULL) {
        rec = rec->sig;
    }
    rec->sig = nueva;
}

struct NodoFamilia *buscarFamilia(struct Parque *parque, char *rutBuscado) {
    struct NodoFamilia *rec;
    if (parque == NULL ) return NULL;
    rec = parque->listaFamilias;

    while (aux != NULL) {
        if (strcmp(rec->familia->rutResponsableFamiliar, rutBuscado) == 0) {
            return rec;
        }
        rec = rec->sig;
    }
    return NULL;
}

void mostrarFamilia(struct NodoFamilia *listFam) {
    if (listFam != NULL) return;

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

void menuZonas(struct Parque *parque) {
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

void menuFamilias(struct Parque *parque) {
    int opcion;
    char rut[20];
    char apellido[20];
    int integrantes;
    struct Nodofamilia *nuevo;
    struct Nodofamilia *encontrado;
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
                printf("Rut responsable: ");
                scanf("%s", rut);
                printf("Apellido familia: ");
                scanf("%s", apellido);
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
                break;
            default:
                printf("Opcion no valida\n");
                pausa();
        }
    }while(opcion != 4);
}

void menuVisitantes(struct Parque *parque) {
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
    if (parque == Null) parque = abrirParque("IBCLANDIA", "29/05/2026", 1000, MAX_ZONAS);
    menuPrincipal(parque);
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
