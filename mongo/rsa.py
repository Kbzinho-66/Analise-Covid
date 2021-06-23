"""

***Explicando o algorítmo RSA

RSA(Rivest–Shamir–Adleman) é um algorítmo assimétrico de criptografia, usado para criptografar
e descriptografar mensagens a partir de chaves públicas e privadas. O algorítmo se
baseia no fato de que achar os fatores de um número muito grande é difícil, principalmente
se os dois fatores forem números primos. Seu uso mais notório é para gereção de chaves SSH.

Ele segue alguns princípios da criptografia, como funções 'one-way', ou seja, gerar um
resultado é computacionalmente fácil, mas reverter o valor gerado não.

***Gerando as chaves

1.Escolha dois grandes números primos P e Q. Ambos deves ser mantidos em segredo.

2.Calcule N = PQ. N será o módulo para as chaves públicas e privadas.

3.Calcule o tociente Φ(n) = (P-1)(Q-1).

4.Escolha um inteiro E tal que 1 < E < Φ(n) e E e Φ(n) são co-primos, ou seja, 
o maior denominador comum ente E e Φ(n) é 1. E é usado como expoente da chave pública.

5.Calcule D para satisfar a relação DE = mod(Φ(n)), ou seja, D = E^-1 mod(Φ(n)).

A chave pública é formada por mod(N) expoente E.
A chave privada é formada por N expoente D. 

***Criptografando a mensagem

Alice manda sua chave pública(N, E) para Bob e mantém a sua privada em segredo,
Caso Bob queira mandar uma mensagem para Alice, ele primeiro deve trensformar a mensagem
em um número inteiro M, e depois calcular o texto cifrado C da seguinte maneira:

C = M^E mod(N)

***Descriptografando a mensagem

Quando Alice recebe uma mensagem criptografada C de Bob, ela pode recuperar seu conteúdo usando sua chave
privada(N, D) fazendo o seguinte cálculo:

M = C^D mod(N)

***Fontes

1. https://www.di-mgt.com.au/rsa_alg.html

2. https://simple.wikipedia.org/wiki/RSA_algorithm

"""

import math
import Crypto
from Crypto.Random import get_random_bytes 
from Crypto.Util.number import bytes_to_long, long_to_bytes

def encrypt(msg, public_key):
    split_key = public_key.split('.')

    N = int(split_key[0])
    encrypt = int(split_key[1])

    c = pow(msg, encrypt, N) # equivale a msg^encrypt mod N
    return c

def decrypt(msg, private_key):
    split_key = private_key.split('.')

    N = int(split_key[0])
    decrypt = int(split_key[1])

    return pow(msg, decrypt, N) # equivale a msg^decrypt mod N

def generate_keys():

    print("Gerando chaves...")

    size = 2048

    # gera dois números primos de 128 dígitos 
    P = Crypto.Util.number.getPrime(size, randfunc=get_random_bytes)
    Q = Crypto.Util.number.getPrime(size, randfunc=get_random_bytes)

    N = Q*P

    phi = calculate_totient(P, Q)

    encrypt = generate_encrypt(phi)
    decrypt = generate_decrypt(phi, encrypt)

    public_key  = str(N) + "." + str(encrypt)
    private_key = str(N) + "." + str(decrypt)

    return (public_key, private_key)

def calculate_totient(P, Q):
    return (P-1)*(Q-1)

def generate_encrypt(phi):
    e = 2

    while(e < phi):
        if math.gcd(e, phi) == 1:
            return e
        else:
            e += 1

def generate_decrypt(phi, e):
    return pow(e, -1, phi) # E^-1 mod(Φ(n))

if __name__ == '__main__':

    
    print("============================================")
    print(" ALGORÍTMO DE CRIPTOGRAFIA ASSIMÉTRICO RSA  ")
    print("============================================")

    print('Digite uma mensagem para criptografar: ')
    print('Obs: Palavras com acentos não ficam formatadas')
    # msg = input('')
    msg = 'The RSA system is probably the most widely used public-key cryptosystem in the world. It is certainly the best known. It provides both digital signatures and public-key encryption, which makes it a very versatile tool, and it is based on the difﬁculty of factoring large numbers, a problem that has fascinated many people over the last few millennia and has been studied extensively.'
    b_msg = bytes_to_long(msg.encode('utf-8'))

    keys = generate_keys()

    print("Chave pública: " + str(keys[0]))

    encrypted = encrypt(b_msg, keys[0])
    decrypted = decrypt(encrypted, keys[1])

    print("Palavra criptografada:")
    print(long_to_bytes(encrypted))

    print("Palavra descriptografada:")
    print(long_to_bytes(decrypted))