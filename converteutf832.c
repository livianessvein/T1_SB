// Você deve criar um arquivo fonte chamado converteutf832.c contendo as duas funções descritas acima, e funções auxiliares, se for o caso. Esse arquivo não deve conter uma função main!

#include <stdio.h>
#include "converteutf832.h"

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida);
// Os dois parâmetros da função são dois arquivos abertos em modo binário: o arquivo de entrada (arquivo_entrada) e o arquivo de saída (arquivo_saida). A sua função NÃO deve abrir ou fechar os arquivos.
// O valor de retorno da função convUtf8p32 é 0, em caso de sucesso e -1, em caso de erro de E/S. Em caso de erro, a função deve emitir, na saída de erro (stderr), uma mensagem indicando qual o tipo de erro ocorrido (leitura ou gravação) e retornar imediatamente. 

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida);
// arquivo de entrada (um texto codificado em UTF-32 com ordenação indicada pelo BOM) e gravar em um arquivo de saída esse mesmo texto, codificado em UTF-8. 
// O arquivo de entrada UTF-32 terá um BOM inicial, mas o arquivo de saída UTF-8 não deverá conter um BOM inicial. 
// Os parâmetros da função são dois arquivos abertos em modo binário: o arquivo de entrada (arquivo_entrada) e o arquivo de saída (arquivo_saida). A sua função NÃO deve abrir ou fechar os arquivos.
//  A função deve inspecionar os primeiros quatro bytes do conteúdo do arquivo de entrada (UTF-32) para verificar se eles contêm um BOM válido!
// Assim como na função anterior, o valor de retorno é 0, em caso de sucesso e -1, em caso de erro. Os procedimentos para os casos de erro são:
//     erro de E/S: a função deve emitir, na saída de erro (stderr), uma mensagem indicando o tipo de erro ocorrido (leitura ou gravação)
//     BOM inválido: a função deve emitir, na saída de erro (stderr) uma mensagem de erro. 
// Nos dois casos, a função deve retornar imediatamente após emitir a mensagem de erro. 

