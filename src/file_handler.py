import csv
import shelve


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


def create_shelf():
    with shelve.open("RS_Binario") as shelf_rs:
        with open("RS_Mini.csv", newline='') as text_file:
            reader = csv.reader(text_file, delimiter=',')
            next(reader)  # Pular o header

            for row in reader:
                patient = Registro(row[0], int(row[1]), row[2],
                                   row[3], int(row[4]), row[5],
                                   int(row[6]), row[7], row[8],
                                   row[9], int(row[10]), row[11])  # Deve ter um jeito melhor de fazer isso
                shelf_rs[patient.paciente_codigo] = patient

    text_file.close()
    shelf_rs.close()


def read_from_shelf():
    with shelve.open("RS_Binario") as shelf_rs:
        for patient_id in shelf_rs:
            print(patient_id, shelf_rs[patient_id])


def verify_shelf():
    try:
        with shelve.open("RS_Binario"):
            read_from_shelf()
    except IOError:
        create_shelf()
        read_from_shelf()


def main():
    verify_shelf()

    """
    if !(ja tem o arquivo shelve) {
        criar um shelve a partir do csv
    }
    
    if !(Ja tem os arquivos de indices) {
        gerar os arquivos de índice para código do paciente e fabricante da vacina
    }
    
    gerar as estruturas de índice em memória (hash pra data de aplicação da vacina e
                                              árvore binaria pra cidade da aplicação da vacina)
    consultar dados a partir das estruturas de índices
    
    """


if __name__ == '__main__':
    main()
