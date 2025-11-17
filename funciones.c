#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "funciones.h"

// MENÚ PRINCIPAL
int menu()
{
    int opcionN;
    int valido;

    do
    {
        // Se muestra el menú principal
        printf("\n MENU\n");
        printf("1. Registrar Inventario\n");
        printf("2. Registrar Productos\n");
        printf("3. Editar Productos\n");
        printf("4. Eliminar Productos\n");
        printf("5. Mostrar los Productos Registrados y el Iventario\n");
        printf("6. Simulacion de Produccion y Demanda\n");
        printf("7. Salir del programa\n");
        printf("Seleccione una opcion >> ");

        valido = scanf("%d", &opcionN); // Se valida entrada

        if (valido != 1 || opcionN < 1 || opcionN > 7)
        {
            printf("Opcion invalida. Intente nuevamente.\n");

            // Limpia el buffer por si el usuario ingresó letras
            while (getchar() != '\n');
        }

    } while (valido != 1 || opcionN < 1 || opcionN > 7);

    return opcionN;
}

// FUNCIONES AUXILIARES

// Borra el salto de línea que deja fgets
void borrarSaltoLinea(char cadena[])
{
    int len = strlen(cadena) - 1;
    cadena[len] = '\0';
}

// Convierte un caracter a mayúscula
char Mayuscula(char caracter)
{
    return toupper(caracter);
}

// Valida que se ingrese un número entero no negativo
int ValidacionNumeros(const char mensaje[], char nom_prod[])
{
    int numero;
    int valido;

    do
    {
        printf(mensaje, nom_prod);
        valido = scanf("%d", &numero);

        if (valido != 1 || numero < 0)
        {
            printf("Entrada invalida. Intente nuevamente.\n");
            while (getchar() != '\n'); // Limpia basura del buffer
        }

    } while (valido != 1 || numero < 0);

    return numero;
}

// Valida letras S/N
char ValidacionLetras(const char mensaje[])
{
    char letra;
    int valido;

    do
    {
        printf("%s (S/N): ", mensaje);
        valido = scanf(" %c", &letra);

        letra = Mayuscula(letra);

        if (valido != 1 || (letra != 'S' && letra != 'N'))
        {
            printf("Entrada invalida. Ingrese S o N.\n");
            while (getchar() != '\n');
        }

    } while (letra != 'S' && letra != 'N');

    return letra;
}

// Valida que exista al menos un producto antes de usar algunas funciones
int ValidacionProdExist(int cont)
{
    if (cont == 0)
    {
        printf("\nDebe registrar al menos un producto antes de usar esta opcion.\n");
        return 0;
    }
    return 1;
}

// Busca un producto o insumo por nombre (sin distinguir mayúsc/minúsc)
int buscarProducto(char *lista[], int total, char buscado[])
{
    for (int i = 0; i < total; i++)
    {
        if (strcasecmp(lista[i], buscado) == 0)
            return i;
    }
    return -1; // No encontrado
}

// REGISTRAR INVENTARIO
int registrarInsumos(char *nom_insumos[], int inventario[], int totalInsumos)
{
    char continuar = 'S';

    while (continuar == 'S' && totalInsumos < MAX_INSUMOS)
    {
        printf("\nIngrese nombre del insumo %d: ", totalInsumos + 1);
        fflush(stdin);
        fgets(nom_insumos[totalInsumos], LEN_NOMBRE, stdin);
        borrarSaltoLinea(nom_insumos[totalInsumos]);

        // Se pide la cantidad disponible del insumo
        inventario[totalInsumos] = ValidacionNumeros(
            "Cantidad disponible del insumo %s: ",
            nom_insumos[totalInsumos]);

        // Pregunta si desea ingresar otro insumo
        if (totalInsumos + 1 < MAX_INSUMOS)
        {
            continuar = ValidacionLetras("Desea ingresar otro insumo");
        }
        else
        {
            printf("Se alcanzo el maximo de insumos.\n");
            continuar = 'N';
        }

        totalInsumos++;
    }

    return totalInsumos;
}

