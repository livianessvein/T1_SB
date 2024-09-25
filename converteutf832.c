// Você deve criar um arquivo fonte chamado converteutf832.c contendo as duas funções descritas acima, e funções auxiliares, se for o caso. Esse arquivo não deve conter uma função main!

// LIVIAN ESSVEIN 2211667 3WA
// LUIZA REGNIER 2211931 3WB

#include <stdio.h>
#include "converteutf832.h"

#define BOM_VALIDO_UTF32_LE 0xFFFE0000  // BOM para UTF-32 little-endian

// Função auxiliar para converter um caractere UTF-8 para UTF-32
size_t qtd_bytes_utf8(const unsigned char byte) {
    if ((byte & 0x80) == 0) {
        return 1;  // 1 byte
    } else if ((byte & 0xE0) == 0xC0) {
        return 2;  // 2 bytes
    } else if ((byte & 0xF0) == 0xE0) {
        return 3;  // 3 bytes
    } else if ((byte & 0xF8) == 0xF0) {
        return 4;  // 4 bytes
    }
    // byte inválido (não esperado, dado que o arquivo está corretamente codificado)
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida) {
    if (arquivo_saida == NULL) {
        fprintf(stderr, "Erro: arquivo inválido.\n");
        return -1;
    }

    // Escreve a marcação BOM para UTF-32 little-endian no início do arquivo de saída
    unsigned int bom_marker = 0xFFFE0000;  // Little-endian BOM para UTF-32
    if (fwrite(&bom_marker, sizeof(unsigned int), 1, arquivo_saida) != 1) {
        fprintf(stderr, "Erro ao escrever BOM no arquivo de saída.\n");
        return -1;
    }

    // Processa cada byte do arquivo de entrada
    int byte;
    while ((byte = fgetc(arquivo_entrada)) != EOF) {
        unsigned char utf8_byte = (unsigned char) byte;
        size_t qtd_bytes = qtd_bytes_utf8(utf8_byte);

        unsigned int utf32_char = 0;
        if (qtd_bytes == 1) { // 1 byte
            utf32_char = utf8_byte;
        } else if (qtd_bytes == 2) { // 2 bytes
            int next_byte = fgetc(arquivo_entrada);
            utf32_char = ((utf8_byte & 0x1F) << 6) | (next_byte & 0x3F);
        } else if (qtd_bytes == 3) { // 3 bytes
            int next_byte1 = fgetc(arquivo_entrada);
            int next_byte2 = fgetc(arquivo_entrada);
            utf32_char = ((utf8_byte & 0x0F) << 12) | ((next_byte1 & 0x3F) << 6) | (next_byte2 & 0x3F);
        } else if (qtd_bytes == 4) { // 4 bytes
            int next_byte1 = fgetc(arquivo_entrada);
            int next_byte2 = fgetc(arquivo_entrada);
            int next_byte3 = fgetc(arquivo_entrada);
            utf32_char = ((utf8_byte & 0x07) << 18) | ((next_byte1 & 0x3F) << 12) | ((next_byte2 & 0x3F) << 6) | (next_byte3 & 0x3F);
        }

        // Escreve o caractere UTF-32 no arquivo de saída
        fwrite(&utf32_char, sizeof(unsigned int), 1, arquivo_saida);
    }

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida) {
    if (arquivo_saida == NULL) {
        fprintf(stderr, "Erro: arquivo inválido.\n");
        return -1;
    }

    // Verificar e remover o BOM UTF-32 do arquivo de entrada
    unsigned int bom;
    if (fread(&bom, sizeof(unsigned int), 1, arquivo_entrada) != 1) {
        fprintf(stderr, "Erro ao ler o BOM do arquivo de entrada.\n");
        return -1;
    }
    if (bom != BOM_VALIDO_UTF32_LE) {
        fprintf(stderr, "BOM inválido.\n");
        return -1;
    }

    // Processar cada caractere UTF-32 e converter para UTF-8
    unsigned int utf32_char;
    while (fread(&utf32_char, sizeof(unsigned int), 1, arquivo_entrada) == 1) {
        unsigned char utf8[4];
        size_t num_bytes;

        if (utf32_char <= 0x7F) {
            utf8[0] = (unsigned char)utf32_char;
            num_bytes = 1;
        } else if (utf32_char <= 0x7FF) {
            utf8[0] = (unsigned char)((utf32_char >> 6) | 0xC0);
            utf8[1] = (unsigned char)((utf32_char & 0x3F) | 0x80);
            num_bytes = 2;
        } else if (utf32_char <= 0xFFFF) {
            utf8[0] = (unsigned char)((utf32_char >> 12) | 0xE0);
            utf8[1] = (unsigned char)(((utf32_char >> 6) & 0x3F) | 0x80);
            utf8[2] = (unsigned char)((utf32_char & 0x3F) | 0x80);
            num_bytes = 3;
        } else if (utf32_char <= 0x10FFFF) {
            utf8[0] = (unsigned char)((utf32_char >> 18) | 0xF0);
            utf8[1] = (unsigned char)(((utf32_char >> 12) & 0x3F) | 0x80);
            utf8[2] = (unsigned char)(((utf32_char >> 6) & 0x3F) | 0x80);
            utf8[3] = (unsigned char)((utf32_char & 0x3F) | 0x80);
            num_bytes = 4;
        } else {
            fprintf(stderr, "Caractere UTF-32 fora do intervalo válido.\n");
            return -1;
        }

        if (fwrite(utf8, num_bytes, 1, arquivo_saida) != 1) {
            fprintf(stderr, "Erro ao escrever no arquivo de saída.\n");
            return -1;
        }
    }

    return 0;
}
