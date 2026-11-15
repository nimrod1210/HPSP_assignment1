#include "messages.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int store_init(MessageStore *store, size_t initial_capacity)
{
    if (store == NULL)
    {
        return 0;
    }
    store->messages = NULL;
    store->count = 0;
    store->capacity = 0;

    if (initial_capacity == 0)
    {
        return 1;
    }
    if (initial_capacity > SIZE_MAX / sizeof(Message))
    {
        return 0;
    }
    store->messages = calloc(initial_capacity, sizeof(Message));

    if (store->messages == NULL)
    {
        return 0;
    }
    store->capacity = initial_capacity;
    return 1;
}

int message_init(Message *msg, int id, const char *text)
{
    if (msg == NULL)
    {
        return 0;
    }
    msg->id = 0;
    msg->text = NULL;
    if (text == NULL || id < 1)
    {
        return 0;
    }
    msg->text = malloc(strlen(text) + 1);
    if (msg->text == NULL)
    {
        return 0;
    }
    strcpy(msg->text, text);
    msg->id = id;
    return 1;
}

int store_reserve(Message **messages, size_t *capacity, size_t required)
{
    if (messages == NULL || capacity == NULL)
    {
        return 0;
    }
    if (*capacity >= required)
    {
        return 1;
    }

    size_t max_capacity = SIZE_MAX / sizeof(Message);
    if (required > max_capacity)
    {
        return 0;
    }
    size_t new_capacity = (*capacity == 0) ? 4 : *capacity;

    if (new_capacity > max_capacity)
    {
        new_capacity = max_capacity;
    }

    while (new_capacity < required)
    {
        if (new_capacity > max_capacity / 2)
        {
            new_capacity = max_capacity;
        }
        else
        {
            new_capacity *= 2;
        }
    }

    Message *temp = realloc(*messages, new_capacity * sizeof(Message));

    if (temp == NULL)
    {
        return 0;
    }

    for (size_t i = *capacity; i < new_capacity; i++)
    {
        temp[i].id = 0;
        temp[i].text = NULL;
    }

    *messages = temp;
    *capacity = new_capacity;

    return 1;
}

int store_append(MessageStore *store, int id, const char *text)
{
    if (store == NULL)
    {
        return 0;
    }
    size_t count = store->count;
    size_t capacity = store->capacity;
    Message **msg_array = &store->messages;
    if (count >= capacity)
    {
        if (store_reserve(msg_array, &store->capacity, count + 1) == 0)
        {
            return 0;
        }
    }
    Message *new_msg = &store->messages[count];
    if (message_init(new_msg, id, text) == 0)
    {
        return 0;
    }
    store->count++;
    return 1;
}

void message_free(Message *msg)
{
    if (msg == NULL)
    {
        return;
    }
    free(msg->text);
    msg->text = NULL;
    msg->id = 0;
}

void store_free(MessageStore *store)
{
    if (store == NULL)
    {
        return;
    }
    for (size_t i = 0; i < store->count; i++)
    {
        message_free(&store->messages[i]);
    }
    free(store->messages);
    store->messages = NULL;
    store->count = 0;
    store->capacity = 0;
}

int store_clone(MessageStore *dest, const MessageStore *source)
{
    if (dest == NULL || source == NULL)
    {
        return 0;
    }
    if (store_init(dest, source->count) == 0)
    {
        return 0;
    }
    for (size_t i = 0; i < source->count; i++)
    {
        if (store_append(dest, source->messages[i].id, source->messages[i].text) == 0)
        {
            store_free(dest);
            return 0;
        }
    }
    return 1;
}
