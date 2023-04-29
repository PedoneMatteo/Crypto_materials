# This program encrypt the content of the file passed as first argument
# and saves the ciphertext in the file whose name is passed as second argument

from Crypto.Cipher import Salsa20
from Crypto.Random import get_random_bytes
import base64
import sys      #FOR PARAMETER FROM COMMAND LINE

salsa_key = b'deadbeeddeadbeef'

nonce = get_random_bytes(8)
streamcipher = Salsa20.new(salsa_key,nonce)

f_output = open(sys.argv[2],"wb")   #OPEN OUTPUT FILE FROM COMMAND LINE IN MODE 'WRITE BINARY'

ciphertext = b''

with  open(sys.argv[1],"rb") as f_input:    #OPEN A FILE FROM COMMAND LINE IN MODE 'READ BINARY'
    plaintext = f_input.read(1024)  #READ 1024  BYTES AT A TIME
    while plaintext:
        ciphertext += streamcipher.encrypt(plaintext)   #CONCATENATION OF CYPHERTEXT
        f_output.write(ciphertext)  #WRITE ON THE OUTPUT FILE
        plaintext = f_input.read(1024)

print("nonce = "+base64.b64encode(streamcipher.nonce).decode())
