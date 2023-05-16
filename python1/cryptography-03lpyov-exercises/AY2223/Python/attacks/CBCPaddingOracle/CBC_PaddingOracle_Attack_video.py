import os
os.environ['PWNLIB_NOTERM'] = 'True'
os.environ['PWNLIB_SILENT'] = 'True'
from pwn import *

from myconfig import HOST, PORT
from mydata import cbc_oracle_iv as iv
from mydata import cbc_oracle_ciphertext as ciphertext

from Crypto.Cipher import AES

if __name__ == '__main__':
    # server = remote(HOST,PORT)            #PROVARE QUESTE RIGHE COMMENTATE
    # server.send(iv)                       #SONO DELLE PROVE PER VEDERE IL SERVER QUALE RISPOSTA FORNISCE PER DETERMINATI INPUT CHE RICEVE
    # server.send(ciphertext)
    # response = server. recv(1024)
    # print(response)
    # server.close()

    # server = remote(HOST,PORT)
    # server.send(iv)
    #
    # edt = bytearray(ciphertext)
    # edt[-1] = 0
    #
    # server.send(edt)
    # response = server. recv(1024)
    # print(response)
    # server.close()

#---------------------------------------------
    print(len(ciphertext)//AES.block_size)  #IN QUANTI BLOCCHI AES SI PUÒ DIVIDERE IL CIPHERTEXT
    N = len(ciphertext)//AES.block_size
    initial_part = ciphertext[:(N-2)*AES.block_size]    #DALL'INIZIO DEL BLOCCO AL BLOCCO N-2 DEL CIPHERTECT
    block_to_modify = bytearray(ciphertext[(N-2)*AES.block_size:(N-1)*AES.block_size])  #PUNULTIMO BLOCCO
    last_block = ciphertext[(N-1)*AES.block_size:]  #ULTIMO BLOCCO


    byte_index = AES.block_size - 1 #INDICE DEL BLOCCO DA MODIFICARE
    c_15 = block_to_modify[byte_index]

    for c_prime_15 in range(256):
        block_to_modify[byte_index] = c_prime_15
        to_send = initial_part + block_to_modify + last_block

        server = remote(HOST, PORT)
        server.send(iv)
        server.send(to_send)
        response = server.recv(1024)
        # print(response)
        server.close()

        if response == b'OK':
            print("c_prime_15="+str(c_prime_15))    #VALORE CHE CI HA FATTO OTTENERE OK DALL'ORACLE
            p_prime_15 = c_prime_15 ^ 1
            p_15 = p_prime_15 ^ c_15
            print("p_prime_15=" + str(p_prime_15))
            print("p_15=" + str(p_15))

    p_prime_15 = 191    #UNO DEI VALORI CHE CI HA FATTO OTTENERE OK DALL'ORACLE
    print("---------------")

    c_second_15 = p_prime_15 ^ 2
    block_to_modify[byte_index] = c_second_15

    byte_index -=1
    c_14 = block_to_modify[byte_index]

    for c_prime_14 in range(256):
        block_to_modify[byte_index] = c_prime_14
        to_send = initial_part + block_to_modify + last_block

        server = remote(HOST, PORT)
        server.send(iv)
        server.send(to_send)
        response = server.recv(1024)
        server.close()

        if response == b'OK':
            print("c_prime_14="+str(c_prime_14))
            p_prime_14 = c_prime_14 ^ 2
            p_14 = p_prime_14 ^ c_14
            print("p_prime_14=" + str(p_prime_14))
            print("p_14=" + str(p_14))

    print("---------------")
