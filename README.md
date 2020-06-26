### Multilayer perceptron
Este programa implementa uma rede neural artificial multicamadas
com a possibilidade de utilizar diferentes funções de ativacao em cada camada.

Para compilar o programa use o make file disponivel na pasta "compile",
ou use o "cmakelist" tambem disponivel na pasta "compile" (este deve ser movido para a raiz do projeto),
Existe tambem a  opção de compilar pelo script cmd (PARA WINDOWS usando o mingw gcc)

Após compilar o programa consulte os argumentos:

#### Argumentos
Caso o numero de argumentos seja diferente de 11 o programa mnostra-ra uma mensagem de erro e encerrará.
 
| Argumento | Significado | Exemplo |
| :---- | :---- | :---- |
| argv[0] | Nome do arquivo executavel | main.exe |
| argv[1] | Nro de camadas ocultas| 3 |
| argv[2] | Tamanho de cada camada oculta, separado por virgula | 4,5,5 | 
| argv[3] | Funcoes de ativacao das camadas ocultas, separado por virgula (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5) | 2,3,1 |
| argv[4] | Tamanho da ultima camada | 1 |
| argv[5] | Funcao de ativacao da ultima camada | 2 |
| argv[6] | Taxa de aprendizado | 0.01 |
| argv[7] | Nro maximo de epocas | 10000 |
| argv[8] | Tolerancia | 0.0001 |
| argv[9] | Arquivo csv dos dados de treinamento| dados/dados_treinamento.csv |
| argv[10] | Arquivo csv dos dados de teste | dados/dados_teste.csv |
 
 As funções são representadas por codigo ID
 Consulte a tabela abaixo para escolher a função desejada
 
| FUNÇÃO | ID |
| :---- | :----: |
| identidade | 1 |
| sigmoide   | 2 |
| tanh       | 3 |
| relu       | 4 |
| softmax    | 5 |
 
  
 
