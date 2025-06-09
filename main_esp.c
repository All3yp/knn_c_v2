#include "ft_knn.h"


/// @file main_embedded.c
/// @brief Versão embarcada do KNN. Classifica uma única amostra de entrada.
/// Versão pro uso no embarcado
int main() {
    Dataset dataset;
    if (!load_dataset_from_csv(DATASET_PATH, &dataset)) {
        return 1;
    }

    // Divisão treino/teste (fixo)
    Dataset train, test;
    shuffle_dataset(&dataset);
    split_dataset(&dataset, &train, &test, 0.8f);

    // Normalização
    float mean[MAX_FEATURES], std[MAX_FEATURES];
    compute_mean_std(&train, mean, std);
    apply_normalization(&train, mean, std);
    apply_normalization(&test, mean, std);

    // Simula a classificação da primeira amostra do teste
    int k = 15;
    int predicted_class = classify(&test.samples[0], &train, train.num_samples, k);

    // A classe está armazenada em predicted_class (nenhum printf)
    (void)predicted_class;

    return 0;
}
