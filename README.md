#### Análise de dados da campanha de vacinação contra o COVID-19 no Rio Grande do Sul 

##### A análise
Para análise dos dados, foi implementado um software em C++ que indexava dados de um arquivo CSV, extraído do Open DataSus, uma plataforma de transaparência de informação do Ministério da Saúde.

##### Para executar o programa
É necessaria a versão `10.2.0` do compilador `gcc` e a revisão `C++ 20` da linguágem.
Para executar o programa, rodar o comando `gcc -o analise -std=c++20 main.cpp` e em seguida executar o comando `./analise` duas vezes. A primeira para criação dos arquivos de dados e de índices e a segunda para realizar as pesquisas.

##### Para executar o programa em Python
É necessário Python3.x instalado, uma instância do MongoDB instalada e conectada na máquina local ou em núvem, a biblioteca Crypto e cryptography para Python3.x.

##### Por que usar o MongoDB?
Como estamos trabalhando com um arquivo .CSV que possui um grande número de
entradas, nós consideramos que o banco de dados NoSQL MongoDB seria a melhor opção
pelos seguintes motivos:
* O MongoDB possui uma função nativa para importar arquivos em vários formatos, o
mongoimport, que nos permitiu importar todo o arquivo com apenas um comando e
de maneira rápida.
* O MongoDB é um NoSQL do tipo de documentos, e guarda os dados no formato
JSON, o que facilita na manipulação dos dados
* A equipe já tinha alguma experiência trabalhando com MongoDB.
