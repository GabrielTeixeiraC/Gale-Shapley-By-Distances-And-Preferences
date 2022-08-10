#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// struct para armazenar um peso e a bike associada a ele
struct bikePeso
{
    int bike;
    int peso;
};

// struct para armazenar uma distancia e a pessoa associada a ela
struct pessoaDistancia
{
    int pessoa;
    int distancia;
};

// gale-shapley como visto na aula
vector<pair<int, int>> galeShapley(vector<vector<int>> &preferenciasPessoas, vector<vector<int>> &preferenciasBikes, int numeroDePares) {
    // vetor retornado pela funcao, com os pares alocados
    vector<pair<int, int>> casamento;

    // vetor com o indice da proxima bike à qual cada pessoa irá propor
    vector<int> proximaProposta(numeroDePares, 0);

    // vetor com os casamentos, inicializados com -1
    vector<int> casamentoPessoas(numeroDePares, -1), casamentoBikes(numeroDePares, -1);
    
    // fila das pessoas que ainda irão propor
    queue<int> pessoasLivres;
    for (int i = 0; i < numeroDePares; i++){
        pessoasLivres.push(i);
    }

    // matriz com a posicao da pessoa j para a bike i no elemento bikesRankings[i], permite decisão em O(1)
    vector<vector<int>> bikesRankings(numeroDePares, vector<int>(numeroDePares, 0));

    // alocação da matriz bikesRankings
    for (int i = 0; i < numeroDePares; i++) {
        for (int j = 0; j < numeroDePares; j++) { 
            // Ex: PB[0]: {1, 2, 0} -> PB[0][0] == 1 & BR[0][1] == 0 -> BR[0]: {2, 0, 1}
            bikesRankings[i][preferenciasBikes[i][j]] = j;
        }
    }

    // enquanto ainda houver pessoas sem par
    while (!pessoasLivres.empty()) {
        int pessoa = pessoasLivres.front();
        // propoe para a proxima bike em sua lista de preferencias
        int bike = preferenciasPessoas[pessoa][proximaProposta[pessoa]];
        // se a bike não tiver par ainda, aloca
        if (casamentoBikes[bike] == -1) {
            casamentoBikes[bike] = pessoa;
            casamentoPessoas[pessoa] = bike;
            pessoasLivres.pop();
        }
        // senão, compara se a proposta melhora a situação da bike. Se sim, aloca 
        else if (bikesRankings[bike][pessoa] < bikesRankings[bike][casamentoBikes[bike]]) {
            int pessoaAntiga = casamentoBikes[bike];
            casamentoBikes[bike] = pessoa;
            casamentoPessoas[pessoa] = bike;
            casamentoPessoas[pessoaAntiga] = -1;
            pessoasLivres.push(pessoaAntiga);
            pessoasLivres.pop();
        }
        proximaProposta[pessoa]++;
    }

    // passa os casamentos para o vetor de casamento
    for (int i = 0; i < numeroDePares; i++) {
        casamento.push_back(make_pair(i, casamentoPessoas[i]));
    }

    // retorna o casamento
    return casamento;
}

// bfs para calcular a distancia de um vertice para todos os outros do mapa
vector<int> bfs(int verticeInicial, vector<vector<int>> & listaDeAdjacencias) {
    // inicia vetor de vértices descobertos todo como falso. descobertos[0] == true se V0 já tiver sido descoberto
    vector<bool> descobertos(listaDeAdjacencias.size(), false);
    // vetor com distancias dos vértices do grafo ao verticeInicial. Inicia todos com distancia "Infinita"
    vector<int> distancias(listaDeAdjacencias.size(), 1000001);
    // verticeInicial é descoberto e tem distancia 0
    descobertos[verticeInicial] = true;
    distancias[verticeInicial] = 0;
    // fila com os vertices a visitar
    queue<int> fila;
    fila.push(verticeInicial);
    while (!fila.empty()) {
        // pega um vertice V da fila
        int vertice = fila.front();
        fila.pop();
        // para cada vertice adjacente a V, se não tiver sido descoberto, adiciona ele na fila,
        // define como descoberto e define sua distancia como a distancia de V ao verticeInicial + 1 
        for (int i = 0; i < listaDeAdjacencias[vertice].size(); i++){
            int verticeAdjacente = listaDeAdjacencias[vertice][i];
            if (descobertos[verticeAdjacente] == false) {
                fila.push(verticeAdjacente);
                descobertos[verticeAdjacente] = true;        
                distancias[verticeAdjacente] = distancias[vertice] + 1;
            }
        }
    }

    // retorna o vetor de distancias
    return distancias;
}

