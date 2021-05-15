"""
    Modulo das classes que vamos usar no projeto.
"""

PACIENTE_CODIGO_INDEX = 0
IDADE_INDEX = 1
DATA_NASCIMENTO_INDEX = 2
SEXO_INDEX = 3
GRUPO_AT_COD_INDEX = 4
GRUPO_AT_NOME_INDEX = 5
CATEGORIA_COD_INDEX = 6
CATEGORIA_NOME_INDEX = 7
FABRICANTE_INDEX = 8 


class RegistryModel:
    """
        Classe que vai representar o objeto 
        da entrada do CSV
    """
    def __init__(self, 
                 paciente_codigo,
                 idade, 
                 data_nascimento, 
                 sexo_biologico, 
                 grupo_atendimento_codigo, 
                 grupo_atendimento_nome,
                 categoria_codigo, 
                 categoria_nome, 
                 fabricante_vacina, 
                 vacina_dose, 
                 vacina_codigo, 
                 vacina_nome):
        self.paciente_codigo = paciente_codigo
        self.idade = idade
        self.data_nascimento = data_nascimento
        self.sexo_biologico = sexo_biologico
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

    @staticmethod
    def factory(paciente_codigo,
                idade, 
                data_nascimento, 
                sexo_biologico, 
                grupo_atendimento_codigo, 
                grupo_atendimento_nome,
                categoria_codigo, 
                categoria_nome, 
                fabricante_vacina, 
                vacina_dose, 
                vacina_codigo, 
                vacina_nome):
        """
            Método responsável por construir um objeto da instância RegistryModel
            
            Args: 
                paciente_codigo: código do paciente do registro,
                idade: idade do paciente do registro, 
                data_nascimento: data de nascimento do paciente do registro, 
                sexo_biologico: sexo do paciente do registro, 
                grupo_atendimento_codigo: código do grupo de vacinação que o paciente pertence, 
                grupo_atendimento_nome: descrição do grupo de vacinação que o paciente pertence,
                categoria_codigo: código da categoria que o paciente pertence, 
                categoria_nome: descrição da categoria que o paciente pertence, 
                fabricante_vacina: fabricante da vacina, 
                vacina_dose: dose da vacina administrada, 
                vacina_codigo: código da vacina, 
                vacina_nome: descrição da vacina
            
            Returns:
                (obj): Objecto RegistryModel

        """

        return RegistryModel(paciente_codigo = paciente_codigo,
                        idade = idade,
                        data_nascimento = data_nascimento,
                        sexo_biologico = sexo_biologico,
                        grupo_atendimento_codigo = grupo_atendimento_codigo,
                        grupo_atendimento_nome = grupo_atendimento_nome,
                        categoria_codigo = categoria_codigo,
                        categoria_nome = categoria_nome,
                        fabricante_vacina = fabricante_vacina,
                        vacina_dose = vacina_dose,
                        vacina_codigo = vacina_codigo,
                        vacina_nome = vacina_nome)


class RegistryModelAdapter:
    """
        Classe responsável pela adaptação de uma entrada de CSV
        para um dicionário da classe RegistryModel
    """

    def __init__(self, registry_str):
        pass
    @property
    def registry_json_str(self):
        pass
    @registry_json_str.setter
    def registry_json_str(self, value):
        pass
    def adapt_to_registry_model(self):
        pass