import base64

from Crypto.Util.Padding import pad, unpad      #importato per fare la funzione di padding
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes


print(AES.key_size) #(16, 24, 32)
print(AES.key_size[2])  #dato che vogliamo generare 256 bit come lunghezza della chiave scegliamo 32

key = get_random_bytes(AES.key_size[2])
iv = get_random_bytes(AES.block_size)
print(len(key))

data = b'These data are to be encrypted!!'   # PLAINTEXT - 32 bytes, that is a multiple of block size,
                                             #             so no need for padding
print("len del plaintext:" , len(data))
cipher_enc = AES.new(key, AES.MODE_CBC, iv)
ct = cipher_enc.encrypt(data)   #dopo che l'oggetto cypher_enc è pronto, viene generato il cyphertext ct
print("cyphertext: ", ct)
print(len(ct))

cipher_dec = AES.new(key, AES.MODE_CBC, iv) #decryption
pt = cipher_dec.decrypt(ct)
print("plaintext: ", pt)
print("\n")




# encryption + PADDING
data = b'Unaligned data to cipher'   # 24 bytes, will need padding
cipher_enc = AES.new(key, AES.MODE_CBC, iv) #CREAZIONE DI UN NUOVO CYPHER OBJECT
padded_data = pad(data,AES.block_size)  #FUNZIONE DI PADDING
print("padded data:", padded_data)
ct = cipher_enc.encrypt(padded_data)

#print Base64
print(base64.b64encode(ct).decode())


#decryption + unpadding
cipher_dec = AES.new(key, AES.MODE_CBC, iv)
decrypted_data = cipher_dec.decrypt(ct)
print(decrypted_data)
pt = unpad(decrypted_data, AES.block_size)  #UNPADDING
assert(data == pt)

