#include "decoder.h"

#include <stdio.h>
#include <string.h>

int decode_message(const char *encoded, int shift, char *decoded, size_t capacity)
{
    //Validation:
    if (encoded == NULL || decoded == NULL) {
        return 0;
    }

    if (shift < 0 || shift > 25) {
        return 0;
    }

    size_t length = strlen(encoded);

    if (capacity <= length) {
        return 0;
    }

    for (size_t i = 0; i < length; i++) {
        char c = encoded[i];

        if (!((c >= 'A' && c <= 'Z') || c == '_')) {
            return 0;
        }
    }

    //Decode:
    for(size_t i = 0 ; i< length; i++){
        char c = encoded[length-1-i];
        c = c + (c - shift +26) %26;
    }

    return 0;
}

int calculate_checksum(const char *message)
{
    /* TODO: sum decoded characters modulo 97. */
    (void)message;
    return -1;
}

int process_transmission(const char *line)
{
    /* TODO: parse, validate, decode, verify, and print one record. */
    (void)line;
    puts("[INVALID]");
    return 0;
}

int decode_advanced(const char *encoded, const int *keys, size_t key_count, char *decoded, size_t capacity)
{
    /* Optional TODO. */
    (void)encoded;
    (void)keys;
    (void)key_count;
    (void)decoded;
    (void)capacity;
    return 0;
}

