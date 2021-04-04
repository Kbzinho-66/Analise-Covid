#include <iostream>
#include <cstdio>
#include <optional>
#include <vector>

#define INVALID_STRING "unknown"
#define INVALID_NUMBER -1

using std::cout;
using std::cin;
using std::endl;

class Movie {

    public:
        int movieID;
        std::string title;
        int releaseYear;
        std::optional<std::string> url;
        std::optional<int> directorID;

        Movie(int movieID,
            std::string title,
            int releaseYear,
            std::optional<std::string> url,
            std::optional<int> directorID
        ){
            this->movieID = movieID;
            this->title = title;
            this->releaseYear = releaseYear;
            this->url = url;
            this->directorID = directorID;
        }

        Movie() {
        }

        void reset() {
            movieID = 0;
            title = "";
            releaseYear = 0;
            url = "";
            directorID = 0;
        }

};

int readID();

void printMovie(std::optional<Movie> movie, std::optional<std::string> director);

std::optional<Movie> searchMovieID(int id, FILE *movies);

std::optional<Movie> searchMovieTitle(std::string name, FILE *movies);

std::optional<std::string> searchDirectorMovieID(int id, FILE *directors);

std::vector<Movie> searchDirectorName(std::string name, FILE *directors);

int main() {

    FILE *moviesFile, *directorsFile;

    moviesFile = fopen("arqMovies.txt", "r");

    if (moviesFile == NULL) {
        cout << "Erro ao abrir o arquivo dos filmes." << endl;
        exit(0);
    }

    directorsFile = fopen("directors.txt", "r");

    if (directorsFile == NULL) {
        cout << "Erro ao abrir o arquivo dos diretores. << endl";
        exit(0);
    }

    int resposta;
    cout << "----- Catálogo de Filmes -----" << endl;

    do {

        std::cout << std::endl << "Escolha uma opção: " << std::endl;
        std::cout << "1: Procurar um filme por ID" << std::endl;
        std::cout << "2: Procurar um filme por nome" << std::endl;
        std::cout << "3: Procurar todos os filmes de um diretor" << std::endl;
        std::cout << "0: Sair." << std::endl;

        // Laço pra leitura de uma resposta válida
        while ((std::cout << "Escolha uma opção:" << std::endl ) 
                && !(std::cin >> resposta) || resposta < 0 || resposta > 4) {
            std::cout << "Opção Inválida!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (resposta) {

            case 1: {

                int id = readID();

                std::cout << "Procurando..." << std::endl;

                std::optional<Movie> movie = searchMovieID(id, moviesFile);
                std::optional<std::string> director = searchDirectorMovieID(id, directorsFile);

                printMovie(movie, director);

                break;
            }

            case 2: {

                cout << "Insira o nome do filme (em inglês):" << endl;
                std::string title;
                cin >> title;

                std::optional<Movie> movie = searchMovieTitle(title, moviesFile);
                std::optional<std::string> director = searchDirectorMovieID(movie->movieID, directorsFile);

                printMovie(movie, director);

                break;
            }

                
            case 3: {

                cout << "Insira o nome do diretor:" << endl;
                std::string name;
                cin >> name;
                std::vector<Movie> directedMovies = (name, directorsFile);

                break;
            }
            
            default:
                break;
        }

    } while (resposta);

    return 0;
}

int readID() {

    int id;
    while ((std::cout << "Digite a ID do filme a ser consultado:" << std::endl ) 
        && !(std::cin >> id)) {

        std::cout << "A ID precisa ser um Inteiro!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    }

    return id;
}

void printMovie(std::optional<Movie> movie, std::optional<std::string> director) {

    if (movie && director) {

        cout << "Título: " << movie->title << " (" << movie->releaseYear << ")" << endl;
        cout << "Diretor: " << director.value() << endl;
        
    } else if (!director) {
        cout << "Não foi catalogado um diretor para esse filme." << endl;
    } else if (!movie) {
        cout << "Erro ao buscar esse filme." << endl;
    }
}

std::optional<Movie> searchMovieID(int id, FILE *movies) {

    std::optional<Movie> temp = Movie();

    while (fscanf(movies, "%d;%s;%d;%s;%d", &temp->movieID, temp->title, temp->releaseYear, temp->url, temp->directorID) > 0
        && (temp->movieID != id)) {
            temp.reset();
        }

    rewind(movies);

    if (temp->movieID == id && temp->title != INVALID_STRING && temp->releaseYear != INVALID_NUMBER) {
        return temp;
    } else {
        return std::nullopt;
    }
}

std::optional<std::string> searchDirectorMovieID(int id, FILE *directors) {

    int movieID;
    std::optional<std::string> name;

    while (fscanf(directors, "%d,%s", &movieID, name) > 0
        && (movieID != id)) {
            movieID = INVALID_NUMBER;
            name = INVALID_STRING;
        }

    rewind(directors);

    if (movieID == id && name != INVALID_STRING) {
        return name;
    } else {
        return std::nullopt;
    }
}

std::optional<Movie> searchMovieTitle(std::string title, FILE *movies) {

    std::optional<Movie> temp = Movie();

    while (fscanf(movies, "%d;%s;%d;%s;%d", &temp->movieID, temp->title, temp->releaseYear, temp->url, temp->directorID) > 0
        && (temp->title != title)) {
            temp.reset();
        }

    rewind(movies);

    if (temp->title == title && temp->movieID != INVALID_NUMBER && temp->releaseYear != INVALID_NUMBER) {
        return temp;
    } else {
        return std::nullopt;
    }
}

std::vector<Movie> searchDirectorName(std::string name, FILE *directors) {

    int movieID;
    std::string tempName;
    std::vector<int> movieIds;

    while (fscanf(directors, "%d,%s", &movieID, tempName) > 0) {

        if (tempName == name) {
            movieIds.push_back(movieID);
        }

        movieID = INVALID_NUMBER;
        tempName = INVALID_STRING;
    }

    rewind(directors);

    //TODO Terminar essa função
    if (movieIds.size() > 0) {
        return movieIds;
    } else {
        return 
    }
}