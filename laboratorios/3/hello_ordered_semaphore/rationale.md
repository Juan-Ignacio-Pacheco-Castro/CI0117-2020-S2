Las diferencias entre usar exclusion mutua y semaforos:
1. Podia inicializar los semaforos en su forma "locked" y solo dejar que el primero se incrementara.
A diferencia de los mutex que tenia que inicializarlos a todos "unlocked" y luego "lockearlos".
2. En terminos de mejor uso (practica correcta) seria el mutex, debido a que el semaforo con solo 1 campo no tiene razon de ser, es mejor el uso mas exacto del mutex.
3. Con 10 000 casos el tiempo de semaforo fue 0.01 mejor que mutex.
Con 100 000 se hicieron 32 750 casos con 1.738 s en el algoritmo de  mutex. Mientras en el algoritmo de semaphore se hicieron 32 750 casos en 1.698 s. En este caso el semaforo fue 0.04 s mas rapido.

