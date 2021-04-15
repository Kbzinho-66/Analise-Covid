// Gabriel Vieira Alles - Algoritmos e Estruturas de Dados II

#include <iostream>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>

#define INVALID_STRING "unknown"
#define INVALID_NUMBER -1

using namespace std;

//OBSERVAÇÃO: REMOVER O HEADER DO ARQUIVO arqMovies.txt

class Movie {

    public:
        int movieID;
        string title;
        int releaseYear;
        optional<string> url;
        optional<int> directorID;

        Movie() {
        }

        Movie(int movieID,
            string title,
            int releaseYear
        ){
            this->movieID = movieID;
            this->title = title;
            this->releaseYear = releaseYear;
        }

        Movie(int movieID,
            string title,
            int releaseYear,
            optional<string> url,
            optional<int> directorID
        ){
            this->movieID = movieID;
            this->title = title;
            this->releaseYear = releaseYear;
            this->url = url;
            this->directorID = directorID;
        }      

        void reset() {
            movieID = 0;
            title = "";
            releaseYear = 0;
            url = "";
            directorID = 0;
        }

};

string StringToUpper(string strToConvert);

void split(const std::string &s, char delim, std::string values[]);

int readID();

void printMovie(optional<Movie> movie, optional<string> director);

optional<Movie> searchMovieID(int id, ifstream &movies);

optional<Movie> searchMovieTitle(string name, ifstream &movies);

optional<string> searchDirectorMovieID(int id, ifstream &directors);

vector<int> searchDirectorName(string name, ifstream &directors);

void printMovieVector(vector<int> directedMovies, ifstream &moviesFile);

void printMoviesFile(ifstream &moviesFile, ifstream &directorsFile);

void printDirectorsFile(ifstream &file);

int main() {

    ifstream moviesFile, directorsFile;

    moviesFile.open("arqMovies.txt", ifstream::in);

    if (!moviesFile) {
        cout << "Erro ao abrir o arquivo dos filmes." << endl;
        exit(0);
    }

    directorsFile.open("directors.txt", ifstream::in);

    if (!directorsFile) {
        cout << "Erro ao abrir o arquivo dos diretores. << endl";
        exit(0);
    }

    int resposta;
    cout << "----- Catálogo de Filmes -----" << endl;

    do {

        cout << endl << "Escolha uma opção: " << endl;
        cout << "1: Procurar um filme por ID" << endl;
        cout << "2: Procurar um filme por nome" << endl;
        cout << "3: Procurar todos os filmes de um diretor" << endl;
        cout << "4: Listar todos os filmes com um diretor cadastrado" << endl;
        cout << "0: Sair." << endl;

        // Laço pra leitura de uma resposta válida
        while ((cout << "Escolha uma opção:" << endl ) 
                && !(cin >> resposta) || resposta < 0 || resposta > 5) {
            cout << "Opção Inválida!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (resposta) {

            case 1: { // Procurar um filme por ID

                int id = readID();

                cout << "Procurando..." << endl;

                optional<Movie> movie = searchMovieID(id, moviesFile);
                optional<string> director = searchDirectorMovieID(id, directorsFile);

                printMovie(movie, director);

                break;
            }

            case 2: { // Procurar um filme por nome

                cout << "Insira o nome do filme (em inglês):" << endl;
                string title;
                getline(cin >> ws, title);

                optional<Movie> movie = searchMovieTitle(StringToUpper(title), moviesFile);

                optional<string> director = searchDirectorMovieID(movie->movieID, directorsFile);

                printMovie(movie, director);

                break;
            }
                
            case 3: { // Procurar todos os filmes de um diretor

                cout << "Insira o nome do diretor:" << endl;
                string name;
                getline(cin >> ws, name);
                vector<int> directedMovies = searchDirectorName(StringToUpper(name), directorsFile);

                if (directedMovies.size() > 0) {
                    cout << "Diretor: " << name << endl;
                    printMovieVector(directedMovies, moviesFile);
                } else {
                    cout << "Não foi encontrado nenhum filme dirigido por esse diretor" << endl;
                }
                
                break;
            }

            case 4: {
                printMoviesFile(moviesFile, directorsFile);
                break;
            }

            default:
                break;
        }

    } while (resposta);

    moviesFile.close();
    directorsFile.close();

    return 0;
}

string StringToUpper(string strToConvert) {

    transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

    return strToConvert;
}

void split(const std::string &s, char delim, std::string values[]) {

    /* Divide a string s através dos values, separando pelo delim */

    std::stringstream ss(s);
    std::string item;

    int i = 0;

    while (getline(ss, item, delim)) {
            values[i++] = item;
    }
}

