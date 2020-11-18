# Laboratorio 8
## Equipo de Desarrollo
- David Xie (B88682)
- Juan Ignacio Pacheco (B85841)

## Datos
Las pruebas se hicieron con el límite de 10 millones, en donde se calculó la cantidad de primos entre 2 y este límite.
Para calcular el tiempo que se tardó en correr el programa para todas las pruebas, para un número grande de n, en este caso 100 millones, se utilizó MPI_Wtime.
A continuación, se muestra la tabla con las duraciones en segundos que se tardó en ejecutar cada caso para n = 100 000 000.
5761455

| numero de procesos | count\_primes\_process | count\_primes\_reduction | count\_primes\_all\_reduce |
|:------------:|-----------|-----------|-----------|
| 1 | 7.51022s | 7.63128s | 8.33427s |
| 1 | 7.52477s | 7.67502s | 8.34476s |
| 1 | 7.519s   | 7.64053s | 8.34663s |
| 1 | 7.52146s | 7.64464s | 8.34208s |
|:------------:|-----------|-----------|-----------|
| 16 | 3.02183s | 3.23884s | 4.34519s |
| 16 | 3.0741s  | 3.32412s | 4.53757s |
| 16 | 3.04431s | 3.21279s | 4.6423s |
| 16 | 3.19999s | 3.39446s | 4.64691s |
|:------------:|-----------|-----------|-----------|
| 32 | 2.99326s | 2.98437s | 4.69418s |
| 32 | 3.53561s | 3.17773s | 4.32963s |
| 32 | 3.25163s | 3.20947s | 4.63863s |
| 32 | 3.02075s | 3.37483s | 4.11232s |
|:------------:|-----------|-----------|-----------|
| 64 | 3.66429s | 3.80354s | 4.65839s |
| 64 | 3.32397s | 4.94199s | 4.52211s |
| 64 | 3.2147s | 4.24185s | 4.53963s |
| 64 | 3.2017s | 4.00413s | 4.61714s |

