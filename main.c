#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>    //time()
#include <unistd.h>  //unix standard - getpid()->muestra el Process ID o pid del proceso que ejecuta el programa

#define MIN_JUGADORES 1
#define MAX_JUGADORES 5
#define MAX_MANO 14
#define PUNTAJE_MAX 7,5
#define POZO_JUGADOR 5000
#define POZO_BANCA 100000
#define TECHO_BANCA 6
#define APUESTA_MIN 100
#define APUESTA_MAX 1500
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

// SON TODAS FUNCIONES
// resetearmazo acomoda las cartas del 0 al 39
// mostrarmazo muestra las cartas en el orden que estan
// mostrar hace lo mismo pero diciendo el nombre de la carta que lo encontramos con "carta (a)"
// carta (a) donde a es el valor num�rico entre 0 y 39 dice qu� carta es
// valor (a) donde a es el valor entre 0 y 39 devuelve el valor de la carta en el juego {1,...,7, 0,5}
// mezclar mezcla las cartas, hay que agregar los rand que no s� por qu� no me los agarra, creo que falta la bibliotec


void resetearmazo (int []);
void mostrarmazo(int []);
int num2palo (int);
int num2numero (int);
void num2carta (int , int []);
void imprimir(int );
void mostrar (int []);
float valor (int);
void mezclar(int []);
float puntaje(int [], int);
int ganador_apuesta(float, float, int[], int);

int main(){
    int mazo [40];
    int cartas_repartidas[40];
    int n_repartidas = 0;

    srand(getpid());

    resetearmazo(mazo);
    mezclar(mazo);

    int i,j;
    int jugadores;

    do {
        printf ("Por favor ingrese el n�mero de jugadores (entre %d y %d)\n", MIN_JUGADORES, MAX_JUGADORES);
        printf (">");
        scanf  ("%d", &jugadores);
    } while( (jugadores <MIN_JUGADORES) || (jugadores >MAX_JUGADORES) );

    int pozo[jugadores];
    int pozoBanca;
    int apuesta[jugadores];

    int cartasJugador[jugadores][MAX_MANO];
    int cartasBanca[MAX_MANO];
    int cantidadCartasJugador[jugadores];
    int cantidadCartasBanca;

    //declaro en -1 todas las manos de jugadores
    for (i=0;i<jugadores;i++)
        for(j=0;j<MAX_MANO;j++)
            cartasJugador[i][j]=-1;

    for(j=0;j<MAX_MANO;j++){
            cartasBanca [j] = -1;
    }

    //reparte pozo
    for (i=0; i < jugadores; i++){
        pozo[i]=POZO_JUGADOR;
    }
    pozoBanca= POZO_BANCA;

    for (int i=0; i<jugadores; i++){
        cantidadCartasJugador[i]=0;
        cartasJugador[i][cantidadCartasJugador[i]] = mazo [n_repartidas++];
        printf ("Jugador %d ha recibido su carta\n", i+1);
    }

    cantidadCartasBanca=0;
    cartasBanca[cantidadCartasBanca]=mazo [n_repartidas++];
    printf ("La Banca ha recibido su carta\n");

    //piden cartas jugadores
    for (int i=0; i < jugadores; i++){

        int respuesta=0;

        if (pozo[i]<0){
                printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                continue;
        }

        printf ("\n\nJugador %d, has recibido un ", i+1);
        imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);

        do {
            printf("�Cu�nto quieres apostar? (Debe apostar un valor entre %d y %d)\n", APUESTA_MIN, min(pozo[i],APUESTA_MAX)); //ver caso si pozo es menor a apuesta min
            printf(">");
            scanf ("%d", &apuesta[i]);
                    printf ("%d", apuesta[i] );
        } while ( (apuesta[i]<APUESTA_MIN) || (apuesta[i] > min(pozo[i], APUESTA_MAX) ));

        do {
            printf ("�Deseas pedir otra carta?\n1: S�\n2: No\n>");
            scanf ("%d",&respuesta);
            if (respuesta == 1){
                cartasJugador[i][cantidadCartasJugador[i]]=mazo[n_repartidas++];
                printf ("\n\nJugador %d, has recibido un ", i+1);
                imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);
            }

        } while (respuesta !=2);


    }

    //pide cartas la banca
        int respuesta=0;

        if (pozoBanca<=0){
                printf ("La Banca no tiene dinero en el pozo... que corra la sangre\n");
        }
        else {

        printf ("La Banca ha recibido un ");
        imprimir(cartasBanca[cantidadCartasBanca++]);
        printf ("\n");

        while (puntaje(cartasBanca,cantidadCartasBanca)<TECHO_BANCA){
            cartasBanca[cantidadCartasBanca]=mazo[n_repartidas++];
            printf ("La Banca pide otra carta\nha recibido un ");
            imprimir(cartasBanca[cantidadCartasBanca++]);
            printf ("\n");
        }
        }



    for (int i=0; i<jugadores; i++){
        for (int j; j<MAX_MANO; j++){
            printf("&d,  ", cartasJugador [i][j]);
        }
        printf ("\n");

    }

    for (int i; i<jugadores; i++){
        printf ("el jugador %d ha recibido %f puntos\n", i+1, puntaje(cartasJugador[i], cantidadCartasJugador[i]));
    }
        printf ("La Banca ha conseguido %f puntos\n", puntaje(cartasBanca, cantidadCartasBanca));

    return 0;

    }

