// Para testar seu programa, crie um outro arquivo, por exemplo, testeconv.c, contendo uma função main. 

#include <stdio.h>
#include "converteutf832.h"

int main() {
    // Abrindo o arquivo UTF-8 para leitura e o arquivo UTF-32 para escrita
    FILE *arquivo_entrada_utf8 = fopen("arquivo_pequeno_utf8.txt", "rb");
    FILE *arquivo_saida_utf32 = fopen("arquivo_pequeno_utf32.bin", "wb");

    if (arquivo_entrada_utf8 == NULL || arquivo_saida_utf32 == NULL) {
        fprintf(stderr, "Erro ao abrir arquivos para conversão de UTF-8 para UTF-32.\n");
        if (arquivo_entrada_utf8) fclose(arquivo_entrada_utf8);
        if (arquivo_saida_utf32) fclose(arquivo_saida_utf32);
        return -1;
    }

    // Converte UTF-8 para UTF-32
    if (convUtf8p32(arquivo_entrada_utf8, arquivo_saida_utf32) != 0) {
        fprintf(stderr, "Erro na conversão de UTF-8 para UTF-32.\n");
        fclose(arquivo_entrada_utf8);
        fclose(arquivo_saida_utf32);
        return -1;
    }

    fclose(arquivo_entrada_utf8);
    fclose(arquivo_saida_utf32);

    // Abrindo o arquivo UTF-32 para leitura e o arquivo UTF-8 para escrita
    FILE *arquivo_entrada_utf32 = fopen("arquivo_pequeno_utf32.bin", "rb");
    FILE *arquivo_saida_utf8 = fopen("arquivo_pequeno_utf8_convertido.txt", "wb");

    if (arquivo_entrada_utf32 == NULL || arquivo_saida_utf8 == NULL) {
        fprintf(stderr, "Erro ao abrir arquivos para conversão de UTF-32 para UTF-8.\n");
        if (arquivo_entrada_utf32) fclose(arquivo_entrada_utf32);
        if (arquivo_saida_utf8) fclose(arquivo_saida_utf8);
        return -1;
    }

    // Converte UTF-32 para UTF-8
    if (convUtf32p8(arquivo_entrada_utf32, arquivo_saida_utf8) != 0) {
        fprintf(stderr, "Erro na conversão de UTF-32 para UTF-8.\n");
        fclose(arquivo_entrada_utf32);
        fclose(arquivo_saida_utf8);
        return -1;
    }

    fclose(arquivo_entrada_utf32);
    fclose(arquivo_saida_utf8);

    printf("Conversões realizadas com sucesso.\n");
    return 0;
}
