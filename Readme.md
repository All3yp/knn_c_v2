# 🧠 KNN Classifier in C - SEMB Project

A high-performance K-Nearest Neighbors classifier implementation in C, optimized for embedded systems like ESP32. This project classifies ultrasonic signals from electrical insulators based on their condition (normal vs. defective).

## 📁 Project Structure

```
knn_c_v2/
├── 📁 bin/                     # Compiled executables
│   ├── knn.exe                 # Main classifier with accuracy testing
│   ├── time.exe                # Inference time measurement
│   └── main_esp.exe            # Embedded version (no printf)
├── 📁 obj/                     # Object files
│   ├── ft_knn.o
│   └── main.o
├── 📁 data/                    # Datasets
│   ├── dataset_features.csv    # Main feature dataset (1465 samples)
│   ├── dataset_improved.csv    # Enhanced dataset
│   └── dataset_lda.csv         # LDA-processed dataset
├── 📄 main.c                   # Main program with debugging
├── 📄 infer_time.c             # Performance measurement
├── 📄 main_esp.c               # Embedded system version
├── 📄 ft_knn.h                 # Header with function declarations
├── 📄 ft_knn.c                 # Core KNN implementation
├── 📄 Makefile                 # Multi-platform build system
└── 📄 README.md                # This file
```

## 🚀 Quick Start

### Building the Project

The project uses a smart Makefile that auto-detects your platform (Windows/Linux) and provides convenient aliases:

```bash
# Build main classifier
make

# Build inference time measurement
make time

# Build embedded version
make esp

# Clean all build artifacts
make clean

# Rebuild everything
make re

# Show all available targets
make help
```

### Running the Programs

```bash
# Run main classifier (shows dataset info and accuracy)
./bin/knn.exe

# Measure inference time and energy consumption
./bin/time.exe

# Run embedded version (silent, for microcontrollers)
./bin/main_esp.exe
```

## 📊 Performance Metrics

### Dataset Information
- **Total samples:** 1,465
- **Classes:** 5 (Sem Corona, Oxidado, Fezes, Salgado, Flashover)
- **Features per sample:** 12 (mean, std, and 10 largest peaks)
- **Train/Test split:** 80%/20% (1,172 train, 293 test)

### Class Distribution
- **Class 0 (Sem Corona):** 416 samples (28.4%)
- **Class 1 (Oxidado):** 290 samples (19.8%)
- **Class 2 (Fezes):** 282 samples (19.3%)
- **Class 3 (Salgado):** 238 samples (16.2%)
- **Class 4 (Flashover):** 239 samples (16.3%)

### Inference Performance
- **Average inference time:** ~0.287 ms (1000 executions)
- **Estimated energy consumption:** ~0.000076 joules per inference
- **Optimal K value:** 15 neighbors
- **Platform:** C implementation with Z-score normalization

### Energy Estimation Parameters
- **Voltage:** 3.3V (typical for ESP32)
- **Current:** 0.08A (estimated during processing)
- **Formula:** E = V × I × t

## 🔧 Technical Features

### Core Algorithms
- **K-Nearest Neighbors:** Euclidean distance with qsort optimization
- **Z-score Normalization:** Feature standardization for better accuracy
- **Dataset Shuffling:** Fisher-Yates algorithm for random data splitting
- **CSV Parsing:** Robust file loading with error handling

### Build System Features
- **Multi-platform support:** Windows, Linux, ESP32
- **Organized output:** Executables in `bin/`, objects in `obj/`
- **Convenient aliases:** `time` and `esp` for quick building
- **Dependency management:** Automatic directory creation
- **Clean targets:** Complete artifact removal

### Code Organization
- **Modular design:** Separate files for different purposes
- **Embedded-ready:** Version without stdio dependencies
- **Memory efficient:** Static arrays, no dynamic allocation
- **Cross-platform:** Standard C99 with minimal dependencies

## 🎯 Usage Examples

### Classification API
```c
// Load and prepare dataset
Dataset dataset;
load_dataset_from_csv(DATASET_PATH, &dataset);

// Split and normalize
Dataset train, test;
shuffle_dataset(&dataset);
split_dataset(&dataset, &train, &test, 0.8f);

float mean[MAX_FEATURES], std[MAX_FEATURES];
compute_mean_std(&train, mean, std);
apply_normalization(&train, mean, std);
apply_normalization(&test, mean, std);

// Classify a sample
int k = 15;
int predicted_class = classify(&test.samples[0], &train, train.num_samples, k);
```

### Build Targets Explanation

| Target | Purpose | Output | Use Case |
|--------|---------|---------|----------|
| `make` | Main classifier | `bin/knn.exe` | Development and testing |
| `make time` | Performance test | `bin/time.exe` | Benchmarking |
| `make esp` | Embedded version | `bin/main_esp.exe` | Microcontroller deployment |
| `make help` | Show help | Terminal output | Quick reference |

## 🛠️ Development

### Requirements
- **Compiler:** GCC with C99 support
- **Platform:** Windows, Linux, or ESP32 toolchain
- **Dependencies:** Standard C library, math library (-lm)

### File Descriptions

| File | Purpose |
|------|---------|
| `ft_knn.c/h` | Core KNN algorithm implementation |
| `main.c` | Development version with debugging output |
| `infer_time.c` | Performance measurement and energy estimation |
| `main_esp.c` | Production version for embedded systems |
| `Makefile` | Cross-platform build automation |

### Compilation Flags
- `-Wall -Wextra -Werror`: Strict error checking
- `-std=c99`: C99 standard compliance
- `-lm`: Math library linking

## 📈 Optimization Features

- **Fast sorting:** Uses standard library `qsort()` for neighbor ranking
- **Memory layout:** Contiguous arrays for cache efficiency
- **Minimal dependencies:** Only standard C library required
- **Platform detection:** Automatic Windows/Linux build configuration

## ❓ Troubleshooting

### Common Issues

**Problem:** Build fails with "command not found"
**Solution:** Ensure GCC is installed and in your PATH

**Problem:** Dataset not found
**Solution:** Verify `data/dataset_features.csv` exists in the project root

**Problem:** Different platforms show different performance
**Solution:** Performance varies by hardware; times shown are for reference

### Getting Help

```bash
# Show all available build targets and options
make help
```

## 📚 Academic Context

This project implements the KNN classifier described in the research paper about ultrasonic signal classification for electrical insulator condition monitoring. The C implementation provides:

- **Portability:** Runs on desktop and embedded systems
- **Performance:** Optimized for real-time classification
- **Accuracy:** Maintains classification quality from Python prototype
- **Efficiency:** Low memory footprint and fast inference

---

**Project:** SEMB
**Course:** PPGCC IFCE
**Name:** Alley P.