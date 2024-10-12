/* LIVIAN ESSVEIN 2211667 3WA */
/* LUIZA REGNIER 2211931 3WB */

#include <stdio.h>
#include "converteutf832.h"

#define BOM 0x0000FEFF  // BOM para UTF-32 little-endian


int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida) {
  unsigned int bom = BOM;
      if (arquivo_saida == NULL) {
          fprintf(stderr, "Erro ao criar o arquivo de saída.\n");
          return -1;
      }

      if (fwrite(&bom, sizeof(unsigned int), 1, arquivo_saida) != 1) {
          fprintf(stderr, "Erro ao escrever BOM no arquivo de saída.\n");
          return -1;
      }

      int byte;
      while ((byte = fgetc(arquivo_entrada)) != EOF) {
          unsigned char utf8_byte = (unsigned char) byte;
          char cont=0;
          int comparaMask= 0x80;
          if((comparaMask & utf8_byte)== 0x00)
          {
            cont=1;
          }

          while((comparaMask & utf8_byte) != 0x00)
          {
            cont++;
            comparaMask>>=1;
          }
          if (cont>4)
          {
            fprintf(stderr, "Erro de leitura: byte UTF8 inicial invalido.\n");
            return -1;
          }
          unsigned int utf32_char = 0;
          if(cont==4){
            int next_byte1 = fgetc(arquivo_entrada);
            if((next_byte1 & 0x80)!= 0x80)
            {
                fprintf(stderr, "Erro de leitura: byte UTF8 de continuação invalido.\n");
                return -1;
            }
            int next_byte2 = fgetc(arquivo_entrada);
            if((next_byte2 & 0x80)!= 0x80)
            {
                fprintf(stderr, "Erro de leitura: byte UTF8 de continuação invalido.\n");
                return -1;
            }
            int next_byte3 = fgetc(arquivo_entrada);
            if((next_byte3 & 0x80)!= 0x80)
            {
                fprintf(stderr, "Erro de leitura: byte UTF8 de continuação invalido.\n");
                return -1;
            }
            if (next_byte1 == EOF || next_byte2 == EOF || next_byte3 == EOF) {
              fprintf(stderr, "Erro de leitura: byte UTF8 esperado, mas EOF encontrado.\n");
              return -1;
            }
            utf32_char = ((utf8_byte & 0x07) << 18) | ((next_byte1 & 0x3F) << 12) | ((next_byte2 & 0x3F) << 6) | (next_byte3 & 0x3F);
          }
          
          else if(cont==3){
            int next_byte1 = fgetc(arquivo_entrada);
            int next_byte2 = fgetc(arquivo_entrada);
            if (next_byte1 == EOF || next_byte2 == EOF) {
                fprintf(stderr, "Erro de leitura: byte UTF8 esperado, mas EOF encontrado.\n");
                return -1;
            }
            utf32_char = ((utf8_byte & 0x0F) << 12) | ((next_byte1 & 0x3F) << 6) | (next_byte2 & 0x3F);
          }
          
          else if (cont==2){
            int next_byte = fgetc(arquivo_entrada);
            if (next_byte == EOF) {
                fprintf(stderr, "Erro de leitura: byte UTF8 esperado, mas EOF encontrado.\n");
                return -1;
            }
              utf32_char = ((utf8_byte & 0x1F) << 6) | (next_byte & 0x3F);
          }
          
          else if (cont ==1){
            utf32_char = utf8_byte;
          }
          
          if (fwrite(&utf32_char, sizeof(unsigned int), 1, arquivo_saida) != 1) {
              fprintf(stderr, "Erro ao escrever caractere UTF-32 no arquivo de saída.\n");
              return -1;
          }
      }

      return 0;
}        


int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida) {
    if (arquivo_saida == NULL) {
        fprintf(stderr, "Erro ao criar o arquivo de saída.\n");
        return -1;
    }

    unsigned int bom;
    if (fread(&bom, sizeof(unsigned int), 1, arquivo_entrada) != 1) {
        fprintf(stderr, "Erro ao ler o BOM do arquivo de entrada.\n");
        return -1;
    }
    if (bom != BOM) {
        fprintf(stderr, "BOM inválido: 0x%X\n", bom);
        return -1;
    }

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
            fprintf(stderr, "Caractere UTF-32 fora do intervalo válido: 0x%X\n", utf32_char);
            return -1;
        }

        if (fwrite(utf8, num_bytes, 1, arquivo_saida) != 1) {
            fprintf(stderr, "Erro ao escrever no arquivo de saída.\n");
            return -1;
        }
    }

    return 0;
}