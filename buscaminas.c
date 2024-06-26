#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

/***************************************
*                                      *
*      Definiciones de constantes      *
*                                      *
****************************************/

#define COLUMNAS 8
#define FILAS 8
#define CASILLA_INEXPLORADA '?'
#define CASILLA_SOSPECHOSA 'X'
#define CASILLA_EXPLORADA ' '
#define CASILLA_MARCADA_MINA 'F'
#define MINA 'B'
#define TOTAL_MINAS 16
#define MINA_ENCONTRADA 1
#define CASILLA_EXPLORADA_ANTERIORMENTE 2
#define CASILLA_EXPLORADA_CORRECTAMENTE 3
#define CASILLA_MARCADA_ANTERIORMENTE 4
#define CASILLA_MARCADA_CORRECTAMENTE 5
#define CASILLA_MARCADA_ES_MINA 6
#define CASILLEROS_MARCADOS_LLEGO_AL_LIMITE 7
#define JUGADA_BUSCAR_EFECTUADA_CORRECTAMENTE 8
#define JUGADA_BUSCAR_NO_SE_PUEDE_APLICAR 9
#define GANO 10
#define PERDIO 11
#define ABANDONO 12
#define FECHA_CORRECTA 13
#define FECHA_INCORRECTA 14

/***************************************
*                                      *
*      Definiciones de funciones       *
*                                      *
****************************************/

// Bienvenida al jugador y reglas del juego.
void bienvenida_jugador();

// Solicita movimiento: E -> Explorar, M -> Marcar, B -> Buscar.
char solicitar_movimiento();

// Solicita el numero de Fila
int solicitar_fila();

// Solicita el numero de Columna
int solicitar_columna();

// Solicita el cedula del usuario
void solicitar_cedula();

// Valida la cedula sea entre 7 y 8 inclusive
int validar_cedula(char cedula[]);

// Solicita el dia de nacimiento del usuario
int solicitar_dia();

// Solicita el mes de nacimiento del usuario
int solicitar_mes();

// Solicita el anio de nacimiento del usuario
int solicitar_anio();

// Verifica si una fecha es correcta
// int verificar_fecha(int primero, int segundo, int tercero);

// Solicita el nombre del usuario
void solicitar_y_guardar_nombre(char nombre[10]);

// Solicita el apellido del usuario
void solicitar_y_guardar_apellido(char alias[10]);

// Solicita el alias del usuario
void solicitar_y_guardar_alias(char alias[10]);

// Inicia el tablero por primera vez agregando a las casillas como Inexploradas.
void iniciarTablero(char tablero[FILAS][COLUMNAS]);

// Imprime los cabezales del tabler dejando un espacio al inicio y de A-H.
void imprimirCabezalesColumnasTablero(int cantColumnas);

// Imprime los cabezales y la fila completa, verificando en cada celda lo que hay que colocar, ejemplo: Si la celda actual hay una mina pero mostrarMinas se encuentra en 0, dichas minas se colocan como inexploradas, si se encuentra explorada se coloca la cantidad de minas que hay a su alrededor, si no la misma se va a mostrar por defecto como inexplorada ya que al iniciar tablero quedaron seteadas.
void imprimirFilasCompletasTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas);

// Imprime el tablero en consola, y segun las celdas muestra la info correcta, ya sea si la misma se encuentra Inexplorada, Explorada, Marcada o si es una Bomba.
void imprimirTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas);

// Se convierte un int a char
char convertirEnteroACaracter(int entero);

// Se obtiene un numero random para luego utilizarla en la funcion de agregarMinasAleatoriamente
int generarNumeroRandom(int minimo, int maximo);

// Se agrega la mina generada
void agregarMina(int fila, int columna, char tablero[FILAS][COLUMNAS]);

// Se generan 16 minas en posiciones aleatorias y se agragan al tablero, luego de la primera jugada
void agregarMinasAleatoriamente(char tablero[FILAS][COLUMNAS], int filaEscogida, int columnaEscogida);

// Se calcula la cantidad de minas alrededor de una casilla
int minasCercanasACasillero(int fila, int columna, char tablero[FILAS][COLUMNAS]);

// Se marcan como exploradas las casillas que no tienen minas cercanas, se ejecuta para todas las casillas adyacentes
void explorarCasillerosCerosAdyacentes(int fila, int columna, char tablero[FILAS][COLUMNAS]);

// Actualiza el tablero verdadero colocando si una casilla fue explorada coloca ' ', si es inexplorada aun ?. Y devuelve un status de la jugada como int.
int casillaEscogidaStatus(int fila, int columna, char tablero[FILAS][COLUMNAS], char movimiento);

// Verifica si hay casilleros libres para continuar el juego, y ademas controla que se hayan explorados 48 casillas para finalizar juego
int verificaCasillerosLibres(char tablero[FILAS][COLUMNAS]);

// Verifica la cantidad de casilleros marcados y que no supere la cantidad de los permitidos en este caso 16
int verificaCasillerosMarcados(char tablero[FILAS][COLUMNAS]);

// Verifica la cantidad de casillas marcadas adyacentes a un casillero
int verificaSiElCasilleroSeEncuentraMarcado(int fila, int columna, char tablero[FILAS][COLUMNAS]);

// Funcionalidad buscar -> En casilleros adyacentes al que se realiza la busqueda para ello dicha celda debe ser inexplorada y ademas debe coincidir el numero de minas adyacentes a esta celda con las casillas marcadas como sospechosas adyacentes, se verifica que si se encuentra una mina adyacente que no fue marcada se finaliza el juego
int buscarExplorarCasillerosAdyacentes(int fila, int columna, char tablero[FILAS][COLUMNAS]);

