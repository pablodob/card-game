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
#define APUESTA_MIN 100
#define APUESTA_MAX 1500
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
float puntaje(int [], int);
int ganador_apuesta(float, float, int[], int);

int main(){
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
    int cantidadCartasJugador[jugadores];

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
        cantidadCartasJugador[i]=1;
        printf ("Jugador %d ha recibido su carta\n", i+1);
    }

    for (int i=0; i < jugadores; i++){

        int respuesta=0;
        int carta_mano[2];

        if (pozo[i]<0){
                printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                continue;
        }

        printf ("\n\nJugador %d, has recibido un ", i+1);
        imprimir(cartasJugador[i][j]);

        do {
            printf("¿Cuánto quieres apostar? (Debe apostar un valor entre %d y %d)\n", APUESTA_MIN, min(pozo[i],APUESTA_MAX)); //ver caso si pozo es menor a apuesta min
            printf(">");
            scanf ("%d", &apuesta[i]);
                    printf ("%d", apuesta[i] );
        } while ( (apuesta[i]<APUESTA_MIN) || (apuesta[i] > min(pozo[i], APUESTA_MAX) ));

        do {
            int j=1;
            printf ("¿Deseas pedir otra carta?\n1: Sí\n2: No\n>");
            scanf ("%d",&respuesta);
            if (respuesta == 1){
                cartasJugador[i][j]=mazo[n_repartidas++];
                cantidadCartasJugador[i]++;
                printf ("\n\nJugador %d, has recibido un ", i+1);
                imprimir(cartasJugador[i][j]);
                j++;
            }

        } while (respuesta !=2);


    }

    for (int i; i<jugadores; i++){
        printf ("el jugador %d ha recibido %f puntos\n", i+1, puntaje(cartasJugador[i], cantidadCartasJugador[i]));
    }

    return 0;

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
        printf("El jugador se pasó. Gana la banca");
        return 1;
    }
    else if(puntos_j < 7,5){
        if (puntos_b>=puntos_j && puntos_b <= 7.5){
            printf("Banca tiene más puntos que jugador. Gana banca");
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
        printf("El jugador gana con tiene 7 y medio pero con más de 2 cartas. Gana 25% de la apuesta");
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
    int carta[2];
    float puntos=0.0;

    for (i = 0; i < cartas_cantidad; i++){
        printf ("%d, ",cartas_mano[i] );
        num2carta(cartas_mano[i],carta);
        if (carta[1]  <= 7)
            puntos = puntos + carta[1]*1.0;
        else
            puntos = puntos + 0.5;

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

//valor carta --- no sirve
float valor (int num) {
    float valorcarta=0.0;
    if (num  <= 7){
        valorcarta = (num)*1.0;
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
