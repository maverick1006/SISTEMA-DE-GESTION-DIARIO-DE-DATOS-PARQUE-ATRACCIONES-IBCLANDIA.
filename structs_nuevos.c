/* acá utilizaremos formato horario tipo militar*/
struct Horario {
    int hora;
    int minuto;
    char temporada; /* A = alta, B = baja, detalle únicamente para tener mayor info para la gestión y tener los 3 datos primitivos*/
};

struct Familia {
    char *rutResponsableFamiliar;
    char *apellidoFamilia;
    int  cantidadIntegrantes;
};
/*nodo para lista simple de familias*/
struct NodoFamilia {
    struct Familia *familia;
    struct NodoFamilia *sig;
};

struct Entrada {
    int   idEntrada;
    char *tipoEntrada;
    char *fechaVisita;
    float valor;
    int   estado; /* 0: no utilizada, 1: utilizada */
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
    int   edad;
    int   dentroParque; /* 0: fuera del parque, 1: dentro del parque */
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
    int   maxCola;
    int   cantidadEnFila;
    int   estadoFila; /* 1 llena, 2 suspendida, 3 vacia, 4 aún con espacio*/
};

struct Atraccion {
    int   idAtraccion;
    char *nombreJuego;
    int   estado; /* 0 cerrada por horario, 1 abierta, 2 en mantenimiento, 3 fuera de servicio */
    int   capacidadDentroDeAtraccion;
    int   duracionCiclo;
    float alturaMinima;
    int   edadMinima;
    int   totalAtendidos;
    struct filaEspera *fila; /* puntero a la fila de espera de la atracción */
};

struct Zona {
    int   idZona;
    char *nombre;
    char *tematica;
    struct Horario horaApertura;
    struct Horario horaCierre;
    int   numEncargados;
    int   capacidadMaxima;
    int   visitantesActuales;
    int   estadoAforo;
    struct Atraccion **arrAtracciones; /* arreglo compacto de punteros a atracciones con tamaño MAX_ATRACCIONES */
    int   cantAtraccion; /* cantidad de atracciones en la zona (pLibre)*/
};

struct Parque {
    char *nombre;
    char *fechaOperacion;
    int   capacidadMaxima;
    int   visitantesActuales;
    int   totalVisitantesHoy;
    int   totalEntradasVendidas;
    int   totalEntradasUtilizadas;
    float ingresosTotales;
    struct Zona **zonas; /* arreglo compacto de punteros a zonas con tamaño MAX_ZONAS */
    int   cantZonas; /* cantidad de zonas en el parque (pLibre)*/
    struct NodoVisitante *raizVisitantes; /* raíz del ABB de visitantes */
    struct NodoFamilia *listaFamilias; /* head a la lista de familias */
};
