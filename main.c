// Trabalho IPC - Jogo Othello
// Joao Pedro Pereira Balieiro nUSP 12676615
// Joao Vitor Valerio Simplicio nUSP 15744492
// Joao Victor Pereira Lacerda nUSP 15463975

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// -------------------------------------------------------------

// cria o tabuleiro com as peças nas posições iniciais
void cria_tabuleiro(int** tabuleiro) {
    int i, j;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            tabuleiro[i][j] = ' ';
        }
    }

    tabuleiro[3][3] = 'B';
    tabuleiro[4][4] = 'B';
    tabuleiro[3][4] = 'P';
    tabuleiro[4][3] = 'P';

    return;
}

// -------------------------------------------------------------

// essa funcao serve para limpar o terminal apos cada movimento valido
// como o comando eh diferente para windows e linus eu coloquei para verificar qual o sistema
// e aplicar o comando correspondente
void limpa_tela() {
    #ifdef __linux__
        system("clear");
    #elif __WIN64__
        system("cls");
    #else

    #endif
    return;
}

// -------------------------------------------------------------

// mostra o tabuleiro com as coordenadas
void mostra_tabuleiro(int** tabuleiro) {
    int i,j;

    printf("  ");

    // mostra as coordenadas na linha superior
    for (i = 0; i < 8; i++)
    {
        printf("%d ", i);
    }

    printf("\n");

    // mostra as peças e as coordenadas nas demais linhas
    for (i = 0; i < 8; i++)
    {
        printf("%d ", i);
        for (j = 0; j < 8; j++)
        {
            // verifica qual caracter colocar em cada posicao
            switch (tabuleiro[i][j])
            {
                case ' ':
                    printf("- ");
                    break;
                case 'P':
                    printf("P ");
                    break;
                case 'B':
                    printf("B ");
                    break;
            }
        }
        printf("\n");
    }
    return;
}

// -------------------------------------------------------------

int movimento_valido(int** tabuleiro, int linha, int coluna, char jogador) {
    int i, j, x, y, tem_adversario;
    int adversario = (jogador == 'P') ? 'B' : 'P';

    // verifica se as coordenadas estao no tabuleiro
    if (linha < 0 || linha >= 8 || coluna < 0 || coluna >= 8) {
        return 0;
    }

    // verifica se a celula esta vazia
    if (tabuleiro[linha][coluna] != ' ') {
        return 0;
    }

    // verifica em todas as direcoes se ha pecas do oponente que podem ser capturadas
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            // ignora a propria posicao e verifica apenas as direcoes
            if (i == 0 && j == 0) {
                continue;
            }

            x = linha + i;
            y = coluna + j;
            tem_adversario = 0;

            // verifica se tem uma peca adversaria
            while (x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro[x][y] == adversario) {
                x += i;
                y += j;
                tem_adversario = 1;
            }

            // volta como verdadeiro se tem uma jogada valida
            if (tem_adversario && x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro[x][y] == jogador) {
                return 1;
            }
        }
    }

    return 0;
}

// -------------------------------------------------------------

// verifica quem eh o vencedor
int jogador_vencedor(int** tabuleiro) {
    char vencedor;
    int i, j, jogador_preto, jogador_branco;

    jogador_preto = 0;
    jogador_branco = 0;

    // conta quantas pecas de cada tipo tem no tabuleiro
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (tabuleiro[i][j] == 'P') {
                jogador_preto++;
            }
            if (tabuleiro[i][j] == 'B') {
                jogador_branco++;
            }
        }
    }

    // verifica quem tem mais pecas e marca como vencedor
    if (jogador_branco > jogador_preto) {
        vencedor = 'B';
    }
    else if (jogador_branco < jogador_preto) {
        vencedor = 'P';
    }
    // caso termine empatado a funcao retorna 0
    else {
        return 0;
    }

    return vencedor;
}

// -------------------------------------------------------------

// funcao para ver quantas pecas sao tomadas a cada jogada
int quantpecas(int** tabuleiro, char jogador, int linha, int coluna){
    int i, j, x, y, tem_adversario, quant_pecas_tomadas;
    char adversario = 'P'; // essa funcao so eh usada quando o computador joga

    quant_pecas_tomadas = 0;

    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            x = linha + i;
            y = coluna + j;
            tem_adversario = 0;

            while (x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro[x][y] == adversario) {
                x += i;
                y += j;
                tem_adversario = 1;
            }

            if (tem_adversario && x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro[x][y] == jogador) {
                x -= i;
                y -= j;
                while (tabuleiro[x][y] == adversario) {
                    quant_pecas_tomadas++;
                    x -= i;
                    y -= j;
                }
            }
        }
    }

    return quant_pecas_tomadas;
}

