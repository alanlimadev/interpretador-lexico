#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

// Declarações de variáveis globais
int charClass; // Variável para armazenar a classe do caractere atual (LETTER, DIGIT, UNKNOWN).
char lexeme[100]; // Um buffer para armazenar o lexema.
char nextChar; // Variável para o caractere atual.
int lexLen; // O comprimento atual do lexema.
int token; // O tipo de token atual.
int nextToken; // O próximo tipo de token.
FILE *in_fp, *fopen(); // Ponteiro para o arquivo de entrada.

// Declarações de funções
void addChar(); // Função para adicionar um caractere ao lexema.
void getChar(); // Função para obter o próximo caractere do arquivo.
void getNonBlank(); // Função para ignorar espaços em branco.
int lex(); // Função principal do analisador léxico.

int comments(); // Função para identificar comentários.

// Classes de caracteres
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Códigos de tokens
#define INT_LIT 10
#define IDENT 11
#define FALSE 20 // "False"
#define TRUE 21 // "True"
#define AND_OP 22 // *
#define OR_OP 23 // +
#define IMP_OP 24 // ->
#define BIIMP_OP 25 // <->
#define NOT_OP 26 // !
#define COMMENT 27 // //
#define LEFT_PAREN 28 // (
#define RIGHT_PAREN 29 // )

//função principal
int main() { 
    setlocale(LC_ALL, "Portuguese"); // Define a localização para Português, se disponível.

    // Abrir o arquivo de dados de entrada e processar seu conteúdo 
    if ((in_fp = fopen("entrada.in", "r")) == NULL) { // Tenta abrir o arquivo "entrada.in" para leitura.
        printf("Erro ao abrir entrada.in \n"); // Se a abertura do arquivo falhar, exibe uma mensagem de erro.
        exit(1); // Encerra o programa com código de erro 1.
    } else { 
        getChar(); // Chama a função getChar para inicializar a análise léxica.
        do { 
            lex(); // Chama a função lex repetidamente até que o próximo token seja o final do arquivo (EOF).
        } while (nextToken != EOF); // Continua a análise léxica até encontrar o final do arquivo.
    } 
}


