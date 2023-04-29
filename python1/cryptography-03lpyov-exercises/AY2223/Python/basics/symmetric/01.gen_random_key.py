from Crypto.Cipher import AES       #this import is important for line 8
from Crypto.Random import get_random_bytes

if __name__ == '__main__':

    print(get_random_bytes(40))    #generates 40 bytes

    print(get_random_bytes(AES.block_size))

