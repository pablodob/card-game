#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// SON TODAS FUNCIONES
// resetearmazo acomoda las cartas del 0 al 39
// mostrarmazo muestra las cartas en el orden que estan
// mostrar hace lo mismo pero diciendo el nombre de la carta que lo encontramos con "carta (a)"
// carta (a) donde a es el valor numérico entre 0 y 39 dice qué carta es
// valor (a) donde a es el valor entre 0 y 39 devuelve el valor de la carta en el juego {1,...,7, 0,5}
// mezclar mezcla las cartas, hay que agregar los rand que no sé por qué no me los agarra, creo que falta la biblioteca

int main()
{
//resetear mazo
int mazo [40];
    int resetearmazo (){
        for (int i=0; i<40; i++)
        {
            mazo[i]=i;
        }
    }
//mostrar mazo
    int mostrarmazo(){
        for (int i=0; i<40; i++)
        {
            printf ("%d""\n", mazo[i]);
        }
    }
//qué carta es
    int carta (a){
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
    int mostrar (){
        for (int i=0; i<40; i++)
        {
            carta (mazo[i]);
        }
        return 0;
    }
//valor carta
    int valor (a) {
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

    int mezclar() {
        for (int i=0; i<100; i++){
            int aux;
            int random1 = (i^i) %40;
            int random2 = (i^2) %40;
            aux = mazo [random1];
            mazo [random1] = mazo [random2];
            mazo [random2] = aux;
        }
        return 0;
    }

    resetearmazo();
    mostrar();
    mezclar();
    mostrar();

    return 0;
    }