// Funcionalidad para ejecutar el juego dentro de una funcion para limpiar el main y poder usarlo varias veces sin que termine la ejecucion del programa, devuelve un numero, dicho numero hace referencia si perdio, gano o abandono
int ejecutarJuegoLoop();

// Solicita la opcion deseada del menu general
int solicitar_opcion_del_menu();

// Solicita la opcion deseada dentro del menu gestion de usuarios
int solicitar_opcion_de_gestion_de_usuarios();

// Solicita la opcion deseada dentro del menu gestion de consultas
int solicitar_opcion_de_consultas();

int solicitar_opcion_de_alta_o_reactivar();


/***************************************
*                                      *
*       Definiciones de Struct         *
*                                      *
****************************************/

typedef struct {
    int dia;
    int mes;
    int anio;
} fecha;

typedef struct {
    char cedula[9];
    char nombre[15];
    char apellido[15];
    char alias[15];
    fecha nacimiento;
    int activo;
} jugador;

typedef struct {
    jugador jugadores[5];
    int tope;
} jugadores;

typedef struct {
    fecha fPartida;
    char alias[15];
    int resultado;
} partida;

typedef struct {
    partida partidas[50];
    int tope;
} partidas;

// Funciones para trabajar con las estructuras

// Crea un jugador del tipo de struct jugador
jugador agregar_jugador();

partida agregar_partida(jugador j, int res);

// Solicita la fecha de nacimiento del usuario
fecha solicitar_fecha();

// Inserta un jugador en la estructura jugadores dentro del array de jugadores
jugador insertarJugadorEnJugadores(jugadores *js);

// Verifica si un alias ya existe dentro del arreglo
int aliasPerteneceAJugadores(jugador jugadores[], int tope, char aliasAlta[]);

int insertarPartidaEnPartidas(partidas *ps, jugador j, int res);

// Compara dos arreglos para verificar si son iguales o no
int compararArreglos(const char arr1[], const char arr2[]);

// Imprimir un jugador
void imprimirJugador(jugador j);

// Recorre jugadores imprimiendo cada jugador
void imprimirJugadores(jugadores js);

void imprimirJugadoresActivos(jugadores js);

void imprimirPartida(partida p);

void imprimirPartidas(partidas ps);

int cantidadPartidasGanadasPorJugador(partidas ps, char aliasIngresado[]);

void imprimirJugadoresPartidasGanadas(jugador j, int cantidadGanadas);

void imprimirJugadoresActivosYPartidas(partidas ps, jugadores js);

void ordenarArregloPorAlias(jugador arr[], int n);

void copiarCadena(char destino[], char origen[]);

jugador jugadorPorAlias(jugadores *js, char aliasBuscado[]);

void darDeBajaAJugador(jugadores *js, char alias[]);

void darDeAltaAJugador(jugadores *js, char alias[]);

/***************************************
*                                      *
*       Se ejecuta el main()           *
*                                      *
****************************************/

int main() {
    srand(time(NULL));
    jugadores js;
    js.tope = 0;

    partidas ps;
    ps.tope = 0;
    int opcionDelMenu = 0, opcionGestionUsuarios = 0, opcionConsultas = 0, sePermiteJugar = 0;

    bienvenida_jugador();
    opcionDelMenu = solicitar_opcion_del_menu();

    while ( opcionDelMenu != 4 ) {
        if ( opcionDelMenu == 1 ) {
            printf("\n\nHa ingresado a Gestion de Usuarios\n");

            opcionGestionUsuarios = solicitar_opcion_de_gestion_de_usuarios();

            if ( opcionGestionUsuarios == 1 ) {
                int opcion = 0;
                opcion = solicitar_opcion_de_alta_o_reactivar();

                if (opcion == 1) {
                    insertarJugadorEnJugadores(&js);
                } 

                if (opcion == 2) {
                    char aliasDarAlta[15];
                    printf("\nIngrese el alias de quien quiere dar de alta nuevamente: ");
                    scanf(" %s", aliasDarAlta);
                    darDeAltaAJugador(&js, aliasDarAlta);
                }

                if ( opcion == 3 ) {
                    printf("Ha salido del menu dar de alta a jugador");
                }

                
            }

            if ( opcionGestionUsuarios == 2 ) {
                char aliasDarBaja[15];
                printf("\n\nHa ingresado a Baja de jugador");
                printf("\nIngrese el alias de quien quiere dar de baja: ");
                scanf(" %s", aliasDarBaja);

                darDeBajaAJugador(&js, aliasDarBaja);
            }

            if ( opcionGestionUsuarios == 3 ) {
                printf("\n\nHa ingresado a Modificacion de jugador");
            }

            if ( opcionGestionUsuarios == 4 ) {
                printf("\n\nHa salido del menu Gestion de Usuarios");
            }
        }

        if ( opcionDelMenu == 2 ) {
            printf("\n\nHa ingresado a Consultas");
            opcionConsultas = solicitar_opcion_de_consultas();

            if ( opcionConsultas == 1 ) {
                printf("\n\nHa ingresado a Listado de jugadores");

                imprimirJugadoresActivosYPartidas(ps, js);
            }

            if ( opcionConsultas == 2 ) {
                printf("\n\nHa ingresado a Listado de todas las partidas");
                imprimirPartidas(ps);
            }

            if ( opcionConsultas == 3 ) {
                printf("\n\nHa ingresado a Listado de partidas por jugador");
            }

            if ( opcionConsultas == 4 ) {
                printf("\n\nHa ingresado a Listado de partidas por fecha");
            }

            if ( opcionConsultas == 5 ) {
                printf("\n\nHa salido del menu consultas");
            }
        }

        if ( opcionDelMenu == 3 ) {
            int resultado = 0;
            jugador j;
            printf("\n\nHa ingresado a Jugar");
            char aliasEnJugar[15];
            
            solicitar_y_guardar_alias(aliasEnJugar);
            sePermiteJugar = aliasPerteneceAJugadores(js.jugadores, js.tope, aliasEnJugar);

            if ( sePermiteJugar == 1 ) {
                printf("\n\nSe permite jugar, el jugador se encuentra dado de alta y activo");
                j = jugadorPorAlias(&js, aliasEnJugar);
                resultado = ejecutarJuegoLoop();
                insertarPartidaEnPartidas(&ps, j, resultado);
            } else {
                printf("\n\nEl jugador no se encuentra dado de alta! Por favor ingresa los datos para dar de alta al jugador: \n");

                j = insertarJugadorEnJugadores(&js);
                char aliasAlta[15];
                copiarCadena(js.jugadores[js.tope].alias, aliasAlta);
                resultado = ejecutarJuegoLoop();

                insertarPartidaEnPartidas(&ps, j, resultado);
            }
        }

        opcionDelMenu = solicitar_opcion_del_menu();

        if ( opcionDelMenu == 4 ) {
            printf("\n\nHa decidido salir, hasta la proxima!");
        }
    }


    return 0;
}

