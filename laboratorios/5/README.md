# Laboratorio 5 Solución

Cabe destacar que el número de rectángulos tiene
Los límites que se usaron para hacer la prueba fueron 1 a 10, a = 1 y b = 10.

Ejecutamos los programas de ambos ejercicios para n = 1, n = 10,n = 1,000 y n = 1,000,000 utilizando los mismos valores de a y b en cada ejecución. Para el caso del programa que utiliza pthreads, se realizó las ejecuciones usando 1, 16, 32 y 64 hilos. A continuación se muestra la duración de cada ejecución y nos aseguramos de obtener las mismas áreas estimadas por la versión serial y concurrente.

| n | area | serial | 1 | 16 | 32 | 64 |
|:------------:|-----------|-----------|-----------|-----------|-----------|-----------|
| 1 | 18.00 | 0.000008 s | 0.000133 s | 0.004166 s | 0.002052 s | 0.003495 s |
| 10 | 298.67 | 0.000007 s | 0.000085 s | 0.000494 s | 0.001018 s | 0.002170 s |
| 1 000 | 341.55 | 0.000020 s | 0.000125 s | 0.003297 s | 0.001324 s | 0.004600 s |
| 1 000 000 | 342.00 | 0.015588 s | 0.035540 s | 0.079962 s | 0.084983 s | 0.084690 s |




