CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -I.
EXT = .exe
OBJ_DIR = obj
BIN_DIR = bin

SRCS = main.c ft_knn.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/knn$(EXT)

all: $(OBJ_DIR) $(BIN_DIR) $(TARGET)

$(OBJ_DIR):
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(BIN_DIR):
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -lm -o $@ $^

$(BIN_DIR)/time$(EXT): infer_time.c $(OBJ_DIR)/ft_knn.o | $(OBJ_DIR) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ infer_time.c $(OBJ_DIR)/ft_knn.o -lm

$(BIN_DIR)/main_esp$(EXT): main_esp.c $(OBJ_DIR)/ft_knn.o | $(OBJ_DIR) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ main_esp.c $(OBJ_DIR)/ft_knn.o -lm

# Aliases
time: $(BIN_DIR)/time$(EXT)
esp: $(BIN_DIR)/main_esp$(EXT)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe *.exe *.elf $(OBJ_DIR)/nul nul

re: clean all

help:
	@echo "Targets disponiveis:"
	@echo "  all      - Compila o programa principal (bin/knn.exe)"
	@echo "  time     - Compila teste de tempo de inferencia (bin/time.exe)"
	@echo "  esp      - Compila versão embarcada (bin/main_esp.exe)"
	@echo "  clean    - Remove arquivos compilados"
	@echo "  re       - Limpa e recompila tudo"
	@echo "  help     - Mostra esta ajuda"
	@echo ""
	@echo "Executaveis ficam no diretorio bin/"

.PHONY: all clean re time esp help