/***************************************
*                                      *
*          FIN del main()              *
*                                      *
****************************************/

void copiarCadena(char destino[], char origen[]) {
    int i = 0;

    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }

    destino[i] = '\0';
}


/***************************************
*                                      *
*           ESTRUCTUTURAS              *
*                                      *
****************************************/
void imprimirJugador(jugador j) {
    printf("\nCedula: %s", j.cedula);
    printf("\nNombre: %s", j.nombre);
    printf("\nApellido: %s", j.apellido);
    printf("\nAlias: %s", j.alias);
    printf("\nFecha de Nacimiento: %02d/%02d/%04d", j.nacimiento.dia, j.nacimiento.mes, j.nacimiento.anio);
    printf("\nActivo: %d\n", j.activo);
}


void imprimirJugadores(jugadores js) {
    for (int i = 0; i < js.tope; i++) {
        printf("\nJugador %d:\n", i + 1);
        imprimirJugador(js.jugadores[i]);
    }
}

void imprimirJugadoresActivos(jugadores js) {
    for (int i = 0; i < js.tope; i++) {
        if ( js.jugadores[i].activo == 1 ) {
            printf("\nJugador %d:\n", i + 1);
            imprimirJugador(js.jugadores[i]);
        }
    }
}

void imprimirPartida(partida p) {
    printf("\nFecha de la Partida: %02d/%02d/%04d", p.fPartida.dia, p.fPartida.mes, p.fPartida.anio);
    printf("\nAlias jugador: %s", p.alias);

    switch(p.resultado) {
        case 12:
            printf("\nResultado es 12, se rindio!");
            break;
        case 11:
            printf("\nResultado es 11, perdio!");
            break;
        case 10:
            printf("\nResultado es 10, gano!");
            break;
        default:
            printf("\nResultado incorrecto!");
            break;
    }
    
}

void imprimirPartidas(partidas ps) {
    for (int i = 0; i < ps.tope; i++) {
        printf("\nPartida %d:\n", i + 1);
        imprimirPartida(ps.partidas[i]);
    }
}

int cantidadPartidasGanadasPorJugador(partidas ps, char aliasIngresado[]) {
    int iguales = 0;
    int cantidadGanadas = 0;

    for( int i = 0; i < ps.tope; i++ ) {
        iguales = compararArreglos(ps.partidas[i].alias, aliasIngresado);

        if ( iguales == 1 && ps.partidas[i].resultado == 10) {
            
            cantidadGanadas += 1;
        }
    }

    return cantidadGanadas;
}

void imprimirJugadoresPartidasGanadas(jugador j, int cantidadGanadas) {
    printf("\nCedula: %s", j.cedula);
    printf("\nNombre: %s", j.nombre);
    printf("\nApellido: %s", j.apellido);
    printf("\nAlias: %s", j.alias);
    printf("\nFecha de Nacimiento: %02d/%02d/%04d", j.nacimiento.dia, j.nacimiento.mes, j.nacimiento.anio);
    printf("\nActivo: %d", j.activo);
    printf("\nCantidad Partidas Ganadas: %d\n",cantidadGanadas);
}


void imprimirJugadoresActivosYPartidas(partidas ps, jugadores js) {
    int cantidadGanadas = 0, tope = 0;

    ordenarArregloPorAlias(js.jugadores, js.tope);

    if ( ps.tope < js.tope) {
        tope = js.tope;
    } else {
        tope = ps.tope;
    }


    for ( int i = 0; i < tope; i++ ) {
        if ( js.jugadores[i].activo == 1 ) {
            cantidadGanadas = cantidadPartidasGanadasPorJugador(ps, js.jugadores[i].alias);

            imprimirJugadoresPartidasGanadas(js.jugadores[i], cantidadGanadas);
        }
    }
}

