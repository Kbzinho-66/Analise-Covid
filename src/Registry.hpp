#include <string>
#include <iostream>
#include <vector>

/**
 * @brief Classe que representa uma linha no csv, ou um paciente
 */
class Registry 
{

    public:

        int patient_code;
        int patient_age;
        std::string patient_birthday;
        char patient_sex;
        std::string application_city;
        int category_code;
        std::string category_name;
        std::string vaccine_maker;
        std::string vaccine_dose;
        std::string vaccine_name;        

        Registry();

        Registry(int patient_code,
                int patient_age,
                std::string patient_birthday,
                char patient_sex,
                std::string application_city,
                int category_code,
                std::string category_name,
                std::string vaccine_maker,
                std::string vaccine_dose,
                std::string vaccine_name);

        Registry(std::vector<std::string>);

        void printRegistry();

        template <class T> void Serialize(T &archive);



        ~Registry();
};

Registry::Registry(){
    /**
     * @brief Construtor vazio, por enquanto não foi necessário
     */
};

Registry::Registry(int patient_code, int patient_age,
                std::string patient_birthday, char patient_sex,
                std::string application_city, int category_code,
                std::string category_name, std::string vaccine_maker,
                std::string vaccine_dose, std::string vaccine_name) 
{
    /**
     * @brief Construtor de um Registro a partir dos valores
     * brutos 
     */

    this->patient_code      = patient_code;
    this->patient_age       = patient_age;
    this->patient_birthday  = patient_birthday;
    this->patient_sex       = patient_sex;
    this->application_city  = application_city;
    this->category_code     = category_code;
    this->category_name     = category_name;
    this->vaccine_maker     = vaccine_maker;
    this->vaccine_dose      = vaccine_dose;
    this->vaccine_name      = vaccine_name;
}

Registry::Registry(std::vector<std::string> row) 
{
    /**
     * @brief Construtor de um Registro a partir do
     * vetor de strings retornado pela função GetRow()
     */

    patient_code            = stoi(row[0]);
    patient_age             = stoi(row[1]);
    patient_birthday        = row[2];
    patient_sex             = row[3].at(0);
    application_city        = row[4];
    category_code           = stoi(row[5]);
    category_name           = row[6];
    vaccine_maker           = row[7];
    vaccine_dose            = row[8];
    vaccine_name            = row[9];
}

void Registry::printRegistry()
{
    std::cout << "Codigo: " << patient_code << std::endl;
    std::cout << "Idade: " << patient_age << std::endl;
    std::cout << "Dt. Nascimento: " << patient_birthday << std::endl;
    std::cout << "Cidade de Aplicação da Vacina: " << application_city << std::endl;
    std::cout << "Categoria: " << category_name << std::endl;
    std::cout << "Fabricante da Vacina: " << vaccine_maker << std::endl;
    std::cout << "Nome da Vacina: " << vaccine_name << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
}

template <class T> void  Registry::Serialize(T &archive)
{
    /**
     * @brief Método para serializar um Registro, permitindo
     * escrever em um arquivo binário
     */

    archive & patient_code & patient_age & patient_birthday &
        patient_sex & application_city & category_code & 
        category_name & vaccine_maker & vaccine_dose & 
        vaccine_name;
}

Registry::~Registry()
{
}
