#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Candidato.h"

#define NUMBER_CANDIDATES 11


// vetor contendo todos os candidatos possíveis
struct Candidato candidates[NUMBER_CANDIDATES];


// thread method que envia os candidatos para o cliente via socket
void *send_candidates(void *socket) {

    // socket descriptor e variaveis que serao usadas para definir o vencedor
    int i, sock = *(int*)socket, max = 0, winner = 0;
    char confirm[1024];

    // cada candidato é enviado por vez e atualiza o vencedor à cada iteração
    for(i=0; i < NUMBER_CANDIDATES; i++){


        // mandando nome do candidato
        send(sock, candidates[i].nome_candidato, strlen(candidates[i].nome_candidato), 0);
        recv(sock , confirm , 256 , 0);

        // mandando nome do partido
        send(sock, candidates[i].partido, strlen(candidates[i].partido), 0);
        recv(sock , confirm , 256 , 0);

        // mandando codigo votacao
        char codigo_votacao[30];
        sprintf(codigo_votacao, "%d", candidates[i].codigo_votacao);
        send(sock, codigo_votacao, strlen(codigo_votacao), 0);
        recv(sock , confirm , 256 , 0);

    }

    // liberando memoria do socket
    free(socket);

    return 0;

}


// thread para votar em candidato
void *vote_candidate(void *socket) {

    // socket descriptor e iterator para realizar a votação
    int i, sock = *(int*)socket;
    char confirm[1024];

    // recebendo o codigo de votação do client
    recv(sock , confirm , 256 , 0);
    int codigo_votacao = atoi( confirm );

    // computando a votação
    for(i=0; i < NUMBER_CANDIDATES; i++){

        // caso encontre o candidato, adicione o voto do client
        if(codigo_votacao == candidates[i].codigo_votacao){
            candidates[i].num_votos++;
            send(sock, "Seu voto foi computado com sucesso!", strlen("Seu voto foi computado com sucesso!"), 0);
            break;
        }
        // caso candidato nao existe, enviar mensagem de candidato nao encontrado para o client
        else if(i + 1 == NUMBER_CANDIDATES)
            send(sock, "Candidato não encontrado!", strlen("Candidato não encontrado!"), 0);
    }

    // liberando memoria do socket
    free(socket);

    return 0;

}


// thread para voto em branco
void *white_vote(void *socket) {

    // socket descriptor e variaveis que serao usadas para definir o candidato a receber o voto
    int i, sock = *(int*)socket, max = 0, winner = 0;
    char confirm[1024];

    // procura candidato com maior numero de votos
    for(i=0; i < NUMBER_CANDIDATES; i++){

        if(candidates[i].num_votos > max){
            winner = i;
            max = candidates[winner].num_votos;
        }
    }

    // verifica se ja há algum candidato com algum voto, caso contrario, não é possivel realizar votacao em branco
    if (max) {

        // computa voto no candidato e envia mensagem de sucesso para o client
        candidates[winner].num_votos++;
        send(sock, "Voto branco computado com sucesso!", strlen("Voto branco computado com sucesso!"), 0);

    }
    else{

        // envia candidato vencedor para o cliente e por fim libera o socket
        send(sock, "nenhum voto recebido", strlen("nenhum voto recebido"), 0);
    }


    // liberando memoria do socket
    free(socket);

    return 0;

}


// thread para voto em nulo
void *null_vote(void *socket) {

    // socket descriptor
    int sock = *(int*)socket;

    // computa voto no "candidato" nulo e envia mensagem para o client
    candidates[10].num_votos++;
    send(sock, "Voto nulo recebido com sucesso!", strlen("Voto nulo recebido com sucesso!"), 0);

    // liberando memoria do socket
    free(socket);

    return 0;

}

// thread method e envia todos os candidados para o cliente e por fim envia o candidato vencedor
void *counting_votes(void *socket) {


    // socket descriptor e variaveis que serao usadas para definir o vencedor
    int i, sock = *(int *) socket, max = 0, winner = 0;
    char confirm[1024];

    // cada candidato é enviado por vez e atualiza o vencedor à cada iteração
    for (i = 0; i < NUMBER_CANDIDATES; i++) {
        if (candidates[i].num_votos > max) {
            winner = i;
            max = candidates[winner].num_votos;
        }
    }

    // verifica se ja há algum candidato com algum voto, caso contrario, não é possivel contacao
    if (max) {

        // envia candidato vencedor para o cliente e por fim libera o socket
        send(sock, candidates[winner].nome_candidato, strlen(candidates[winner].nome_candidato), 0);
        recv(sock, confirm, 256, 0);

        // mandando numero de votos
        char num_votos[30];
        sprintf(num_votos, "%d", candidates[winner].num_votos);
        send(sock, num_votos, strlen(num_votos), 0);
        recv(sock , confirm , 256 , 0);

    }
    else{

        // envia candidato vencedor para o cliente e por fim libera o socket
        send(sock, "nenhum voto recebido", strlen("nenhum voto recebido"), 0);
        recv(sock, confirm, 256, 0);
    }

    free(socket);

    return 0;
}




// main function
int main(int argc, char *argv[]) {
    int server_socket, client_socket, n, c, op_code, *n_socket;
    struct sockaddr_in server, client;
    char buf[1024], *op_code_c;
    pthread_t thread_h;

    // method que vai fazer o load dos candidatos
    loadCandidates(candidates);


    // configurando o server para utilizar IPv4 e SOCK_STREAM para TCP (reliable-connection)
    server_socket=socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(40002);

    // fazendo bing
    bind(server_socket, (struct sockaddr*) &server, sizeof(server));

    // escutando ate 10 clientes ao mesmo tempo
    listen(server_socket, 10);

    c = sizeof(client);

    while (1) {

        // cria conexao com o cliente e prossiga, caso contrario, tenta novamente até obter sucesso na conexão
        if((client_socket = accept(server_socket, (struct sockaddr*)&client, &c )) > 0){

      printf("Conexão estabelecida.\n");

            // recebe o op_code do cliente e converte para int
            recv(client_socket , buf , 2048 , 0);
            op_code_c = malloc(sizeof(char) * n - 1);
            strncpy(op_code_c, buf, n-1);
            op_code = atoi(op_code_c);
            free(op_code_c);

            printf("Número de op_code recebido: %d\n", op_code);


            switch(op_code) {

                // votar em candidato
                case 1 :
                    n_socket = malloc(1);
                    *n_socket = client_socket;
                    pthread_create(&thread_h, NULL, vote_candidate, (void*) n_socket);
                    break;

                // voto em branco
                case 2 :
                    n_socket = malloc(1);
                    *n_socket = client_socket;
                    pthread_create(&thread_h, NULL, white_vote, (void*) n_socket);
                    break;

                // voto nulo
                case 3 :
                    n_socket = malloc(1);
                    *n_socket = client_socket;
                    pthread_create(&thread_h, NULL, null_vote, (void*) n_socket);
                    break;

                // enviar candidatos
                case 999  :
                    n_socket = malloc(1);
                    *n_socket = client_socket;
                    pthread_create(&thread_h, NULL, send_candidates, (void*) n_socket);
                    break;

                // computar vencedor
                case 888  :
                    n_socket = malloc(1);
                    *n_socket = client_socket;
                    pthread_create(&thread_h, NULL, counting_votes, (void*) n_socket);
                    break;

            }


        }
        else
           printf("Conexão falhou. boa sorte\n");
    }


}
