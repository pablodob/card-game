#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>    //time()
#include <unistd.h>  //unix standard - getpid()->muestra el Process ID o pid del proceso que ejecuta el programa

#define MIN_JUGADORES 1
#define MAX_JUGADORES 5
#define MAX_MANO 14
#define POZO_JUGADOR 5000

// SON TODAS FUNCIONES
// resetearmazo acomoda las cartas del 0 al 39
// mostrarmazo muestra las cartas en el orden que estan
// mostrar hace lo mismo pero diciendo el nombre de la carta que lo encontramos con "carta (a)"
// carta (a) donde a es el valor numérico entre 0 y 39 dice qué carta es
// valor (a) donde a es el valor entre 0 y 39 devuelve el valor de la carta en el juego {1,...,7, 0,5}
// mezclar mezcla las cartas, hay que agregar los rand que no sé por qué no me los agarra, creo que falta la bibliotec


void resetearmazo (int []);
//mostrar mazo
void mostrarmazo(int []);
//qué carta es
void carta (int);
//prueba mostrar carta
void mostrar (int []);
int valor (int);
void mezclar(int []);

int main()
{
    int mazo [40];
    int cartas_repartidas[40];
    int n_repartidas = 0;

    resetearmazo(*mazo);

    int i;
    int jugadores;

    do {
        printf ("Por favor ingrese el número de jugadores (entre %d y %d)\n", MIN_JUGADORES, MAX_JUGADORES);
        printf (">");
        scanf  ("%d", &jugadores);
    }
    while((jugadores <MIN_JUGADORES) || (jugadores >MAX_JUGADORES) );

    int pozo[jugadores];
    int apuesta[jugadores];
    int cartasJugador[jugadores][MAX_MANO];

    //reparte pozo
    for (i=0; i < jugadores; i++){
        pozo[i]=POZO_JUGADOR;
    }

    for (int i=0; i<jugadores; i++){
        cartasJugador[i][0] = mazo [i];
        printf ("Jugador %d ha recibido su carta\n", i+1);
    }
    int n=0;

    for (int i=0; i <cantidad; i++){

        int respuesta;
        srand(getpid());
        int carta_mano;

        if (pozo[i]<0){
                printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                continue;
        }

        //reparte carta
        do {
            carta_mano = rand() %40;
        } while( esta_en(carta_mano,cartas_repartidas[40],n_repartidas))

        n_repartidas++;

        cartasJugador[i][0] = carta_mano;

        printf ("\n\nJugador %d, has recibido un ", i+1);
        carta(cartasJugador[i][0]);

        printf("¿Cuánto quieres apostar?\n");
        printf(">");
        scanf ("%d", &apuesta[i]);

        for (int j=0; j<MAX_MANO; j++){

            do{
                printf ("¿Desea pedir una nueva carta?\n SI: 1 \n NO: 2)\n");
                printf(">");
                scanf ("%d", &respuesta);
            } while (respuesta!=1 && respuesta!=2)

            if (respuesta == 2)
                continue;

            do {
                carta_mano = rand() %40;
            } while( esta_en(carta_mano,cartas_repartidas[40],n_repartidas))

            n_repartidas++;

            cartasJugador[i][j+1]=carta_mano;
            printf ("Jugador %d, has recibido un ",i+1); carta(cartasJugador[i][j+1]);
        }

    }

    return 0;
    }

//TODO: bool esta_en(){}


void resetearmazo (int mazo[]){
    int i;
    for (i=0; i<40; i++)
    {
        mazo[i]=i;
    }
}
//mostrar mazo
void mostrarmazo(int mazo[]){
    int i;
    for (i=0; i<40; i++)
    {
        printf ("%d""\n", mazo[i]);
    }
}
//qué carta es
void carta (int a){
    if ((a)%10< 7){
    printf ("Es un %d ", (a%10)+1);
    }
    else {
    printf ("Es un %d ", a%10+3);
    }
    if (a < 10){
    printf ("de oro \n");
    }
    else if (a < 20){
    printf ("de basto \n");
    }
    else if (a < 30){
    printf ("de copa \n");
    }
    else {
    printf ("de espada \n");
    }
    return 0;
}

//prueba mostrar carta
void mostrar (int mazo[]){
    for (int i=0; i<40; i++)
    {
        carta (mazo[i]);
    }
    return 0;
}

//valor carta
int valor (int a) {
float valorcarta;
    if (a % 10 < 7){
    valorcarta = a+1;
    }
    else {
    valorcarta = 0,5;
    }
    return valorcarta;
}

//intercambiar cartas HAY QUE METER LOS RAND()
void mezclar(int mazo[]) {
    for (int i=0; i<200; i++){
        int aux;
        srand(getpid())
        int random = rand() %40;
        aux = mazo [random];
        mazo [random] = mazo [0];
        mazo [0] = aux;
    }
}
