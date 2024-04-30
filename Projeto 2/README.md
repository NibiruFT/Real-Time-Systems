# Design do Sistema

O sistema foi projetado para calcular os tempos de ciclo primário e secundário para um conjunto de tarefas e utilizar uma heurística para sugerir um possível escalonamento das tarefas dentro desses ciclos.

# Estrutura de Dados
A estrutura de dados principal usada é a estrutura `Tarefa`, encontrada no arquivo [tarefas.json](https://github.com/NibiruFT/Sistemas-em-Tempo-Real/blob/main/Projeto%202/projeto%202/tarefas.json), que contém as seguintes propriedades:

`id`: Uma string que identifica a tarefa.

`periodo`: Um inteiro que representa o período da tarefa.

`tempo_execucao`: Um inteiro que representa o tempo de execução da tarefa.

`prioridade`: Um inteiro que representa a prioridade da tarefa.

# Funções Principais
O sistema contém várias funções para calcular os tempos de ciclo e escalonar as tarefas. As funções principais podem ser encontradas no arquivo [projeto 2.cpp](https://github.com/NibiruFT/Sistemas-em-Tempo-Real/blob/main/Projeto%202/projeto%202/projeto%202.cpp) e incluem `calcularCicloPrimario`, `calcularCicloSecundario`, `escolherHeuristica` e `imprimirEscalonamento`.

# Decisões de Implementação
O sistema foi implementado em C++. Além disso, o sistema suporta duas heurísticas de escalonamento: Menor Tempo de Execução Primeiro (SETF) e Maior Taxa de Periodicidade Primeiro (HRF). A heurística é escolhida com base na taxa de periodicidade média das tarefas. As tarefas são ordenadas de acordo com a heurística escolhida. Para a heurística SETF, as tarefas são ordenadas por tempo de execução.

## Vídeo Explicativo

> **Descrição Geral**: Criar um sistema executivo cíclico.
> 
> **Nota 1**: [Video](https://youtu.be/PVLUpZMb-Ac) 
