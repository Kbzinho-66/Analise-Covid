import re
import pymongo
import pprint
from pymongo import MongoClient


class DatabaseHandler(object):

    """
    Classe responsável por realizar comunicação e gerência com
    o banco de dados MongoDB 
    """

    def __init__(self, mongo_client, database_name, collection_name):
        self._client = mongo_client
        self._database_name = database_name
        self._collection_name = collection_name

    @staticmethod
    def handler_factory(database_name=None, collection_name=None, host="localhost", port=27017):
        """
        Factory para criar uma instância do DatabaseHandler

            Params: 
                database_name (str): O nome do banco de dados Mongo para 
                                     ser acessado
                host          (str): Hostname, ou IP, ou socket UNIX para 
                                     o qual vai ser feita a conexão
                port          (int): Porta do host para qual vai ser direcionada
                                     a conexão
            Return: 
                (obj): instância da classe DatabaseHandler

        """
        client = DatabaseHandler.client(host, port)
        return DatabaseHandler(client, database_name, collection_name)

    @staticmethod
    def client(host, port):
        """
        Método para criar uma instância do MongoClient para se
        conectar com o banco de dados

            Params: 
                host  (str): Hostname, ou IP, ou socket UNIX para 
                             o qual vai ser feita a conexão
                port  (int): Porta do host para qual vai ser direcionada
                             a conexão
            Return:
                (obj): instância MongoClient
        """
        return MongoClient(host, port)

    @property
    def database(self):
        """
        Propriedade do banco de dados para o qual o handler conecta

        Return:
            (obj): Database do MongoDB
        """
        try:
            return self._client[self._database_name]
        except TypeError:
            print("MongoClient inválido")

    @property
    def database_name(self):
        """
        Propriedade do nome do banco de dados
        """
        return self._database_name

    @database_name.setter
    def database_name(self, value):
        self._database_name = value

    @property
    def collection_name(self):
        """
        Propriedade do nome do banco de dados
        """
        return self._collection_name

    @collection_name.setter
    def collection_name(self, value):
        self._collection_name = value

    def list_collections(self, collection=None):
        """
        Método que imprime todos os registros de uma coleção do banco de dados

        Params:
            collection (str): coleção do banco de dados que se deseja
                              imprimir os dados
        """
        collection = self.get_collection()
        for document in collection.find():
            pprint.pprint(document)

    def get_collection(self, collection=None):
        """ 
        Método que retorna uma coleção do banco de dados

        Params:
            collection (str): coleção do banco de dados que se deseja
                              retornar
        Return: 
            (obj): coleção do banco de dados
        """
        try:
            if collection is None:
                collection = self.collection_name
            return self.database[collection]
        except pymongo.errors.InvalidName:
            print("nome da coleção não pode ser vazio")

    def get_collection_size(self, collection=None):
        return self.get_collection(collection)

    def close(self):
        self._client.close()

    def search_query(self, query):
        """
        Função para executar uma query expecificada

        Params: 
            query (dict): query para ser executada
        """
        collection = self.get_collection()
        return collection.find(query)

    def search_element(self, query):
        """ 
        Acho que não precisaria dessa função pra pesquisar só um,
        mas teria que ver a diferença do find e find_one
        """

        collection = self.get_collection()
        return collection.find_one(query)

if __name__ == "__main__":
    handler = DatabaseHandler.handler_factory("covid", "registers")
    print(handler.get_collection())
