#include "ft_knn.h"

#define NUM_EXECUCOES 1000

/// @file infer_time.c
/// @brief Mede o tempo médio de inferência do classificador KNN em C.
/// Realiza múltiplas classificações da mesma amostra para estimar desempenho.
int main() {
    Dataset dataset;
    if (!load_dataset_from_csv(DATASET_PATH, &dataset)) return 1;

    Dataset train, test;
    shuffle_dataset(&dataset);
    split_dataset(&dataset, &train, &test, TRAIN_PERCENTAGE);

    float mean[MAX_FEATURES], std[MAX_FEATURES];
    compute_mean_std(&train, mean, std);
    apply_normalization(&train, mean, std);
    apply_normalization(&test, mean, std);

    // --- Medição do tempo só da inferência ---
    int k = 15;
    clock_t start = clock();
    for (int i = 0; i < NUM_EXECUCOES; i++) {
        classify(&test.samples[0], &train, train.num_samples, k);
    }
    clock_t end = clock();

    double tempo_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0 / NUM_EXECUCOES;

    // Teste pra debug
    printf("Tempo medio por inferencia (k=%d, %d execucoes): %.6f ms\n", k, NUM_EXECUCOES, tempo_ms);

    // Energia estimada
    float tensao = 3.3f;    // volts
    float corrente = 0.08f; // amperes
    float tempo_seg = tempo_ms / 1000.0f;
    float energia = tensao * corrente * tempo_seg;
    printf("Energia estimada por inferencia: %.8f J\n", energia);

    return 0;
}