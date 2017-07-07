#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define NUMBER_CANDIDATES 11



int main(int argc, char *argv[])
{
    int sock, length, n, i;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];
    struct Candidato *candidate_p;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("143.107.232.253");
    server.sin_family = AF_INET;
    server.sin_port = htons(40002);


    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }


    printf("1   - Votar\n");
    printf("2   - Votar branco\n");
    printf("3   - Votar nulo\n");
    printf("999 - Carregar a lista de candidatos do servidor\n");
    printf("888 – Finalizar as votações da urna e enviar ao servidor\n");
    printf("Inserir o op_code de acordo com o desejado: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    send(sock, buffer, strlen(buffer), 0);
    char *op_code_c = malloc(sizeof(char) * strlen(buffer) - 1);
    memcpy(op_code_c, &buffer[0], strlen(buffer) - 1);
    int op_code = atoi(op_code_c);

    switch(op_code) {

        // votar em candidato
        case 1  :
            printf("Por favor digite o numero do seu candidato: ");
            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
            send(sock, buffer, strlen(buffer), 0);
            recv(sock, buffer, 256, 0);
            printf("%s\n", buffer);
            break;

        // voto em branco
        case 2  :
            recv(sock, buffer, 256, 0);
            strcmp(buffer, "nenhum voto recebido") ? printf("\n%s\n", buffer) : printf("\nImpossivel computar voto branco, pois não há candidato votado.\n");
            break;

        // voto nulo
        case 3  :
            recv(sock, buffer, 256, 0);
            printf("%s\n", buffer);
            break;

        // enviar candidatos
        case 999  :

            for (i = 0; i < NUMBER_CANDIDATES; i++) {

                // get nome
                recv(sock, buffer, 256, 0);
                printf("\nCandidate name: %s\n", buffer);
                send(sock, "confirmation", strlen("confirmation"), 0);

                // esvaziando buffer
                memset(buffer, '\0', sizeof(buffer));

                // get nome partido
                recv(sock, buffer, 256, 0);
                printf("Candidate party: %s\n", buffer);
                send(sock, "confirmation", strlen("confirmation"), 0);

                // esvaziando buffer
                memset(buffer, '\0', sizeof(buffer));

                // get nome partido
                recv(sock, buffer, 256, 0);
                printf("Candidate voting number: %s\n", buffer);
                send(sock, "confirmation", strlen("confirmation"), 0);

            }

            break;

        // computar vencedor
        case 888  :

            // get nome
            recv(sock, buffer, 256, 0);

            if (strcmp(buffer, "nenhum voto recebido")) {
                printf("\nWinner: %s! ", buffer);
                send(sock, "confirmation", strlen("confirmation"), 0);

                // esvaziando buffer
                memset(buffer, '\0', sizeof(buffer));

                // get numero votos
                recv(sock, buffer, 256, 0);
                printf("Number of votes: %s\n", buffer);
                send(sock, "confirmation", strlen("confirmation"), 0);
            }
            else{
                printf("\nNão foi possivel computar vencedor, pois não foi recebido nenhum voto! ", buffer);
            }



            break;

    }










}

