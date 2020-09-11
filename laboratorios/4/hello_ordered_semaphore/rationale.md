Las diferencias entre usar exclusion mutua y semaforos:
1. Podia inicializar los semaforos en su forma "locked" y solo dejar que el primero se incrementara.
A diferencia de los mutex que tenia que inicializarlos a todos "unlocked" y luego "lockearlos".
2. En terminos de mejor uso seria el mutex, debido a que el semaforo con solo 1 campo no tiene razon
de ser, es mejor el uso mas exacto del mutex.
3. 
