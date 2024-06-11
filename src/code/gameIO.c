#include "../include/toml.h"
#include "../include/control.h"

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave)
{

}

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave)
{
    // 打開劇本檔案
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "w")) == NULL)
    {
        perror("Error opening save");
        return EXIT_FAILURE;
    }

    // 開始進行存檔，使用toml格式
    fprintf(fpSave,"sizePlayerInventory = %ld\n", GameSave->sizePlayerInventory.u.i);
    fprintf(fpSave, "[playerInventory]\n");
    for (int i = 0; i < GameSave->sizePlayerInventory.u.i; i++)
    {
        fprintf(fpSave, "item%d = %s\n", i, GameSave->playerInventory);
    }
    fclose(fpSave);
    return EXIT_SUCCESS;
}