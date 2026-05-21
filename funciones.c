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
