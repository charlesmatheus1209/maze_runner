#include <iostream>
#include <fstream>
#include <stack>
#include <unistd.h>
#include <thread>

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	 
	FILE *arquivo;
	int result, i;
	char Str[50];

	arquivo = fopen(file_name, "r");

	if (arquivo == NULL)
	{
		printf("Problemas na CRIACAO do arquivo\n");
	}else{
		printf("Maze.txt aberto\n");
	}

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols

	fscanf(arquivo, "%d %d", &num_rows, &num_cols);

	printf("colunas: %d linhas %d\n", num_cols, num_rows);

	maze = (char**)malloc(num_rows * sizeof(char*));
    for (i = 0; i < num_rows; ++i) {
        maze[i] = (char*)malloc(num_cols * sizeof(char));
    }
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			fscanf(arquivo, " %c", &maze[i][j]);
			if(maze[i][j] == 'e'){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}

	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	

	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	
		// Marcar a posição atual com o símbolo '.'
		maze[pos.i][pos.j] = 'o';
		// Limpa a tela
		
		system("clear");
		
		// Imprime o labirinto
		print_maze();
		//sleep(0.5);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1
		 		- pos.i, pos.j-1
		 		- pos.i+1, pos.j
		 		- pos.i-1, pos.j
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	*/
	 	
		if(pos.i >= 0 && pos.i < num_rows && pos.j >= 0 && pos.j < num_cols){
			if(pos.j + 1 < num_cols){
				if(maze[pos.i][pos.j+1] == 's'){
					return true;
				}else if(maze[pos.i][pos.j + 1] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i;
					next_pos.j = pos.j + 1;
					valid_positions.push(next_pos);
				}
			}
			if(pos.j - 1 >= 0){
				if(maze[pos.i][pos.j-1] == 's'){
					return true;
				}else if(maze[pos.i][pos.j-1] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i;
					next_pos.j = pos.j - 1;
					valid_positions.push(next_pos);
				}
			}
			if(pos.i + 1 < num_rows){
				if(maze[pos.i +1][pos.j] == 's'){
					return true;
				}else if(maze[pos.i + 1][pos.j] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i + 1;
					next_pos.j = pos.j;
					valid_positions.push(next_pos);
				}
			}
			if(pos.i - 1 >= 0){
				if(maze[pos.i -1][pos.j] == 's'){
					return true;
				}else if(maze[pos.i - 1][pos.j] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i - 1;
					next_pos.j = pos.j;
					valid_positions.push(next_pos);
				}
			}
		}


		
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		maze[pos.i][pos.j] = '.';
		if (!valid_positions.empty()) {
			pos_t next_position;
			while(true){
				if(valid_positions.size() == 0){
					return false;
				}
				next_position = valid_positions.top();
				valid_positions.pop();
				if(maze[next_position.i][next_position.j] != '.'){
					break;
				}
			}
			return walk(next_position);
		}

	
		
	return false;
}

int main(int argc, char* argv[]) {
	
	if(argv[1] != nullptr){		
		// const char * nome_do_arquivo = "../data/maze2.txt";
		// std::cout << nome_do_arquivo << std::endl;
		// carregar o labirinto com o nome do arquivo recebido como argumento
		// pos_t initial_pos = load_maze(nome_do_arquivo);
		pos_t initial_pos = load_maze(argv[1]);

		printf("Posição inicial: %d %d\n", initial_pos.i, initial_pos.j);
		//print_maze();

		// chamar a função de navegação
		bool exit_found = walk(initial_pos);

		// Tratar o retorno (imprimir mensagem)
		if(exit_found){
			system("clear");	
			print_maze();

			printf("\n\nA saida foi encontrada\n\n");
		}else{
			printf("\n\nA saida NÃO foi encontrada\n\n");
		}
	}

	
	return 0;
}
