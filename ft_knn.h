
#ifndef FT_KNN_H
#define FT_KNN_H

#define TRAIN_PERCENTAGE 0.8
#define TEST_PERCENTAGE  0.2
#define MAX_FEATURES 12
#define MAX_SAMPLES 1500
#define NUM_CLASSES 5
#define DATASET_PATH "data/dataset_features.csv"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    float features[MAX_FEATURES];
    int class;
} Sample;

typedef struct {
    Sample samples[MAX_SAMPLES];
    int num_samples;
    int num_features;
} Dataset;

typedef struct {
    float distance;
    int class;
} Neighbor;

/// @brief Carrega um dataset a partir de um arquivo CSV.
/// @param filename Caminho para o arquivo.
/// @param dataset Estrutura onde o dataset será armazenado.
/// @return true se carregou com sucesso, false caso contrário.
bool load_dataset_from_csv(const char *filename, Dataset *dataset);

/// @brief Embaralha aleatoriamente as amostras do dataset.
/// @param dataset Ponteiro para o dataset.
void shuffle_dataset(Dataset *dataset);

/// @brief Divide o dataset em treino e teste.
/// @param original Dataset original.
/// @param train Ponteiro para receber o conjunto de treino.
/// @param test Ponteiro para receber o conjunto de teste.
/// @param train_ratio Proporção de treino (ex: 0.8f).
void split_dataset(const Dataset *original, Dataset *train, Dataset *test, float train_ratio);

/// @brief Conta quantas amostras pertencem a uma dada classe.
/// @param dataset Dataset a ser verificado.
/// @param target_class Classe alvo.
/// @return Número de amostras da classe.
int count_class(const Dataset *dataset, int target_class);

/// @brief Calcula a média e o desvio padrão das features no dataset.
/// @param dataset Ponteiro para o dataset.
/// @param mean Vetor de médias.
/// @param std Vetor de desvios.
void compute_mean_std(const Dataset *dataset, float *mean, float *std);

/// @brief Aplica normalização Z-score nas features.
/// @param dataset Dataset a ser normalizado.
/// @param mean Médias das features.
/// @param std Desvios padrão das features.
void apply_normalization(Dataset *dataset, const float *mean, const float *std);

/// @brief Classifica uma amostra com KNN.
/// @param input Amostra a ser classificada.
/// @param train Dataset de treino.
/// @param train_size Número de amostras no treino.
/// @param k Número de vizinhos.
/// @return Classe predita.
int classify(const Sample *input, const Dataset *train, int train_size, int k);

/// @brief Função de comparação para qsort dos vizinhos por distância.
/// @param a Ponteiro para o primeiro vizinho.
/// @param b Ponteiro para o segundo vizinho.
/// @return Valor negativo se a < b, positivo se a > b, zero se iguais.
int compare_neighbors(const void *a, const void *b);

/// @brief Testa múltiplos valores de K e exibe a acurácia para cada um.
/// @param train Dataset de treino.
/// @param test Dataset de teste.
/// @param k_values Array com valores de K a testar.
/// @param num_k_values Número de valores de K no array.
void debug_test_multiple_k(const Dataset *train, const Dataset *test, const int *k_values, int num_k_values);

#endif