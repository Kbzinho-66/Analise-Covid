#include <string>
#include <iostream>

class Registry
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
    Registry(int patient_code,
             int patient_age,
             std::string patient_b_day,
             std::string vaccination_city,
             std::string category_name);
    void printRegistry();

    template <class T> void Serialize(T &archive);

    ~Registry();
};

Registry::Registry(int patient_code,
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

void Registry::printRegistry()
{
    std::cout << "Codigo " << patient_code << std::endl;
    std::cout << "Idade " << patient_age << std::endl;
    std::cout << "Dt. Nascimento " << patient_b_day << std::endl;
    std::cout << "Cidade " << vaccination_city << std::endl;
    std::cout << "Categoria " << category_name << std::endl;

    std::cout << std::endl;
}

template <class T> void  Registry::Serialize(T &archive)
{
    archive & patient_code & patient_age & patient_b_day;
}

Registry::~Registry()
{
}