jugador agregar_jugador() {
    jugador j;

    solicitar_cedula(j.cedula);
    solicitar_y_guardar_nombre(j.nombre);
    solicitar_y_guardar_apellido(j.apellido);
    solicitar_y_guardar_alias(j.alias);

    printf("\nIngresar fecha de nacimiento: ");
    fecha f = solicitar_fecha();
    j.nacimiento = f;
    j.activo = 1;

    return j;
}

jugador insertarJugadorEnJugadores(jugadores *js) {
    if ( js->tope < ( 5 ) ) {
        jugador j = agregar_jugador();

        js->jugadores[js->tope] = j;
        
        printf("\nEl jugador %s ha sido agregado correctamente!", j.alias);

        js->tope += 1;

        return j;

    } else {
        printf("\nNO ES POSIBLE AGREGAR MAS JUGADORES HAS LLEGADO AL LIMITE DE CANTIDAD DE USUARIOS PERMITIDOS (5)");
    }
}

partida agregar_partida(jugador j, int res) {
    partida p;
    int i = 0;

    while ( i != 15 ) {
        p.alias[i] = j.alias[i];

        if ( j.alias[i] == '\0' ) {
            i = 15;
        } else {
            i++;
        }
    }

    printf("\nIngrese la fecha de hoy: ");
    fecha f = solicitar_fecha();
    p.fPartida = f;
    p.resultado = res;
    

    return p;
}

int insertarPartidaEnPartidas(partidas *ps, jugador j, int res) {
    if ( ps->tope < 50 ) {
        partida p = agregar_partida(j, res);

        ps->partidas[ps->tope] = p;
        
        printf("\nLa partida del jugador %s ha sido agregado correctamente!", p.alias);

        ps->tope += 1;

        return 1;

    } else {
        printf("\nNO ES POSIBLE AGREGAR MAS PARTIDAS HAS LLEGADO AL LIMITE DE CANTIDAD DE PARTIDAS PERMITIDAS (50)");
    }

    return 0;
}


int compararArreglos(const char arr1[], const char arr2[]) {
    int i = 0;

    while (arr1[i] != '\0' && arr2[i] != '\0') {
        if (arr1[i] != arr2[i]) {
            return 0; 
        }
        i++;
    }

    return arr1[i] == '\0' && arr2[i] == '\0'; 
}

void ordenarArregloPorAlias(jugador arr[], int n) {
    jugador temp;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            int k = 0;

            while ( arr[j].alias[k] != '\0' && arr[j + 1].alias[k] != '\0' && arr[j].alias[k] == arr[j + 1].alias[k] ) {
                k++;
            }

            if ( arr[j].alias[k] > arr[j + 1].alias[k] ) {
                temp = arr[j];

                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
            
        }
    }
}

int aliasPerteneceAJugadores(jugador jugadores[], int tope, char aliasAlta[]) {
    int iguales = 0;
    for( int i = 0; i < tope; i++ ) {
        iguales = compararArreglos(jugadores[i].alias, aliasAlta);

        if ( iguales == 1 ) {
            return 1;
            break;
        }
    }

    return iguales;
}

jugador jugadorPorAlias(jugadores *js, char aliasBuscado[]) {
    int iguales = 0;
    
    for( int i = 0; i < js->tope; i++ ) {
        iguales = compararArreglos(js->jugadores[i].alias, aliasBuscado);

        if ( iguales == 1 ) {
            return js->jugadores[i];
        }
    }
}

void darDeBajaAJugador(jugadores *js, char alias[]) {
    int iguales = 0;
    
    for( int i = 0; i < js->tope; i++ ) {
        iguales = compararArreglos(js->jugadores[i].alias, alias);

        if ( iguales == 1 ) {
            js->jugadores[i].activo = 0;
            printf("\nSe ha dado de baja al jugador %s correctamente", js->jugadores[i].alias);
            break;
        } 
    }

    if ( iguales == 0 ) {
        printf("\nNo ha sido posible dar de baja al jugador %s , ya que no se encuentra en la lista dejugadores", alias);
    }
}


void darDeAltaAJugador(jugadores *js, char alias[]) {
    int iguales = 0;
    
    for( int i = 0; i < js->tope; i++ ) {
        iguales = compararArreglos(js->jugadores[i].alias, alias);

        if ( iguales == 1 ) {
            js->jugadores[i].activo = 1;
            printf("\nSe ha dado de alta al jugador %s correctamente", js->jugadores[i].alias);
            break;
        }
    }

    if ( iguales == 0 ) {
        printf("\nNo ha sido posible dar de alta al jugador %s , ya que no se encuentra en la lista dejugadores", alias);
    }
}




/****************************************
*                                       *
*        Bienvenida al usuario          *
*                                       *
****************************************/

void bienvenida_jugador() {
    printf(
        "Bienvenido al juego Buscaminas!\n\n"

        "Antes de comenzar, las reglas del juego son:\n\n"
        "Usted tiene 3 tipos de movimientos:\n\n"
        "E- Explorar: revela lo que se encontraba en una casilla previamente inexplorada.\n"
        "M- Marcar: deja marcada como sospechosa una casilla previamente inexplorada, o remueve la marca de una casilla ya marcada.\n"
        "B- Buscar: revela lo que se encontraba en todas las casillas no exploradas ni marcadas adyacentes a la casilla explorada en donde se realiza la búsqueda.\n"
        "    Solo se puede Buscar en casillas exploradas cuyo numero (entre '0' y '8') coincide con la cantidad de casillas adyacentes marcadas como sospechosas.\n"
        "R- Rendirse: opcion para abandonar el juego.\n\n"
    );
}


