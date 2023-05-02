#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) {
        handleErrors();
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        handleErrors();
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        handleErrors();
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

void read_file(char *filename, char *contents) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        handleErrors();
    }
    fread(contents, 1, 10000, file);
    fclose(file);
}

void write_file(char *filename, char *contents) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        handleErrors();
    }
    fwrite(contents, 1, strlen(contents), file);
    fclose(file);
}

char* replace_strings(char *c_code, unsigned char *key) {
    int in_string = 0;
    int i, j;
    int string_start, string_end;
    int string_len;
    char *new_code = (char*)malloc(10000 * sizeof(char));
    int new_code_index = 0;
    unsigned char *plaintext, *ciphertext;

    for (i = 0; i < strlen(c_code); i++) {
        if (c_code[i] == '"' && !in_string) {
            in_string = 1;
            string_start = i + 1;
        } else if (c_code[i] == '"' && in_string) {
            in_string = 0;
            string_end = i - 1;
            string_len = string_end - string_start + 1;

            plaintext = (unsigned char*)malloc(string_len * sizeof(unsigned char));
            ciphertext = (unsigned char*)malloc(string_len * sizeof(unsigned char));

            for (j = 0; j < string_len; j++) {
                plaintext[j] = c_


int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int fd_input;
    if ((fd_input = open(input_file, O_RDONLY)) == -1) {
        perror("open input_file");
        return 1;
    }

    struct stat st;
    if (fstat(fd_input, &st) == -1) {
        perror("fstat");
        return 1;
    }

    char *c_code = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd_input, 0);
    if (c_code == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    unsigned char key[16];
    if (RAND_bytes(key, 16) != 1) {
        handleErrors();
    }

    int fd_output;
    if ((fd_output = open(output_file, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("open output_file");
        return 1;
    }

    size_t output_size = st.st_size * 2;
    char *encrypted_c_code = malloc(output_size);
    if (!encrypted_c_code) {
        perror("malloc");
        return 1;
    }

    int in_string = 0;
    size_t string_start = 0;
    size_t string_end = 0;
    size_t output_index = 0;
    for (size_t i = 0; i < st.st_size; i++) {
        if (c_code[i] == '"' && !in_string) {
            in_string = 1;
            string_start = i + 1;
        } else if (c_code[i] == '"' && in_string) {
            in_string = 0;
            string_end = i;

            size_t string_len = string_end - string_start;
            unsigned char *plaintext = (unsigned char *) &c_code[string_start];
            unsigned char ciphertext[string_len];

            EVP_CIPHER_CTX *ctx;
            if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

            if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1) handleErrors();

            int len;
            if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext,
