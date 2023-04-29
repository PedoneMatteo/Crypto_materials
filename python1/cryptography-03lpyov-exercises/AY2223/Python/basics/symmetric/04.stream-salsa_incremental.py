from Crypto.Cipher import Salsa20
from Crypto.Random import get_random_bytes

# these are bytes
key = b'deadbeefdeadbeef'   #key = get_random_bytes(Salsa20.key_size[1])        QUESTE DUE SCRITTURE PRODUCONO ENTRAMBE UNA CHIAVE DA 16 BIT
cipher = Salsa20.new(key)

#incremental encryption with a stream cipher
ciphertext =  cipher.encrypt(b'The first part of the secret message. ')
ciphertext += cipher.encrypt(b'The second part of the message.')
#also the ciphertext is made of bytes

#print the nonce you will have to share
nonce = cipher.nonce
print(nonce)

# check: decryption works
cipher2 = Salsa20.new(key,nonce)
plaintext = cipher2.decrypt(ciphertext)
#print("Decrypted = ", plaintext.decode("utf-8"))

