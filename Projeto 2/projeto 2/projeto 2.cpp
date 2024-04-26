#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <locale>
#include <string.h>
using namespace std;

using json = nlohmann::json;

struct Tarefa {
    std::string id;
    int periodo;
    int tempo_execucao;
    int prioridade;
};

int mdc(int a, int b) {
    
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mmc(int a, int b) {
    return a * (b / mdc(a, b));
}

int calcularCicloPrimario(const std::vector<Tarefa>& tarefas) {
    int ciclo_primario = tarefas[0].periodo;
    for (size_t i = 1; i < tarefas.size(); i++) {
        ciclo_primario = mmc(ciclo_primario, tarefas[i].periodo);
    }
    return ciclo_primario;
}

int calcularCicloSecundario(const std::vector<Tarefa>& tarefas) {
    int ciclo_secundario = tarefas[0].periodo;
    for (size_t i = 1; i < tarefas.size(); i++) {
        ciclo_secundario = mdc(ciclo_secundario, tarefas[i].periodo);
    }
    return ciclo_secundario;
}

bool compararPorTempoExecucao(const Tarefa& a, const Tarefa& b) {
    return a.tempo_execucao < b.tempo_execucao;
}

bool compararPorPrioridade(const Tarefa& a, const Tarefa& b) {
    return a.prioridade < b.prioridade;
}

bool compararPorTaxaPeriodicidade(const Tarefa& a, const Tarefa& b) {
    return (1.0 / a.periodo) > (1.0 / b.periodo);
}


int escolherHeuristica(const std::vector<Tarefa>& tarefas) {
    float taxa_periodicidade_media = 0;
    for (const auto& tarefa : tarefas) {
        taxa_periodicidade_media += 1.0 / tarefa.periodo;
    }
    taxa_periodicidade_media /= tarefas.size();

    if (taxa_periodicidade_media < 1)
        return 0; // SETF
    else
        return 1; // HRF
}

void imprimirEscalonamento(std::vector<Tarefa>& tarefas, int ciclo_primario, int ciclo_secundario, int heuristica) {
    std::cout << "Escalonamento Sugerido (Heuristica: ";
    if (heuristica == 0)
        std::cout << "Menor Tempo de Execucao Primeiro - SETF):\n";
    else
        std::cout << "Maior Taxa de Periodicidade Primeiro - HRF):\n";

    std::cout << "----------------------------------------------------------------------\n";

    int num_ciclos = ciclo_primario / ciclo_secundario;

    // Ordenando as tarefas por taxa de periodicidade se a heurística for HRF
    if (heuristica == 1)
        std::sort(tarefas.begin(), tarefas.end(), compararPorTaxaPeriodicidade);

    for (int ciclo = 0; ciclo < num_ciclos; ciclo++) {
        std::cout << "Ciclo " << ciclo + 1 << ":\n";
        for (const auto& tarefa : tarefas) {
            if (ciclo % (tarefa.periodo / ciclo_secundario) == 0) {
                std::cout << "  - " << tarefa.id << ": tempo de execucao = " << tarefa.tempo_execucao
                    << ", periodo = " << tarefa.periodo << ", prioridade = " << tarefa.prioridade << "\n";
            }
        }
    }
    std::cout << "\n";
}

int main() {
    
    std::locale::global(std::locale(""));
    std::ifstream arquivo("tarefas.json");
    if (!arquivo.is_open()) {
        std::cerr << "Nao foi possível abrir o arquivo.\n";
        return 1;
    }

    json j;
    arquivo >> j;

    std::vector<Tarefa> tarefas;
    for (const auto& tarefa_json : j["tarefas"]) {
        Tarefa t;
        t.id = tarefa_json["id"];
        t.periodo = tarefa_json["periodo"];
        t.tempo_execucao = tarefa_json["tempo_execucao"];
        t.prioridade = tarefa_json["prioridade"];
        tarefas.push_back(t);
    }

    arquivo.close();

    // Calculando tempos de ciclo
    int ciclo_primario = calcularCicloPrimario(tarefas);
    int ciclo_secundario = calcularCicloSecundario(tarefas);

    // Escolhendo a heurística apropriada
    int heuristica = 1;

    // Ordenando as tarefas de acordo com a heurística escolhida
    if (heuristica == 0)
        std::sort(tarefas.begin(), tarefas.end(), compararPorTempoExecucao);
    else
        std::sort(tarefas.begin(), tarefas.end(), compararPorTaxaPeriodicidade);

    // Imprimindo os tempos de ciclo
    std::cout << "Calculo de Ciclos para o Executivo Ciclico:\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "Tempo de Ciclo Primario: " << ciclo_primario << " unidades de tempo\n";
    std::cout << "Tempo de Ciclo Secundario: " << ciclo_secundario << " unidades de tempo\n\n";

    // Imprimindo o escalonamento sugerido
    imprimirEscalonamento(tarefas, ciclo_primario, ciclo_secundario, heuristica);

    return 0;
}
