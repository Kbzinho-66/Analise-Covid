import random
import re
import time
from database_handler import DatabaseHandler
from cryptography.fernet import Fernet

def main():
    while True:
        answer = menu()

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
    print("======================================")
    print("Escolha uma opção a seguir:")
    print("1) Procurar um registro pelo seu código.")
    print("2) Procurar todos os registros de uma cidade.")
    print("3) Procurar todos os registros de uma data.")
    print("4) Procurar todos os pacientes vacinados com uma vacina.")
    print("5) Provar hipóteses.")
    print("0) Sair.")

    while True:
        answer = int(input("Escolha uma opção:"))
        if answer < 0 or answer > 5:
            continue
        else:
            return answer


def inner_menu():
    print("Pesquisa secundária:")
    print("1) Procurar um código nesses registros.")
    print("2) Mostrar registros aleatórios.")
    print("0) Voltar para o menu principal.")

    while True:
        answer = int(input("Escolha uma opção:"))
        if answer < 0 or answer > 2:
            continue
        else:
            return answer


def search_by_code():
    code = int(input("Insira o código a ser pesquisado: "))
    query = {'Paciente_Codigo': code}
    patient = registries.find_one(query)

    print("======================================\n")
    print_patient(patient)
    time.sleep(2)
    pass


def search_by_city():
    city = input("Insira o nome da cidade (Sem acentos):")
    query = {'CidadeAplicacaoVacina': city.upper()}
    patients = handler.search_query(query)

    secondary_search(patients, query)


def search_by_date():
    date = input("Insira a data a ser pesquisada no formato YYYY-MM-DD: ")
    date_pattern = r"([12]\d{3}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]))"
    query = {
        '$or': [
            {'Data_Aplicacao': f"{date}T00:00:00.000Z"},
            {'Data_Aplicacao': f"{date}T03:00:00.000Z"}
        ]
    }

    if re.match(date_pattern, date):
        patients = handler.search_query(query)

        secondary_search(patients, query)

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
    query = {'Vacina_Nome': vaccines[index]}
    patients = handler.search_query(query)

    secondary_search(patients, query)


def secondary_search(patients, query):

    print()
    while True:
        answer = inner_menu()

        if answer == 1:
            code = int(input("Insira o código: "))
            query['Paciente_Codigo'] = code
            patient = handler.search_element(query)

            if patient is not None:
                print("======================================")
                print_patient(patient)
            else:
                print("Código não encontrado\n")
            continue

        elif answer == 2:
            print("======================================")

            for patient in patients:
                if int(random.random() * 1000) % 256 == 0:
                    print_patient(patient)

            patients.rewind()
            continue

        elif answer == 0:
            break


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
    e_birthday = patient['DataNascimento']
    sex = patient['SexoBiologico']
    city = patient['CidadeAplicacaoVacina']
    category = patient['Categoria_Nome']
    vaccine = patient['Vacina_Nome']

    birthday = decrypt(e_birthday)

    print(f"Código do paciente: {code}")
    print(f"Idade: {age}")
    print(f"Data de nascimento: {birthday}")
    print(f"Sexo Biológico: {sex} \n")
    print(f"Cidade de vacinação: {city}")
    print(f"Categoria de vacinação: {category}")
    print(f"Vacina utilizada: {vaccine}")
    print("======================================")

def encrypt_fields():
    encrypted_collection = handler.get_collection("encrypted_registers")
    results = encrypted_collection.count_documents({})

    if results == 0:
        print('Criptografando...')
        for entry in registries.find():
            new_entry = entry
            new_entry['DataNascimento'] = crypto_key.encrypt(str.encode(new_entry['DataNascimento']))
            encrypted_collection.insert(new_entry)

def decrypt(field):
    return crypto_key.decrypt(field).decode('utf-8')


def generate_key():
    key_collection = handler.get_collection('ey')
    # print(key_collection.find_one())
    key = key_collection.find_one()
    
    if key is None: 
        key = Fernet.generate_key()
        key_collection.insert({
            'Chave': key
        })

    else:
        key = key['Chave']

    return Fernet(key)

if __name__ == '__main__':

    handler = DatabaseHandler.handler_factory("covid", "registers")
    # handler = DatabaseHandler.handler_factory("Covid2", "Registros")
    crypto_key = generate_key()
    encrypt_fields()
    registries = handler.get_collection("encrypted_registers")
    main()
