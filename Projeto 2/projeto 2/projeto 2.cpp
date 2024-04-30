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

void imprimirEscalonamento(const std::vector<Tarefa>& tarefas, int ciclo_primario, int ciclo_secundario, int heuristica) {
    std::cout << "Escalonamento Sugerido (Heuristica: ";
    if (heuristica == 0)
        std::cout << "Menor Tempo de Execucao Primeiro - SETF):\n";
    else
        std::cout << "Maior Taxa de Periodicidade Primeiro - HRF):\n";

    std::cout << "----------------------------------------------------------------------\n";

    int num_ciclos = ciclo_primario / ciclo_secundario;

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

void imprimirResumo(int total_ciclos, int total_intercambios, double utilizacao_cpu) {
    std::cout << "Resumo:\n";
    std::cout << "-------\n";
    std::cout << "Total de Ciclos: " << total_ciclos << "\n";
    std::cout << "Total de Intercambios de Tarefa por Ciclo: " << total_intercambios << "\n";
    std::cout << "Utilizacao da CPU: " << utilizacao_cpu * 100 << "%\n";
}

int main() {

    std::locale::global(std::locale(""));
    std::ifstream arquivo("tarefas2.json");
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
        if (t.tempo_execucao > t.periodo) {  //VERIFICA SE A TAREFA É ESCALONAVEL
            std::cout << "" << t.id << " nao escalonavel\n";
            exit(1);
        }
        tarefas.push_back(t);
    }

    arquivo.close();

    if (tarefas.empty()) {
        std::cerr << "Nenhuma tarefa escalonável encontrada.\n";
        return 1;
    }

    // Calculando tempos de ciclo
    int ciclo_primario = calcularCicloPrimario(tarefas);
    int ciclo_secundario = calcularCicloSecundario(tarefas);

    // Escolhendo a heurística apropriada
    int heuristica = escolherHeuristica(tarefas);

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

    // Imprimindo o resumo
    int num_ciclos = ciclo_primario / ciclo_secundario;
    int num_intercambios = tarefas.size();
    double utilizacao_cpu = (static_cast<double>(num_intercambios) / num_ciclos);
    imprimirResumo(num_ciclos, num_intercambios, utilizacao_cpu);



    return 0;
}