/***************************************
*                                      *
*       Solicitar al usuario           *
*                                      *
****************************************/

char solicitar_movimiento() {
    char movimientoEscogido;

    do {
        printf("\n\nPor favor, escoga primero su movimiento, recuerde que debe ser E, M, B o R: ");
        scanf(" %c", &movimientoEscogido);

        movimientoEscogido = toupper(movimientoEscogido);
    } while (movimientoEscogido != 'E' && movimientoEscogido != 'M' && movimientoEscogido != 'B' && movimientoEscogido != 'R');

    return movimientoEscogido;
}

int solicitar_columna() {
    int columnaEscogida = 0;
    
    do {
        printf("\n\nPor favor, escoga la columna, va de 1 a 8: ");
        scanf(" %d", &columnaEscogida);
    } while ( columnaEscogida < 1 || columnaEscogida > 8 );

    return columnaEscogida;
}

int solicitar_fila() {
    int filaEscogida = 0;

    do {
        printf("\n\nPor favor, escoga la fila, va de 1 a 8: ");
        scanf(" %d", &filaEscogida);
    } while ( filaEscogida < 1 || filaEscogida > 8 );

    return filaEscogida;
}

int solicitar_opcion_del_menu() {
    int opcion = 0;

    do {
        printf(
            "\n\nPor favor, escoga la opcion deseada:  "
            "\n 1 - Gestionar Usuario"
            "\n 2 - Consultas"
            "\n 3 - Jugar"
            "\n 4 - Salir"
            "\n\n Ingrese su opcion: "
        );
        scanf(" %d", &opcion);
    } while ( opcion < 1 || opcion > 4 );

    return opcion;
}

int solicitar_opcion_de_gestion_de_usuarios() {
    int opcion = 0;

    do {
        printf(
            "\n\nPor favor, escoga la opcion deseada:  "
            "\n 1 - Alta de jugador"
            "\n 2 - Baja de jugador"
            "\n 3 - Modificacion de jugador"
            "\n 4 - Salir"
            "\n\n Ingrese su opcion: "
        );
        scanf(" %d", &opcion);
    } while ( opcion < 1 || opcion > 4 );

    return opcion;
}


int solicitar_opcion_de_consultas() {
    int opcion = 0;

    do {
        printf(
            "\n\nPor favor, escoga la opcion deseada:  "
            "\n 1 - Listado de jugadores"
            "\n 2 - Listado de todas las partidas"
            "\n 3 - Listado de partidas por jugador"
            "\n 4 - Listado de partidas por fecha"
            "\n 5 - Salir"
            "\n\n Ingrese su opcion: "
        );
        scanf(" %d", &opcion);
    } while ( opcion < 1 || opcion > 5 );

    return opcion;
}

int solicitar_opcion_de_alta_o_reactivar() {
    int opcion = 0;

    do {
        printf(
            "\n\nPor favor, escoga la opcion deseada:  "
            "\n 1 - Dar de alta a un nuevo jugador"
            "\n 2 - Reactivar jugador existente"
            "\n 3 - Salir"
            "\n\n Ingrese su opcion: "
        );
        scanf(" %d", &opcion);
    } while ( opcion < 1 || opcion > 3 );

    return opcion;
}

                


/***************************************
*                                      *
*      Solicitar datos al usuario      *
*                                      *
****************************************/

void solicitar_cedula(char cedula[]) {
    int longitud = 0, validacion = 0;

    do {
        printf("\nIngrese su cedula sin puntos ni guiones (entre 7 y 8 caracteres): ");
        scanf(" %s", cedula);
        
        longitud = 0;

        while ( cedula[longitud] != '\0' ) {
            longitud++;
        }
        

        if (longitud >= 7 && longitud <= 8) {
            validacion = 1;
        } else {
            printf("\nCedula invalida, intente nuevamente");
        }
        

    } while ( validacion == 0 );
}


void solicitar_y_guardar_nombre(char nombre[]) {
    printf("\nIngrese su nombre (Max. de 10 caracteres): ");
    scanf(" %s", nombre);
}

void solicitar_y_guardar_apellido(char apellido[]) {
    printf("\nIngrese su apellido (Max. de 10 caracteres): ");
    scanf(" %s", apellido);
}

void solicitar_y_guardar_alias(char alias[]) {
    printf("\nIngrese su alias (Max. de 10 caracteres): ");
    scanf(" %s", alias);
}


int verificar_fecha(int primero, int segundo, int tercero) {
    int dia = primero, mes = segundo, anio = tercero;
    int biciesto = 1; // false

    if ( anio % 4 == 0 ) {
        biciesto = 0;
    }

    switch ( mes ) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if ( dia <= 31 ) {
                printf("\nLa fecha es valida: %d/%d/%d", dia, mes, anio);
            } else {
                printf("\nLa fecha es invalida: %d/%d/%d", dia, mes, anio);
            }
            return FECHA_CORRECTA;
            break;
        
        case 2: 
            if ( biciesto == 0 ) {
                if ( dia <= 29 ) {
                    printf("\nLa fecha es valida: %d/%d/%d", dia, mes, anio);
                    return FECHA_CORRECTA;
                } else {
                    printf("\nLa fecha es invalida: %d/%d/%d", dia, mes, anio);
                    return FECHA_INCORRECTA;
                }
            } else {
                if ( dia <= 28 ) {
                    printf("\nLa fecha es valida: %d/%d/%d", dia, mes, anio);
                    return FECHA_CORRECTA;
                } else {
                    printf("\nLa fecha es invalida: %d/%d/%d", dia, mes, anio);
                    return FECHA_INCORRECTA;
                }
            }
            break;
        
        case 4: case 6: case 9: case 11:
            if ( dia <= 30 ) {
                printf("\nLa fecha es valida: %d/%d/%d", dia, mes, anio);
                return FECHA_CORRECTA;
            } else {
                printf("\nLa fecha es invalida: %d/%d/%d", dia, mes, anio);
                return FECHA_INCORRECTA;
            }
            break;

        default:
            printf("\nLa fecha es invalida: %d/%d/%d", dia, mes, anio);
            return FECHA_INCORRECTA;
            break;
    }
}

