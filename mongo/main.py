from pymongo import MongoClient

def main():
    while True:
        menu()
        
        awnser = int(input("Escolha uma opção:"))
        if awnser < 0 or awnser > 5:
            continue
        
        if awnser == 1:
            search_by_code()
            continue
        elif awnser == 2:
            search_by_city()
            continue
        elif awnser == 3:
            search_by_date()
            continue
        elif awnser == 4:
            search_by_vaccine()
            continue
        elif awnser == 5:
            prove_hypothesis()
            continue
        elif awnser == 0:
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
    pass

def search_by_city():
    print("Insira o nome da cidade (Sem acentos):")
    
def search_by_date():
    pass

def search_by_vaccine():
    pass

def prove_hypothesis():
    pass