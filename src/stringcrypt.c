#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define KEY_LENGTH 16
#define IV_LENGTH 16

char *decrypt_data(const char *ciphertext, int ciphertext_len, const char *key, int key_len, const char *iv, int iv_len) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    char *plaintext;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return NULL;
    }

    /* Initialise the decryption operation. */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        return NULL;
    }

    /* Allocate memory for the plaintext */
    if(!(plaintext = (char *)malloc(ciphertext_len))) {
        return NULL;
    }

    /* Decrypt the ciphertext */
    if(1 != EVP_DecryptUpdate(ctx, (unsigned char *)plaintext, &len, (unsigned char *)ciphertext, ciphertext_len)) {
        return NULL;
    }
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at this stage. */
    if(1 != EVP_DecryptFinal_ex(ctx, (unsigned char *)(plaintext + len), &len)) {
        return NULL;
    }
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

int main(int argc, char **argv) {
    const char *ciphertext;
    const char *key = "0123456789abcdef";
    const char *iv = "abcdefghijklmnop";
    char *plaintext;
    int ciphertext_len = 0;

    /* Read the encrypted string from file */
    FILE *file = fopen("encrypted_string.bin", "rb");
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    ciphertext_len = ftell(file);
    fseek(file, 0, SEEK_SET);
    ciphertext = (char *)malloc(ciphertext_len);
    fread((void *)ciphertext, ciphertext_len, 1, file);
    fclose(file);

    /* Decrypt the data */
    plaintext = decrypt_data(ciphertext, ciphertext_len, key, KEY_LENGTH, iv, IV_LENGTH);

    /* Use the decrypted string */
    printf("%s\n", plaintext);

    /* Clean up */
    free((void *)ciphertext);
    free(plaintext);

    return 0;
}
