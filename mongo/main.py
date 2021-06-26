import re
import time
from database_handler import DatabaseHandler


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
            prove_hypotheses()
            continue
        elif answer == 0:
            handler.close()
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
    code = int(input("Insira o código a ser pesquisado: "))
    patient = registries.find_one({'Paciente_Codigo': code})

    print("======================================\n")
    print_patient(patient)
    time.sleep(2)
    pass


def search_by_city():
    city = input("Insira o nome da cidade (Sem acentos):")
    patients = handler.search_query({'CidadeAplicacaoVacina': city.upper()})

    # TODO(Implementar o segundo nível de pesquisa)

    print("======================================\n")
    for patient in patients:
        print_patient(patient)


def search_by_date():
    date = input("Insira a data a ser pesquisada no formato YYYY-MM-DD: ")
    date_pattern = r"/^\d{4}-\d{2}-\d{2}$/"
    if re.match(date_pattern, date):
        patients = handler.search_query(
            {'$or': [
                {'Data_Aplicacao': f"{date}T00:00:00.000Z"},
                {'Data_Aplicacao': f"{date}T03:00:00.000Z"}
            ]}
        )

        # TODO(Implementar o segundo nível de pesquisa)

        print("======================================\n")
        for patient in patients:
            print_patient(patient)

    else:
        print("Data inválida")
        return


def search_by_vaccine():
    vaccines = registries.find().distinct('Vacina_Nome')

    index = 0
    for v in vaccines:
        print(f"{index}) {v}")
        index += 1

    index = int(input("Escolha a vacina a ser pesquisada: "))
    patients = registries.find(
        {'Vacina_Nome': vaccines[index]}
    )
    # TODO (Implementar o segundo nível de pesquisa)
    for patient in patients:
        print_patient(patient)


def prove_hypotheses():
    hypothesis_one()
    hypothesis_two()
    hypothesis_three()


def hypothesis_one():
    print("\nHipótese 1: Coronavac é a vacina mais usada no Brasil")
    vaccines = registries.aggregate(
        [
            {'$group': {'_id': '$Vacina_Nome', 'count': {'$sum': 1}}},
            {'$sort': {'count': -1}}
        ]
    )
    for vaccine in vaccines:
        print(f"Vacina: {vaccine['_id']} - Vacinados: {vaccine['count']}")

    print("De fato, a Coronavac tem mais do que 4 vezes o número de usos da segunda mais usada.")
    time.sleep(5)


def hypothesis_two():
    print("\nHipótese 2: A quantidade de pessoas vacinadas é proporcional à população da cidade")
    cities = registries.aggregate(
        [
            {'$group': {'_id': '$CidadeAplicacaoVacina', 'count': {'$sum': 1}}},
            {'$sort': {'count': -1}},
            {'$limit': 5}
        ]
    )
    for city in cities:
        print(f"Cidade: {city['_id']} - Vacinados: {city['count']}")

    print("Parece que é uma hipótese verdadeira, com duas metrópoles gaúchas em 1º e 2º.")
    time.sleep(5)


def hypothesis_three():
    print("\nHipótese 3: A categoria mais vacinada é a de trabalhadores de saúde")
    categories = registries.aggregate(
        [
            {'$group': {'_id': '$Categoria_Nome', 'count': {'$sum': 1}}},
            {'$sort': {'count': -1}},
            {'$limit': 5}
        ]
    )
    for category in categories:
        print(f"Categoria: {category['_id']} - Vacinados: {category['count']}")

    print("A hipótese é falsa. Os vacinados por sua faixa etária são mais do que 3 vezes maiores que "
          "os trabalhadores de saúde.\n")
    time.sleep(5)


def print_patient(patient):
    code = patient['Paciente_Codigo']
    age = patient['Idade']
    birthday = patient['DataNascimento']
    sex = patient['SexoBiologico']
    city = patient['CidadeAplicacaoVacina']
    category = patient['Categoria_Nome']
    vaccine = patient['Vacina_Nome']

    print(f"Código do paciente: {code}")
    print(f"Idade: {age}")
    print(f"Data de nascimento: {birthday}")
    print(f"Sexo Biológico: {sex} \n")
    print(f"Cidade de vacinação: {city}")
    print(f"Categoria de vacinação: {category}")
    print(f"Vacina utilizada: {vaccine}\n")
    print("======================================\n")


if __name__ == '__main__':

    # só muda pros teus banco de dados e coleção

    # handler = DatabaseHandler.handler_factory("covid", "registers")
    handler = DatabaseHandler.handler_factory("Covid2", "Registros")
    registries = handler.get_collection()

    main()
