from pymongo import MongoClient

client = MongoClient('localhost', 27017)
db = client.Covid2
registries = db.Registros


def main():
    while True:
        menu()

        answer = int(input("Escolha uma opção:"))
        if answer < 0 or answer > 5:
            continue

        if answer == 1:
            search_by_code()
            continue
        elif answer == 2:
            search_by_city()
            continue
        elif answer == 3:
            search_by_date()
            continue
        elif answer == 4:
            search_by_vaccine()
            continue
        elif answer == 5:
            prove_hypothesis()
            continue
        elif answer == 0:
            client.close()
            break


def menu():
    print("Escolha uma opção a seguir:")
    print("1) Procurar um registro pelo seu código.")
    print("2) Procurar todos os registros de uma cidade.")
    print("3) Procurar todos os registros de uma data.")
    print("4) Procurar todos os pacientes vacinados com uma vacina.")
    print("5) Provar hipóteses.")
    print("0) Sair.")


def inner_menu():
    print("Escolha uma opção:")
    print("1) Procurar um código nesses registros.")
    print("2) Mostrar alguns dos registros.")
    print("0) Voltar para o menu principal.")


def search_by_code():
    code = int(input("Insira o código a ser pesquisado:"))
    patient = registries.find_one({'Paciente_Codigo': code})

    print("======================================\n")
    print_patient(patient)
    pass


def search_by_city():
    city = input("Insira o nome da cidade (Sem acentos):")
    patients = registries.find({'CidadeAplicacaoVacina': city.upper()})

    # TODO(Implementar o segundo nível de pesquisa)

    print("======================================\n")
    for patient in patients:
        print_patient(patient)


def search_by_date():
    date = input("Insira a data a ser pesquisada no formato YYYY-MM-DD:")
    # TODO(Colocar um regex pra verificar se é uma data válida)

    patients = registries.find(
        {'$or': [
            {'Data_Aplicacao': f"{date}T00:00:00.000Z"},
            {'Data_Aplicacao': f"{date}T03:00:00.000Z"}
        ]}
    )

    # TODO(Implementar o segundo nível de pesquisa)

    print("======================================\n")
    for patient in patients:
        print_patient(patient)
    pass


def search_by_vaccine():
    pass


def prove_hypothesis():
    pass


def print_patient(patient):
    code = patient['Paciente_Codigo']
    age = patient['Idade']
    birthday = patient['DataNascimento']
    sex = patient['SexoBiologico']
    category = patient['Categoria_Nome']
    vaccine = patient['Vacina_Nome']

    print(f"Código do paciente: {code}")
    print(f"Idade: {age}")
    print(f"Data de nascimento: {birthday}")
    print(f"Sexo Biológico: {sex} \n")
    print(f"Categoria de vacinação: {category}")
    print(f"Vacina utilizada: {vaccine}\n")
    print("======================================\n")


if __name__ == '__main__':
    main()
