#include "ft_knn.h"

#define NUM_EXECUCOES 1000

/// @file infer_time.c
/// @brief Mede o tempo médio de inferência do classificador KNN em C.
/// Realiza múltiplas classificações da mesma amostra para estimar desempenho.
int main() {
    Dataset dataset;
    if (!load_dataset_from_csv(DATASET_PATH, &dataset)) {
        fprintf(stderr, "Erro ao carregar dataset\n");
        return 1;
    }

    Dataset train, test;
    shuffle_dataset(&dataset);
    split_dataset(&dataset, &train, &test, 0.8f);

    float mean[MAX_FEATURES], std[MAX_FEATURES];
    compute_mean_std(&train, mean, std);
    apply_normalization(&train, mean, std);
    apply_normalization(&test, mean, std);

    int k = 15;
    clock_t start = clock();
    for (int i = 0; i < NUM_EXECUCOES; i++) {
        classify(&test.samples[0], &train, train.num_samples, k);
    }
    clock_t end = clock();

    double tempo_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0 / NUM_EXECUCOES;
    printf("Tempo medio de inferencia (%d execucoes): %.6f ms\n", NUM_EXECUCOES, tempo_ms);

    // Estimativa de energia: E = V × I × t
    float tensao = 3.3f;    // em volts
    float corrente = 0.08f; // em amperes
    float tempo_seg = tempo_ms / 1000.0f;
    float energia = tensao * corrente * tempo_seg;
    printf("Energia estimada por inferencia: %.6f joules\n", energia);

    return 0;
}
