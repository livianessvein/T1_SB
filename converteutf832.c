// Você deve criar um arquivo fonte chamado converteutf832.c contendo as duas funções descritas acima, e funções auxiliares, se for o caso. Esse arquivo não deve conter uma função main!

// LIVIAN ESSVEIN 2211667 3WA

#include <stdio.h>
#include <stdint.h>
#include "converteutf832.h"

#define BOM_VALIDO_UTF32_LE 0xFFFE0000  // BOM para UTF-32 little-endian

// Função auxiliar para converter um caractere UTF-8 para UTF-32
size_t qtd_bytes_utf8(const uint8_t byte) {
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
// Como Funciona:

//     1 byte: Se o bit mais significativo (MSB) do byte é 0 (0x80 é 10000000 em binário), é um caractere ASCII básico, que ocupa 1 byte.
//     2 bytes: Se os dois primeiros bits são 110 (0xE0 é 11100000 em binário), o caractere é representado por 2 bytes.
//     3 bytes: Se os três primeiros bits são 1110 (0xF0 é 11110000 em binário), o caractere é representado por 3 bytes.
//     4 bytes: Se os quatro primeiros bits são 11110 (0xF8 é 11111000 em binário), o caractere é representado por 4 bytes.





int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
// Os dois parâmetros da função são dois arquivos abertos em modo binário: o arquivo de entrada (arquivo_entrada) e o arquivo de saída (arquivo_saida). A sua função NÃO deve abrir ou fechar os arquivos.
// O valor de retorno da função convUtf8p32 é 0, em caso de sucesso e -1, em caso de erro de E/S. Em caso de erro, a função deve emitir, na saída de erro (stderr), uma mensagem indicando qual o tipo de erro ocorrido (leitura ou gravação) e retornar imediatamente. 

    if (arquivo_saida == NULL) {
        fprintf(stderr,"Erro: arquivo inválido.\n");
        return -1;
    }
    // Não verifiquei se o arquivo de entrada é válido pq ele fala queos arquivos de entrada sempre conterão um texto CORRETAMENTE CODIFICADO

    // Escreve a marcação BOM para UTF-32 little-endian no início do arquivo de saída
    uint32_t bom_marker = 0xFFFE0000; // Little-endian BOM para UTF-32
    if (fwrite(&bom_marker, sizeof(uint32_t), 1, arquivo_saida) != 1) {
        fprintf(stderr,"Erro ao escrever BOM no arquivo de saída.\n");
        return -1;
    }

    // Processa cada byte do arquivo de entrada
    int byte;
    while ((byte = fgetc(arquivo_entrada)) != EOF) {
        // lê um byte do arquivo de entrada. A função fgetc retorna o próximo caractere como um int, ou EOF se o final do arquivo for atingido.
        uint8_t utf8_byte = (uint8_t)byte;
        // converte o byte lido para uint8_t, que é um tipo de dado sem sinal de 8 bits, representando o byte como um valor de 0 a 255.
        size_t qtd_bytes = qtd_bytes_utf8(utf8_byte);
        // calcular o número de bytes que o caractere UTF-8 ocupa

        uint32_t utf32_char = 0;
        if (qtd_bytes == 1) { // 1 byte
            utf32_char = utf8_byte;
            // Se o caractere UTF-8 é de 1 byte (ASCII), é diretamente atribuído a utf32_char
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
        fwrite(&utf32_char, sizeof(uint32_t), 1, arquivo_saida);
    }

    return 0;
}


int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){
// arquivo de entrada (um texto codificado em UTF-32 com ordenação indicada pelo BOM) e gravar em um arquivo de saída esse mesmo texto, codificado em UTF-8. 
// O arquivo de entrada UTF-32 terá um BOM inicial, mas o arquivo de saída UTF-8 não deverá conter um BOM inicial. 
// Os parâmetros da função são dois arquivos abertos em modo binário: o arquivo de entrada (arquivo_entrada) e o arquivo de saída (arquivo_saida). A sua função NÃO deve abrir ou fechar os arquivos.
//  A função deve inspecionar os primeiros quatro bytes do conteúdo do arquivo de entrada (UTF-32) para verificar se eles contêm um BOM válido!
// Assim como na função anterior, o valor de retorno é 0, em caso de sucesso e -1, em caso de erro. Os procedimentos para os casos de erro são:
//     erro de E/S: a função deve emitir, na saída de erro (stderr), uma mensagem indicando o tipo de erro ocorrido (leitura ou gravação)
//     BOM inválido: a função deve emitir, na saída de erro (stderr) uma mensagem de erro. 
// Nos dois casos, a função deve retornar imediatamente após emitir a mensagem de erro. 

    if (arquivo_saida == NULL) {
        printf("Erro: arquivo inválido.\n");
        return -1;
    }
    // Não verifiquei se o arquivo de entrada é válido pq ele fala queos arquivos de entrada sempre conterão um texto CORRETAMENTE CODIFICADO

    // Verificar e remover o BOM UTF-32 do arquivo de entrada
    uint32_t bom;
    if (fread(&bom, sizeof(uint32_t), 1, arquivo_entrada) != 1) {
        printf("Erro ao ler o BOM do arquivo de entrada.\n");
        return -1;
    }
    if (bom != BOM_VALIDO_UTF32_LE) {
        printf("BOM inválido.\n");
        return -1;
    }


    // Processar cada caractere UTF-32 e converter para UTF-8
    uint32_t utf32_char;
    while (fread(&utf32_char, sizeof(uint32_t), 1, arquivo_entrada) == 1) {
        uint8_t utf8[4];
        size_t num_bytes;

        if (utf32_char <= 0x7F) {
            utf8[0] = (uint8_t)utf32_char;
            num_bytes = 1;
        } else if (utf32_char <= 0x7FF) {
            utf8[0] = (uint8_t)((utf32_char >> 6) | 0xC0);
            utf8[1] = (uint8_t)((utf32_char & 0x3F) | 0x80);
            num_bytes = 2;
        } else if (utf32_char <= 0xFFFF) {
            utf8[0] = (uint8_t)((utf32_char >> 12) | 0xE0);
            utf8[1] = (uint8_t)(((utf32_char >> 6) & 0x3F) | 0x80);
            utf8[2] = (uint8_t)((utf32_char & 0x3F) | 0x80);
            num_bytes = 3;
        } else if (utf32_char <= 0x10FFFF) {
            utf8[0] = (uint8_t)((utf32_char >> 18) | 0xF0);
            utf8[1] = (uint8_t)(((utf32_char >> 12) & 0x3F) | 0x80);
            utf8[2] = (uint8_t)(((utf32_char >> 6) & 0x3F) | 0x80);
            utf8[3] = (uint8_t)((utf32_char & 0x3F) | 0x80);
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
