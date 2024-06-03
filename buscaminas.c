#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define COLUMNAS 8
#define FILAS 8
#define CASILLA_INEXPLORADA '?'
#define CASILLA_SOSPECHOSA 'X'
#define CASILLA_EXPLORADA ' '
#define MINA 'B'
#define TOTAL_MINAS 16
#define MINA_ENCONTRADA 1
#define CASILLA_EXPLORADA_ANTERIORMENTE 2
#define CASILLA_EXPLORADA_CORRECTAMENTE 3

void bienvenida_jugador();
char solicitar_movimiento();
int solicitar_columna();
int solicitar_fila();
void iniciarTablero(char tablero[FILAS][COLUMNAS]);
void imprimirTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas);
int generarNumeroRandom(int minimo, int maximo);
void agregarMina(int fila, int columna, char tablero[FILAS][COLUMNAS]);
void agregarMinasAleatoriamente(char tablero[FILAS][COLUMNAS]);
int minasCercanasCelda(int fila, int columna, char tablero[FILAS][COLUMNAS]);
char convertirEnteroACaracter(int entero);
int casillaEscogida(int fila, int columna, char tablero[FILAS][COLUMNAS], char movimiento);
int sinCasillasParaExplorar(char tablero[FILAS][COLUMNAS]);

int main() {
    char tablero[FILAS][COLUMNAS];
    int mostrarMinas = 0;
    srand(time(NULL));


    iniciarTablero(tablero);
    agregarMinasAleatoriamente(tablero);
    bienvenida_jugador();

    while (1) {
        imprimirTablero(tablero, mostrarMinas);
            if (mostrarMinas) {
            break;
        }

        char movimiento = solicitar_movimiento();
        int fila = solicitar_fila();
        int columna = solicitar_columna();

        int status = casillaEscogida(fila, columna, tablero, movimiento);

        if (sinCasillasParaExplorar(tablero)) {
            printf("\n ----> Pfff… fue pura suerte, la próxima no será tan fácil <-----\n\n");
            mostrarMinas = 1;
        } else if (status == CASILLA_EXPLORADA_ANTERIORMENTE) {
            printf("\n ----> CASILLA EXPLORADA ANTERIORMENTE <-----\n\n");
        } else if (status == MINA_ENCONTRADA) {
            printf("\n ----> ¡¡¡¡ PERDISTE !!!! Suerte la próxima, se nota que la necesitás <-----\n\n");
            mostrarMinas = 1;
        }
    }

    return 0;
}

void bienvenida_jugador() {
    printf(
        "Bienvenido al juego Buscaminas!\n\n"
        "Usted tiene 3 tipos de movimientos:\n\n"
        "E- Explorar: revela lo que se encontraba en una casilla previamente inexplorada.\n"
        "M- Marcar: deja marcada como sospechosa una casilla previamente inexplorada, o remueve la marca de una casilla ya marcada.\n"
        "B- Buscar: revela lo que se encontraba en todas las casillas no exploradas ni marcadas adyacentes a la casilla explorada en donde se realiza la búsqueda.\n"
        "    Solo se puede Buscar en casillas exploradas cuyo número (entre '0' y '8') coincide con la cantidad de casillas adyacentes marcadas como sospechosas.\n\n"
    );
}

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

void iniciarTablero(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = CASILLA_INEXPLORADA;
        }
    }
}

void imprimirTablero(char tablero[FILAS][COLUMNAS], int mostrarMinas) {
    printf("  ");

    for (int j = 0; j < COLUMNAS; j++) {
        printf("%c ", 'A' + j);
    }

    printf("\n");

    for (int i = 0; i < FILAS; i++) {
        printf("%c ", 'A' + i);

        for (int j = 0; j < COLUMNAS; j++) {
            char letra = CASILLA_INEXPLORADA;
            char letraActual = tablero[i][j];

            if (letraActual == MINA) {
                letra = CASILLA_INEXPLORADA;
            } else if (letraActual == CASILLA_EXPLORADA) {
                int minasCercanas = minasCercanasCelda(i, j, tablero);
                letra = convertirEnteroACaracter(minasCercanas);
            }

            if (letraActual == MINA && mostrarMinas) {
                letra = MINA;
            }

            printf("%c ", letra);
        }

        printf("\n");
    }
}

int generarNumeroRandom(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

void agregarMina(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    tablero[fila][columna] = MINA;
}

void agregarMinasAleatoriamente(char tablero[FILAS][COLUMNAS]) {
    int minas_colocadas = 0;
    while (minas_colocadas < TOTAL_MINAS) {
        int fila = generarNumeroRandom(0, FILAS - 1);
        int columna = generarNumeroRandom(0, COLUMNAS - 1);
        if (tablero[fila][columna] != MINA) {
            agregarMina(fila, columna, tablero);
            minas_colocadas++;
        }
    }
}

int minasCercanasCelda(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
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
            if (tablero[i][j] == MINA) {
                cantidad++;
            }
        }
    }

    return cantidad;
}

char convertirEnteroACaracter(int entero) {
    return entero + '0';
}

int casillaEscogida(int fila, int columna, char tablero[FILAS][COLUMNAS], char movimiento) {
    fila--;
    columna--;

    if ( movimiento == 'E' ) {
        if (tablero[fila][columna] == MINA) {
            return MINA_ENCONTRADA;
        }

        if (tablero[fila][columna] == CASILLA_EXPLORADA) {
            return CASILLA_EXPLORADA_ANTERIORMENTE;
        }

        tablero[fila][columna] = CASILLA_EXPLORADA;
        return CASILLA_EXPLORADA_CORRECTAMENTE;
    } 
    
}


int sinCasillasParaExplorar(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            char actual = tablero[i][j];
            if (actual == CASILLA_INEXPLORADA) {
                return 0;
            }
        }
    }
    return 1;
}