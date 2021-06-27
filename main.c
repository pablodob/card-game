#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>    //time()
#include <unistd.h>  //unix standard - getpid()->muestra el Process ID o pid del proceso que ejecuta el programa

#define MIN_JUGADORES 1
#define MAX_JUGADORES 5
#define MAX_MANO 12
#define PUNTAJE_MAX 7,5
#define POZO_JUGADOR 5000
#define APUESTA_MIN 50
#define APUESTA_MAX 200
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

// SON TODAS FUNCIONES
// resetearmazo acomoda las cartas del 0 al 39
// mostrarmazo muestra las cartas en el orden que estan
// mostrar hace lo mismo pero diciendo el nombre de la carta que lo encontramos con "carta (a)"
// carta (a) donde a es el valor numérico entre 0 y 39 dice qué carta es
// valor (a) donde a es el valor entre 0 y 39 devuelve el valor de la carta en el juego {1,...,7, 0,5}
// mezclar mezcla las cartas, hay que agregar los rand que no sé por qué no me los agarra, creo que falta la bibliotec


void resetearmazo (int []);
void mostrarmazo(int []);
void num2carta (int , int []);
void carta2impresion (int []);
void imprimir(int );
void mostrar (int []);
float valor (int);
void mezclar(int []);

int main()
{
    int mazo [40];
    int cartas_repartidas[40];
    int n_repartidas = 0;

    srand(getpid());

    resetearmazo(mazo);
    mostrarmazo(mazo);
    mezclar(mazo);
    mostrarmazo(mazo);

    int i,j;
    int jugadores;

    do {
        printf ("Por favor ingrese el número de jugadores (entre %d y %d)\n", MIN_JUGADORES, MAX_JUGADORES);
        printf (">");
        scanf  ("%d", &jugadores);
    } while( (jugadores <MIN_JUGADORES) || (jugadores >MAX_JUGADORES) );

    int pozo[jugadores];
    int apuesta[jugadores];
    int cartasJugador[jugadores][MAX_MANO];

    //declaro en -1 todas las manos de jugadores
    for (i=0;i<jugadores;i++)
        for(j=0;j<MAX_MANO;j++)
            cartasJugador[i][j]=-1;

    //reparte pozo
    for (i=0; i < jugadores; i++){
        pozo[i]=POZO_JUGADOR;
    }

    for (int i=0; i<jugadores; i++){
        cartasJugador[i][0] = mazo [n_repartidas++];
        printf ("Jugador %d ha recibido su carta\n", i+1);
    }

    for (int i=0; i < jugadores; i++){

        int respuesta=0;
        int carta_mano[2];

        if (pozo[i]<0){
                printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                continue;
        }

        //reparte carta
        cartasJugador[i][0] = mazo[n_repartidas++];

        printf ("\n\nJugador %d, has recibido un ", i+1);
        imprimir(cartasJugador[i][j]);

        do {
            printf("¿Cuánto quieres apostar? (Debe apostar un valor entre %d y %d)\n", APUESTA_MIN, min(pozo[i],APUESTA_MAX)); //ver caso si pozo es menor a apuesta min
            printf(">");
            scanf ("%d", &apuesta[i]);
                    printf ("%d", apuesta[i] );
        } while ( (apuesta[i]<APUESTA_MIN) || (apuesta[i] > min(pozo[i], APUESTA_MAX) ));



        /* to do
        for (int j=1; j<MAX_MANO && respuesta != 2 && puntaje < PUNTAJE_MAX; j++){

            do{
                printf ("¿Desea pedir una nueva carta?\n SI: 1 \n NO: 2\n");
                printf(">");
                scanf ("%d", &respuesta);


                if (respuesta == 1){
                    cartasJugador[i][j]=mazo[n_repartidas++];
                    printf ("Has recibido un ");
                    imprimir(cartasJugador[i][j]);
                }

                num2carta(cartasJugador[i][j], carta_mano);
                puntaje = puntaje + valor(carta_mano[1]);

                printf("\nSu puntaje acualmente es de %f\n", puntaje);

            } while (respuesta!='1' && respuesta!='2');

        }*/

    }

    return 0;
    }


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
        imprimir(mazo[i]);
        printf (", ");
    }
    printf ("\n");
}

//qué carta es
void num2carta (int num, int carta[]){
    carta [0] = (num/10);

    if ((num)%10< 7){
        carta[1] = (num%10)+1;
    }
    else {
        carta[1] = (num%10+3);
    }
}

void carta2impresion (int carta[]){

    printf("%d de ", carta[1]);
    switch (carta[0]){
        case 0: {
            printf("oro");
            break;
        }
        case 1: {
            printf("basto");
            break;
        }
        case 2: {
            printf("copa");
            break;
        }
        case 3: {
            printf("espada");
            break;
            }
    }
}

void imprimir(int num){
    int carta[2];
    num2carta(num , carta);
    carta2impresion (carta);
}

//prueba mostrar carta
void mostrar (int mazo[]){
    for (int i=0; i<40; i++)
    {
        imprimir (mazo[i]);
    }
}

//valor carta
float valor (int num) {
    float valorcarta;
    if (num  <= 7){
        valorcarta = num;
    }
    else {
        valorcarta = 0.5;
    }
    return valorcarta;
}

void mezclar(int mazo[]) {
    int r1,r2;
    int temp;
    printf("Mezclando mazo \n");
    for (int i=0; i<100; i++){
        r1 = rand() %40;
        r2 = rand() %40;

        printf("Intecambio carta %d por carta %d, ", r1, r2);

        temp = mazo[r1];
        mazo[r1] = mazo[r2];
        mazo[r2] = temp;
    }
}