int readID() {

    /* Leitura de um id válido */

    int id;
    while ((cout << "Digite a ID do filme a ser consultado:" << endl ) 
        && !(cin >> id)) {

        cout << "A ID precisa ser um Inteiro!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    }

    return id;
}

void printMovie(optional<Movie> movie, optional<string> director) {

    /* Se ambos filme e diretor foram encontrados, mostra os dois */

    if (movie && director) {
        cout << "Título: " << movie->title << " (" << movie->releaseYear << ")" << endl;
        cout << "Diretor: " << director.value() << endl;
        
    } else if (!movie) {
        cout << "Erro ao buscar esse filme." << endl;
    } else if (!director) {
        cout << "Não foi catalogado um diretor para esse filme." << endl;
    }
}

optional<Movie> searchMovieID(int id, ifstream &movies) { 

    /* Procura um filme por sua id */

    Movie temp = Movie();

    string buffer;
    string values[5];

    while (getline(movies, buffer)) {
        split(buffer, ';', values);

        temp.movieID = stoi(values[0], nullptr, 10);
        temp.title = values[1];
        if (!values[2].empty()) temp.releaseYear = stoi(values[2], nullptr, 10);
        temp.url = values[3];
        if (!values[4].empty()) temp.directorID = stoi(values[4], nullptr, 10);

        if (temp.movieID == id) {
            break;
        } else {
            temp.reset();
        }
    }

    movies.clear();
    movies.seekg(0);

    if (temp.movieID == id && temp.title != INVALID_STRING && temp.releaseYear != INVALID_NUMBER) {
        return temp;
    } else {
        return nullopt;
    }
}

optional<Movie> searchMovieTitle(string title, ifstream &movies) {

    /* Procura um filme pelo título */

    optional<Movie> temp = Movie();

    string buffer;
    string values[5];

    while (getline(movies, buffer)) {
        split(buffer, ';', values);

        temp->movieID = stoi(values[0], nullptr, 10);
        temp->title = values[1];
        if (!values[2].empty()) temp->releaseYear = stoi(values[2], nullptr, 10);
        temp->url = values[3];
        if (!values[4].empty()) temp->directorID = stoi(values[4], nullptr, 10);

        if (StringToUpper(temp->title) == title) {
            break;
        } else {
            temp.reset();
        }
    }

    movies.clear();
    movies.seekg(0);

    if (StringToUpper(temp->title) == title && temp->movieID != INVALID_NUMBER && temp->releaseYear != INVALID_NUMBER) {
        return Movie(temp->movieID, temp->title, temp->releaseYear);
    } else {
        return nullopt;
    }
}

optional<string> searchDirectorMovieID(int id, ifstream &directors) {

    /* Procura um diretor pelo id do filme que dirigiu */

    int movieID;
    optional<string> name;

    string values[2];
    string buffer;

    while (getline(directors, buffer)) {
        split(buffer, ',', values);

        movieID = stoi(values[0], nullptr, 10);
        name = values[1];

        if (movieID == id) {
            break;
        } else {
            movieID = INVALID_NUMBER;
            name = INVALID_STRING;
        }
    }

    directors.clear();
    directors.seekg(0);

    if (movieID == id && name != INVALID_STRING) {
        return name;
    } else {
        return nullopt;
    }
}

vector<int> searchDirectorName(string name, ifstream &directors) {

    /* Procura um diretor por seu nome,
    retorna um vetor com o movieID de todos os filmes que dirigiu */

    vector<int> directedMovies;

    int movieID;
    optional<string> tempName;

    string values[2];
    string buffer;

    while (getline(directors, buffer)) {
        split(buffer, ',', values);

        movieID = stoi(values[0], nullptr, 10);
        tempName = values[1];
        tempName->pop_back();

        if (StringToUpper(tempName.value()) == name) {            
            directedMovies.push_back(movieID);
        } else {
            movieID = INVALID_NUMBER;
            tempName = INVALID_STRING;
        }
    }

    directors.clear();
    directors.seekg(0);

    return directedMovies;
}

void printMovieVector(vector<int> movies, ifstream &moviesFile) {

    optional<Movie> temp;

    for (int i=0; i<movies.size(); i++) {
        temp = searchMovieID(movies[i], moviesFile);
        cout << "Título: " << temp->title << " (" << temp->releaseYear << ")" << endl;
    }
}

void printMoviesFile(ifstream &moviesFile, ifstream &directorsFile) {

    Movie tempMovie = Movie();
    optional<string> tempDirector;

    string buffer;
    string values[5];

    while (getline(moviesFile, buffer)) {
        split(buffer, ';', values);

        tempMovie.movieID = stoi(values[0], nullptr, 10);
        tempMovie.title = values[1];
        if (!values[2].empty()) tempMovie.releaseYear = stoi(values[2], nullptr, 10);
        tempMovie.url = values[3];
        if (!values[4].empty()) tempMovie.directorID = stoi(values[4], nullptr, 10);

        tempDirector = searchDirectorMovieID(tempMovie.movieID, directorsFile);
        if (tempDirector) {
            cout << "--------------------------------" << endl;
            printMovie(tempMovie, tempDirector);
        }

        // cout << id << " " << title << " (" << year << ")" << endl;
    }
    
}

/* Função usada somente para teste de parsing
    void printDirectorsFile(ifstream &file) {

        int movieId;
        string name;
        string values[2];
        string buffer;

        while (getline(file, buffer)) {
            split(buffer, ',', values);

            movieId = stoi(values[0], nullptr, 10);
            name = values[1];
            name.pop_back();

            cout << movieId << " " << name << endl;
        }
    }
*/