// -------------------------------------------------------------

// retorna a quantidade maxima de pecas que o humano pode tomar de acordo com a jogada do computador
int pecastomadashumano(int** tabuleiro, int linha, int coluna){
    int i,j,pecas_tomadas_atual,pecas_tomadas_total,x,y,tem_adversario;
    char jogador = 'B';
    char adversario = 'P';

    int** tabuleiro_simulado = (int **) malloc(8*sizeof(int *));
    for (i = 0; i < 8; i++) {
        tabuleiro_simulado[i] = (int*) malloc(8*sizeof(int));
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            // verifica qual caracter colocar em cada posicao no novo tabuleiro
            switch (tabuleiro[i][j]) {
                case ' ':
                    tabuleiro_simulado[i][j] = ' ';
                    break;
                case 'P':
                    tabuleiro_simulado[i][j] = 'P';
                    break;
                case 'B':
                    tabuleiro_simulado[i][j] = 'B';
                    break;
            }
        }
    }

    // executa a jogada do computador no tabuleiro simulado
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            x = linha + i;
            y = coluna + j;
            tem_adversario = 0;

            while (x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro_simulado[x][y] == adversario) {
                x += i;
                y += j;
                tem_adversario = 1;
            }

            if (tem_adversario && x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro_simulado[x][y] == jogador) {
                // gira as peças do adversario
                x -= i;
                y -= j;
                // captura as pecas do adversario
                while (tabuleiro_simulado[x][y] == adversario) {
                    tabuleiro_simulado[x][y] = jogador;
                    x -= i;
                    y -= j;
                }
            }
        }
    }

    // similar a funcao minimax a diferenca eh que agora eh uma possivel jogada humana
    // ai retorna o maximo de pecas que o jogador pode tomar dependendo da jogada do computador
    pecas_tomadas_total = 0;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (movimento_valido(tabuleiro_simulado,i,j,adversario) == true) {
                pecas_tomadas_atual = quantpecas(tabuleiro_simulado,adversario,i,j);
                if (pecas_tomadas_atual > pecas_tomadas_total) {
                    pecas_tomadas_total = pecas_tomadas_atual;
                }
            }
        }
    }

    free(tabuleiro_simulado);
    return pecas_tomadas_total;
}

// -------------------------------------------------------------

// funcao para ver qual jogada vale mais a pena para o computador
// ela ve a diferenca entre pecas que o computador pode tomar
// e pecas que o humano pode tomar dependendo da jogada do computador
// ai ela escolhe a com maior diferenca
// ela retorna um vetor por isso a alocacao dinamica
int* minimax(int** tabuleiro,char jogador){
    int i,j,linha,coluna,minmax_atual,minmax_total;

    // comeca como - 64 pois eh o numero maximo de peca que se pode perder
    minmax_total = -64;

    // nessa parte verifica se uma jogada eh valida
    // se ela for verifica quantas pecas sao tomadas
    // depois comparadas com a quantidades total que da pra tomar de pecas
    // ai salva as coordenadas da jogada que mais da pecas
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (movimento_valido(tabuleiro,i,j,jogador) == true) {
                minmax_atual = quantpecas(tabuleiro,jogador,i,j) - pecastomadashumano(tabuleiro,i,j);
                if (minmax_atual > minmax_total) {
                    minmax_total = minmax_atual;
                    linha = i;
                    coluna = j;
                }
            }
        }
    }

    int* coordenadas = (int *) malloc(2*sizeof(int));
    if (coordenadas == 0) {
        printf("memoria nao alocada");
        return 0;
    }

    coordenadas[0] = linha;
    coordenadas[1] = coluna;

    return coordenadas;
}

// -------------------------------------------------------------

