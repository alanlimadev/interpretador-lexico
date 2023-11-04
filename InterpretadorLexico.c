#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

/*
	/****OBSERVAÇÕES
	
	-> Para o operador NOT foi usado '!' oa invés de '¬'
	-> Não é permitido usar número no início de uma variável
	
*/

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

//Booleanos
#define FALSE 10 // "False"
#define TRUE 11 // "True"

// Códigos de tokens
#define IDENT 20

#define AND_OP 30 // *
#define OR_OP 31 // +
#define IMP_OP 32 // ->
#define BIIMP_OP 33 // <->
#define NOT_OP 34 // !
#define COMMENT 35 // //
#define LEFT_PAREN 36 // (
#define RIGHT_PAREN 37 // )

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
	
	return 0; 
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
				
				//Tratando os caracteres após o IMP_OP
		        if(nextChar == '<' || nextChar == '-' || nextChar == '>'){
		            	printf("ERRO - A sintaxe correta é: ->");
		            	exit(1);
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
		            
		            //Tratando os caracteres após o BIIMP_OP
		            if(nextChar == '<' || nextChar == '-' || nextChar == '>'){
		            	printf("ERRO - A sintaxe correta é: <->");
		            	exit(1);
					}
					
		            nextToken = BIIMP_OP; // Define o token como BIIMP_OP (operador de bi-implicação)
		            break; 
		        }
		        ungetc(nextCharAux, in_fp); // Coloca o caractere de volta no arquivo
		    }
		    
		    nextToken = UNKNOWN; // Se não corresponder ao operador de bi-implicação, define o token como UNKNOWN (desconhecido)
		    break;
                                   
        default:
            addChar(); // Adiciona o caractere desconhecido ao lexema.
            nextToken = UNKNOWN; 
            printf("ERRO - Caracter não permitido\n");
            exit(1);
            break;
    }
    return nextToken; // Retorna o token identificado.
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

        // Reconhecer variáveis que começam com números (RESTRIÇÃO)
        case DIGIT:
            printf("ERRO: Não é possível ter uma variável que inícia com número\n"); 
            exit(1);
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
    
	int i;         // Declara uma variável inteira 'i' para uso no loop.
	int isTrue = 1; // Inicializa uma variável booleana 'isTrue' como verdadeira (1).
	int isFalse = 1; // Inicializa uma variável booleana 'isFalse' como verdadeira (1).
	
	// Inicia um loop que percorre os primeiros 4 caracteres do lexema (índices 0 a 3).
	for (i = 0; i < 4; i++) {
	    // Compara o caractere atual do lexema com o caractere correspondente na palavra "True".
	    // Se não corresponder, define 'isTrue' como falso (0).
	    if (lexeme[i] != "True"[i]) {
	        isTrue = 0;
	    }
	    
	    // Compara o caractere atual do lexema com o caractere correspondente na palavra "False".
	    // Se não corresponder, define 'isFalse' como falso (0).
	    if (lexeme[i] != "False"[i]) {
	        isFalse = 0;
	    }
	}
	
	// Verifica se 'isTrue' ainda é verdadeira (ou seja, o lexema corresponde a "True").
	if (isTrue) {
	    nextToken = TRUE; // Define 'nextToken' como o token TRUE.
	} 
	// Verifica se 'isFalse' ainda é verdadeira (ou seja, o lexema corresponde a "False").
	else if (isFalse) {
	    nextToken = FALSE; // Define 'nextToken' como o token FALSE.
	}
    
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

