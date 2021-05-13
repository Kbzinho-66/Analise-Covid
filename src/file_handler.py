import csv
import pickle


class Registro:

    def __init__(self, paciente_codigo,
                 idade, data_nascimento, sexo_biologico, grupo_atendimento_codigo, grupo_atendimento_nome,
                 categoria_codigo, categoria_nome, fabricante_vacina, vacina_dose, vacina_codigo, vacina_nome):
        self.paciente_codigo = paciente_codigo
        self.idade = idade
        self.data_nascimento = data_nascimento
        self.sexo_biologico = sexo_biologico  # Quem sabe um enum
        self.grupo_atendimento_codigo = grupo_atendimento_codigo
        self.grupo_atendimento_nome = grupo_atendimento_nome
        self.categoria_codigo = categoria_codigo
        self.categoria_nome = categoria_nome
        self.fabricante_vacina = fabricante_vacina
        self.vacina_dose = vacina_dose
        self.vacina_codigo = vacina_codigo
        self.vacina_nome = vacina_nome

    def __str__(self):
        return ("""Código Paciente: {}\t Idade: {}\t Data de Nascimento: {}\t Sexo Biológico: {}
                 Nome Grupo: {}\t Nome Categoria: {}
                 Fabricante Vacina: {}\t Dose da Vacina: {}\t Nome Vacina: {}\n """
                .format(self.paciente_codigo, self.idade, self.data_nascimento, self.sexo_biologico,
                        self.grupo_atendimento_nome, self.categoria_nome,
                        self.fabricante_vacina, self.vacina_dose, self.vacina_nome))

    def __iter__(self):
        return self


def write_to_pickle():
    output_file = open("RS.pickle", "wb")
    with open("RS_Mini.csv", newline='') as input_file:
        reader = csv.reader(input_file, delimiter=',')
        next(reader)  # Pular o header

        for row in reader:
            patient = Registro(int(row[0]), int(row[1]), row[2],
                               row[3], int(row[4]), row[5],
                               int(row[6]), row[7], row[8],
                               row[9], int(row[10]), row[11])  # Deve ter um jeito melhor de fazer isso
            pickle.dump(patient, output_file)
        output_file.close()


def read_from_pickle():
    # Por enquanto só mostra todos os registros
    input_file = open("RS.pickle", "rb")

    while True:
        try:
            print(pickle.load(input_file))
        except EOFError:
            break
    input_file.close()


def main():
    write_to_pickle()
    read_from_pickle()


if __name__ == '__main__':
    main()
