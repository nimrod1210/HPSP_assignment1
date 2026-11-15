#include "decoder.h"

#include <stdio.h>
#include <string.h>

int decode_message(const char *encoded, int shift, char *decoded, size_t capacity)
{
    // Validation:
    if (encoded == NULL || decoded == NULL)
    {
        return 0;
    }

    if (shift < 0 || shift > 25)
    {
        return 0;
    }

    size_t length = strlen(encoded);

    if (capacity <= length)
    {
        return 0;
    }

    for (size_t i = 0; i < length; i++)
    {
        char c = encoded[i];

        if (!((c >= 'A' && c <= 'Z') || c == '_'))
        {
            return 0;
        }
    }

    // Decode:
    for (size_t i = 0; i < length; i++)
    {
        char c = encoded[length - 1 - i];
        if (c >= 'A' && c <= 'Z')
        {
            c = 'A' + (c - 'A' - shift + 26) % 26;
        }
        decoded[i] = c;
    }
    decoded[length] = '\0';
    return 1;
}

int calculate_checksum(const char *message)
{
    if (message == NULL)
    {
        return -1;
    }
    size_t length = strlen(message);
    size_t sum = 0;
    for (size_t i = 0; i < length; i++)
    {
        sum += message[i];
    }
    return sum % 97;
}

int process_transmission(const char *line)
{
    if (line == NULL)
    {
        puts("[INVALID]");
        return 0;
    }
    size_t length = strlen(line);

    // validate 3 seperatrs
    int separators = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (line[i] == '|')
        {
            separators++;
        }
    }
    if (separators != 3)
    {
        puts("[INVALID]");
        return 0;
    }

    char copy[length + 1];
    strcpy(copy, line);
    char *sender = strtok(copy, "|");
    char *key_str = strtok(NULL, "|");
    char *checksum_str = strtok(NULL, "|");
    char *encrypted = strtok(NULL, "|");

    // validates all fields are nto null
    if (sender == NULL || key_str == NULL ||
        checksum_str == NULL || encrypted == NULL)
    {
        puts("[INVALID]");
        return 0;
    }

    // validate sender
    if (strlen(sender) < 1 || strlen(sender) > 24)
    {
        puts("[INVALID]");
        return 0;
    }
    for (size_t i = 0; sender[i] != '\0'; i++)
    {
        char c = sender[i];
        if (!((c >= 'A' && c <= 'Z') || c == '_'))
        {
            puts("[INVALID]");
            return 0;
        }
    }

    // validate key
    int key = 0;
    for (size_t i = 0; key_str[i] != '\0'; i++)
    {
        char c = key_str[i];
        if (c < '0' || c > '9')
        {
            puts("[INVALID]");
            return 0;
        }
        key = key * 10 + (c - '0');
        if (key > 25)
        {
            puts("[INVALID]");
            return 0;
        }
    }

    // validate checksum
    if (strlen(checksum_str) != 2)
    {
        puts("[INVALID]");
        return 0;
    }

    for (size_t i = 0; i < 2; i++)
    {
        if (checksum_str[i] < '0' || checksum_str[i] > '9')
        {
            puts("[INVALID]");
            return 0;
        }
    }

    int expected_checksum =(checksum_str[0] - '0') * 10 +(checksum_str[1] - '0');

    if (expected_checksum > 96)
    {
        puts("[INVALID]");
        return 0;
    }

    size_t message_length = strlen(encrypted);

    if (message_length < 1 || message_length > 160)
    {
        puts("[INVALID]");
        return 0;
    }

    for (size_t i = 0; i < message_length; i++)
    {
        char c = encrypted[i];

        if (!((c >= 'A' && c <= 'Z') || c == '_'))
        {
            puts("[INVALID]");
            return 0;
        }
    }

    // decode the message
    char decoded[161];
    if (decode_message(encrypted, key, decoded, sizeof(decoded)) == 0)
    {
        puts("[INVALID]");
        return 0;
    }

    //verify checksum is correct
    if (calculate_checksum(decoded) != expected_checksum)
    {
        puts("[INVALID]");
        return 0;
    }

    // everythign correct print
    printf("[%s] %s\n", sender, decoded);
    return 1;
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
