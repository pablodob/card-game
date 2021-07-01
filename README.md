### Proyecto: Juego 7 y medio
El archivo presenta un juego de 7 y medio de una baraja española sin 8s u 9s. Nos consulta la cantidad de jugadores y las rondas a jugar.
Al finalizar cada ronda hace un análisis de las ganancias de cada jugador y las imprime. Para eso utiliza la función ganador_apuesta.
Luego de toda la partida imprime el jugador más afortunado (se utiliza la función mas_afortunado), cuál fue la apuesta máxima y quién la realizó, sí tuvieron más ganancias los jugadores o la banca, qué jugadores lograron 7 + figura durante el juego, un top 5 de las cartas numéricas que más salieron (con la función top_5_numeros), lista los participantes perdieron por pasarse durante toda la partida (en cualquiera de sus rondas), ordena los jugadores en función de las ganancias (función ordenar_segun_ganancias) y por último indica si de algún palo salieron todas las cartas (las cuenta con all_cartas_palo y las imprime con print_all_cartas_palo).

El mazo es indicado por el array ´´´mazo[]´´´. Al ser un array líneal están construidas las funciones
```num2palo();
num2numero();``` para identificar el número de la carta y su palo. Antes de cada mano la función mezclar(mazo) mezcla aleatoreamente las cartas.
Para repartir cartas se usa el contador n_repartidas.
El pozo de cada jugador es guardado en el array ```pozo[]``` y el de la banca en la variable pozoBanca. CartasJugador y cartasBanca guardan las cartas de la actual mano.
La matriz mazoXpalos nos permite identificar si salieron todas las cartas del mismo palo, ya que es utilizado en la función ```all_cartas_palo();```
La función count_cartas cuenta las cartas sin importar palo.
Las apuestas maximas por jugador son guardadas en ```apuesta_max_jugador[]``` y por último tenemos los contadores ```contador_pasados[]
contador_7med[]```

Antes de cada ingreso del usuario se borra el caché con la función fflush (stdin), esta función resuelve el problema de los bucles infinitos en SO Windows en caso de ingresos inesperados por parte del usuario, no así en SO linux. El programa es óptimo para SO Windows, si se usara en SO linux el usuario deberá ingresar siempre valores enteros como se solicita.