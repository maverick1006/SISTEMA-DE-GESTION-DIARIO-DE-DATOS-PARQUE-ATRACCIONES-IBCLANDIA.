# Structs Nuevos — Estructuras de Datos en C

Proyecto de estructuras de datos en C para modelar un parque de atracciones. Incluye estructuras para gestionar visitantes, familias, entradas, atracciones, zonas y el parque completo.

---

## Estructuras incluidas

| Estructura | Descripción |
|---|---|
| `Parque` | Estructura principal que contiene todo el sistema |
| `Zona` | Área del parque con atracciones propias |
| `Atraccion` | Juego o atracción con filas de espera |
| `filaEspera` | Cola de visitantes esperando una atracción |
| `Visitante` | Persona dentro del parque (almacenado en ABB) |
| `NodoVisitante` | Nodo del árbol binario de búsqueda de visitantes |
| `Familia` | Grupo familiar vinculado a un visitante |
| `NodoFamilia` | Nodo de lista enlazada de familias |
| `Entrada` | Ticket de acceso del visitante |
| `NodoEntradas` | Nodo de lista enlazada de entradas |

---

## Cómo contribuir de forma segura

### 1. Clona el repositorio

```bash
git clone https://github.com/maverick1006/structs-nuevos.git
cd structs-nuevos
```

### 2. Crea una rama para tus cambios

**Nunca trabajes directamente en `master`.** Crea siempre una rama con tu nombre o la función que vas a agregar:

```bash
git checkout -b nombre/descripcion-del-cambio
# Ejemplo:
git checkout -b juan/agregar-funcion-buscar-visitante
```

### 3. Haz tus cambios

Edita o agrega código en tu rama. Cuando termines:

```bash
git add .
git commit -m "Descripción clara de lo que hiciste"
# Ejemplo:
git commit -m "Agrega funcion para buscar visitante por RUT en el ABB"
```

### 4. Sube tu rama a GitHub

```bash
git push origin nombre/descripcion-del-cambio
```

### 5. Abre un Pull Request

1. Ve al repositorio en GitHub
2. Haz clic en **"Compare & pull request"**
3. Describe qué cambiaste y por qué
4. Pide revisión a un compañero antes de hacer merge

---

## Reglas para modificar el código

### Al agregar una nueva estructura

- Defínela **antes** de cualquier estructura que la referencie
- Si es un nodo de lista o árbol, agrégala junto a su estructura de datos asociada
- Ejemplo correcto:
```c
struct NuevoElemento {
    int id;
    char *nombre;
};

struct NodoNuevoElemento {
    struct NuevoElemento *elemento;
    struct NodoNuevoElemento *sig;
};
```

### Al agregar campos a una estructura existente

- Verifica que no rompas funciones que ya usan esa estructura
- Agrega campos al **final** de la estructura para minimizar impacto
- Usa `int` para contadores/estados, `char *` para textos, `float` para decimales

### Al agregar funciones

- El nombre debe describir claramente lo que hace: `buscarVisitantePorRut`, `agregarEntradaAVisitante`
- Una función debe hacer **una sola cosa**
- Si la función puede recibir `NULL`, valídalo al inicio:
```c
if (parametro == NULL) return NULL;
```

---

## Estados definidos en el código

| Campo | Valor | Significado |
|---|---|---|
| `Entrada.estado` | `1` | Activa / no usada |
| `Entrada.estado` | `0` | Inactiva / usada |
| `filaEspera.estadoFila` | `1` | Llena |
| `filaEspera.estadoFila` | `2` | Suspendida |
| `filaEspera.estadoFila` | `3` | Vacía |
| `filaEspera.estadoFila` | `4` | Con espacio disponible |
| `Visitante.dentroParque` | `1` | Dentro del parque |
| `Visitante.dentroParque` | `0` | Fuera del parque |

---

## Convenciones de código

- **Nombres en camelCase:** `buscarVisitante`, `totalAtendidos`
- **Punteros con `*` pegado al tipo:** `char *nombre`, `struct Zona **zonas`
- **Comentarios solo cuando el propósito no es obvio**
- **Indentación con tabs**

---

## Autores

- Matias Santelices Sepulveda — estructura base del proyecto
