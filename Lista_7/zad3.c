#include <stdio.h>
#include <stdbool.h>

int main(){
    int N = 0;
    printf("Podaj liczbe\n");
    scanf("%d", &N);
    int aktualna_liczba = 2;
    int ilosc_znalezionych_liczb = 0;
    bool czy_pierwsza = true;
    int suma = 0;

    for (; ilosc_znalezionych_liczb< N; aktualna_liczba++){
        czy_pierwsza = true;
        // Sprawdzanie czy liczba jest pierwsza
        for (int i = 2; i < aktualna_liczba; i++) {
            if (aktualna_liczba % i ==0){
                printf("%d to nie jest liczba pierwsza, jest podzielne dla i = %d\n", aktualna_liczba, i);
                czy_pierwsza = false;
                break;
            }
        }

        // Jeżeli tak sprawdź warunki
        if (czy_pierwsza){
            printf("%d jest liczbą pierwsza.\n", aktualna_liczba);


            suma += aktualna_liczba;
            ilosc_znalezionych_liczb ++;
        }
    }

    printf("Suma wszystkich liczb pierwszych %d liczb pierwszych jest równa: %d\n", N, suma);

    return 0;
}