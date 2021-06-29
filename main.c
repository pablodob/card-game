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
#define PUNTAJE_MAX 7.5
#define POZO_JUGADOR 5000.0
#define POZO_BANCA 100000.0
#define TECHO_BANCA 6
#define APUESTA_MIN 100.0
#define APUESTA_MAX 1500.0
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

// SON TODAS FUNCIONES
// resetearmazo acomoda las cartas del 0 al 39
// mostrarmazo muestra las cartas en el orden que estan
// mostrar hace lo mismo pero diciendo el nombre de la carta que lo encontramos con "carta (a)"
// carta (a) donde a es el valor num�rico entre 0 y 39 dice qu� carta es
// valor (a) donde a es el valor entre 0 y 39 devuelve el valor de la carta en el juego {1,...,7, 0,5}
// mezclar mezcla las cartas, hay que agregar los rand que no s� por qu� no me los agarra, creo que falta la bibliotec


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
float ganador_apuesta (float, int[], int, short int [], short int [], int);
int   consigna_1      (float[], float, int);

int main(){
    int mazo [40];
    int cartas_repartidas[40];
    int n_repartidas = 0;
    int count_cartas[10];

    srand(getpid());

    resetearmazo(mazo);
    mezclar(mazo);

    int g,i,j;
    int jugadores;
    int rondas;

    do {
        printf ("Por favor ingrese el numero de jugadores (entre %d y %d)\n", MIN_JUGADORES, MAX_JUGADORES);
        printf (">");
        scanf  ("%d", &jugadores);
    }
    while( (jugadores <MIN_JUGADORES) || (jugadores >MAX_JUGADORES) );

    float pozo[jugadores];
    float pozoBanca;
    int   apuesta[jugadores];

    int   cartasJugador[jugadores][MAX_MANO];
    int   cartasBanca[MAX_MANO];
    int   cantidadCartasJugador[jugadores];
    int   cantidadCartasBanca;
    int   respuesta;

    float ganador;
    float apuesta_max = 0;
    short int apuesta_max_jugador[jugadores];
    short int contador_pasados[jugadores];
    short int contador_7med[jugadores];


    //reparte pozo e iniciaiza arrays contadores
    for (i=0; i < jugadores; i++){
        pozo[i]=POZO_JUGADOR;
        apuesta_max_jugador[i] = 0;
        contador_pasados[i] = 0;
        contador_7med[i] = 0;
    }
    pozoBanca= POZO_BANCA;

    do { //no entra al do no se por que
        printf ("HOLIS\n");
        printf ("Por favor ingrese el numero de rondas (entre %d y %d)\n", MIN_RONDAS, MAX_RONDAS);
        printf (">");
        scanf  ("%d", &rondas);
    }
    while( (rondas < MIN_RONDAS) || (jugadores > MAX_RONDAS) );

    //empiezan rondas
    for(g=0;g<rondas;g++){
        //inicializa en -1 todas las manos de jugadores
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
            respuesta=0;

            if (pozo[i]<APUESTA_MIN){
                    printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                    continue;
            }

            printf ("\nJugador %d, has recibido un ", i+1);
            imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);

            do {
                printf("Cuánto quieres apostar? (Debe apostar un valor entre %f y %f)\n", APUESTA_MIN, min(pozo[i],APUESTA_MAX));
                printf(">");
                scanf ("%d", &apuesta[i]);
                printf ("%d", apuesta[i] );
            } while ( (apuesta[i]<APUESTA_MIN) || (apuesta[i] > min(pozo[i], APUESTA_MAX) ));

            // En caso de ser apuesta máxima actualiza variables apuesta_max y apuesta_max_jugador
            if (apuesta_max < apuesta[i]){
                apuesta_max = apuesta[i];
                for(j=0;j<jugadores;j++)
                    apuesta_max_jugador[j]=0;
                apuesta_max_jugador[i] = 1;
            } else if (apuesta_max == apuesta[i])
                apuesta_max_jugador[i] = 1;

            do {
                printf ("Deseas pedir otra carta?\n1: Si\n2: No\n>");
                scanf ("%d",&respuesta);
                if (respuesta == 1){
                    cartasJugador[i][cantidadCartasJugador[i]]=mazo[n_repartidas++];
                    printf ("\n\nJugador %d, has recibido un ", i+1);
                    imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);
                }
            } while (respuesta !=2);
        }

        respuesta=0;

        //pide cartas la banca
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
            ganador = ganador_apuesta( puntaje(cartasBanca, cantidadCartasBanca), cartasJugador[i], cantidadCartasJugador[i], contador_pasados, contador_7med, i);
            if (ganador == -1.0){
                printf ("El jugador %d ha perdido su apuesta\n", i+1);
                pozo[i]=pozo[i]-apuesta[i];
                pozoBanca=pozoBanca + apuesta [i];
            }
            else {
                printf("El jugador %d ha ganado un %f %% \n", i+1, ganador*100);
                pozo[i]=pozo[i]+apuesta[i]*ganador;
                pozoBanca=pozoBanca-apuesta[i]*ganador;
            }
        }

        for (i=0; i<jugadores; i++){
            printf ("%f ,", pozo[i]);
        }
        printf ("%f \n", pozoBanca);

    }
 
    //Jugador que más dinero ganó (1)
    printf ("El jugador %d ha sido el m�s afortunado hoy.\n", consigna_1( pozo,pozoBanca,jugadores)+1);
    //Apuesta máxima
    printf ("La apuesta máxima fue de %f y la hicieron el/los siguientes: ", apuesta_max);
    for(i=0;i<jugadores;i++)
        if (apuesta_max_jugador[i]==1)
            printf("Jugador %d, ", i);
    printf("\n");

    //Indica si para los jugadores en conjunto fueron más las ganancias que las pérdidas
    if(pozoBanca<POZO_BANCA)
        printf("Para los jugadores en conjunto fueron más las ganancias que las pérdidas\n");

    //Cuantos jugadores lograron 7+figura durante el juego.  No responde a la consigna
    printf ("Los siguientes jugadores lograrion un 7 y una figura:\n");
    for(i=0;i<jugadores;i++)
        if (contador_7med[i]==1)
            printf("Jugador %d, ", i);
    printf("\n");

    //To do
    //Mostrar un top 5 de las cartas numéricas que más salieron sin importar el palo. Falta ordenar
    for (i=0;i<10;i++)
        count_cartas[i]=0;
    
    for (i=0;i<n_repartidas;i++)
        count_cartas[mazo[i]%10]++;
    
    for (i=0;i<10;i++)
        printf("La carta %d salió %d veces", num2numero(i),count_cartas[i]);

    //Determinar cuántos participantes perdieron por pasarse durante toda la partida. No responde.
    printf ("Los siguientes jugadores se pasaron de 7 y medio durante toda la partida:\n");
    for(i=0;i<jugadores;i++)
        if (contador_pasados[i]==1)
            printf("Jugador %d, ", i);
    printf("\n");

    //To do
    //Enumerar los jugadores desde el más al menos exitoso en ganancias

    //To do
    //Indicar si en una partida salieron todas las cartas de un mismo palo, indicar cual o cuales palos

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

