Cabe destacar que el numero de rectangulos tiene 
Los limites que se usaron para hacer la prueba fueron 1 a 10, a = 1 y b = 10.

Ejecutamos los programas de ambos ejercicios para n = 1, n = 10,n = 1,000, n = 1,000,000, n = 100,000,000 y n = 1,000,000,000  utilizando los mismos valores de a y b en cada ejecución. Se realizo las ejecuciones usando 1, 16, 32, 64 y 128 hilos en openmp. A continuacion se muestra la duración de cada ejecución y nos aseguramos de obtener las mismas áreas estimadas por la versión serial y concurrente. 

| n | area | 1 | 16 | 32 | 64 | 128 |
|:------------:|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
| 1 | 18.00 | 0.000013 s | 0.000489 s | 0.006469 s | 0.001512 s | 0.002268 s |
| 10 | 298.67 | 0.000016 s | 0.000489 s | 0.001310 s | 0.001522 s | 0.002853 s |
| 1 000 | 341.55 | 0.000046 s | 0.000371 s | 0.001136 s | 0.004938 s | 0.004648 s |
| 1 000 000 | 342.00 | 0.018166 s | 0.008048 s | 0.007966 s | 0.012277 s | 0.008914 s |
| 100 000 000 | 342.00 | 1.704137 s | 0.583628 s | 0.553943 s | 0.557012 s | 0.601220 s |
| 1 000 000 000 | 342.00 | 16.952506 s | 5.522615 s | 5.417085 s | 5.427441 s | 5.361968 s |




