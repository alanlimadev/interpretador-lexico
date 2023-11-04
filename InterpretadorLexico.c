#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

/*
	/****OBSERVA��ES
	
	-> Para o operador NOT foi usado '!' oa inv�s de '�'
	-> N�o � permitido usar n�mero no in�cio de uma vari�vel
	
*/

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

//Booleanos
#define FALSE 10 // "False"
#define TRUE 11 // "True"

// C�digos de tokens
#define IDENT 20

#define AND_OP 30 // *
#define OR_OP 31 // +
#define IMP_OP 32 // ->
#define BIIMP_OP 33 // <->
#define NOT_OP 34 // !
#define COMMENT 35 // //
#define LEFT_PAREN 36 // (
#define RIGHT_PAREN 37 // )

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
	
	return 0; 
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

		case '-':
		    addChar(); // Adiciona o caractere '-' ao lexema
		    getChar(); // Obt�m o pr�ximo caractere
		    
		    if (nextChar != '>') { // Verifica se o pr�ximo caractere n�o � '>'
		    
		        printf("ERRO: A sintaxe correta �: ->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		        exit(1); // Encerra o programa com um c�digo de erro
		    } else {
		    	
		        addChar(); // Adiciona o caractere '>' ao lexema
		        getChar(); // Obt�m o pr�ximo caractere
				
				//Tratando os caracteres ap�s o IMP_OP
		        if(nextChar == '<' || nextChar == '-' || nextChar == '>'){
		            	printf("ERRO - A sintaxe correta �: ->");
		            	exit(1);
				}
		
		        nextToken = IMP_OP; // Define o token como IMP_OP (operador de implica��o)
		        break; 
		    }
		    
		    nextToken = UNKNOWN; // Se n�o corresponder ao operador de implica��o, define o token como UNKNOWN (desconhecido)
		    break; 
		
		case '<':
		    addChar(); // Adiciona o caractere '<' ao lexema
		    getChar(); // Obt�m o pr�ximo caractere
		    
		    if (nextChar != '-') { // Verifica se o pr�ximo caractere n�o � '-'
		    
		        printf("ERRO: A sintaxe correta �: <->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		        exit(1); // Encerra o programa com um c�digo de erro
		    } else {
		    	
		        char nextCharAux = getc(in_fp); // L� o pr�ximo caractere do arquivo
		        
		        if (nextCharAux != '>') { // Verifica se o pr�ximo caractere n�o � '>'
		        
		            printf("ERRO: A sintaxe correta �: <->\n"); // Exibe uma mensagem de erro para a sintaxe incorreta
		            exit(1); // Encerra o programa com um c�digo de erro
		        } else {
		        	
		            addChar(); // Adiciona o caractere '-' ao lexema
		            
		            ungetc(nextCharAux, in_fp); // Coloca o caractere de volta no arquivo
		            
		            getChar(); // Obt�m o pr�ximo caractere
		            addChar(); // Adiciona o caractere '>' ao lexema
		            getChar(); // Obt�m o pr�ximo caractere
		            
		            //Tratando os caracteres ap�s o BIIMP_OP
		            if(nextChar == '<' || nextChar == '-' || nextChar == '>'){
		            	printf("ERRO - A sintaxe correta �: <->");
		            	exit(1);
					}
					
		            nextToken = BIIMP_OP; // Define o token como BIIMP_OP (operador de bi-implica��o)
		            break; 
		        }
		        ungetc(nextCharAux, in_fp); // Coloca o caractere de volta no arquivo
		    }
		    
		    nextToken = UNKNOWN; // Se n�o corresponder ao operador de bi-implica��o, define o token como UNKNOWN (desconhecido)
		    break;
                                   
        default:
            addChar(); // Adiciona o caractere desconhecido ao lexema.
            nextToken = UNKNOWN; 
            printf("ERRO - Caracter n�o permitido\n");
            exit(1);
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

        // Reconhecer vari�veis que come�am com n�meros (RESTRI��O)
        case DIGIT:
            printf("ERRO: N�o � poss�vel ter uma vari�vel que in�cia com n�mero\n"); 
            exit(1);
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
    
	int i;         // Declara uma vari�vel inteira 'i' para uso no loop.
	int isTrue = 1; // Inicializa uma vari�vel booleana 'isTrue' como verdadeira (1).
	int isFalse = 1; // Inicializa uma vari�vel booleana 'isFalse' como verdadeira (1).
	
	// Inicia um loop que percorre os primeiros 4 caracteres do lexema (�ndices 0 a 3).
	for (i = 0; i < 4; i++) {
	    // Compara o caractere atual do lexema com o caractere correspondente na palavra "True".
	    // Se n�o corresponder, define 'isTrue' como falso (0).
	    if (lexeme[i] != "True"[i]) {
	        isTrue = 0;
	    }
	    
	    // Compara o caractere atual do lexema com o caractere correspondente na palavra "False".
	    // Se n�o corresponder, define 'isFalse' como falso (0).
	    if (lexeme[i] != "False"[i]) {
	        isFalse = 0;
	    }
	}
	
	// Verifica se 'isTrue' ainda � verdadeira (ou seja, o lexema corresponde a "True").
	if (isTrue) {
	    nextToken = TRUE; // Define 'nextToken' como o token TRUE.
	} 
	// Verifica se 'isFalse' ainda � verdadeira (ou seja, o lexema corresponde a "False").
	else if (isFalse) {
	    nextToken = FALSE; // Define 'nextToken' como o token FALSE.
	}
    
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