// REGISTRAR PRODUCTOS
int registrarProductosSistema(
    char *nom_prod[],
    int tiempo[],
    int CantInsumosPorProducto[][MAX_INSUMOS],
    char *nom_insumos[],
    int inventario[],
    int *totalInsumos,
    int totalProd)
{
    char continuar = 'S';

    // Verificar que existan insumos antes de registrar productos
    if (*totalInsumos == 0)
    {
        printf("\nAun no hay insumos registrados.\n");
        continuar = ValidacionLetras("Desea registrar insumos ahora");

        if (continuar == 'S')
        {
            *totalInsumos = registrarInsumos(nom_insumos, inventario, *totalInsumos);
        }
        else
        {
            printf("No se pueden crear productos sin insumos.\n");
            return totalProd;
        }
    }

    // Registrar productos mientras haya espacio
    while (continuar == 'S' && totalProd < MAX_PRODUCTOS)
    {
        printf("\nIngrese el nombre del producto %d: ", totalProd + 1);
        fflush(stdin);
        fgets(nom_prod[totalProd], LEN_NOMBRE, stdin);
        borrarSaltoLinea(nom_prod[totalProd]);

        // Tiempo de producción del producto
        tiempo[totalProd] = ValidacionNumeros(
            "Ingrese el tiempo de produccion (en minutos) del producto %s: ",
            nom_prod[totalProd]);

        // Inicializar la fila de insumos del producto
        for (int j = 0; j < MAX_INSUMOS; j++)
            CantInsumosPorProducto[totalProd][j] = 0;

        // Cuántos insumos utiliza este producto
        int numInsumos = ValidacionNumeros(
            "Cuantos insumos usa el producto %s: ",
            nom_prod[totalProd]);

        // Verificar si pide más insumos que los registrados
        if (numInsumos > *totalInsumos)
        {
            printf("Numero mayor a insumos registrados (%d).\n", *totalInsumos);

            // Permite corregir la cantidad
            do
            {
                continuar = ValidacionLetras("Desea volver a ingresar la cantidad de insumos");
                if (continuar == 'S')
                {
                    numInsumos = ValidacionNumeros(
                        "Cuantos insumos usa el producto %s: ",
                        nom_prod[totalProd]);
                }
            } while (continuar == 'S');

            // También permite agregar más insumos si el usuario desea
            do
            {
                continuar = ValidacionLetras("Desea agregar mas insumos");
                if (continuar == 'S')
                {
                    *totalInsumos = registrarInsumos(nom_insumos, inventario, *totalInsumos);
                }
            } while (continuar == 'S' && *totalInsumos < MAX_INSUMOS);
        }

        // Registro de cada insumo del producto
        for (int i = 0; i < numInsumos; i++)
        {
            char nombreI[LEN_NOMBRE];
            int aux_indice = -1;

            do
            {
                printf("\nNombre del insumo #%d: ", i + 1);
                fflush(stdin);
                fgets(nombreI, LEN_NOMBRE, stdin);
                borrarSaltoLinea(nombreI);

                // Buscar insumo en la lista
                aux_indice = buscarProducto(nom_insumos, *totalInsumos, nombreI);

                if (aux_indice == -1)
                {
                    // Insumo no existe → permite agregarlo
                    char agregar = ValidacionLetras("Desea agregar este insumo al inventario");

                    if (agregar == 'S')
                    {
                        if (*totalInsumos < MAX_INSUMOS)
                        {
                            // Guarda nuevo insumo
                            strcpy(nom_insumos[*totalInsumos], nombreI);

                            inventario[*totalInsumos] = ValidacionNumeros(
                                "Cantidad disponible del insumo %s: ",
                                nom_insumos[*totalInsumos]);

                            aux_indice = *totalInsumos;

                            (*totalInsumos)++;
                            printf("Insumo agregado exitosamente.\n");
                        }
                        else
                        {
                            printf("No se pueden agregar mas insumos.\n");
                        }
                    }
                    else
                    {
                        printf("Intente con otro nombre de insumo.\n");
                    }
                }
                else if (CantInsumosPorProducto[totalProd][aux_indice] != 0)
                {
                    // Evita repetir el mismo insumo en un producto
                    printf("Ese insumo ya fue agregado para este producto.\n");
                    aux_indice = -1;
                }

            } while (aux_indice == -1);

            // Cuántas unidades de ese insumo se requieren
            int cantidadNecesaria = ValidacionNumeros(
                "Cantidad necesaria de %s: ",
                nom_insumos[aux_indice]);

            CantInsumosPorProducto[totalProd][aux_indice] = cantidadNecesaria;
        }

        // Preguntar si desea ingresar otro producto
        if (totalProd + 1 < MAX_PRODUCTOS)
        {
            continuar = ValidacionLetras("Desea ingresar otro producto");
        }
        else
        {
            printf("Se alcanzo el maximo de productos (%d).\n", MAX_PRODUCTOS);
            continuar = 'N';
        }

        totalProd++;
    }

    return totalProd;
}

