struct NodoFamilia{
	struct Familia *familia;
	struct NodoFamilia *sig;
};

struct Familia{
    char *rutResponsableFamiliar;
    char *apellidoFamilia;
    int cantidadIntegrantes; /*max 5*/
};

struct NodoEntradas{ /*sea lista simple*/
	struct Entrada *entrada;
	struct NodoEntradas *sig;
};

struct Entrada{
    int idEntrada;
    char *tipoEntrada;
    char *fechaVisita;
    float valor;
    int estado; /* 1 activo o no usado, 0 inactivo o usado*/
};

struct NodoVisitante{ /* abb visitantes*/
    struct Visitante *visitante;
    struct NodoVisitante *izq;
    struct NodoVisitante *der;
};

struct Visitante{
    char *rut; /*podria usarce para recorrer el ABB*/
    char *nombre;
    float altura;
    int edad;
    int dentroParque;
    struct NodoEntradas *listaEntrada; /*lista con entradas del visitante (1 o +1)*/
    struct Familia *familia;
};

struct filaEspera{
	char *tipoFila;
    struct Visitante **cola;
	int maxCola;
	int cantidadEnFila;
    int estadoFila;/*1 llena, 2 suspendida, 3 vacia, 4 aun con espacio*/
};

struct Atraccion{
    int idAtraccion;
    char *nombreJuego;
	char *horaCierre;
	char *horaApertura;
    int estado;
    int capacidadDentroDeAtraccion;
    int duracionCiclo;
    float alturaMinima;
    int edadMinima;
    int totalAtendidos;
    struct filaEspera **tipoFila;
    int cantDeFilas;
};

struct Zona{
    int idZona;
    char *nombre;
    char *tematica;
	char *horaCierre;
	char *horaApertura;
    int numEncargados; /*cantidad de personal encargado de la zona*/
    int capacidadMaxima; /*capacidad maxima de la zona de visitantes*/
    int visitantesActuales;
    int estadoAforo; /*indica si se alcanso el maximo de visitantes*/
    struct Atraccion **arrAtracciones;
	int cantAtraccion;
};

struct Parque{
    char *nombre;
    char *fechaOperacion;
    int capacidadMaxima;
    int visitantesActuales;
    int totalVisitantesHoy;
    int totalEntradasVendidas;
    int totalEntradasUtilizadas;
    float ingresosTotales;
    struct Zona **zonas; /*arreglo de zonas*/
    int cantZonas; /*pLibre para recorrer las zonas*/
    struct NodoVisitante *raizVisitantes; /* abb con los visitantes*/
    struct Familia *listaFamilias;/*lista para ingreasar el ingreso de las familias de manera individual*/
};