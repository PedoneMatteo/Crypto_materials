from Crypto.Hash import SHA3_256

#1 METODO
hash_gen = SHA3_256.new()

with open(__file__) as f_input:  #SE NON È PRESENTE LA CLAUSOLA "rb" SI DEVE METTERE .encode() DOPO read() PER TRASFORMARE CARATTERI ASCII IN SEQUENZE DI BYTES
    hash_gen.update(f_input.read().encode())

print(hash_gen.digest())
print(hash_gen.hexdigest())

#2 METODO
hash_gen = SHA3_256.new()
with open(__file__,"rb") as f_input:
    hash_gen.update(f_input.read())

print(hash_gen.digest())
print(hash_gen.hexdigest())
