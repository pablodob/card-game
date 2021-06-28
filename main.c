#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>    //time()
#include <unistd.h>  //unix standard - getpid()->muestra el Process ID o pid del proceso que ejecuta el programa

#define MIN_JUGADORES 1
#define MAX_JUGADORES 5
#define MIN_RONDAS 2
#define MAX_RONDAS 4
#define MAX_MANO 14
#define PUNTAJE_MAX 7,5
#define POZO_JUGADOR 5000.0
#define POZO_BANCA 100000
#define TECHO_BANCA 6
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


void  resetearmazo    (int []);
void  mostrarmazo     (int []);
int   num2palo        (int);
int   num2numero      (int);
void  num2carta       (int, int []);
void  imprimir        (int );
void  mostrar         (int []);
float valor           (int);
void  mezclar         (int []);
float puntaje         (int [], int);
float ganador_apuesta (float, int[], int, int*, int*);
int   consigna_1      (float[], float, int);

int main(){
    int mazo [40];
    int cartas_repartidas[40];
    int n_repartidas = 0;

    srand(getpid());

    resetearmazo(mazo);
    mezclar(mazo);

    int g,i,j;
    int jugadores;
    int rondas;

    do {
        printf ("Por favor ingrese el número de jugadores (entre %d y %d)\n", MIN_JUGADORES, MAX_JUGADORES);
        printf (">");
        scanf  ("%d", &jugadores);
    } while( (jugadores <MIN_JUGADORES) || (jugadores >MAX_JUGADORES) );

    float pozo[jugadores];
    float pozoBanca;
    int   apuesta[jugadores];

    int   cartasJugador[jugadores][MAX_MANO];
    int   cartasBanca[MAX_MANO];
    int   cantidadCartasJugador[jugadores];
    int   cantidadCartasBanca;

    int contador_pasados = 0;
    int contador_7med = 0;
    //reparte pozo
    for (i=0; i < jugadores; i++){
        pozo[i]=POZO_JUGADOR;
    }
    pozoBanca= POZO_BANCA;

     do {
        printf ("Por favor ingrese el número de rondas (entre %d y %d)\n", MIN_RONDAS, MAX_RONDAS);
        printf (">");
        scanf  ("%d", &rondas);
    } while( (rondas <MIN_RONDAS) || (jugadores >MAX_RONDAS) );



    //empiezan rondas
    for(g=0;g<rondas;g++){
    //declaro en -1 todas las manos de jugadores
    for (i=0;i<jugadores;i++)
        for(j=0;j<MAX_MANO;j++)
            cartasJugador[i][j]=-1;

    for(j=0;j<MAX_MANO;j++){
            cartasBanca [j] = -1;
    }



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

        if (pozo[i]<APUESTA_MIN){
                printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                continue;
        }

        printf ("\n\nJugador %d, has recibido un ", i+1);
        imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);

        do {
            printf("¿Cuánto quieres apostar? (Debe apostar un valor entre %d y %f)\n", APUESTA_MIN, min(pozo[i],APUESTA_MAX)); //ver caso si pozo es menor a apuesta min
            printf(">");
            scanf ("%d", &apuesta[i]);
            printf ("%d", apuesta[i] );
        } while ( (apuesta[i]<APUESTA_MIN) || (apuesta[i] > min(pozo[i], APUESTA_MAX) ));

        do {
            printf ("¿Deseas pedir otra carta?\n1: Sí\n2: No\n>");
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

    for (int i; i<jugadores; i++){
        printf ("el jugador %d ha recibido %f puntos\n", i+1, puntaje(cartasJugador[i], cantidadCartasJugador[i]));
    }
        printf ("La Banca ha conseguido %f puntos\n", puntaje(cartasBanca, cantidadCartasBanca));

    for (i=0; i<jugadores; i++){
        float aux = ganador_apuesta( puntaje(cartasBanca, cantidadCartasBanca), cartasJugador[i], cantidadCartasJugador[i], contador_pasados, contador_7med);
        if (aux == -1.0){
            printf ("El jugador %d ha perdido su apuesta\n", i+1);
            pozo[i]=pozo[i]-apuesta[i];
            pozoBanca=pozoBanca + apuesta [i];
        }
        else {
            printf("El jugador %d ha ganado un %f%\n", i+1, aux*100);
            pozo[i]=pozo[i]+apuesta[i]*aux;
            pozoBanca=pozoBanca-apuesta[i]*aux;
        }
    }

    for (i=0; i<jugadores; i++){
        printf ("%f.2 ,", pozo[i]);
    }
    printf ("%f.2 \n", pozoBanca);

    printf ("Los jugadores se pasaron de 7 y medio en %d manos\n", contador_pasados);
    printf ("Hubo %d manos de jugadores con un 7 y una figura", contador_7med);
    }
    printf ("\nEl jugador %d ha sido el más afortunado hoy.", consigna_1( pozo,pozoBanca,jugadores)+1);
    return 0;

}

