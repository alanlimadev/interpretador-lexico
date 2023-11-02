# Interpretador Léxico

### Construir um interpretador léxico para expressões lógicas. Seu analisador léxico deverá analisar expressões da forma:
` (A+B)*C -> D `
> que significa "(A ou B) e C implica D" onde A, B e C são variáveis.

### As variáveis deverão ser formadas por qualquer tipo de palavra e número. Não é permitido o uso de símbolos especiais no nome de uma variável, nem é permitido variáveis muito longas, com mais de 100 caractéres.

### Seu analisador léxico deverá ignorar comentários. Os comentários sempre serão escritos após \ \ e até o final da linha.

### Uma expressão poderá usar:

* __()__
* __+__ para "ou"
* __*__ para "e" 
* __True__ para "verdadeiro"
* __False__ para "falso"
* __->__ para "implica"
* __<->__ para "se e só se"
* __!__ para "não" 

### Por fim, seu programa deverá ler a entrada a partir de um arquivo chamado entrada.in
