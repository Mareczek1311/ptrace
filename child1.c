#include <stdio.h>
#include<time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
char* szyfrowanie(char mess[], int przes, int len){

    for(int i=0; i<len; i++){
        mess[i] += przes;
    }
    return mess;
}
*/

int main(int argc, char **argv) {

    if(argc < 2){
        printf("<< WRONG NUMBER OF ARGUMENTS >>\n");
        printf("<< ./serwer PASSWORD SHIFT >> \n");
        exit(EXIT_FAILURE);
    }

    int dlugosc = strlen(argv[1]);

    if(dlugosc > 7){
        printf("<< PASSWORD TOO LONG >>\n");
        printf("<< PASSWORDS WITH LENGTH <= 7 >>\n");
        exit(EXIT_FAILURE);
    }

    int przes = atoi(argv[2]);
    int* p1 = &dlugosc;
    int* p2 = &przes;

    char* szyfr = (char*)malloc(dlugosc*sizeof(char));
    char klucz[dlugosc];

   /*
    char dlstr[3];
    sprintf(dlstr, "%d", dlugosc);
    char pazstr[3];
    sprintf(przstr, "%d", przes);
    */



    for(int i=0; i<dlugosc; i++){
        klucz[i] = argv[1][i];
    }


    if (szyfr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(szyfr, argv[1]);
    szyfr[dlugosc] = '\0';
    klucz[dlugosc] = '\0';


    for (int i = 0; i < 7; i++) {
        szyfr[i] += przes;
    }

    printf("%s \n", szyfr);
    asm (
        "mov $1, %%rbx\n"
        "L1: mov %0, %%rsi\n"
        "mov %1, %%rcx\n"
        "mov %2, %%rdx\n"
        "cmp $1, %%rbx\n"
        "je L1\n\t"
        "jne L2\n"
        "L2:\n"
        :
        : "r" (szyfr),   // Wejście dla zmienna_a
          "r" (p1),   // Wejście dla zmienna_b
          "r" (p2)
        : "%rax", "%rdi", "%rsi", "%rdx", "%rbx", "%rcx"
    );

    //printf("szyfr:  %s \n", szyfr);

    //printf("KLUCZ:  %s \n", klucz);
    if (strcmp(szyfr, klucz) == 0) {
        printf("-------DANE------- \n");
        printf("IMIE: Marek \n");
        printf("NAZWISKO: Kretkowski \n");
        printf("STAN KONTA: 1zl \n");
        printf("HASLO: ABCD \n");
    }

    return 0;
}