int solicitar_dia() {
    int dia = 0;

    printf("\nIngrese el dia: ");
    scanf("%d", &dia);

    return dia;
}

int solicitar_mes() {
    int mes = 0;

    printf("\nIngrese el mes: ");
    scanf("%d", &mes);

    return mes;
}

int solicitar_anio() {
    int anio = 0;

    printf("\nIngrese el anio: ");
    scanf("%d", &anio);

    return anio;
}


fecha solicitar_fecha() {
    fecha f;
    int dia, mes, anio, statusFecha;

    do {
        dia = solicitar_dia();

        mes = solicitar_mes();

        anio = solicitar_anio();

        statusFecha = verificar_fecha(dia, mes, anio);
    } while ( statusFecha != FECHA_CORRECTA );

    f.dia = dia;
    f.mes = mes;
    f.anio = anio;

    return f;

}


/***************************************
*                                      *
*             TABLERO                  *
*                                      *
****************************************/
void iniciarTablero(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = CASILLA_INEXPLORADA;
        }
    }
}

void imprimirTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas) {
    imprimirCabezalesColumnasTablero(COLUMNAS);
    imprimirFilasCompletasTablero(tablero, mostrarMinas);
}


void imprimirCabezalesColumnasTablero(int cantColumnas) {
    printf("  ");

    for (int j = 0; j < cantColumnas; j++) {
        printf("%c ", 'A' + j);
    }

    printf("\n");
}


void imprimirFilasCompletasTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas) {
    for (int i = 0; i < FILAS; i++) {
        printf("%c ", 'A' + i);

        for (int j = 0; j < COLUMNAS; j++) {
            // Seteamos a la letra a colocar en la casilla como inexplorada
            char letra = CASILLA_INEXPLORADA;

            // Obtenemos la letra actual en la posicion del tablero.
            char letraActual = tablero[i][j];

            // Si la letra actial del tablero es una mina la colocamos como inexplorada para ocultarla
            if (letraActual == MINA) {
                letra = MINA;
            } 

            // Si la letra actual del tablero es explorada se coloca como letra en la celda la cantidad de Minas que se encuentran a su alrededor
            else if (letraActual == CASILLA_EXPLORADA) {
                // Busca las minas cercanas la guarda como int
                int minasCercanas = minasCercanasACasillero(i, j, tablero);

                // Convierte a la cantidad de minas cercanas a char
                letra = convertirEnteroACaracter(minasCercanas);
            }

            // Si la letra actual se encuentra como marcada sospechosa y es una bomba si marca como sospechosa pero en el tablero verdadero se guarda como una F
            else if (letraActual == CASILLA_MARCADA_MINA) {
                letra = CASILLA_SOSPECHOSA;
            } 

            else if (letraActual == CASILLA_SOSPECHOSA) {
                letra = CASILLA_SOSPECHOSA;
            }


            // Si la letra actual es una mina y se mostrarMinas es 1, se perdio el juego y se muestra el tablero como se encuentra mas todas las bombas.
            if ((letraActual == MINA || letraActual == CASILLA_MARCADA_MINA ) && mostrarMinas == 1) {
                letra = MINA;
            }

            printf("%c ", letra);
        }

        printf("\n");
    }
}

char convertirEnteroACaracter(int entero) {
    return entero + '0';
}

int verificaCasillerosLibres(char tablero[FILAS][COLUMNAS]) {
    int cantidadCasillerosExplorados = 0;

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            char actual = tablero[i][j];
            if ( actual == CASILLA_INEXPLORADA ) {
                return 0; // continua el juego ya que se encontro un casillero inexplorado
            }

            if ( actual == CASILLA_EXPLORADA ) {
                cantidadCasillerosExplorados++;
            }
        }
    }

    if ( cantidadCasillerosExplorados >= 48 ) {
        return 1; // Se finaliza el juego ya que que se exploraron 48 casillas
    }

    return 0; // Se continua el juego ya que no hay 48 casillas exploradas, existen casilleros marcados

}

int verificaCasillerosMarcados(char tablero[FILAS][COLUMNAS]) {
    int cantidadCasillerosMarcados = 0;

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            char actual = tablero[i][j];

            if ( actual == CASILLA_SOSPECHOSA || actual == CASILLA_MARCADA_MINA ) {
                cantidadCasillerosMarcados++;
            }


        }
    }

    return cantidadCasillerosMarcados;
}

int verificaSiElCasilleroSeEncuentraMarcado(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    int casilleroMarcado = 0; // 0 - No se encuentra marcado
    char actual = tablero[fila][columna];

    if ( actual == CASILLA_SOSPECHOSA || actual == CASILLA_MARCADA_MINA ) {
        casilleroMarcado = 1;
    }

    return casilleroMarcado;
}


/***************************************
*                                      *
*               MINA                   *
*                                      *
****************************************/