// verifica se a jogada eh valida e registra ela na matriz
void jogada(int** tabuleiro, char jogador, int modo) {
    char adversario;
    int linha, coluna, tem_adversario, x, y, i, j, jogadas_validas;

    // isso serve para iniciar o while
    // depois o valor vai variar de acordo com as jogadas validas
    jogadas_validas = 1;

    // mantem o codigo rodando ate nao ter mais jogadas validas
    while (jogadas_validas != 0) {

        if (modo == 2 && jogador == 'B') {
            int *coordenadas = minimax(tabuleiro,jogador);
            linha = coordenadas[0];
            coluna = coordenadas[1];
            free(coordenadas);
            printf("\nO computador jogou na casa %d X %d\n\n", linha, coluna);
        }
        else {
            // recebe as coordenadas da jogada humana
            printf("\nJogador %c, insira a linha e a coluna da sua jogada separadas por um espaco:", jogador);
            scanf("%d %d", &linha, &coluna);
            printf("\n");

            // verifica se as coordenadas são válidas
            if (movimento_valido(tabuleiro,linha,coluna,jogador) == false) {
                printf("Jogada invalida. Tente novamente.\n");
                continue;
            }
        }

        // limpa a tela apos cada jogada
        limpa_tela();

        // salva a jogada na matriz
        tabuleiro[linha][coluna] = jogador;

        // define o adversario
        adversario = (jogador == 'P') ? 'B' : 'P';

        // faz o loop para verificar quais pecas podem ser capturadas
        // o codigo eh praticamente igual ao para verificar a validade do movimento
        // a unica adicao eh para capturar as pecas
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }

                x = linha + i;
                y = coluna + j;
                tem_adversario = 0;

                while (x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro[x][y] == adversario) {
                    x += i;
                    y += j;
                    tem_adversario = 1;
                }

                if (tem_adversario && x >= 0 && x < 8 && y >= 0 && y < 8 && tabuleiro[x][y] == jogador) {
                    // gira as peças do adversario
                    x -= i;
                    y -= j;
                    // captura as pecas do adversario
                    while (tabuleiro[x][y] == adversario) {
                        tabuleiro[x][y] = jogador;
                        x -= i;
                        y -= j;
                    }
                }
            }
        }

        // mostra o tabuleiro atualizado
        mostra_tabuleiro(tabuleiro);

        // essa parte verifica se existem jogadas validas para o adversario em qualquer casa do tabuleiro
        jogadas_validas = 0;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (movimento_valido(tabuleiro,i,j,adversario) == true) {
                    jogadas_validas = jogadas_validas + 1;
                }
            }
        }

        // essa parte eh usada caso o adversario nao tenha jogadas validas
        // aqui verifica se tem jogadas disponiveis para o jogador atual
        // se sim o jogo continua, se nao ele acaba
        if (jogadas_validas == 0) {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    if (movimento_valido(tabuleiro,i,j,jogador) == true) {
                        // aqui esta igualando a -1 para diferenciar da jogada valida do adversario
                        jogadas_validas = -1;
                    }
                }
            }
        }

        // passa a vez para o adversario se ele tem jogada disponivel
        // se nao continua no mesmo jogador
        if (jogadas_validas > 0){
            jogador = adversario;
        }
        else{
            jogador = jogador;
        }
    }

    return;
}

// -------------------------------------------------------------

int main() {
    char jogador, caracter_sair;
    int** tabuleiro, modo, i;

    tabuleiro = (int **) malloc(8*sizeof(int *));
    for (i = 0; i < 8; i++) {
        tabuleiro[i] = (int*) malloc(8*sizeof(int));
    }

    printf("Bem vindo(a) ao jogo Othello!\n"
           "As pecas pretas sao representadas pela letra P e as pecas brancas pela letra B\n"
           "O jogador com as pecas pretas inicia o jogo\n"
           "Digite 1 para jogar no modo Humano X Humano, ou, digite 2 para jogar no modo Humano X Computador: ");
    scanf("%d", &modo);
    printf("\n");

    // cria e mostra o tabuleiro inicial
    cria_tabuleiro(tabuleiro);
    mostra_tabuleiro(tabuleiro);

    // forca a comecar com as pecas pretas
    jogador = 'P';

    // comeca o jogo pelo jogador preto e da sequencia nas jogadas
    jogada(tabuleiro, jogador, modo);

    // mostra o vencedor ou se deu empate
    if (jogador_vencedor(tabuleiro) == 0) {
        printf("\nO jogo terminou empatado");
    }
    else {
        printf("\nO Jogador %c venceu!", jogador_vencedor(tabuleiro));
    }

    // finaliza o jogo
    printf("\nDigite qualquer caracter para sair");
    scanf("%c", &caracter_sair);

    free(tabuleiro);
    return 0;
}
