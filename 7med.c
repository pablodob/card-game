#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "aux_7med.h"

int main(){

    int mazo [CARTAS_MAZO];             //array de 40 posiciones que guardan una carta cada una.
    int cartas_repartidas[CARTAS_MAZO]; //array que guarda las cartas que fueron repartidas.
    int n_repartidas;                   //cotador de cartas repartidas. Se inicializa en cada ronda
    int g,i,j;                          //contadores.
    int jugadores;                      //cantidad de jugadores. Ingresada por el usuario.
    int rondas;                         //cantidad de rondas. Ingresada por el usuario

    srand(getpid());
    resetearmazo(mazo);                 //función que asigna valores a cada posición de mazo[]

    int mazoXpalos[4][10];              //inicializo el mazoXpalos utilizado en la función all_cartas_palo
    for (i=0; i<4;i++)
        for (j=0; j<10; j++)
            mazoXpalos[i][j] = 0;
    int count_cartas[10];               //inicializo el count_cartas utilizado en la función top_5_numeros
    for (int i=0; i<10; i++){
        count_cartas[i]=0;
    }

    //Se solicita al usuario ingresar la cantidad de jugadores.
    do {
        printf ("Por favor ingrese el numero de jugadores (entre %d y %d)\n", MIN_JUGADORES, MAX_JUGADORES);
        printf (">");
        fflush (stdin);
        scanf  ("%d", &jugadores);
    }
    while( (jugadores <MIN_JUGADORES) || (jugadores >MAX_JUGADORES) );

    float pozo[jugadores];              //array que guarda el dinero de cada jugador
    float pozoBanca;                    //guarda el dinero de la banca
    int   apuesta[jugadores];           //guarda las apuestas de cada jugador. Ingresada por el usuario.

    int   cartasJugador[jugadores][MAX_MANO];   //array que guarda la mano actual de cada jugador. Se inicializa en cada ronda.
    int   cartasBanca[MAX_MANO];                //array que guarda la mano actual de la baca. Se inicializa en cada ronda.
    int   cantidadCartasJugador[jugadores];     //array que guarda la cantidad de cartas que pidió cada jugador.
    int   cantidadCartasBanca;                  //array que guarda la cantidad de cartas que pidió la banca.
    int   respuesta;                            //variable utilizada para solicitar más cartas.

    float porcentajeGanado = 0.0;               //se utiliza para recordar el porcentaje ganado. Ver línea 202.
    float apuesta_max = 0.0;                    //recuerda la apuesta máxima.
    int   ronda_apuesta_max[jugadores];
    short int apuesta_max_jugador[jugadores];   //recuerda la apuesta máxima de cada jugador.
    short int contador_pasados[jugadores];      //cuenta cuantos jugadores se pasaron de 7,5
    short int contador_7med[jugadores];         //cuenta cuantos jugadores consiguieron 7,5

    //reparte pozo e iniciaiza arrays contadores
    for (i=0; i < jugadores; i++){
        pozo[i]=POZO_JUGADOR;
        apuesta_max_jugador[i] = 0;
        contador_pasados[i] = 0;
        contador_7med[i] = 0;
    }
    pozoBanca= POZO_BANCA;

    //solicita el numero de rondas
    do {
        printf ("Por favor ingrese el numero de rondas (entre %d y %d)\n", MIN_RONDAS, MAX_RONDAS);
        printf (">");
        fflush (stdin);
        scanf  ("%d", &rondas);
    }
    while( (rondas < MIN_RONDAS) || (rondas > MAX_RONDAS) );

    //comienzan rondas
    for(g=0;g<rondas;g++){

        mezclar(mazo);
        printf("\n");
        n_repartidas=0;

        //inicializa en -1 todas las manos de jugadores y la Banca.
        for (i=0;i<jugadores;i++){
            for(j=0;j<MAX_MANO;j++){
                cartasJugador[i][j]=-1;
            }
        }
        for(j=0;j<MAX_MANO;j++){
                cartasBanca [j] = -1;
        }

        //reparte la primer carta a los jugadores y la Banca
        for (int i=0; i<jugadores; i++){
            cantidadCartasJugador[i]=0;
            cartasJugador[i][cantidadCartasJugador[i]] = mazo [n_repartidas++];
            printf ("Jugador %d ha recibido su carta\n", i+1);
        }

        cantidadCartasBanca=0;
        cartasBanca[cantidadCartasBanca]=mazo [n_repartidas++];
        printf ("La Banca ha recibido su carta\n");

        //pregunta a cada jugador si quiere más cartas.
        for (int i=0; i < jugadores; i++){
            respuesta=0;

            if (pozo[i]<APUESTA_MIN){
                printf ("El jugador %d, no tiene dinero en el pozo\n", i+1);
                continue;
            }

            printf ("\n\nJugador %d, has recibido un ", i+1);
            imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);         //muestra la primer carta

            //pregunta cuánto desea apostar.
            do {
                printf("\n%cCu\240nto quieres apostar? (Debe apostar un valor entre %.0f y %.0f)\n", 168, APUESTA_MIN, min(pozo[i],APUESTA_MAX));
                printf(">");
                fflush (stdin);
                scanf ("%d", &apuesta[i]);
            } while ( (apuesta[i]<APUESTA_MIN) || (apuesta[i] > min(pozo[i], APUESTA_MAX) ));


            //en caso de ser apuesta máxima actualiza variables apuesta_max y apuesta_max_jugador
            if (apuesta_max < apuesta[i]){
                apuesta_max = apuesta[i];
                ronda_apuesta_max[i] = g;
                for(j=0;j<jugadores;j++)
                    apuesta_max_jugador[j]=0;
                apuesta_max_jugador[i] = 1;
            } else if (apuesta_max == apuesta[i])
                apuesta_max_jugador[i] = 1;
                ronda_apuesta_max[i] = g;

            //pregunta si se desean más cartas
            do {
                printf ("%cDeseas pedir otra carta?\n1: Si\n2: No\n>", 168);
                fflush (stdin);
                scanf ("%d", &respuesta);
                if (respuesta == 1){
                    cartasJugador[i][cantidadCartasJugador[i]]=mazo[n_repartidas++];
                    printf ("\nJugador %d, has recibido un ", i+1);
                    imprimir(cartasJugador[i][cantidadCartasJugador[i]++]);
                    printf (". ");
                }
            } while (respuesta !=2 & puntaje(cartasJugador[i],cantidadCartasJugador[i])<7.5);

            if (puntaje(cartasJugador[i],cantidadCartasJugador[i])>=7.5){ //Avisa al jugador que no puede seguir pidiendo porque se plantó o porque llagó a 7.5
                printf ("Has alcanzado el m\240ximo, no puedes seguir pidiendo cartas\n");
            }
            else{
                printf ("El jugador se planta. \n");
            }
        }

        respuesta=0;

        //pide cartas la banca
        if (pozoBanca<=0){
            printf ("La Banca no tiene dinero en el pozo... que corra la sangre.\n");  //si la banca se queda sin dinero convoca una revuelta contra la timba.
        }
        else {
            printf ("La Banca ha recibido un ");
            imprimir(cartasBanca[cantidadCartasBanca++]);                               //imprime cada carta de la banca
            printf (".\n");
            while (puntaje(cartasBanca,cantidadCartasBanca)<TECHO_BANCA){               //la Banca pide mientras esté entre los valores solicitados.
                cartasBanca[cantidadCartasBanca]=mazo[n_repartidas++];
                printf ("La Banca pide otra carta... Ha recibido un ");
                imprimir(cartasBanca[cantidadCartasBanca++]);
                printf ("\n");
            }
        }
        printf ("\n");

        //imprime los puntajes de cada jugador y la banca
        for (int i=0; i<jugadores; i++){
            printf ("El jugador %d ha conseguido %.1f puntos\n", i+1, puntaje(cartasJugador[i], cantidadCartasJugador[i]));
        }
        printf ("La Banca ha conseguido %.1f puntos\n", puntaje(cartasBanca, cantidadCartasBanca));

        //usa la función ganador_apuesta() para ver qué jugador le ganó a la banca. Dice qué porcentaje ganó cada jugador.
        for (i=0; i<jugadores; i++){
            switch (ganador_apuesta(puntaje(cartasBanca, cantidadCartasBanca), cartasJugador[i], cantidadCartasJugador[i], contador_pasados, contador_7med, i)){
                case 0: {
                    printf("El jugador %d se pas\242.\n", i+1);
                    pozo[i]=pozo[i]+apuesta[i]*-1;
                    pozoBanca=pozoBanca-apuesta[i]*-1.0;
                    break;
                }
                case 1: {
                    printf("Ha ganado la banca con 7.5\n");
                    pozo[i]=pozo[i]+apuesta[i]*-1;
                    pozoBanca=pozoBanca-apuesta[i]*-1.0;
                    break;
                }
                case 2: {
                    printf("Ha ganado la banca\n");
                    pozo[i]=pozo[i]+apuesta[i]*-1;
                    pozoBanca=pozoBanca-apuesta[i]*-1.0;
                    break;
                    }
                case 3: {
                    printf("El jugador %d ha ganado con 7 y rey de oro. Gana un 100%%\n", i+1);
                    pozo[i]=pozo[i]+apuesta[i]*1.0;
                    pozoBanca=pozoBanca-apuesta[i]*1.0;
                    break;
                }
                case 4: {
                    printf("El jugador %d ha ganado con 7 y figura del mismo palo. Gana un 75%%\n", i+1);
                    pozo[i]=pozo[i]+apuesta[i]*0.75;
                    pozoBanca=pozoBanca-apuesta[i]*0.75;
                    break;
                    }
                case 5: {
                    printf("El jugador %d ha ganado con 7 y figura. Gana un 50%%\n", i+1);
                    pozo[i]=pozo[i]+apuesta[i]*0.5;
                    pozoBanca=pozoBanca-apuesta[i]*0.5;
                    break;
                    }
                case 6: {
                    printf("El jugador %d ha ganado con 7,5. Gana un 25%%\n", i+1);
                    pozo[i]=pozo[i]+apuesta[i]*0.25;
                    pozoBanca=pozoBanca-apuesta[i]*0.25;
                    break;
                    }
                case 7: {
                    printf("El jugador %d ha ganado. Gana un 25%%\n", i+1);
                    pozo[i]=pozo[i]+apuesta[i]*0.25;
                    pozoBanca=pozoBanca-apuesta[i]*0.25;
                    break;
                    }
                }

        }
        printf ("\n\n");

        /*  este bloque sirve para verificar los pozos parciales de cada jugador.
        for (i=0; i<jugadores; i++){
            printf ("%d, %f ,", i+1, pozo[i]);
        }
        printf ("%f \n", pozoBanca);*/

        //Se suman al contador -count_cartas[]- cuantas cartas hay de cada número
        for (i=0; i<n_repartidas; i++)
            ++count_cartas[mazo[i]%10];

        //Se marcan en mazoXpalos cada una de las cartas repartidas en la mano
        all_cartas_palo(mazo, n_repartidas, mazoXpalos);
    }

    //Utiliza la función mas_afortunado para devolver quién fue el que más dinero ganó (1)
    mas_afortunado(pozo, pozoBanca, jugadores);

    //Apuesta máxima (2)

    printf ("La apuesta m\240xima fue de %.0f y la hicieron el/los siguientes: ", apuesta_max);
    for(i=0;i<jugadores;i++)
        if (apuesta_max_jugador[i]==1)
            printf("Jugador %d, en la ronda %d.", i+1, ronda_apuesta_max[i]+1);
    printf("\n");

    //Indica si para los jugadores en conjunto fueron más las ganancias que las pérdidas (3)
    if(pozoBanca<POZO_BANCA){
        printf("Para los jugadores en conjunto fueron m\240s las ganancias que las pérdidas\n");
    }

    //Cuantos jugadores lograron 7+figura durante el juego. (4)
    printf ("Los siguientes jugadores lograrion un 7 y una figura:\n");
    for(i=0;i<jugadores;i++)
        if (contador_7med[i]==1)
            printf("Jugador %d, ", i+1);
    printf("\n");

    //muestra un top 5 de las cartas numéricas que más salieron sin importar el palo con la función top_5_cartas (5)
    top_5_numeros (count_cartas);

    //determina cuántos participantes perdieron por pasarse durante toda la partida. (6)
    printf ("Los siguientes jugadores se pasaron de 7 y medio durante toda la partida:\n");
    for(i=0;i<jugadores;i++)
        if (contador_pasados[i]==1)
            printf("Jugador %d, ", i+1);
    printf("\n");

    //enumerar los jugadores desde el más al menos exitoso en ganancias con la función ordenar_segun_ganancias() (7)
    ordenar_segun_ganancias(pozo, jugadores);

    //indica si en una partida salieron todas las cartas de un mismo palo, indicar cual o cuales palos
    print_all_cartas_palo(mazoXpalos);

    return 0;

}