int generarNumeroRandom(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

void agregarMina(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    tablero[fila][columna] = MINA;
}

void agregarMinasAleatoriamente(char tablero[FILAS][COLUMNAS], int filaEscogida, int columnaEscogida) {
    filaEscogida--;
    columnaEscogida--;
    int minasColocadas = 0;
    
    while (minasColocadas < TOTAL_MINAS) {
        int fila = generarNumeroRandom(0, FILAS - 1);
        int columna = generarNumeroRandom(0, COLUMNAS - 1);
        if ((fila != filaEscogida || columna != columnaEscogida) && tablero[fila][columna] != MINA) {
            agregarMina(fila, columna, tablero);
            minasColocadas++;
        }
    }
}

int minasCercanasACasillero(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    int cantidad = 0, filaInicio, filaFin, columnaInicio, columnaFin;

    if (fila <= 0) {
        filaInicio = 0;
    } else {
        filaInicio = fila - 1;
    }

    if (fila + 1 >= FILAS) {
        filaFin = FILAS - 1;
    } else {
        filaFin = fila + 1;
    }

    if (columna <= 0) {
        columnaInicio = 0;
    } else {
        columnaInicio = columna - 1;
    }

    if (columna + 1 >= COLUMNAS) {
        columnaFin = COLUMNAS - 1;
    } else {
        columnaFin = columna + 1;
    }

    for (int i = filaInicio; i <= filaFin; i++) {
        for (int j = columnaInicio; j <= columnaFin; j++) {
            if (tablero[i][j] == MINA || tablero[i][j] == CASILLA_MARCADA_MINA) {
                cantidad++;
            }
        }
    }

    return cantidad;
}

void explorarCasillerosCerosAdyacentes(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
     // Se verifica que la casilla sea solamente inexplorada, sino se sale de la funcion
    if ( tablero[fila][columna] != CASILLA_INEXPLORADA ) {
        return;
    }

    int minasCercanas = minasCercanasACasillero(fila, columna, tablero);
    
    if ( minasCercanas == 0 ) {
        // Marcamos la casilla como explorada
        tablero[fila][columna] = CASILLA_EXPLORADA;
        
        // Exploramos casillas adyacentes 
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) { 
                    int nuevaFila = fila + i;
                    int nuevaColumna = columna + j;
                    if (nuevaFila >= 0 && nuevaFila < FILAS && nuevaColumna >= 0 && nuevaColumna < COLUMNAS) {
                        explorarCasillerosCerosAdyacentes(nuevaFila, nuevaColumna, tablero);
                    }
                }
            }
        }
    }
}

int buscarExplorarCasillerosAdyacentes(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    int cantidadMarcadasComoSospechosa = 0;
    int cantidadDeMinasAdyacentes = minasCercanasACasillero(fila, columna, tablero);
    
    // Exploramos casillas adyacentes 
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i != 0 || j != 0) { 
                int nuevaFila = fila + i;
                int nuevaColumna = columna + j;
                if (nuevaFila >= 0 && nuevaFila < FILAS && nuevaColumna >= 0 && nuevaColumna < COLUMNAS) {
                    if ( tablero[nuevaFila][nuevaColumna] == CASILLA_SOSPECHOSA || tablero[nuevaFila][nuevaColumna] == CASILLA_MARCADA_MINA ) {
                        cantidadMarcadasComoSospechosa++;
                    }
                }
            }
        }
    }

    if ( cantidadMarcadasComoSospechosa == cantidadDeMinasAdyacentes ) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) { 
                    int nuevaFila = fila + i;
                    int nuevaColumna = columna + j;
                    if (nuevaFila >= 0 && nuevaFila < FILAS && nuevaColumna >= 0 && nuevaColumna < COLUMNAS) {
                        if ( tablero[nuevaFila][nuevaColumna] == MINA ) {
                            return MINA_ENCONTRADA;
                        }
                        if ( tablero[nuevaFila][nuevaColumna] == CASILLA_INEXPLORADA ) {
                            tablero[nuevaFila][nuevaColumna] = CASILLA_EXPLORADA;
                        }
                    }
                }
            }
        }

        return JUGADA_BUSCAR_EFECTUADA_CORRECTAMENTE;
    }

    return JUGADA_BUSCAR_NO_SE_PUEDE_APLICAR;
}


