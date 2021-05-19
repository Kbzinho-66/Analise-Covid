#include <string>
#include <iostream>

class Register
{
private:
    char patient_sex;
    int patient_code;
    int patient_age;
    int category_code;
    std::string patient_b_day;
    std::string vaccination_city;
    std::string category_name;

public:
    Register(int patient_code,
             int patient_age,
             std::string patient_b_day,
             std::string vaccination_city,
             std::string category_name);
    void printRegister();
    ~Register();
};

Register::Register(int patient_code,
                   int patient_age,
                   std::string patient_b_day,
                   std::string vaccination_city,
                   std::string category_name)
{
    // this->patient_b_day = new std::string(13, '\0');
    // this->vaccination_city = new std::string(32, '\0');
    // this->category_name = new std::string(100, '\0');
    this->patient_code = patient_code;
    this->patient_age = patient_age;
    this->patient_b_day = patient_b_day;
    this->vaccination_city = vaccination_city;
    this->category_name = category_name;
}

void Register::printRegister()
{
    std::cout << "Codigo " << this->patient_code << std::endl;
    std::cout << "Idade " << this->patient_age << std::endl;
    std::cout << "Dt. Nascimento " << this->patient_b_day << std::endl;
    std::cout << "Cidade " << this->vaccination_city << std::endl;
    std::cout << "Categoria " << this->category_name << std::endl;

    std::cout << std::endl;
}

Register::~Register()
{
}
