#define MIN_JUGADORES 1
#define MAX_JUGADORES 5
#define MIN_RONDAS 2
#define MAX_RONDAS 4
#define MAX_MANO 13
#define PUNTAJE_MAX 7.5
#define POZO_JUGADOR 5000.0
#define POZO_BANCA 100000.0
#define TECHO_BANCA 6
#define APUESTA_MIN 100.0
#define APUESTA_MAX 1500.0
#define CARTAS_MAZO 40
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

void  resetearmazo              (int []);
void  mostrarmazo               (int []);
int   num2palo                  (int);
int   num2numero                (int);
void  num2carta                 (int, int []);
void  imprimir                  (int );
void  mostrar                   (int []);
float valor                     (int);
void  mezclar                   (int []);
float puntaje                   (int [], int);
int   ganador_apuesta           (float, int[], int, short int [], short int [], int);
void  mas_afortunado            (float[], float, int);;
void  ordenar_segun_ganancias   (float[], int);
void  all_cartas_palo           (int [], int, int [4][10]);
void  top_5_numeros             (int []);
void  print_all_cartas_palo     (int [4][10]);

void top_5_numeros (int count_cartas[]){
    int numerosMazo [10][2];
    int i;
    int j;
    int aux[2];

    //se inicializa numerosMazo con los valores de count_carta asociados a los números de cada carta
    for (i=0; i<10; i++){
        numerosMazo[i][0] = count_cartas[i];
        numerosMazo[i][1] = i;
    }

    //ordenamos numeros mazo por los valores de count_carta (numeros_mazo[i][0])
    for (int i=0; i<10; i++){
        for (j=i; j<10; j++){
            if (numerosMazo [i][0] < numerosMazo[j][0]){
                aux[0] = numerosMazo [i][0];
                aux[1] = numerosMazo [i][1];
                numerosMazo [i][0] = numerosMazo [j][0];
                numerosMazo [i][1] = numerosMazo [j][1];
                numerosMazo [j][0] = aux [0];
                numerosMazo [j][1] = aux [1];
            }
        }
    }

    for (i=0; i<5; i++)
        printf ("\n%d. la carta %d ha salido %d veces",i+1, num2numero(numerosMazo[i][1]), numerosMazo [i][0]);

    printf("\n");
}

void all_cartas_palo(int mazo[], int repartidas, int mazoXpalos[4][10]){
    int k;

    //marcamos en el mazo por palos las cartas que salieron
    for (k=0;k<repartidas;k++)
        mazoXpalos[mazo[k]/10][mazo[k]%10] = 1;

}

void print_all_cartas_palo(int mazoXpalos[4][10]){

    int i, j;
    int flag;
    int print_flag = 0;


    for (i=0; i<4;i++) {
        flag = 1;

        //Se marca la bandera si salieron todas las cartas de ese palo. Si alguna carta no salio pasa al siguiente palo
        for (j=0; (j<10) && (flag == 1); j++)
            flag = mazoXpalos[i][j];

        //Si salieron todas las cartas de un palo, la bandera sigue marcando uno y por lo tanto se imprime el palo
        if (flag == 1){
            switch (i) {
                case 0: {
                    printf("En esta partida salieron todas las cartas de oro\n");
                    print_flag = 1;
                    break;
                }
                case 1: {
                    printf("En esta partida salieron todas las cartas de basto\n");
                    print_flag = 1;
                    break;
                }
                case 2: {
                    printf("En esta partida salieron todas las cartas de copa\n");
                    print_flag = 1;
                    break;
                }
                case 3: {
                    printf("En esta partida salieron todas las cartas de espada\n");
                    print_flag = 1;
                    break;
                }
            }
        }
    }

    //En caso que no se haya impreso ningun resultado en los palos sale un mensaje
    if (print_flag == 0)
        printf("En esta partida no hubo ning\243n palo que salieran todas las cartas\n");

    /* El siguiente codigo es para control
    printf("Control. Salieron las siguientes cartas?:\n");
    for (i=0; i<4;i++)
        for (j=0; j<10; j++)
            printf ("%d,%d = %d | ", i, j, mazoXpalos[i][j]);
    printf("\n");
    */
}

void mas_afortunado (float pozo_j[], float pozo_b, int cantidad){
    float ganancias [cantidad];
    float ganancias_b;
    int i;
    int j;
    float ganador;
    int jugador_ganador;

    //se inicializa el array ganancias con con los valores de las ganancias en cada indice [i]
    for (i=0; i<cantidad; i++){
        ganancias[i] = pozo_j[i]-POZO_JUGADOR;
    }

    //se identifica el pozo ganador entre los jugaores y se asignan resultados
    ganador  = 0.0;
    jugador_ganador = 0;

    for (int i=0; i<cantidad; i++){
        if (ganancias [i]>ganador){
            ganador = ganancias [i];
            jugador_ganador = i;
        }
    }

    //se calculan ganancias banca
    ganancias_b=pozo_b-POZO_BANCA;


    //se imprimen los resultados
    if (ganador>ganancias_b){
        printf ("\nEl jugador %d se ha llevado el mayor n\243mero de ganancias\n", jugador_ganador+1);
    }
    else {
        printf ("\nHoy la Banca ha salido ganando\n");
    }
}