int consigna_1 (float pozo_j[], float pozo_b, int cantidad){
    float ganancias [cantidad+1];
    for (int i=0; i<cantidad; i++){
        ganancias[i] = pozo_j[i]-POZO_JUGADOR;
    }
    ganancias [cantidad+1]=pozo_b-POZO_BANCA;

    int aux = 0;
    for (int i=0; i<cantidad; i++){
        if (ganancias [i]>ganancias[i+1]){
            ganancias[i+1]=ganancias[i];
        }
        else {
            aux = i+1;
        }
    }
    return aux;
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

float ganador_apuesta(float puntos_b, int mano[], int cartas_mano, int *contador_pasados, int *contador_7med){
    /*
    Referencias:
    - 1 gana banca
    - 2 gana jugador 25%
    - 3 gana jugador 50%
    - 4 gana jugador 75%
    - 5 gana jugador 100%
    */
    float puntos_j;
    int carta0[2],carta1[2];
    carta0[0] = num2palo(mano[0]);
    carta0[1] = num2numero(mano[0]);
    carta1[0] = num2palo(mano[1]);
    carta1[1] = num2numero(mano[1]);
    puntos_j=puntaje(mano,cartas_mano);

    if(puntos_j == 7.5 && ((carta0[1] == 7) || (carta1[1] == 7)))
        *contador_7med++;

    if(puntos_j > 7.5){
        //printf("El jugador se pasó. Gana la banca");
        *contador_pasados++;
        return -1.0;
    }
    else if(puntos_j < 7,5){
        if (puntos_b>=puntos_j && puntos_b <= 7.5){
            //printf("Banca tiene más puntos que jugador. Gana banca");
            return -1.0;
            }
        else{
            //printf("Gana el jugador el 25% de la apuesta");
            return 0.25;
        }
    }
    else if (puntos_b == 7.5){
        //printf("La banca tiene 7 y medio. Gana la banca");
        return -1.0;
        }
    else if (cartas_mano>2){
        //printf("El jugador gana con tiene 7 y medio pero con más de 2 cartas. Gana 25% de la apuesta");
        return 0.25;
        }
    else if (carta0[0]!=carta1[0]){
        //printf("Gana el jugador con 7 y medio y cartas de distinto palo. Gana 50% de la apuesta");
        return 0.5;
        }
    else if (carta0[0]==0 && (carta0[1]==12 || carta1[1]==12)){
        //printf("Gana el jugador con 7 y medio con un rey y cartas de oro. Gana 100% de la apuesta");
        return 1.0;
        }
    else
        {
        //printf("Gana el jugador con 7 y medio con cartas del mismo palo. Gana 75% de la apuesta");
        return 0.75;
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
    for (int i=0; i<100; i++){
        r1 = rand() %40;
        r2 = rand() %40;
        temp = mazo[r1];
        mazo[r1] = mazo[r2];
        mazo[r2] = temp;
    }
}
