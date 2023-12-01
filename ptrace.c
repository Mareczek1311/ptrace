#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>

void title(){






printf(" ███▄ ▄███▓   ▓█████     ███▄ ▄███▓    ▒█████      ██▀███     ▓██   ██▓       ██▓ ██▓          ██▓       ▓█████     ▄▄▄          ██ ▄█▀\n");
printf("▓██▒▀█▀ ██▒   ▓█   ▀    ▓██▒▀█▀ ██▒   ▒██▒  ██▒   ▓██ ▒ ██▒    ▒██  ██▒      ▓██▒▓██▒         ▓██▒       ▓█   ▀    ▒████▄        ██▄█▒ \n");
printf("▓██    ▓██░   ▒███      ▓██    ▓██░   ▒██░  ██▒   ▓██ ░▄█ ▒     ▒██ ██░      ▒██▒▒██▒         ▒██░       ▒███      ▒██  ▀█▄     ▓███▄░ \n");
printf("▒██    ▒██    ▒▓█  ▄    ▒██    ▒██    ▒██   ██░   ▒██▀▀█▄       ░ ▐██▓░      ░██░░██░         ▒██░       ▒▓█  ▄    ░██▄▄▄▄██    ▓██ █▄ \n");
printf("▒██▒   ░██▒   ░▒████▒   ▒██▒   ░██▒   ░ ████▓▒░   ░██▓ ▒██▒     ░ ██▒▓░      ░██░░██░         ░██████▒   ░▒████▒    ▓█   ▓██▒   ▒██▒ █▄\n");
printf("░ ▒░   ░  ░   ░░ ▒░ ░   ░ ▒░   ░  ░   ░ ▒░▒░▒░    ░ ▒▓ ░▒▓░      ██▒▒▒       ░▓  ░▓           ░ ▒░▓  ░   ░░ ▒░ ░    ▒▒   ▓▒█░   ▒ ▒▒ ▓▒\n");
printf("░  ░      ░    ░ ░  ░   ░  ░      ░     ░ ▒ ▒░      ░▒ ░ ▒░    ▓██ ░▒░        ▒ ░ ▒ ░         ░ ░ ▒  ░    ░ ░  ░     ▒   ▒▒ ░   ░ ░▒ ▒░\n");
printf("░      ░         ░      ░      ░      ░ ░ ░ ▒       ░░   ░     ▒ ▒ ░░         ▒ ░ ▒ ░           ░ ░         ░        ░   ▒      ░ ░░ ░ \n");
printf("       ░         ░  ░          ░          ░ ░        ░         ░ ░            ░   ░               ░  ░      ░  ░         ░  ░   ░  ░   \n");
printf("                                                               ░ ░                                                                     \n");








}


void get(int pid, long addr, char* laddr, int len){

    union u {
        long val;
        char chars[len];
    }data;

    int i,j;
    i = 0;
    j = len/sizeof(long);

    while(i<j){
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr+i*8, 0);
        ++i;
        memcpy(laddr, data.chars, sizeof(long));
    }

    j = len % sizeof(long);

    if(j!=0){
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr +i*8, 0);
        memcpy(laddr, data.chars, j);
    }
    laddr[len]='\0';

}

void insert(int pid, long addr, char* laddr, int len){
    union u {
        long val;
        char chars[len];
    }data;
    int i=0;
    int j=len/sizeof(long);
    printf("INJECTING DECODED PASSWORD... \n");
    while(i<len){
        memcpy(data.chars, laddr, sizeof(long));
        ptrace(PTRACE_POKEDATA, pid,
        addr + i * 8, data.val);
        ++i;
        laddr += sizeof(long);
        i++;
    }

    j = len%sizeof(long);
    if(j != 0){
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, pid,
        addr + i * 8, data.val);
    }
}

void decode(char* laddr, int shift, int len){

    for(int i=0; i<len; i++){
        laddr[i] -= shift;
    }
}

int main(int argc, char **argv) {

    system("clear");
    title();
    int pid, status;
    struct user_regs_struct regs;

    printf("PROCESS PID >>> ");
    scanf("%d", &pid);
    if(ptrace(PTRACE_ATTACH, pid, 0, 0)){
        perror("ptrace ATTACH");
        exit(EXIT_FAILURE);
    }
    wait(&status);
    if(ptrace(PTRACE_CONT, pid, 0, 0) == -1){
        perror("ptrace CONT");
        exit(EXIT_FAILURE);
    }

    sleep(1);
    kill(pid, SIGINT);
    wait(&status);

    if(ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1){
        perror("ptrace GETREGS");
        exit(EXIT_FAILURE);
    }

    int len = ptrace(PTRACE_PEEKDATA, pid, regs.rcx, 0);
    int shift = ptrace(PTRACE_PEEKDATA, pid, regs.rdx, 0);
    printf("PASSWORD LENGTH << : %d \n", len);

    printf("SHIFT << %d \n", shift);

    //zmiana flagi aby wyjsc z petli w asm
    regs.rbx = 2; // equal crash without print

    long addr = regs.rsi;
    //TUTAJ

    char *laddr = (char*) malloc((64)*sizeof(char));
    get(pid, addr, laddr, len);

    printf("CODED PASSWORD << %s \n", laddr);

    printf("DECODING...\n");
    decode(laddr, shift, len);
    sleep(1);

    printf("DECODED PASSWORD << %s \n", laddr);
    insert(pid, addr, laddr, len);
    sleep(1);
    printf("PASSWORD INJECTED! \n");
    ptrace(PTRACE_SETREGS, pid, 0, &regs);

    ptrace(PTRACE_CONT, pid, 0, 0);
    wait(&status);

    if (WIFEXITED(status)) {
        printf("Serwer zakonczony \n\n");
    }

    return 0;
}