//uma função para processar operadores e parênteses e retornar o token
int lookup(char ch){
    switch (ch){
        case '*':
            addChar(); // Adiciona o caractere '*' ao lexema.
            nextToken = AND_OP; // Define o token como AND_OP (operador de multiplicação).
            break;
        case '+':
            addChar(); // Adiciona o caractere '+' ao lexema.
            nextToken = OR_OP; // Define o token como OR_OP (operador de adição).
            break;
        case '!':
            addChar(); // Adiciona o caractere '!' ao lexema.
            nextToken = NOT_OP; // Define o token como NOT_OP (operador de negação).
            break; 
        case '(':
            addChar(); // Adiciona o caractere '(' ao lexema.
            nextToken = LEFT_PAREN; // Define o token como LEFT_PAREN (parêntese esquerdo).
            break;
        case ')':
            addChar(); // Adiciona o caractere ')' ao lexema.
            nextToken = RIGHT_PAREN; // Define o token como RIGHT_PAREN (parêntese direito).
            break;

		case '-':
		    addChar(); // Adiciona o caractere '-' ao lexema
		    getChar(); // Obtém o próximo caractere
		    if (nextChar != '>') { // Verifica se o próximo caractere não é '>'
		        printf("ERRO: A sintaxe correta é: ->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		        exit(1); // Encerra o programa com um código de erro
		    } else {
		        addChar(); // Adiciona o caractere '>' ao lexema
		        getChar(); // Obtém o próximo caractere
		
		        char nextCharAux = getc(in_fp); // Lê o próximo caractere do arquivo
		        if (nextCharAux == '>' || nextCharAux == '-') { // Verifica se o próximo caractere é '>' ou '-'
		            ungetc(nextCharAux, in_fp); // Coloca o caractere de volta no arquivo
		            printf("ERRO: A sintaxe correta é: ->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		            exit(1); // Encerra o programa com um código de erro
		        }
		
		        nextToken = IMP_OP; // Define o token como IMP_OP (operador de implicação)
		        break; 
		    }
		    nextToken = UNKNOWN; // Se não corresponder ao operador de implicação, define o token como UNKNOWN (desconhecido)
		    break; 
		
		case '<':
		    addChar(); // Adiciona o caractere '<' ao lexema
		    getChar(); // Obtém o próximo caractere
		    if (nextChar != '-') { // Verifica se o próximo caractere não é '-'
		        printf("ERRO: A sintaxe correta é: <->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		        exit(1); // Encerra o programa com um código de erro
		    } else {
		        char nextCharAux = getc(in_fp); // Lê o próximo caractere do arquivo
		        if (nextCharAux != '>') { // Verifica se o próximo caractere não é '>'
		            printf("ERRO: A sintaxe correta é: <->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		            exit(1); // Encerra o programa com um código de erro
		        } else {
		            addChar(); // Adiciona o caractere '-' ao lexema
		            ungetc(nextCharAux, in_fp); // Coloca o caractere de volta no arquivo
		            getChar(); // Obtém o próximo caractere
		            addChar(); // Adiciona o caractere '>' ao lexema
		            getChar(); // Obtém o próximo caractere
		            nextToken = BIIMP_OP; // Define o token como BIIMP_OP (operador de bi-implicação)
		            break; 
		        }
		        ungetc(nextCharAux, in_fp); // Coloca o caractere de volta no arquivo
		    }
		    nextToken = UNKNOWN; // Se não corresponder ao operador de bi-implicação, define o token como UNKNOWN (desconhecido)
		    break;
                                   
        default:
            addChar(); // Adiciona o caractere desconhecido ao lexema.
            nextToken = EOF; // Define o token como EOF (final do arquivo).
            break;
    }
    return nextToken; // Retorna o token identificado.
}

void addAndGetNextChar(){
    addChar();
    getChar();
}

//uma função para adicionar nextChar ao vetor lexeme
void addChar() {
    if (lexLen <= 99) { // Verifica se o tamanho atual do lexema não ultrapassou o limite de 100 caracteres.
        lexeme[lexLen++] = nextChar; // Adiciona o caractere atual (nextChar) ao lexema e atualiza o índice (lexLen).
        lexeme[lexLen] = '\0'; // Garante que o lexema seja uma string válida, adicionando o caractere nulo (terminador de string).
    } else {
        printf("ERRO - ultrapassou o limite de caracteres (máx. 100)\n"); // Exibe uma mensagem de erro caso o limite seja ultrapassado.
    }
}

void getChar() {
    nextChar = getc(in_fp); // Lê o próximo caractere do arquivo de entrada e o armazena em 'nextChar'
    
    comments(); // Chama a função 'comments()' para identificar e ignorar comentários no código, se houver

    if (nextChar != EOF) { // Verifica se 'nextChar' não é o fim do arquivo (EOF)
        if (isalpha(nextChar)) { // Verifica se 'nextChar' é uma letra
            charClass = LETTER; // Define 'charClass' como LETTER para indicar que o caractere é uma letra
        } else if (isdigit(nextChar)) { // Verifica se 'nextChar' é um dígito
            charClass = DIGIT; // Define 'charClass' como DIGIT para indicar que o caractere é um dígito
        } else {
            charClass = UNKNOWN; // Caso contrário, define 'charClass' como UNKNOWN para indicar que o caractere é desconhecido
        }
    } else {
        charClass = EOF; // Se 'nextChar' for EOF, define 'charClass' como EOF para indicar o fim do arquivo
    }
}

//uma função para chamar getChar até que ela retorne um caractere diferente de espaço em branco
void getNonBlank() {
    // Enquanto o caractere atual (nextChar) for um espaço em branco, continue o loop.
    while (isspace(nextChar))
        getChar(); // Chama a função getChar() para ler o próximo caractere.
}

int lex() {
	
    lexLen = 0; // Inicializa o comprimento do lexema como 0
    getNonBlank(); // Remove espaços em branco e avança para o próximo caractere não em branco
	
    switch (charClass) {
        // Reconhecer identificadores 
        case LETTER:
            addChar(); // Adiciona o caractere atual ao lexema
            getChar(); // Obtém o próximo caractere
            while (charClass == LETTER || charClass == DIGIT) {
                addChar(); // Continua a construir o lexema enquanto forem letras ou dígitos
                getChar();
            }
            nextToken = IDENT; // Define o tipo do token como IDENT (identificador)
            break;

        // Reconhecer literais inteiros 
        case DIGIT:
            addChar(); // Adiciona o caractere atual ao lexema
            getChar(); // Obtém o próximo caractere
            while (charClass == DIGIT) {
                addChar(); // Continua a construir o lexema enquanto forem dígitos
                getChar();
            }
            nextToken = INT_LIT; // Define o tipo do token como INT_LIT (inteiro literal)
            break;

		// Parênteses e operadores
        case UNKNOWN:
            lookup(nextChar); // Chama a função lookup para identificar o operador ou caractere desconhecido
            getChar(); // Obtém o próximo caractere
            break;
	
        // Fim do arquivo
        case EOF:
            nextToken = EOF; // Define o tipo do token como EOF (fim de arquivo)
            lexeme[0] = 'E'; // Cria um lexema com "EOF" para representar o fim de arquivo
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0; // Termina o lexema com nulo
            break;
    } // Fim do switch 

    // Verifica se o lexema é "True" ou "False" e atualiza o tipo do token
    if (strcmp(lexeme, "True") == 0)
        nextToken = TRUE;
    else if (strcmp(lexeme, "False") == 0)
        nextToken = FALSE;
        
    // Imprime o tipo do próximo token e o lexema correspondente
    printf("Próximo token é: %d, próximo lexema é %s\n", nextToken, lexeme);

    return nextToken;
}


int comments() {
	
	// Verifica se o caractere atual é uma barra '/'
    if (nextChar == '/') {
    	// Lê o próximo caractere do arquivo (in_fp)
    	if((nextChar = getc(in_fp)) == EOF){
    		// Se o próximo caractere for o final do arquivo (EOF), define charClass como EOF e retorna 0
    		charClass = EOF;
    		return 0;
		}
        
        // Verifica se o próximo caractere é uma segunda barra '/'
        if (nextChar == '/') {
        	// Se for uma segunda barra, começa a ler e ignorar os caracteres até encontrar uma quebra de linha '\n' ou o final do arquivo (EOF)
            do {
                nextChar = getc(in_fp);
            } while (nextChar != '\n' && nextChar != EOF);
            return 1; // Retorna 1 se um comentário foi encontrado
        } else {
        	// Se o próximo caractere não for uma segunda barra, exibe uma mensagem de erro e encerra o programa com código de erro 1
            printf("ERRO: A sintaxe correta para comentários é: //\n"); //caso haja ocorrência de apenas uma '/', haverá um erro.
            exit(1);
        }
    }
    // Se o caractere atual não for uma barra '/', retorna 0 (não é um comentário)
    return 0;
}

