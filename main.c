#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"

int main(int argc, char *argv[])
{
    /* arreglos principales */

    // Arreglo donde realmente se guardan todos los nombres (insumos + productos)
    char almacenamiento_nombres[MAX_PRODUCTOS + MAX_INSUMOS][LEN_NOMBRE];

    // Arreglo de punteros que apuntan a la zona de memoria de los insumos
    char *nom_insumos[MAX_INSUMOS];

    // Arreglo de punteros que apuntan a la zona de memoria de los productos
    char *nom_prod[MAX_PRODUCTOS];

    // Inventario disponible para cada insumo
    int inventario[MAX_INSUMOS] = {0};
    int totalInsumos = 0; // Contador de insumos registrados

    // Tiempo de producción de cada producto
    int tiempo[MAX_INSUMOS] = {0};

    // Matriz que guarda cuántos insumos usa cada producto
    int CantInsumosPorProducto[MAX_PRODUCTOS][MAX_INSUMOS];

    int totalProd = 0; // Cantidad total de productos registrados

    char continuar = 'S'; // Controla el ciclo principal del menú

    int demanda[MAX_PRODUCTOS] = {0}; // Demanda ingresada en la simulación

    // Relacionar punteros de insumos con el almacenamiento real
    for (int i = 0; i < MAX_INSUMOS; i++)
    {
        nom_insumos[i] = almacenamiento_nombres[i];
    }

    // Relacionar punteros de productos con otra parte del almacenamiento
    for (int i = 0; i < MAX_PRODUCTOS; i++)
    {
        nom_prod[i] = almacenamiento_nombres[MAX_INSUMOS + i];
    }

    int opcionN;
    printf("\nSistema de Optimizacion de Produccion de Fabrica de Componentes Electronicos\n");

    // Bucle principal del menú
    do
    {
        opcionN = menu(); // Se llama al menú y se obtiene la opción seleccionada

        switch (opcionN)
        {
        case 1:
            // Registrar insumos en el sistema
            totalInsumos = registrarInsumos(nom_insumos, inventario, totalInsumos);
            break;

        case 2:
            // Registrar productos, pero antes exige que existan insumos
            if (totalInsumos == 0)
            {
                char r = ValidacionLetras("No hay insumos registrados. Desea registrar insumos ahora");
                if (r == 'S')
                    totalInsumos = registrarInsumos(nom_insumos, inventario, totalInsumos);
                else
                {
                    printf("Debe registrar insumos para poder crear productos.\n");
                    break;
                }
            }
            totalProd = registrarProductosSistema(
                nom_prod, tiempo, CantInsumosPorProducto,
                nom_insumos, inventario, &totalInsumos, totalProd);
            break;

        case 3:
            // Permite editar un producto existente
            editarProducto(nom_prod, tiempo, CantInsumosPorProducto,
                           nom_insumos, inventario, totalProd, totalInsumos);
            break;

        case 4:
            // Elimina un producto del sistema
            totalProd = eliminarProducto(nom_prod, tiempo,
                                         CantInsumosPorProducto, totalProd);
            break;

        case 5:
            // Muestra productos e inventario actual
            mostrarProductos(nom_prod, tiempo, CantInsumosPorProducto,
                             nom_insumos, totalProd, totalInsumos);
            mostrarInventario(nom_insumos, inventario, totalInsumos);
            break;

        case 6:
            // Simulación de demanda y verificación de recursos
            VerificarDemanda(CantInsumosPorProducto, tiempo,
                             nom_prod, nom_insumos, inventario,
                             totalProd, totalInsumos);
            break;

        case 7:
            printf("Saliendo del Programa...\n");
            break;

        default:
            printf("Opcion Invalida");
            break;
        }

        // Pregunta para repetir el menú
        continuar = ValidacionLetras("\nDesea regresar al menu");

    } while (continuar == 'S');

    return 0;
}