void ordenar_segun_ganancias (float pozo_j[], int cantidad){
    float ganancias [cantidad][2];
    int i;
    int j;

    //inicializa variable cantidad con ganancias en el indice [0] y el jugador en el indice [1]
    for (i=0; i<cantidad; i++){
        ganancias[i][0] = pozo_j[i]-POZO_JUGADOR;
        ganancias[i][1] = i*1,0;
    }

    //se ordenan las ganancias desde el mas alto al mas bajo
    float aux[2];
    for (int i=0; i<cantidad; i++){
        for (j=i; j<cantidad; j++){
            if (ganancias [i][0]>ganancias[j][0]){
                aux[0] = ganancias [i][0];
                aux[1] = ganancias [i][1];
                ganancias [i][0] = ganancias [j][0];
                ganancias [i][1] = ganancias [j][1];
                ganancias [j][0] = aux [0];
                ganancias [j][1] = aux [1];
            }
        }
    }

    //se imprimen los resultados
    int n;
    for (i=0; i<cantidad; i++){
        n = (int)ganancias[i][1];
        if (ganancias[i][0]>=0){
            printf ("%d. el jugador %d ha ganado %.0f \n",i+1, n+1, ganancias [i][0]);
        }
        else {

            printf ("%d. el jugador %d ha perdido %.0f \n",i+1, n+1, -ganancias [i][0]);
        }
    }
}

int  num2palo (int numerocarta){
    if (numerocarta >=0 && numerocarta <CARTAS_MAZO){
        return numerocarta/10;
    }
}

int  num2numero (int numerocarta){
    if (numerocarta>=0 && numerocarta <CARTAS_MAZO){
        if (numerocarta%10 <= 6){
            return (numerocarta%10)+1;
        }
        else {
            return (numerocarta%10)+3;
        }
    }
}

int ganador_apuesta(float puntos_b, int mano[], int cartas_mano, short int contador_pasados[], short int contador_7med[], int n_jugador){
    /*
    Se dejan como comentarios prints que sirven para verificar la función.

    Referencias de los returns:

    - 0 = el jugador se pasó
    - 1 = ha ganado la banca con 7.5
    - 2 = la banca ha ganado.
    - 3 = gana 100% con rey y 7 de oro
    - 4 = gana 75% con 7 y figura del mismo palo
    - 5 = gana 50% con 7 y fugura
    - 6 = gana 25% con 7.5
    - 7 = gana 25%
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

    if (puntos_j> 7.5){
        //printf ("El jugador %d se pasó. Pierde su apuesta. Gana la Banca.\n", n_jugador +1);
        contador_pasados[n_jugador]=1;
        return 0;
    }
    else if (puntos_b >= puntos_j && puntos_b <= 7.5){
        if (puntos_b == 7.5){
        //printf ("La Banca ha ganado con 7.5. El jugador %d pierde su apuesta.", n_jugador +1);
        return 1;}
        else {
        //printf ("La banca ha ganado");
        return 2;
        }
    }
    else if (puntos_j>puntos_b || (puntos_j<=7.5 && puntos_b>7.5) ){                                  //en todos estos casos gana el jugador
        if (puntos_j==7.5){                                                                         //gana con 7,5
            if (carta0[1]==7 || carta1[1]== 7){                                                     //en todos estos casos gana con 7+figura
                if (carta0[0]==carta1[0]){                                                          //en todos estos casos gana con 7+figura del mismo palo
                    if (carta0[0]==0 && carta1[0]==0 && (carta0[1]==12 || carta1[1]==12)){          //7 oro rey oro
                        //printf("Gana el jugador %d con 7 y medio con un rey y cartas de oro. Gana 100%% de la apuesta.\n", n_jugador+1);
                        return 3;
                    }
                    else {
                        //printf("Gana el jugador %d con 7 y medio con cartas del mismo palo. Gana 75%% de la apuesta.\n", n_jugador+1);
                        return 4;
                    }
                }
                else {
                    //printf("Gana el jugador %d con 7 m\240s figura y cartas de distinto palo. Gana 50%% de la apuesta.\n", n_jugador+1);
                    return 5;
                }
            }
            //printf("Gana el jugador %d el 25%% de la apuesta.\n", n_jugador+1);
            return 6;
            }
        //printf("Gana el jugador %d el 25%% de la apuesta.\n", n_jugador+1);
        return 7;
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
    for (i=0; i<CARTAS_MAZO; i++)
    {
        mazo[i]=i;
    }
}

//Para control: mostrar todas las cartas de un mazo
void mostrarmazo(int mazo[]){
    int i;
    for (i=0; i<CARTAS_MAZO; i++)
    {
        imprimir(mazo[i]);
        printf (", ");
    }
    printf ("\n");
}

//Imprime una carta con numero y palo desde un numero del 0 al 39
void imprimir(int num){
    printf("%d de ", num2numero(num));
    switch (num2palo(num)){
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

void mezclar(int mazo[]) {
    int r1,r2;
    int temp;
    printf("Mezclando mazo... \n");
    for (int i=0; i<100; i++){
        r1 = rand() %CARTAS_MAZO;
        r2 = rand() %CARTAS_MAZO;
        temp = mazo[r1];
        mazo[r1] = mazo[r2];
        mazo[r2] = temp;
    }
}
