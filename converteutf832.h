#include <stdio.h>

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida);
int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida);
size_t qtd_bytes_utf8(const unsigned char byte);