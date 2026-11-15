#include "decoder.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Helper function to print an error message and return 0. */
static int reject(void)
{
    puts("[INVALID]");
    return 0;
}

/* Helper function to validate that a string contains only uppercase letters and underscores, and its length. */
static int is_valid_text(const char *s, size_t max_len)
{
    if (s == NULL || strlen(s) > max_len || strlen(s) == 0) {
        return 0;
    }
    for (size_t i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c != '_' && (c < 'A' || c > 'Z')) {
            return 0;
        }
    }
    return 1;
}

/* Helper function to parse a string as a number and validate its value. */
static int parse_number(const char *s, int *out)
{
    if (s[0] == '\0') {
        return 0;
    }
    int value = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c < '0' || c > '9') {
            return 0;   
        }
        value = value * 10 + (c - '0');
        if (value > 1000) {
            return 0;
        }
    }
    *out = value;
    return 1;
}

int decode_message(const char *encoded, int shift, char *decoded, size_t capacity)
{
    if (encoded == NULL || decoded == NULL) {
        return 0;
    }
    if (shift < 0 || shift > 25) {
        return 0;
    }
    size_t len = strlen(encoded);
    if (len + 1 > capacity) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        decoded[i] = encoded[len-1-i];
    }
    decoded[len] = '\0';
    for (size_t i = 0; i < len; i++) {
        char c = decoded[i];
        if (c =='_') {
            continue;           
        }
        if (c < 'A' || c > 'Z') {
            return 0;            
        }
        decoded[i] = ((c - 'A' - shift + 26) % 26) + 'A';
    }
    return 1;
}

int calculate_checksum(const char *message)
{
    if (message == NULL) {
        return -1;
    }
    int checksum = 0;
    for (size_t i = 0; message[i] != '\0'; i++) {
        char c = message[i];
        checksum += c;
    }
    return checksum % 97;
}

int process_transmission(const char *line)
{
    /* TODO: parse, validate, decode, verify, and print one record. */

    /* Validate the input line. */
    if (line == NULL || strlen(line) == 0 || strlen(line) > 200) {
        return reject();
    }
    int counter = 0;
    for (size_t i = 0; line[i] != '\0'; i++) {
        if(line[i]=='|') {
            counter++;
        }
    }
    if (counter != 3) {
        return reject();
    }
    /* Split the line into its components. */
    char copy[201];
    strcpy(copy, line);
    char *sender = strtok(copy, "|");
    char *key = strtok(NULL, "|");
    char *checksum = strtok(NULL, "|");
    char *encrypted_message = strtok(NULL, "|");

    /* Validate the components. */
    if (sender == NULL || key == NULL || checksum == NULL || encrypted_message == NULL) {
        return reject();
    }
    if(!is_valid_text(sender, 24) || !is_valid_text(encrypted_message, 160)) {
        return reject();
    }
    int key_value;
    if (!parse_number(key, &key_value) || key_value < 0 || key_value > 25) {
        return reject();
    }
    if (strlen(checksum) != 2 || !isdigit(checksum[0]) || !isdigit(checksum[1])) {
        return reject();
    } 
    int checksum_value;
    if(!parse_number(checksum, &checksum_value) || checksum_value < 0 || checksum_value > 96) {
        return reject();
    }

    /* Decode the message. */
    char decoded_message[161];
    if (!decode_message(encrypted_message, key_value, decoded_message, sizeof(decoded_message))) {
        return reject();
    }
    if (calculate_checksum(decoded_message) != checksum_value) {
        return reject();
    }
    printf("[%s] %s\n", sender, decoded_message);
    return 1;
}
