#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

// Declara��es de vari�veis globais
int charClass; // Vari�vel para armazenar a classe do caractere atual (LETTER, DIGIT, UNKNOWN).
char lexeme[100]; // Um buffer para armazenar o lexema.
char nextChar; // Vari�vel para o caractere atual.
int lexLen; // O comprimento atual do lexema.
int token; // O tipo de token atual.
int nextToken; // O pr�ximo tipo de token.
FILE *in_fp, *fopen(); // Ponteiro para o arquivo de entrada.

// Declara��es de fun��es
void addChar(); // Fun��o para adicionar um caractere ao lexema.
void getChar(); // Fun��o para obter o pr�ximo caractere do arquivo.
void getNonBlank(); // Fun��o para ignorar espa�os em branco.
int lex(); // Fun��o principal do analisador l�xico.

int comments(); // Fun��o para identificar coment�rios.

// Classes de caracteres
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// C�digos de tokens
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

//fun��o principal
int main() { 
    setlocale(LC_ALL, "Portuguese"); // Define a localiza��o para Portugu�s, se dispon�vel.

    // Abrir o arquivo de dados de entrada e processar seu conte�do 
    if ((in_fp = fopen("entrada.in", "r")) == NULL) { // Tenta abrir o arquivo "entrada.in" para leitura.
        printf("Erro ao abrir entrada.in \n"); // Se a abertura do arquivo falhar, exibe uma mensagem de erro.
        exit(1); // Encerra o programa com c�digo de erro 1.
    } else { 
        getChar(); // Chama a fun��o getChar para inicializar a an�lise l�xica.
        do { 
            lex(); // Chama a fun��o lex repetidamente at� que o pr�ximo token seja o final do arquivo (EOF).
        } while (nextToken != EOF); // Continua a an�lise l�xica at� encontrar o final do arquivo.
    } 
}


//uma fun��o para processar operadores e par�nteses e retornar o token
int lookup(char ch){
    switch (ch){
        case '*':
            addChar(); // Adiciona o caractere '*' ao lexema.
            nextToken = AND_OP; // Define o token como AND_OP (operador de multiplica��o).
            break;
        case '+':
            addChar(); // Adiciona o caractere '+' ao lexema.
            nextToken = OR_OP; // Define o token como OR_OP (operador de adi��o).
            break;
        case '!':
            addChar(); // Adiciona o caractere '!' ao lexema.
            nextToken = NOT_OP; // Define o token como NOT_OP (operador de nega��o).
            break; 
        case '(':
            addChar(); // Adiciona o caractere '(' ao lexema.
            nextToken = LEFT_PAREN; // Define o token como LEFT_PAREN (par�ntese esquerdo).
            break;
        case ')':
            addChar(); // Adiciona o caractere ')' ao lexema.
            nextToken = RIGHT_PAREN; // Define o token como RIGHT_PAREN (par�ntese direito).
            break;                                    
        default:
            addChar(); // Adiciona o caractere desconhecido ao lexema.
            nextToken = EOF; // Define o token como EOF (final do arquivo).
            break;
    }
    return nextToken; // Retorna o token identificado.
}

//uma fun��o para adicionar nextChar ao vetor lexeme
void addChar() {
    if (lexLen <= 99) { // Verifica se o tamanho atual do lexema n�o ultrapassou o limite de 100 caracteres.
        lexeme[lexLen++] = nextChar; // Adiciona o caractere atual (nextChar) ao lexema e atualiza o �ndice (lexLen).
        lexeme[lexLen] = '\0'; // Garante que o lexema seja uma string v�lida, adicionando o caractere nulo (terminador de string).
    } else {
        printf("ERRO - ultrapassou o limite de caracteres (m�x. 100)\n"); // Exibe uma mensagem de erro caso o limite seja ultrapassado.
    }
}

