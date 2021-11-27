# My naive attempt at creating a malware
Purely for research and learning purposes. Should not be used in any circumstances. Originally written in Python, but rewritten in C for better performance.
Uses the Advanced Encryption Standard (AES) for encrypting files. The PyCryptoDome library is used with Python and tiny-AES-c (https://github.com/kokke/tiny-AES-c) with C.

TO-DO:
- Encrypt big files in parts. Current method loads the entire file into memory. This is not a good approach because files can be very large. Instead, I should encrypt files over a certain threshold in blocks.
- Choose between encrypting/decrypting mode with argument when running binary file.
