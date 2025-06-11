#include "ft_knn.h"
#include <time.h>

#define NUM_EXECUCOES 1000
#define IDX_AMOSTRA 0
#define K 15
#define TENS_V 3.3f
#define CORR_MA 80.0f

/// @brief Estima energia consumida em Joules: E = V x I x t
static float estimar_energia(float tempo_ms, float corrente_mA, float tensao_V) {
    float tempo_s = tempo_ms / 1000.0f;
    float corrente_A = corrente_mA / 1000.0f;
    return tensao_V * corrente_A * tempo_s;
}

/// @brief Mede tempo médio de inferência (milissegundos)
static double medir_tempo_medio(const Sample *amostra, const Dataset *treino, int k, int repeticoes) {
    clock_t ini = clock();
    for (int i = 0; i < repeticoes; i++) {
        classify(amostra, treino, treino->num_samples, k);
    }
    clock_t fim = clock();
    double total_s = (double)(fim - ini) / CLOCKS_PER_SEC;
    return (total_s / repeticoes) * 1000.0;
}

int main() {
    Dataset dataset;
    if (!load_dataset_from_csv(DATASET_PATH, &dataset)) {
        fprintf(stderr, "Erro ao carregar dataset.\n");
        return 1;
    }

    shuffle_dataset(&dataset);

    Dataset treino, teste;
    split_dataset(&dataset, &treino, &teste, TRAIN_PERCENTAGE);

    float media[MAX_FEATURES], desvio[MAX_FEATURES];
    compute_mean_std(&treino, media, desvio);
    apply_normalization(&treino, media, desvio);
    apply_normalization(&teste, media, desvio);

    printf("=== INFORMACOES DO DATASET ===\n");
    printf("Total: %d amostras | Treino: %d | Teste: %d\n", dataset.num_samples, treino.num_samples, teste.num_samples);
    printf("Classe real da amostra %d: %d\n\n", IDX_AMOSTRA, teste.samples[IDX_AMOSTRA].class);

    printf("=== MEDICAO DE PERFORMANCE ===\n");
    double tempo_ms = medir_tempo_medio(&teste.samples[IDX_AMOSTRA], &treino, K, NUM_EXECUCOES);
    float energia = estimar_energia(tempo_ms, CORR_MA, TENS_V);
    int predita = classify(&teste.samples[IDX_AMOSTRA], &treino, treino.num_samples, K);

    printf("K = %d | Execucoes = %d\n", K, NUM_EXECUCOES);
    printf("Tempo medio: %.4f ms\n", tempo_ms);
    printf("Energia estimada: %.6f J (%.2f uJ)\n", energia, energia * 1e6);
    printf("Classe predita: %d | Acerto: %s\n", predita, (predita == teste.samples[IDX_AMOSTRA].class) ? "Sim" : "Nao");

    return 0;
}
