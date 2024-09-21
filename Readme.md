## Algoritmo Genético Genérico

- Versão em C++20 do algoritmo genético [feito em python](https://github.com/xLowZ/pin-ga-optimization) pelo grupo ProTεuS com algumas alterações e utilizando do [OpenMP](https://www.openmp.org/). Portanto, tem o mesmo objetivo: minimizar funções de benchmark para otimização.

### *Aviso*: 
- Há um bug com a função Sphere ao atingir certas condições específicas, então, excepcionalmente para esta, a paralelização interna será desativada.

## Sobre

- O algoritmo conta com as funções de:

    - [Rastrigin](https://en.wikipedia.org/wiki/Rastrigin_function)
    - [Ackley](https://en.wikipedia.org/wiki/Ackley_function)
    - [Sphere](https://www.sfu.ca/~ssurjano/spheref.html)
    - [Easom](https://www.sfu.ca/~ssurjano/easom.html)
    - [McCormick](https://www.sfu.ca/~ssurjano/mccorm.html)

- Seguem os parâmetros necessários e opções disponíveis, que devem ser passados no formato em que atualmente estão no arquivo de texto disponibilizado (funcionará para qualquer arquivo `.txt` no mesmo formato de `parameters.txt`):

    - Número de iterações

    - Tamanho da população
    - Taxa de mutação inicial e final
    - Força da mutação inicial e final (distribuição gaussiana)
    - Porcentagem do número de indivíduos para manter na próxima geração
    - Função de otimização
    - Número de dimensões
    - Método de seleção:
        - Torneio (tournament)
        - Seleção proporcional ao fitness (fps)
        - Seleção por ranking (ranking)
    - Número de pontos para recombinação: 
        - Um ponto (one) (automaticamente se  2 dimensões)
        - Dois pontos (two)
        - Recombinação uniforme (uniform)
    - Número de casas decimais desejadas para exibição no terminal
    - Número de algoritmos a serem executados
    
    O resultado exibido será a melhor solução em n execuções (diz-se n execuções o último parâmetro mencionado acima).

## Instalação
Para executar este projeto, siga os passos abaixo:

1. Clone o repositório:
    ```bash
    git clone https://github.com//xLowZ/genetic-algorithm-optimization/.git
    ```
2. Navegue até o diretório do projeto:
    ```bash
    cd genetic-algorithm-optimization 
    ```
3. Usando o CMake sem a extensão do VSCode:
    ```bash	
    mkdir build
    cd build
    ```
4. Padrão (gera solução do VisualStudio no Windows):
    ```bash	
        cmake ..
        cmake --build .
    ```
5. Com o MinGW:
    ```bash	
        cmake -G "MinGW Makefiles" ..
        cmake --build .
    ```


**OBS:**
- Para simplificação das 3 últimas etapas, recomendo as extensões `CMake` e `Cmake Tools` do Visual Studio Code.

## Uso
Para executar o algoritmo genético, utilize o seguinte comando:

```bash
.\gao.exe [caminho_arquivo_de_configurações] # Windows

./gao [caminho_arquivo_de_configurações]     # Linux
```

Opcionalmente, pode-se executar o programa com o argumento `--help` para descrição adicional:

```bash
.\gao.exe --help # Windows

./gao --help     # Linux
```
## Observações:

- Está disponibilizada minha pasta `.vscode` com tasks configuradas de debug e release para o compilador gcc para Windows. Se estiver utilizando o mesmo OS, basta verificar e ajustar o caminho para o compilador e para o debugger nos arquivos `.json`.
- Está disponibilizado também um arquivo Make separado pronto para uso.
- Há também um arquivo de texto adicional com algumas flags opcionais + uso do Makefile separadamente.
