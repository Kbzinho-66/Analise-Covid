#include <iostream>
#include <cstdio>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

class Movie {

    public:
        int movieID;
        std::string title;
        int releaseYear;
        std::string url;
        int directorID;
        Director director;

        Movie(int movieID,
            std::string title,
            int releaseYear,
            std::string url,
            int directorID
        ){
            this->movieID = movieID;
            this->title = title;
            this->releaseYear = releaseYear;
            this->url = url;
            this->directorID = directorID;
            //TODO Definir um diretor nullable https://www.bfilipek.com/2018/05/using-optional.html
            //Teste
        }

        void printMovie() {
            cout << "Título: " << this->title << " (" << this->releaseYear << ")" << endl;
            cout << "Diretor: " << this->director.name << endl;
        }


};

class Director {

    public:
        int directorID;
        std::string name;
        std::vector<Movie> moviesDirected;

};

int main() {
    
    return 0;
}
