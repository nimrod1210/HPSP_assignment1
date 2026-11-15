#include "decoder.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[203];
    int recovered = 0;
    int rejected = 0;

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        size_t length = strlen(line);
        while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r'))
        {
            length--;
            line[length] = '\0';
        }
        if (process_transmission(line) == 1)
        {
            recovered++;
        }
        else
        {
            rejected++;
        }
    }
    printf("Recovered: %d | Rejected: %d\n", recovered, rejected);
    return 0;
}

