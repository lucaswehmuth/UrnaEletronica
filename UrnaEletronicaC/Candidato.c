//
// Created by guerra on 15/06/17.
//

#include "Candidato.h"


void loadCandidates(struct Candidato candidates[]){

    candidates[0] = (struct Candidato) {
            .codigo_votacao = 1,
            .nome_candidato = "Karl Marx",
            .partido = "TCM",
            .num_votos = 0
    };

    candidates[1] = (struct Candidato) {
            .codigo_votacao = 2,
            .nome_candidato = "Friedrich Engels",
            .partido = "TCM",
            .num_votos = 0
    };

    candidates[2] = (struct Candidato) {
            .codigo_votacao = 3,
            .nome_candidato = "Rosa Luxemburg",
            .partido = "ISDPG",
            .num_votos = 0
    };

    candidates[3] = (struct Candidato) {
            .codigo_votacao = 4,
            .nome_candidato = "Vladimir Lenin",
            .partido = "CPSU",
            .num_votos = 0
    };

    candidates[4] = (struct Candidato) {
            .codigo_votacao = 5,
            .nome_candidato = "Leon Trotsky",
            .partido = "SDP",
            .num_votos = 0
    };

    candidates[5] = (struct Candidato) {
            .codigo_votacao = 6,
            .nome_candidato = "Joseph Stalin",
            .partido = "CPSU",
            .num_votos = 0
    };

    candidates[6] = (struct Candidato) {
            .codigo_votacao = 7,
            .nome_candidato = "Ethel Rosenberg",
            .partido = "CPUSA",
            .num_votos = 0
    };

    candidates[7] = (struct Candidato) {
            .codigo_votacao = 8,
            .nome_candidato = "Mao Tse-tung",
            .partido = "CPChina",
            .num_votos = 0
    };

    candidates[8] = (struct Candidato) {
            .codigo_votacao = 9,
            .nome_candidato = "Fidel Castro",
            .partido = "CPCuba",
            .num_votos = 0
    };

    candidates[9] = (struct Candidato) {
            .codigo_votacao = 10,
            .nome_candidato = "Che Guevara",
            .partido = "CPCuba",
            .num_votos = 0
    };

    candidates[10] = (struct Candidato) {
            .codigo_votacao = 0,
            .nome_candidato = "Voto Nulo",
            .partido = "NULO",
            .num_votos = 0
    };



}