// EDITAR PRODUCTOS
void editarProducto(char *nom_prod[], int tiempo[],
    int CantInsumosPorProducto[][MAX_INSUMOS],
    char *nom_insumos[], int inventario[],
    int totalProd, int totalInsumos)
{
    if (!ValidacionProdExist(totalProd))
        return;

    char Prod_buscado[LEN_NOMBRE];
    printf("\nIngrese el nombre del producto a editar: ");
    fflush(stdin);
    fgets(Prod_buscado, LEN_NOMBRE, stdin);
    borrarSaltoLinea(Prod_buscado);

    int aux_indice = buscarProducto(nom_prod, totalProd, Prod_buscado);

    if (aux_indice == -1)
    {
        printf("Producto no encontrado.\n");
        return;
    }

    printf("Producto encontrado: %s\n", nom_prod[aux_indice]);

    // Editar nombre
    char opcionL = ValidacionLetras("Desea editar el nombre del producto (S/N)");
    if (opcionL == 'S')
    {
        printf("Ingrese el nuevo nombre del producto: ");
        fflush(stdin);
        fgets(nom_prod[aux_indice], LEN_NOMBRE, stdin);
        borrarSaltoLinea(nom_prod[aux_indice]);
    }

    // Editar tiempo de producción
    opcionL = ValidacionLetras("Desea editar tiempo de produccion (S/N)");
    if (opcionL == 'S')
    {
        tiempo[aux_indice] = ValidacionNumeros(
            "Ingrese el nuevo tiempo (minutos) para '%s': ",
            nom_prod[aux_indice]);
    }

    // Editar insumos
    opcionL = ValidacionLetras("Desea editar los insumos del producto (S/N)");
    if (opcionL == 'S')
    {
        // Limpia insumos anteriores
        for (int j = 0; j < MAX_INSUMOS; j++)
            CantInsumosPorProducto[aux_indice][j] = 0;

        int numInsumos = ValidacionNumeros(
            "Cuantos insumos tendra ahora el producto %s: ",
            nom_prod[aux_indice]);

        if (numInsumos > totalInsumos)
        {
            printf("Numero mayor a insumos registrados (%d). Se ajustara.\n", totalInsumos);
            numInsumos = totalInsumos;
        }

        // Registrar nuevamente insumos del producto
        for (int i = 0; i < numInsumos; i++)
        {
            char nombreI[LEN_NOMBRE];
            int indice = -1;

            do
            {
                printf("Ingrese nombre del insumo #%d: ", i + 1);
                fflush(stdin);
                fgets(nombreI, LEN_NOMBRE, stdin);
                borrarSaltoLinea(nombreI);

                indice = buscarProducto(nom_insumos, totalInsumos, nombreI);

                if (indice == -1)
                {
                    printf("Insumo no encontrado.\n");

                    char agregar = ValidacionLetras("Desea agregar este insumo al inventario");

                    if (agregar == 'S')
                    {
                        if (totalInsumos < MAX_INSUMOS)
                        {
                            strcpy(nom_insumos[totalInsumos], nombreI);

                            inventario[totalInsumos] = ValidacionNumeros(
                                "Cantidad disponible de '%s': ",
                                nom_insumos[totalInsumos]);

                            indice = totalInsumos;
                            totalInsumos++;
                            printf("Insumo agregado exitosamente.\n");
                        }
                        else
                        {
                            printf("No se pueden agregar mas insumos.\n");
                        }
                    }
                    else
                    {
                        printf("Intente con otro nombre de insumo.\n");
                    }
                }
                else if (CantInsumosPorProducto[aux_indice][indice] != 0)
                {
                    printf("Ese insumo ya fue agregado.\n");
                    indice = -1;
                }

            } while (indice == -1);

            int cant = ValidacionNumeros(
                "Cantidad necesaria de '%s': ",
                nom_insumos[indice]);

            CantInsumosPorProducto[aux_indice][indice] = cant;
        }
    }

    printf("Edicion finalizada.\n");
}

