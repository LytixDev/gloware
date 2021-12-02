#!/bin/env bash

FILES_TO_ENCRYPT="dummy_files/*"
ENCRYPTION_KEY="NFTs_are_a_scam"

# argument parsing
# 0 -> encrypt files, 1 -> decrypt files
enc_or_dec=0
case "$1" in
    -e)
        enc_or_dec=0
        ;;
    -d)
        enc_or_dec=1
        ;;
    *)
        echo "Unknown argument <$1>, default to encrypt mode. Try -e (encrypt mode) or -d (decrypt mode)"
esac

# loop over all files and encrypt/decrypt them 
for file in $FILES_TO_ENCRYPT
do
    # avoids encrypting this file
    [ "$file" -eq "$0" ] && continue

    if [ "$enc_or_dec" -eq "0" ]
    then
        # SC2094: 'reading and writing to file in same pipeline', so have to byapss this by creating temp file
        openssl aes-256-cbc -in "$file" -a -k $ENCRYPTION_KEY -e > "$file.enc" && mv "$file.enc" "$file"
    else
        openssl aes-256-cbc -in "$file" -a -k $ENCRYPTION_KEY -d > "$file.enc" && mv "$file.enc" "$file"
    fi
done