int  num2palo (int numerocarta){
    if (numerocarta >=0 && numerocarta <=39){
        return numerocarta/10;
    }
}

int  num2numero (int numerocarta){
    if (numerocarta>=0 && numerocarta <=39){
        if (numerocarta%10 <= 6){
            return (numerocarta%10)+1;
        }
        else {
            return (numerocarta%10)+3;
        }
    }
}

void num2carta (int num, int carta[]){
    carta [0] = num2palo;
    carta [1] = num2numero(num);
}

int ganador_apuesta(float puntos_j, float puntos_b, int mano[], int cartas_mano){
    /*
    Referencias:
    - 1 gana banca
    - 2 gana jugador 25%
    - 3 gana jugador 50%
    - 4 gana jugador 75%
    - 5 gana jugador 100%
    */
    int carta0[2],carta1[2];
    num2carta(mano[0],carta0);
    num2carta(mano[1],carta1);


    if(puntos_j > 7.5){
        printf("El jugador se pas�. Gana la banca");
        return 1;
    }
    else if(puntos_j < 7,5){
        if (puntos_b>=puntos_j && puntos_b <= 7.5){
            printf("Banca tiene m�s puntos que jugador. Gana banca");
            return 1;
            }
        else{
            printf("Gana el jugador el 25% de la apuesta");
            return 2;
        }
    }
    else if (puntos_b == 7.5){
        printf("La banca tiene 7 y medio. Gana la banca");
        return 1;
        }
    else if (cartas_mano>2){
        printf("El jugador gana con tiene 7 y medio pero con m�s de 2 cartas. Gana 25% de la apuesta");
        return 2;
        }
    else if (carta0[0]!=carta1[0]){
        printf("Gana el jugador con 7 y medio y cartas de distinto palo. Gana 50% de la apuesta");
        return 3;
        }
    else if (carta0[0]==0 && (carta0[1]==12 || carta1[1]==12)){
        printf("Gana el jugador con 7 y medio con un rey y cartas de oro. Gana 100% de la apuesta");
        return 5;
        }
    else
        {
        printf("Gana el jugador con 7 y medio con cartas del mismo palo. Gana 75% de la apuesta");
        return 4;
        }
}

float puntaje(int cartas_mano[], int cartas_cantidad){
    int i;
    float puntos = 0.0;
    for (i=0; i<cartas_cantidad; i++){
        if (num2numero(cartas_mano[i])<=7){
            puntos = puntos + num2numero(cartas_mano[i])*1.0;
        }
        else {
            puntos = puntos + 0.5;
        }
    }
    return puntos;
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

void imprimir(int num){
    printf("%d de ", num2numero(num));
    switch (num2palo(num)){
        case 0: {
            printf("oro ");
            break;
        }
        case 1: {
            printf("basto ");
            break;
        }
        case 2: {
            printf("copa ");
            break;
        }
        case 3: {
            printf("espada ");
            break;
            }
    }

}


//prueba mostrar carta
void mostrar (int mazo[]){
    for (int i=0; i<40; i++)
    {
        imprimir (mazo[i]);
    }
}

void mezclar(int mazo[]) {
    int r1,r2;
    int temp;
    printf("Mezclando mazo \n");
    for (int i=0; i<100; i++){ //se puede poner un rand tambi�n despu�s del <
        r1 = rand() %40;
        r2 = rand() %40;
        temp = mazo[r1];
        mazo[r1] = mazo[r2];
        mazo[r2] = temp;
    }
}