// ELIMINAR PRODUCTO
int eliminarProducto(char *nom_prod[],
                     int tiempo[],
                     int CantInsumosPorProducto[][MAX_INSUMOS],
                     int totalProd)
{
    if (!ValidacionProdExist(totalProd))
        return totalProd;

    char Prod_buscado[LEN_NOMBRE];
    printf("\nIngrese el nombre del producto a eliminar: ");
    fflush(stdin);
    fgets(Prod_buscado, LEN_NOMBRE, stdin);
    borrarSaltoLinea(Prod_buscado);

    int aux_indice = buscarProducto(nom_prod, totalProd, Prod_buscado);

    if (aux_indice == -1)
    {
        printf("Producto no encontrado.\n");
        return totalProd;
    }

    // Se recorre y desplaza los productos posteriores para ocupar el espacio
    for (int j = aux_indice; j < totalProd - 1; j++)
    {
        strcpy(nom_prod[j], nom_prod[j + 1]);
        tiempo[j] = tiempo[j + 1];

        for (int k = 0; k < MAX_INSUMOS; k++)
            CantInsumosPorProducto[j][k] = CantInsumosPorProducto[j + 1][k];
    }

    // Limpia el último registro
    nom_prod[totalProd - 1][0] = '\0';
    tiempo[totalProd - 1] = 0;

    for (int k = 0; k < MAX_INSUMOS; k++)
        CantInsumosPorProducto[totalProd - 1][k] = 0;

    totalProd--;
    printf("Producto eliminado correctamente.\n");

    return totalProd;
}

// MOSTRAR PRODUCTOS
void mostrarProductos(char *nom_prod[],
                      int tiempo[],
                      int CantInsumosPorProducto[][MAX_INSUMOS],
                      char *nom_insumos[],
                      int totalProd,
                      int totalInsumos)
{
    if (!ValidacionProdExist(totalProd))
        return;

    printf("\n----- LISTADO DE PRODUCTOS -----\n");

    for (int i = 0; i < totalProd; i++)
    {
        printf("\nProducto %d: %s\n", i + 1, nom_prod[i]);
        printf("  Tiempo por unidad: %d minutos\n", tiempo[i]);
        printf("  Insumos necesarios:\n");

        int any = 0;

        for (int j = 0; j < totalInsumos; j++)
        {
            if (CantInsumosPorProducto[i][j] > 0)
            {
                printf("    - %s : %d unidades\n",
                       nom_insumos[j],
                       CantInsumosPorProducto[i][j]);

                any = 1;
            }
        }

        if (!any)
            printf("    (No definio insumos para este producto)\n");
    }
}

