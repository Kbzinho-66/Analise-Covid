#include <iostream>
#include <cstdio>
#include <vector>
#include <optional>

#define INVALID_STRING "unknown"
#define INVALID_NUMBER -1

using std::cout;
using std::cin;
using std::endl;

class Movie;
class Director {

    public:
        int directorID;
        std::string name;
        std::vector<Movie> moviesDirected;

        Director(int directorID, std::string name) {
            this->directorID = directorID;
            this->name = name;
        }

        Director() {

        }

        void reset() {
            directorID = 0;
            name = "";
        }

};

class Movie {

    public:
        int movieID;
        std::string title;
        int releaseYear;
        std::optional<std::string> url;
        std::optional<int> directorID;
        std::optional<Director> director;

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

        void printMovie() {
            cout << "Título: " << this->title << " (" << this->releaseYear << ")" << endl;
            cout << "Diretor: " << this->director->name << endl;
        }


};

FILE *gerarArquivoOrganizado();

std::vector<Movie> parseMovies(FILE *moviesFile);

std::vector<Director> parseDirectors(FILE *directors, std::vector<Movie> movies);

int main() {
    
    std::vector<Movie> movies;
    std::vector<Director> directors;
    FILE *moviesFile, *directorsFile;

    moviesFile = fopen("arqMovies.txt", "r");

    if (moviesFile == NULL) {
        cout << "Erro ao abrir o arquivo dos filmes." << endl;
        exit(0);
    } else {
        movies = parseMovies(moviesFile);
        fclose(moviesFile);
        for (int i=0; i<movies.size(); i++) {
            movies.at(i).printMovie();
        }
    }

    // directorsFile = fopen("directors.txt", "r");

    // if (directorsFile == NULL) {
    //     cout << "Erro ao abrir o arquivo dos diretores. << endl";
    //     exit(0);
    // } else {
    //     directors = parseDirectors(directorsFile, movies);
    // }

    return 0;
}

std::vector<Movie> parseMovies(FILE *moviesFile) {

    Movie temp = Movie();
    std::vector<Movie> movies;
    
    while (fscanf(moviesFile, "%d;%s;%d;%s;%d", &temp.movieID, temp.title, temp.releaseYear, temp.url, temp.directorID) > 0) {

        // Se não tiver as informações básicas, colocar o filme como inválido para manter a indexação original
        if (temp.movieID == 0 || temp.title == "" || temp.releaseYear == 0) { 
            temp.title = INVALID_STRING;
            temp.releaseYear = INVALID_NUMBER;
            temp.url = INVALID_STRING;
            temp.directorID = INVALID_NUMBER;
            movies.push_back(temp);
            temp.reset();
        }
        movies.push_back(temp);
        temp.reset();
        
    }

    return movies;

}

std::vector<Director> parseDirectors(FILE *directorsFile, std::vector<Movie> movies) {

    Director temp = Director();
    std::vector<Director> directors;
    int movieIndex;
    Movie directedMovie = Movie();

    while (fscanf(directorsFile, "%d,%s", directedMovie, temp.name) > 0) {

        directedMovie = movies.at(movieIndex);


        temp.directorID = directedMovie.directorID.value_or(INVALID_NUMBER);
        temp.moviesDirected.push_back(directedMovie);

    }

    return directors;
}
