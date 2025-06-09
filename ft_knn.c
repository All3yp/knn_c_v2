#include "ft_knn.h"
#include <math.h>
#include <string.h>

/// @brief Função de comparação para qsort dos vizinhos por distância.
/// @param a Ponteiro para o primeiro vizinho.
/// @param b Ponteiro para o segundo vizinho.
/// @return Valor negativo se a < b, positivo se a > b, zero se iguais.
int compare_neighbors(const void *a, const void *b) {
    const Neighbor *na = (const Neighbor *)a;
    const Neighbor *nb = (const Neighbor *)b;
    if (na->distance < nb->distance) return -1;
    if (na->distance > nb->distance) return 1;
    return 0;
}

/// @brief Carrega um dataset a partir de um arquivo CSV.
/// @param filename Caminho para o arquivo.
/// @param dataset Estrutura onde o dataset será armazenado.
/// @return true se carregou com sucesso, false caso contrário.
bool load_dataset_from_csv(const char *filename, Dataset *dataset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }

    dataset->num_samples = 0;
    dataset->num_features = MAX_FEATURES;
    char line[1024];

    // Skip header if exists
    if (fgets(line, sizeof(line), file)) {
        // Check if first line contains numbers (no header) or text (header)
        float test_val;
        if (sscanf(line, "%f", &test_val) != 1) {
            // Has header, continue to next line
        } else {
            // No header, rewind and process this line
            rewind(file);
        }
    }

    while (fgets(line, sizeof(line), file) && dataset->num_samples < MAX_SAMPLES) {
        Sample *sample = &dataset->samples[dataset->num_samples];
        
        // Parse features and class from CSV
        char *token = strtok(line, ",");
        int feature_count = 0;
        
        while (token && feature_count < MAX_FEATURES) {
            sample->features[feature_count] = atof(token);
            token = strtok(NULL, ",");
            feature_count++;
        }
        
        // Last token should be the class
        if (token) {
            sample->class = atoi(token);
            dataset->num_samples++;
        }
    }

    fclose(file);
    return dataset->num_samples > 0;
}

/// @brief Embaralha aleatoriamente as amostras do dataset.
/// @param dataset Ponteiro para o dataset.
void shuffle_dataset(Dataset *dataset) {
    srand((unsigned int)time(NULL));
    for (int i = dataset->num_samples - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Sample temp = dataset->samples[i];
        dataset->samples[i] = dataset->samples[j];
        dataset->samples[j] = temp;
    }
}

/// @brief Divide o dataset em treino e teste.
/// @param original Dataset original.
/// @param train Ponteiro para receber o conjunto de treino.
/// @param test Ponteiro para receber o conjunto de teste.
/// @param train_ratio Proporção de treino (ex: 0.8f).
void split_dataset(const Dataset *original, Dataset *train, Dataset *test, float train_ratio) {
    int train_size = (int)(original->num_samples * train_ratio);
    
    train->num_samples = train_size;
    train->num_features = original->num_features;
    
    test->num_samples = original->num_samples - train_size;
    test->num_features = original->num_features;
    
    for (int i = 0; i < train_size; i++) {
        train->samples[i] = original->samples[i];
    }
    
    for (int i = 0; i < test->num_samples; i++) {
        test->samples[i] = original->samples[train_size + i];
    }
}

/// @brief Conta quantas amostras pertencem a uma dada classe.
/// @param dataset Dataset a ser verificado.
/// @param target_class Classe alvo.
/// @return Número de amostras da classe.
int count_class(const Dataset *dataset, int target_class) {
    int count = 0;
    for (int i = 0; i < dataset->num_samples; i++) {
        if (dataset->samples[i].class == target_class) {
            count++;
        }
    }
    return count;
}

/// @brief Calcula a média e o desvio padrão de cada feature no dataset.
/// @param dataset Ponteiro para o dataset.
/// @param mean Vetor onde a média de cada feature será armazenada.
/// @param std Vetor onde o desvio padrão de cada feature será armazenado.
void compute_mean_std(const Dataset *dataset, float *mean, float *std) {
    for (int j = 0; j < dataset->num_features; j++) {
        float sum = 0.0f;
        for (int i = 0; i < dataset->num_samples; i++) {
            sum += dataset->samples[i].features[j];
        }
        mean[j] = sum / dataset->num_samples;
    }

    for (int j = 0; j < dataset->num_features; j++) {
        float sum_sq = 0.0f;
        for (int i = 0; i < dataset->num_samples; i++) {
            float diff = dataset->samples[i].features[j] - mean[j];
            sum_sq += diff * diff;
        }
        std[j] = sqrtf(sum_sq / dataset->num_samples);
        if (std[j] == 0.0f) std[j] = 1.0f; // evita divisão por zero
    }
}

/// @brief Aplica normalização Z-score nos dados (feature-wise).
/// @param dataset Ponteiro para o dataset a ser normalizado.
/// @param mean Vetor de médias de cada feature.
/// @param std Vetor de desvios padrão de cada feature.
void apply_normalization(Dataset *dataset, const float *mean, const float *std) {
    for (int i = 0; i < dataset->num_samples; i++) {
        for (int j = 0; j < dataset->num_features; j++) {
            dataset->samples[i].features[j] = (dataset->samples[i].features[j] - mean[j]) / std[j];
        }
    }
}

/// @brief Calcula a distância Euclidiana entre duas amostras.
/// @param a Ponteiro para a primeira amostra.
/// @param b Ponteiro para a segunda amostra.
/// @return A distância Euclidiana entre as duas amostras.
float euclidean_distance(const Sample *a, const Sample *b) {
    float sum = 0.0f;
    for (int i = 0; i < MAX_FEATURES; i++) {
        float diff = a->features[i] - b->features[i];
        sum += diff * diff;
    }
    return sqrtf(sum);
}

/// @brief Classifica uma amostra usando o algoritmo KNN.
/// @param input Amostra a ser classificada.
/// @param train Dataset de treino.
/// @param train_size Número de amostras no dataset de treino.
/// @param k Número de vizinhos a considerar.
/// @return Classe predita para a amostra de entrada.
int classify(const Sample *input, const Dataset *train, int train_size, int k) {
    Neighbor neighbors[MAX_SAMPLES];

    // Calcula a distância da amostra de entrada para cada amostra de treino
    for (int i = 0; i < train_size; i++) {
        neighbors[i].distance = euclidean_distance(input, &train->samples[i]);
        neighbors[i].class = train->samples[i].class;
    }

    qsort(neighbors, train_size, sizeof(Neighbor), compare_neighbors);

    int class_votes[NUM_CLASSES] = {0};
    for (int i = 0; i < k; i++) {
        class_votes[neighbors[i].class]++;
    }

    int max_votes = 0;
    int predicted_class = -1;
    for (int i = 0; i < NUM_CLASSES; i++) {
        if (class_votes[i] > max_votes) {
            max_votes = class_votes[i];
            predicted_class = i;
        }
    }

    return predicted_class;
}

// MARK: - SÓ PRA TESTE
void debug_test_multiple_k(const Dataset *train, const Dataset *test, const int *k_values, int num_k_values) {
    printf("\n=== Resultado da acuracia para multiplos valores de K ===\n");
    for (int i = 0; i < num_k_values; i++) {
        int k = k_values[i];
        int correct = 0;
        for (int j = 0; j < test->num_samples; j++) {
            int predicted = classify(&test->samples[j], train, train->num_samples, k);
            if (predicted == test->samples[j].class) correct++;
        }
        float acc = (float)correct / test->num_samples * 100.0f;
        printf("K = %2d -> Acuracia: %.2f%%\n", k, acc);
    }
}