int casillaEscogidaStatus(int fila, int columna, char tablero[FILAS][COLUMNAS], char movimiento) {
    fila--;
    columna--;

    // Verificamos el tipo de movimiento
    if ( movimiento == 'E' ) {
        if (tablero[fila][columna] == MINA) {
            return MINA_ENCONTRADA;
        }

        else if (tablero[fila][columna] == CASILLA_EXPLORADA || tablero[fila][columna] == CASILLA_MARCADA_MINA) {
            return CASILLA_EXPLORADA_ANTERIORMENTE;
        }

        else if ( tablero[fila][columna] == CASILLA_SOSPECHOSA ) {
            return CASILLA_MARCADA_ANTERIORMENTE;
        }

        else {
            int minasCercanas = minasCercanasACasillero(fila, columna, tablero);

            if (minasCercanas == 0) {
                // Se exploran casillas adyacentes y ademas se marca el casillero como explorada en dicha funcion, y los casilleros adyacentes que tengan cero minas adyacentes tambbien se maracaran como marcada
                explorarCasillerosCerosAdyacentes(fila, columna, tablero);
            } else {
                tablero[fila][columna] = CASILLA_EXPLORADA;
            }

            return CASILLA_EXPLORADA_CORRECTAMENTE;
        }

        tablero[fila][columna] = CASILLA_EXPLORADA;
        return CASILLA_EXPLORADA_CORRECTAMENTE;

    } 

    if ( movimiento == 'M' ) {
        int cantidadCasillerosMarcados = verificaCasillerosMarcados(tablero);

        int casilleroMarcado = verificaSiElCasilleroSeEncuentraMarcado(fila, columna, tablero);

        // Si no se llega a las 16 casillas marcadas se guardan correctamente segun corresponda, sino se devuelve que se llego al limite de casillas marcadas permitidas
        if (casilleroMarcado == 1 || cantidadCasillerosMarcados <= 15 ) {
            if ( tablero[fila][columna] == CASILLA_EXPLORADA ) {
                return CASILLA_EXPLORADA_ANTERIORMENTE;
            }

            if (tablero[fila][columna] == MINA) {
                tablero[fila][columna] = CASILLA_MARCADA_MINA;
                return CASILLA_MARCADA_CORRECTAMENTE;
            }

            if ( tablero[fila][columna] == CASILLA_INEXPLORADA ) {
                tablero[fila][columna] = CASILLA_SOSPECHOSA;
                return CASILLA_MARCADA_CORRECTAMENTE;
            }

            if ( tablero[fila][columna] == CASILLA_SOSPECHOSA ) {
                tablero[fila][columna] = CASILLA_INEXPLORADA;
                return CASILLA_MARCADA_CORRECTAMENTE;
            }

            if (tablero[fila][columna] == CASILLA_MARCADA_MINA) {
                tablero[fila][columna] = MINA;
                return CASILLA_MARCADA_CORRECTAMENTE;
            }
        } else {
            return CASILLEROS_MARCADOS_LLEGO_AL_LIMITE;
        }
    }


    if ( movimiento == 'B' ) {
        return buscarExplorarCasillerosAdyacentes(fila, columna, tablero);
    }

    return 0;

}

/***************************************
*                                      *
*        EJECUCION del JUEGO           *
*                                      *
****************************************/



int ejecutarJuegoLoop() {
    int resultadoJuego = 0;
     // Se declara el tablero 8x8
    char tablero[FILAS][COLUMNAS];
    int mostrarMinas = 0;
    srand(time(NULL));

    // Se inicia el tabler con ?
    iniciarTablero(tablero);

    bienvenida_jugador();

    // Se imprime el tablero por primera vez
    imprimirTablero(tablero, mostrarMinas);

    printf("\n\nSu primer jugada corresponde a Explorar (E), se le pedira ingresar fila y columna para comenzar el juego:\n");
    
    // Primero se realiza el primer jugada, para luego colocar las Minas de forma aleatoria
    char movimiento = 'E';
    int filaEscogida = solicitar_fila();
    int columnaEscogida = solicitar_columna();
    agregarMinasAleatoriamente(tablero, filaEscogida, columnaEscogida);
    casillaEscogidaStatus(filaEscogida, columnaEscogida, tablero, movimiento);
    imprimirTablero(tablero, mostrarMinas);

    do {
        movimiento = solicitar_movimiento();

        if ( movimiento != 'R' ) {
            filaEscogida = solicitar_fila();
            columnaEscogida = solicitar_columna();

            int status = casillaEscogidaStatus(filaEscogida, columnaEscogida, tablero, movimiento);

            imprimirTablero(tablero, mostrarMinas);


            if (verificaCasillerosLibres(tablero)) {
                printf("\n ----> Pfff fue pura suerte, la proxima no sera tan facil <-----\n\n");
                resultadoJuego = GANO;
                break;
            } 

            else if (status == MINA_ENCONTRADA) {
                printf("\n ----> PERDISTE Suerte la proxima, se nota que la necesitas <-----\n\n");
                mostrarMinas = 1;
            }
            
            else if (status == CASILLA_EXPLORADA_ANTERIORMENTE) {
                printf("\n ----> CASILLA EXPLORADA ANTERIORMENTE <-----\n\n");
            } 

            else if (status == CASILLA_MARCADA_ANTERIORMENTE) {
                printf("\n ----> CASILLA MARCADA ANTERIORMENTE <-----\n\n");
            } 

            else if (status == CASILLA_MARCADA_CORRECTAMENTE) {
                printf("\n ----> CASILLA MARCADA CORRECTAMENTE <-----\n\n");
            } 

            else if (status == CASILLA_EXPLORADA_CORRECTAMENTE) {
                printf("\n ----> CASILLA EXPLORADA CORRECTAMENTE <-----\n\n");
            } 

            else if (status == CASILLEROS_MARCADOS_LLEGO_AL_LIMITE) {
                printf("\n ----> CASILLEROS MARCADOS LLEGO AL LIMITE DE 16 <-----\n\n");
            }

            else if (status == JUGADA_BUSCAR_EFECTUADA_CORRECTAMENTE) {
                printf("\n ----> JUGADA BUSCAR EFECTUADA CORRECTAMENTE <-----\n\n");
            }

            else if (status == JUGADA_BUSCAR_NO_SE_PUEDE_APLICAR) {
                printf("\n ----> JUGADA BUSCAR NO SE PUEDE APLICAR <-----\n\n");
            }

            imprimirTablero(tablero, mostrarMinas);
            if (mostrarMinas == 1) {
                resultadoJuego = PERDIO;
                break;
            }

        } else {
            resultadoJuego = ABANDONO;
            break;
        }


    } while (1);

    return resultadoJuego;
}