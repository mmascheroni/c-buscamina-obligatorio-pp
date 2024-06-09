#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

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

// Bienvenida al jugador y reglas del juego.
void bienvenida_jugador();

// Solicita movimiento: E -> Explorar, M -> Marcar, B -> Buscar.
char solicitar_movimiento();

// Solicita el numero de Fila
int solicitar_fila();

// Solicita el numero de Columna
int solicitar_columna();

// Inicia el tablero por primera vez agregando a las casillas como Inexploradas.
void iniciarTablero(char tablero[FILAS][COLUMNAS]);

// Imprime los cabezales del tabler dejando un espacio al inicio y de A-H.
void imprimirCabezalesColumnasTablero(int cantColumnas);

// Imprime los cabezales y la fila completa, verificando en cada celda lo que hay que colocar, ejemplo: Si la celda actual hay una mina pero mostrarMinas se encuentra en 0, dichas minas se colocan como inexploradas, si se encuentra explorada se coloca la cantidad de minas que hay a su alrededor, si no la misma se va a mostrar por defecto como inexplorada ya que al iniciar tablero quedaron seteadas.
void imprimirFilasCompletasTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas);

// Imprime el tablero en consola, y segun las celdas muestra la info correcta, ya sea si la misma se encuentra Inexplorada, Explorada, Marcada o si es una Bomba.
void imprimirTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas);

// Marcar una casilla como sospechosa, se muestra como X
void marcarCasillaComoSospechosa(int fila, int columna, char tablero[FILAS][COLUMNAS]);

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

int verificaSiElCasilleroSeEncuentraMarcado(int fila, int columna, char tablero[FILAS][COLUMNAS]);



/***************************************
*                                      *
*       Se ejecuta el main()           *
*                                      *
****************************************/

int main() {
    // Se declara el tablero 8x8
    char tablero[FILAS][COLUMNAS];
    int mostrarMinas = 0;
    srand(time(NULL));

    // Se inicia el tabler con ?
    iniciarTablero(tablero);

    // marcarCasillaComoSospechosa(1, 1, tablero);
    bienvenida_jugador();

    // Se imprime el tablero por primera vez
    imprimirTablero(tablero, mostrarMinas);

    printf("\n\nSu primer jugada corresponde a Explorar (E), se le pedira ingresar fila y columna para comenzar el juego:\n");
    
    // Primero se realiza el primer jugada, para luego colocar las Minas de forma aleatoria
    char movimiento = 'E';
    int fila_escogida = solicitar_fila();
    int columna_escogida = solicitar_columna();
    agregarMinasAleatoriamente(tablero, fila_escogida, columna_escogida);
    casillaEscogidaStatus(fila_escogida, columna_escogida, tablero, movimiento);
    imprimirTablero(tablero, mostrarMinas);

    do {
        movimiento = solicitar_movimiento();
        fila_escogida = solicitar_fila();
        columna_escogida = solicitar_columna();

        int status = casillaEscogidaStatus(fila_escogida, columna_escogida, tablero, movimiento);

        imprimirTablero(tablero, mostrarMinas);


        if (verificaCasillerosLibres(tablero)) {
            printf("\n ----> Pfff fue pura suerte, la proxima no sera tan facil <-----\n\n");
            mostrarMinas = 1;
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

        imprimirTablero(tablero, mostrarMinas);
        if (mostrarMinas == 1) {
            break;
        }


    } while (1);


    return 0;
}


/****************************************
*                                       *
*        Bienvenida al usuario          *
*                                       *
****************************************/

void bienvenida_jugador() {
    printf(
        "Bienvenido al juego Buscaminas!\n\n"
        "Usted tiene 3 tipos de movimientos:\n\n"
        "E- Explorar: revela lo que se encontraba en una casilla previamente inexplorada.\n"
        "M- Marcar: deja marcada como sospechosa una casilla previamente inexplorada, o remueve la marca de una casilla ya marcada.\n"
        "B- Buscar: revela lo que se encontraba en todas las casillas no exploradas ni marcadas adyacentes a la casilla explorada en donde se realiza la búsqueda.\n"
        "    Solo se puede Buscar en casillas exploradas cuyo numero (entre '0' y '8') coincide con la cantidad de casillas adyacentes marcadas como sospechosas.\n\n"
    );
}


/***************************************
*                                      *
*       Solicitar al usuario           *
*                                      *
****************************************/

char solicitar_movimiento() {
    char movimiento_escogido;

    do {
        printf("\nPor favor, escoga primero su movimiento, recuerde que debe ser E, M o B: ");
        scanf(" %c", &movimiento_escogido);

        movimiento_escogido = toupper(movimiento_escogido);
    } while (movimiento_escogido != 'E' && movimiento_escogido != 'M' && movimiento_escogido != 'B');

    return movimiento_escogido;
}

int solicitar_columna() {
    int columna_escogida = 0;
    
    do {
        printf("\nPor favor, escoga la columna, va de 1 a 8: ");
        scanf(" %d", &columna_escogida);
    } while ( columna_escogida < 1 || columna_escogida > 8 );

    return columna_escogida;
}

int solicitar_fila() {
    int fila_escogida = 0;

    do {
        printf("\nPor favor, escoga la fila, va de 1 a 8: ");
        scanf(" %d", &fila_escogida);
    } while ( fila_escogida < 1 || fila_escogida > 8 );

    return fila_escogida;
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
                letra = CASILLA_INEXPLORADA;
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

void marcarCasillaComoSospechosa(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    fila--;
    columna--;
    tablero[fila][columna] = CASILLA_SOSPECHOSA;
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
    int minas_colocadas = 0;
    
    while (minas_colocadas < TOTAL_MINAS) {
        int fila = generarNumeroRandom(0, FILAS - 1);
        int columna = generarNumeroRandom(0, COLUMNAS - 1);
        if ((fila != filaEscogida || columna != columnaEscogida) && tablero[fila][columna] != MINA) {
            agregarMina(fila, columna, tablero);
            minas_colocadas++;
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
        
        // Exploramos casillas adyacentes y ademas verificamos si tienen o no filas o columnas antes y despues
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
}