float ganador_apuesta(float puntos_b, int mano[], int cartas_mano, short int contador_pasados[], short int contador_7med[], int n_jugador){
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
        contador_7med[n_jugador]=1;

    if(puntos_j > 7.5){
        printf("El jugador se pas�. Gana la banca");
        contador_pasados[n_jugador]=1;
        return -1.0;
    }
    else if(puntos_j < 7,5){
        if (puntos_b>=puntos_j && puntos_b <= 7.5){
            printf("Banca tiene más puntos que jugador. Gana banca");
            return -1.0;
            }
        else{
            printf("Gana el jugador el 25%% de la apuesta");
            return 0.25;
        }
    }
    else if (puntos_b == 7.5){
        printf("La banca tiene 7 y medio. Gana la banca");
        return -1.0;
        }
    else if (cartas_mano>2){
        printf("El jugador gana con tiene 7 y medio pero con m�s de 2 cartas. Gana 25%% de la apuesta");
        return 0.25;
        }
    else if (carta0[0]!=carta1[0]){
        printf("Gana el jugador con 7 y medio y cartas de distinto palo. Gana 50%% de la apuesta");
        return 0.5;
        }
    else if (carta0[0]==0 && (carta0[1]==12 || carta1[1]==12)){
        printf("Gana el jugador con 7 y medio con un rey y cartas de oro. Gana 100%% de la apuesta");
        return 1.0;
        }
    else
        {
        printf("Gana el jugador con 7 y medio con cartas del mismo palo. Gana 75%% de la apuesta");
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
