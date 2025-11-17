#ifndef FUNCIONES_H
#define FUNCIONES_H

// Se definen constantes globales para limitar tamaños del sistema
#define MAX_PRODUCTOS 50      // Cantidad máxima de productos que se pueden registrar
#define MAX_INSUMOS 50        // Cantidad máxima de insumos permitidos
#define LEN_NOMBRE 50         // Tamaño máximo de los nombres de productos e insumos

// PROTOTIPOS DE FUNCIONES

// Muestra el menú principal y retorna la opción seleccionada
int menu();

// Elimina el salto de línea que deja fgets al final de las cadenas
void borrarSaltoLinea(char cadena[]);

// Convierte un carácter a mayúscula
char Mayuscula(char caracter);

// Valida la entrada de números enteros no negativos
int ValidacionNumeros(const char mensaje[], char nom_prod[]);

// Valida letras de tipo S/N
char ValidacionLetras(const char mensaje[]);

// Verifica que exista al menos un producto antes de ciertas operaciones
int ValidacionProdExist(int cont);

// Busca un producto o insumo por nombre (sin importar mayúsc/minúsc)
// Retorna el índice donde se encuentra o -1 si no existe
int buscarProducto(char *lista[], int total, char buscado[]);

// Registro de insumos del inventario inicial
int registrarInsumos(char *nom_insumos[], int inventario[], int totalInsumos);

// Registro de nuevos productos con sus tiempos e insumos asociados
int registrarProductosSistema(
    char *nom_prod[],
    int tiempo[],
    int CantInsumosPorProducto[][MAX_INSUMOS],
    char *nom_insumos[],
    int inventario[],
    int *totalInsumos,
    int totalProd);

// Edición de productos ya registrados
void editarProducto(char *nom_prod[],
                    int tiempo[],
                    int CantInsumosPorProducto[][MAX_INSUMOS],
                    char *nom_insumos[],
                    int inventario[],
                    int totalProd,
                    int totalInsumos);

// Eliminación de productos (reduce cantidad total)
int eliminarProducto(char *nom_prod[],
                     int tiempo[],
                     int CantInsumosPorProducto[][MAX_INSUMOS],
                     int totalProd);

// Mostrar todos los productos registrados con sus insumos y tiempos
void mostrarProductos(char *nom_prod[],
                      int tiempo[],
                      int CantInsumosPorProducto[][MAX_INSUMOS],
                      char *nom_insumos[],
                      int totalProd,
                      int totalInsumos);

// Simulación de producción y verificación de demanda
void VerificarDemanda(int CantInsumosPorProducto[][MAX_INSUMOS],
                      int tiempo[],
                      char *nom_prod[],
                      char *nom_insumos[],
                      int inventario[],
                      int totalProd,
                      int totalInsumos);

// Mostrar inventario completo de insumos
void mostrarInventario(char *nom_insumos[],
                       int inventario[],
                       int totalInsumos);

#endif
