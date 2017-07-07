//
// Created by guerra on 15/06/17.
//

#ifndef SERVER_CANDIDATO_H
#define SERVER_CANDIDATO_H

#endif //SERVER_CANDIDATO_H


struct Candidato {
    int codigo_votacao;
    char nome_candidato[25];
    char partido[10];
    int num_votos;
};

void loadCandidates(struct Candidato candidates[]);