// SIMULACIÓN DE PRODUCCIÓN
void VerificarDemanda(int CantInsumosPorProducto[][MAX_INSUMOS],
                      int tiempo[],
                      char *nom_prod[],
                      char *nom_insumos[],
                      int inventario[],
                      int totalProd,
                      int totalInsumos)
{
    if (!ValidacionProdExist(totalProd))
        return;

    int demanda[MAX_PRODUCTOS] = {0};
    int productosSimulados = 0;
    char continuar = 'S';

    printf("\nPRODUCCION Y VERIFICACION DE DEMANDA\n");

    // Se ingresan productos que se desean fabricar y su demanda
    while (continuar == 'S' && productosSimulados < totalProd)
    {
        char productoBuscado[LEN_NOMBRE];

        printf("\nIngrese el nombre del producto a fabricar: ");
        fflush(stdin);
        fgets(productoBuscado, LEN_NOMBRE, stdin);
        borrarSaltoLinea(productoBuscado);

        int aux_indice = buscarProducto(nom_prod, totalProd, productoBuscado);

        if (aux_indice == -1)
        {
            printf("Producto no encontrado.\n");
        }
        else
        {
            printf("Producto encontrado: %s\n", nom_prod[aux_indice]);

            demanda[aux_indice] =
                ValidacionNumeros("Cantidad demandada de %s: ",
                                  nom_prod[aux_indice]);

            productosSimulados++;
        }

        if (productosSimulados < totalProd)
        {
            continuar = ValidacionLetras("Desea ingresar otro producto para producir");
        }
        else
        {
            printf("Se han ingresado todos los productos disponibles.\n");
            continuar = 'N';
        }
    }

    // Calcular tiempo total requerido
    int tiempoTotalReq = 0;

    for (int i = 0; i < totalProd; i++)
        tiempoTotalReq += tiempo[i] * demanda[i];

    // Tiempo disponible para la simulación
    int tiempoDisponible = ValidacionNumeros(
        "\nIngrese el tiempo total disponible (en minutos): ",
        "");

    // Calcular recursos necesarios
    int recursosNecesarios[MAX_INSUMOS] = {0};

    for (int j = 0; j < totalInsumos; j++)
    {
        for (int i = 0; i < totalProd; i++)
        {
            recursosNecesarios[j] +=
                CantInsumosPorProducto[i][j] * demanda[i];
        }
    }

    // Mostrar resumen
    printf("\n RESUMEN DE CALCULO \n");
    printf("Tiempo total requerido: %d minutos\n", tiempoTotalReq);
    printf("Recursos necesarios para la produccion total:\n");

    for (int j = 0; j < totalInsumos; j++)
    {
        printf(" - %s : %d unidades\n",
               nom_insumos[j], recursosNecesarios[j]);
    }

    printf("\n--- RESULTADO DE LA VERIFICACION ---\n");

    // Copia del inventario para no modificar el original
    int inventarioAux[MAX_INSUMOS];

    for (int j = 0; j < totalInsumos; j++)
        inventarioAux[j] = inventario[j];

    int Fallo = 0;

    // Verificar si hay recursos suficientes
    for (int j = 0; j < totalInsumos; j++)
    {
        if (recursosNecesarios[j] > inventarioAux[j])
            Fallo = 1;
    }

    // Evaluación final
    if (tiempoTotalReq <= tiempoDisponible && !Fallo)
    {
        printf("La fabrica puede cumplir la demanda con los recursos disponibles.\n");
    }
    else
    {
        printf("No se puede cumplir la demanda.\n");

        if (tiempoTotalReq > tiempoDisponible)
        {
            printf("  Tiempo insuficiente. (Requerido: %d minutos. Disponible: %d minutos)\n",
                   tiempoTotalReq, tiempoDisponible);
        }

        for (int j = 0; j < totalInsumos; j++)
        {
            if (recursosNecesarios[j] > inventarioAux[j])
            {
                printf("  Falta %d unidades de %s (Necesario: %d, Disponible: %d)\n",
                       recursosNecesarios[j] - inventarioAux[j],
                       nom_insumos[j],
                       recursosNecesarios[j],
                       inventarioAux[j]);
            }
        }
    }
}

// MOSTRAR INVENTARIO
void mostrarInventario(char *nom_insumos[], int inventario[], int totalInsumos)
{
    if (totalInsumos == 0)
    {
        printf("\nNo hay insumos registrados.\n");
        return;
    }

    printf("\n--- INVENTARIO ---\n");

    for (int i = 0; i < totalInsumos; i++)
    {
        printf("%d) %s : %d unidades\n",
               i + 1, nom_insumos[i], inventario[i]);
    }
}