// funcao para ordenar as preferencias das bikes pela distancia e pelo id da pessoa
bool ordenaDistancias (const pessoaDistancia& a, const pessoaDistancia& b)
{
    if ( a.distancia == b.distancia )
        return a.pessoa < b.pessoa ;
    else
        return a.distancia < b.distancia;
}

// funcao para ordenar as preferencias das pessoas pelo peso e pelo id da bike
bool ordenaPesos (const bikePeso& a, const bikePeso& b)
{
    if ( a.peso == b.peso )
        return a.bike < b.bike ;
    else
        return a.peso > b.peso;
}

int main() {
    // recebendo numero de pares
    int numeroDePares;
    cin >> numeroDePares;
    
    // recebendo tamanho do mapa
    int linhas, colunas;
    cin >> linhas >> colunas;
    int numeroDeVertices = linhas * colunas;
    
    // atribuição dos vertices
    // vetor de booleanos que salva vertices que são obstaculos. Ex: obstaculo[1][0] == true, V4 é obstáculo
    vector<bool> obstaculo (numeroDeVertices, false);
    // vetores que salvam vertices com pessoas e bikes
    vector<int> verticesComPessoas;
    vector<int> verticesComBikes;
    // salva os conteúdos dos vertices do mapa. Ex: conteudoVertices[0] == '*', conteudoVertices[0] == 'a' 
    vector<char> conteudoVertices (numeroDeVertices);
   
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // converte os indices i, j do mapa para vertices de 0 a N*M - 1
            int vertice = i * colunas + j;
            char conteudoVertice;
            cin >> conteudoVertice;
            // salva conteudo do vertice
            conteudoVertices[vertice] = conteudoVertice;
            if (conteudoVertice == '-') {   // obstaculo
                obstaculo[vertice] = true;
            }
            else if (conteudoVertice == '*') {  // caminho
                continue;
            }            
            else if (conteudoVertice > 96 && conteudoVertice < 107) {   // se for uma pessoa, [a, j]
                verticesComPessoas.push_back(vertice);
            }
            else {  // bicicleta, [0, 9]
                verticesComBikes.push_back(vertice);
            }
        }
    }
    
    // criação da lista de adjacências do mapa
    vector<vector<int>> listaDeAdjacencias (numeroDeVertices);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // não tem adjacencias se for obstaculo
            int verticeAtual = i * colunas + j;
            if (obstaculo[verticeAtual] == true) {
                continue;
            }
            
            // calcula índices dos vértices adjacentes
            int colunaDireita = j + 1;
            int verticeDireita = i * colunas + colunaDireita;
            
            int colunaEsquerda = j - 1;
            int verticeEsquerda = i * colunas + colunaEsquerda;
            
            int linhaAcima = i - 1;
            int verticeAcima = (linhaAcima) * colunas + j;
            
            int linhaAbaixo = i + 1;
            int verticeAbaixo = (linhaAbaixo) * colunas + j;
            
            // se for posição do grid e não for obstáculo, é adjacente. Considera posicoes 
            // acima, abaixo, à direita e à esquerda.
            if (colunaDireita < colunas && obstaculo[verticeDireita] == false) {
                listaDeAdjacencias[verticeAtual].push_back(verticeDireita);
            }
            if (colunaEsquerda >= 0 && obstaculo[verticeEsquerda] == false) {
                listaDeAdjacencias[verticeAtual].push_back(verticeEsquerda);
            }
            if (linhaAcima >= 0 && obstaculo[verticeAcima] == false) {
                listaDeAdjacencias[verticeAtual].push_back(verticeAcima);
            }
            if (linhaAbaixo < linhas && obstaculo[verticeAbaixo] == false) {
                listaDeAdjacencias[verticeAtual].push_back(verticeAbaixo);
            }
        }
    }

    // recebendo os pesos das bikes para as pessoas
    vector<vector<bikePeso>> pesosPreferenciasPessoas (numeroDePares, vector<bikePeso>(numeroDePares));

    for (int i = 0; i < numeroDePares; i++) {
        for (int j = 0; j < numeroDePares; j++) {
            int peso;
            cin >> peso;
            pesosPreferenciasPessoas[i][j].bike = j;
            pesosPreferenciasPessoas[i][j].peso = peso;
        }        
    }

    // convertendo pesos para preferencias das pessoas
    vector<vector<int>> preferenciasPessoas(numeroDePares, vector<int>(numeroDePares));
    // ordena as listas de pesos
    for (int i = 0; i < numeroDePares; i++) {
        sort(pesosPreferenciasPessoas[i].begin(), pesosPreferenciasPessoas[i].end(), ordenaPesos);
    }

    // salva pesos ordenados como preferencias de bikes
    for (int i = 0; i < numeroDePares; i++) {
        for (int j = 0; j < numeroDePares; j++) {
            preferenciasPessoas[i][j] = pesosPreferenciasPessoas[i][j].bike;
        }
    }
    
    // calculando distancias entre as bikes e as pessoas com bfs
    vector<vector<pessoaDistancia>> distanciaBikePessoa (numeroDePares, vector<pessoaDistancia>(numeroDePares));

    for (int i = 0; i < numeroDePares; i++){
        int verticeBike = verticesComBikes[i];  // seleciona uma bike
        int bike = conteudoVertices[verticeBike] - 48;  //acha o id da bike, char - '0' -> numero [0,9]
        vector<int> distancias = bfs(verticeBike, listaDeAdjacencias);  // calcula distancias da bike atual
        for (int j = 0; j < numeroDePares; j++){
            int verticePessoa = verticesComPessoas[j];  // seleciona uma pessoa
            distanciaBikePessoa[bike][j].pessoa = conteudoVertices[verticePessoa] - 97; // acha e salva id da pessoa, char - 'a' -> numero [0, 9]
            distanciaBikePessoa[bike][j].distancia = distancias[verticePessoa]; // salva distancia da bike à pessoa atual
        }   
    }

    // convertendo distancias para preferencias das bikes
    vector<vector<int>> preferenciasBikes(numeroDePares, vector<int>(numeroDePares));
    // ordena as listas de distancias
    for (int i = 0; i < numeroDePares; i++) {
        sort(distanciaBikePessoa[i].begin(), distanciaBikePessoa[i].end(), ordenaDistancias);
    }

    // salva distancias ordenadas como preferencias de bikes
    for (int i = 0; i < numeroDePares; i++) {
        for (int j = 0; j < numeroDePares; j++) {
            preferenciasBikes[i][j] = distanciaBikePessoa[i][j].pessoa;
        }
    }

    // aplica o gale-shapley
    vector<pair<int, int>> casamento = galeShapley(preferenciasPessoas, preferenciasBikes, numeroDePares);
    
    // imprimindo o casamento
    for (int i = 0; i < numeroDePares; i++) {
        // converte numero da pessoa para letra [a, j]
        char pessoa = 97 + i;
        // imprime casamentos
        cout << pessoa << " " << casamento[i].second << endl;
    }

    return 0;
}