"""
    Módulo com classes para a leitura dos arquivos
"""

import csv

class ReaderCSV:
    """
    Classe para a leitura de um arquivo .csv
    """
    def __init__(self):
        pass

    def read(self, file_path):
        """
        Realiza a leitura do arquivo .csv

        Args:
            file_path(str): caminho para o arquivo que será lido

        Returns:
            (obj Reader): retorna um objeto Reader do csv lido
        """
        print(f'Iniciando a leitura do arquivo: {file_path}')

        file_obj = open(file_path)

        if not file_obj:
            print('Erro ao abrir arquivo')
            return None
        
        raw = file_obj.read()

        raw = raw.split('\n')

        reader = csv.reader(raw)

        for row in reader:
            print(row)

if __name__ == "__main__":
    readerCSV = ReaderCSV()
    readerCSV.read("RS_Mini.csv")