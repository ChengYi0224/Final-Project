# 編譯器
CC = gcc

# 編譯選項
CFLAGS = -g -I./src/include/

# C源文件
SRC_DIR = ./src/code
SRC_CFILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_DIR = ./obj
OBJS = $(SRC_CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 輸出可執行文件
EXEC = engine
EXEC_TEST = es

# SDL庫
SDL_LIB = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# 默認目標
all: $(EXEC)

# 連結物件文件創建可執行文件
$(EXEC): main.o $(OBJS)
	$(CC) -o $@ main.o $(OBJS) $(SDL_LIB) $(CFLAGS)

# 編譯源文件成為物件文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 單獨編譯main.c
main.o: main.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 調試構建
debug: CFLAGS += -g
debug: $(EXEC)

# 安裝SDL庫
install-SDL:
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

# 測試可執行文件
test: test.o $(OBJS)
	$(CC) -o $(EXEC_TEST) test.o $(OBJS) $(SDL_LIB) $(CFLAGS)

test.o: test.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 清理構建目錄
clean:
	rm -f $(EXEC) $(EXEC_TEST) main.o test.o globalVariables.o $(OBJ_DIR)/*.o

# 如果物件文件目錄不存在，則創建該目錄
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 確保在構建物件文件之前存在物件文件目錄
$(OBJS): | $(OBJ_DIR)