void getChar() {
    nextChar = getc(in_fp); // L� o pr�ximo caractere do arquivo de entrada e o armazena em 'nextChar'
    
    comments(); // Chama a fun��o 'comments()' para identificar e ignorar coment�rios no c�digo, se houver

    if (nextChar != EOF) { // Verifica se 'nextChar' n�o � o fim do arquivo (EOF)
        if (isalpha(nextChar)) { // Verifica se 'nextChar' � uma letra
            charClass = LETTER; // Define 'charClass' como LETTER para indicar que o caractere � uma letra
        } else if (isdigit(nextChar)) { // Verifica se 'nextChar' � um d�gito
            charClass = DIGIT; // Define 'charClass' como DIGIT para indicar que o caractere � um d�gito
        } else {
            charClass = UNKNOWN; // Caso contr�rio, define 'charClass' como UNKNOWN para indicar que o caractere � desconhecido
        }
    } else {
        charClass = EOF; // Se 'nextChar' for EOF, define 'charClass' como EOF para indicar o fim do arquivo
    }
}


//uma fun��o para chamar getChar at� que ela retorne um caractere diferente de espa�o em branco
void getNonBlank() {
    // Enquanto o caractere atual (nextChar) for um espa�o em branco, continue o loop.
    while (isspace(nextChar))
        getChar(); // Chama a fun��o getChar() para ler o pr�ximo caractere.
}



int lex() {
    lexLen = 0; // Inicializa o comprimento do lexema como 0
    getNonBlank(); // Remove espa�os em branco e avan�a para o pr�ximo caractere n�o em branco

    switch (charClass) {
        // Reconhecer identificadores 
        case LETTER:
            addChar(); // Adiciona o caractere atual ao lexema
            getChar(); // Obt�m o pr�ximo caractere
            while (charClass == LETTER || charClass == DIGIT) {
                addChar(); // Continua a construir o lexema enquanto forem letras ou d�gitos
                getChar();
            }
            nextToken = IDENT; // Define o tipo do token como IDENT (identificador)
            break;

        // Reconhecer literais inteiros 
        case DIGIT:
            addChar(); // Adiciona o caractere atual ao lexema
            getChar(); // Obt�m o pr�ximo caractere
            while (charClass == DIGIT) {
                addChar(); // Continua a construir o lexema enquanto forem d�gitos
                getChar();
            }
            nextToken = INT_LIT; // Define o tipo do token como INT_LIT (inteiro literal)
            break;

        // Par�nteses e operadores
        case UNKNOWN:
            lookup(nextChar); // Chama a fun��o lookup para identificar o operador ou caractere desconhecido
            getChar(); // Obt�m o pr�ximo caractere
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

    // Verifica se o lexema � "True" ou "False" e atualiza o tipo do token
    if (strcmp(lexeme, "True") == 0)
        nextToken = TRUE;
    else if (strcmp(lexeme, "False") == 0)
        nextToken = FALSE;

    // Imprime o tipo do pr�ximo token e o lexema correspondente
    printf("Pr�ximo token �: %d, pr�ximo lexema � %s\n", nextToken, lexeme);

    return nextToken;
}


int comments() {
	
	// Verifica se o caractere atual � uma barra '/'
    if (nextChar == '/') {
    	// L� o pr�ximo caractere do arquivo (in_fp)
    	if((nextChar = getc(in_fp)) == EOF){
    		// Se o pr�ximo caractere for o final do arquivo (EOF), define charClass como EOF e retorna 0
    		charClass = EOF;
    		return 0;
		}
        
        // Verifica se o pr�ximo caractere � uma segunda barra '/'
        if (nextChar == '/') {
        	// Se for uma segunda barra, come�a a ler e ignorar os caracteres at� encontrar uma quebra de linha '\n' ou o final do arquivo (EOF)
            do {
                nextChar = getc(in_fp);
            } while (nextChar != '\n' && nextChar != EOF);
            return 1; // Retorna 1 se um coment�rio foi encontrado
        } else {
        	// Se o pr�ximo caractere n�o for uma segunda barra, exibe uma mensagem de erro e encerra o programa com c�digo de erro 1
            printf("ERRO: A sintaxe correta para coment�rios �: //\n"); //caso haja ocorr�ncia de apenas uma '/', haver� um erro.
            exit(1);
        }
    }
    // Se o caractere atual n�o for uma barra '/', retorna 0 (n�o � um coment�rio)
    return 0;
}
