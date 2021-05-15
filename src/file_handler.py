import csv
import shelve


def _create_shelf():
    """
    Abre um arquivo binário .shelf que vai conter todos os registros
    presentes no arquivo .csv

    Essa função é chamada exclusivamente quando não é encontrado
    um arquivo .shelf em verify_shelf() e não deve ser chamada diretamente.
    """

    with shelve.open("RS_Binario.shelf") as shelf_rs:
        with open("RS_Mini.csv", newline='') as text_file:
            reader = csv.reader(text_file, delimiter=',')
            next(reader)  # Pular o header

            for row in reader:
                # TODO (Mudar pra RegistryModel)
                patient = Registro(row[0], int(row[1]), row[2],
                                   row[3], int(row[4]), row[5],
                                   int(row[6]), row[7], row[8],
                                   row[9], int(row[10]), row[11])  # Deve ter um jeito melhor de fazer isso
                shelf_rs[patient.paciente_codigo] = patient

    text_file.close()
    shelf_rs.close()


def _read_from_shelf():
    """
    Lê o arquivo .shelf e por enquanto mostra todos os registros,
    mas no futuro deve ser a função usada para a leitura exaustiva do arquivo.

    Essa função é chamada exclusivamente quando é encontrado um arquivo .shelf
    em verify_shelf() e não deve ser chamada diretamente.
    """
    with shelve.open("RS_Binario.shelf") as shelf_rs:
        for codigo in shelf_rs:
            print(codigo, shelf_rs[codigo])


def verify_shelf():
    """
    Verifica a existência do arquivo binário .shelf. Caso ele já exista,
    invoca a leitura exaustiva do arquivo; caso não exista, invoca a
    criação de um.

    Qualquer leitura do arquivo binário deve ser feita através dessa função
    para garantir a existência dos arquivos.
    """
    try:
        with shelve.open("RS_Binario.shelf"):
            _read_from_shelf()
    except IOError:
        _create_shelf()
        _read_from_shelf()


def main():

    verify_shelf()

    """
    
    if !(Ja tem os arquivos de indices) {
        gerar os arquivos de índice para código do paciente e fabricante da vacina
    }
    
    gerar as estruturas de índice em memória (hash pra data de aplicação da vacina e
                                              árvore binaria pra cidade da aplicação da vacina)
    consultar dados a partir das estruturas de índices, tanto em arquivos quanto em memória
    
    """


if __name__ == '__main__':
    main()
