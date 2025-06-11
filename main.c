#include "ft_knn.h"

int main() {
    Dataset dataset;
    if (!load_dataset_from_csv(DATASET_PATH, &dataset)) {
        fprintf(stderr, "Erro ao carregar dataset\n");
        return 1;
    }

    printf("Total de amostras: %d\n", dataset.num_samples);
    printf("Classe 0: %d amostras (%.1f%%)\n", count_class(&dataset, 0), (count_class(&dataset, 0) * 100.0) / dataset.num_samples);
    printf("Classe 1: %d amostras (%.1f%%)\n", count_class(&dataset, 1), (count_class(&dataset, 1) * 100.0) / dataset.num_samples);
    printf("Classe 2: %d amostras (%.1f%%)\n", count_class(&dataset, 2), (count_class(&dataset, 2) * 100.0) / dataset.num_samples);
    printf("Classe 3: %d amostras (%.1f%%)\n", count_class(&dataset, 3), (count_class(&dataset, 3) * 100.0) / dataset.num_samples);
    printf("Classe 4: %d amostras (%.1f%%)\n", count_class(&dataset, 4), (count_class(&dataset, 4) * 100.0) / dataset.num_samples);

    Dataset train, test;
    shuffle_dataset(&dataset);
    split_dataset(&dataset, &train, &test, TRAIN_PERCENTAGE);

    float mean[MAX_FEATURES], std[MAX_FEATURES];
    compute_mean_std(&train, mean, std);
    apply_normalization(&train, mean, std);
    apply_normalization(&test, mean, std); 
    printf("\nDados divididos: %d treino, %d teste\n", train.num_samples, test.num_samples);

    printf("\n=== Classificando amostras de teste individualmente ===\n");
    int acertos = 0;
    for (int i = 0; i < test.num_samples; i++) {
        int predita = classify(&test.samples[i], &train, train.num_samples, 15);
        int real = test.samples[i].class;

        printf("Amostra %d: Classe real = %d | Classe predita = %d %s\n",
            i, real, predita, (real == predita ? "(OK)" : "(ERRO)"));

        if (real == predita) acertos++;
    }

    float acc = (float)acertos / test.num_samples * 100.0f;
    printf("Acuracia final com K=15: %.2f%%\n", acc);

    // int k_values[] = {1, 3, 5, 7, 9, 11, 13, 15};
    // int num_k_values = sizeof(k_values) / sizeof(int);
    // // MARK: - Só pra debug, mostra os valores do k
    // debug_test_multiple_k(&train, &test, k_values, num_k_values);

    return 0;
}
