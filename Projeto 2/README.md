# Design do Sistema

O sistema foi projetado para calcular os tempos de ciclo primário e secundário para um conjunto de tarefas e utilizar uma heurística para sugerir um possível escalonamento das tarefas dentro desses ciclos.


# Funções Principais
O sistema contém várias funções para calcular os tempos de ciclo e escalonar as tarefas. As funções principais podem ser encontradas no arquivo [projeto 2.cpp](https://github.com/NibiruFT/Sistemas-em-Tempo-Real/blob/main/Projeto%202/projeto%202/projeto%202.cpp) e incluem `calcularCicloPrimario`, `calcularCicloSecundario`, `escolherHeuristica` e `imprimirEscalonamento`.

## 1. Estrutura de Dados

A estrutura de dados principal usada é a estrutura `Tarefa`, encontrada no arquivo [tarefas.json](https://github.com/NibiruFT/Sistemas-em-Tempo-Real/blob/main/Projeto%202/projeto%202/tarefas.json), que contém as seguintes propriedades:

`id`: Uma string que identifica a tarefa.

`periodo`: Um inteiro que representa o período da tarefa.

`tempo_execucao`: Um inteiro que representa o tempo de execução da tarefa.

`prioridade`: Um inteiro que representa a prioridade da tarefa.

## 2. Cálculo dos Tempos de Ciclo

### `calcularCicloPrimario`

- Essa função calcula o tempo de ciclo primário, que é o mínimo múltiplo comum (MMC) dos períodos de todas as tarefas.
- Ela itera pelas tarefas e usa a função mmc(a, b) para calcular o MMC.
- O MMC é essencial para garantir que todas as tarefas se repitam sincronizadas.

### `calcularCicloSecundario`

- Essa função calcula o tempo de ciclo secundário, que é a soma dos tempos de execução de todas as tarefas.
- Ela simplesmente soma os tempos de execução de todas as tarefas.

## **3. Escolha da Heurística**

### `escolherHeuristica`

- Essa função escolhe a heurística de escalonamento com base na taxa de periodicidade média das tarefas.
- Se a taxa média for menor que 1, a heurística escolhida é SETF (Menor Tempo de Execução Primeiro).
- Caso contrário, a heurística escolhida é HRF (Maior Taxa de Periodicidade Primeiro).

## **4. Escalonamento das Tarefas**

### `imprimirEscalonamento`

- Essa função imprime o escalonamento sugerido para as tarefas.
- Ela itera pelos ciclos e verifica quais tarefas podem ser executadas em cada ciclo.


# Decisões de Implementação
O sistema foi implementado em C++. Além disso, o sistema suporta a heurística de escalonamento mais utilizada: Menor Tempo de Execução Primeiro (SETF). As tarefas são ordenadas de acordo com a heurística escolhida. Para a heurística SETF, as tarefas são ordenadas por tempo de execução.

## Vídeo Explicativo

> **Descrição Geral**: Criar um sistema executivo cíclico.
> 
> **Nota 1**: [Video](https://youtu.be/PVLUpZMb-Ac) 
