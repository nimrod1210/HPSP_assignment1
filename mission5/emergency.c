#include "emergency.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int emergency_request_is_empty(const EmergencyRequest *dest)
{
    if (dest == NULL || dest->device != NULL || dest->steps != NULL ||
        dest->request_id != 0 || dest->unit_id != 0 || dest->step_count != 0 ||
        dest->authorization.request_id != 0 || dest->authorization.unit_id != 0 || dest->authorization.approver_id != 0 ||
        dest->authorization.clearance != 0 || dest->authorization.approved != 0)
    {
        return 0;
    }

    for (size_t i = 0; i < M5_MODE_CAPACITY; i++)
    {
        if (dest->mode[i] != '\0')
            return 0;
    }

    for (size_t i = 0; i < M5_ACTION_CAPACITY; i++)
    {
        if (dest->action[i] != '\0')
            return 0;
    }

    return 1;
}

/* helper function that checks if a record is valid */
static int is_valid_record(const AuthorizationRecord *record)
{
    if(record->request_id <= 0 || record->unit_id <= 0 || record->approver_id <= 0){
        return 0;
    }
    if(record->approved != 1 && record->approved != 0){
        return 0;
    }
    if(record->clearance < 0 || record->clearance > 5){
        return 0;
    }
    return 1;
}

int parse_launch_options(int argc, char *const argv[], LaunchOptions *out)
{
    if (out == NULL)
    {
        return 0;
    }

    out->device[0] = '\0';
    out->mode[0] = '\0';
    out->action[0] = '\0';

    if (argc != 7 || argv == NULL)
    {
        return 0;
    }

    int seen_device = 0;
    int seen_mode = 0;
    int seen_action = 0;

    for (int i = 1; i < argc; i += 2)
    {
        if (argv[i] == NULL || argv[i + 1] == NULL)
        {
            return 0;
        }

        const char *option = argv[i];
        const char *value = argv[i + 1];

        if (strcmp(option, "--device") == 0)
        {
            if (seen_device || strcmp(value, "BEERSHEVANATOR") != 0)
            {
                return 0;
            }
            seen_device = 1;
        }
        else if (strcmp(option, "--mode") == 0)
        {
            if (seen_mode || strcmp(value, "emergency") != 0)
            {
                return 0;
            }
            seen_mode = 1;
        }
        else if (strcmp(option, "--action") == 0)
        {
            if (seen_action || strcmp(value, "self-destruct") != 0)
            {
                return 0;
            }
            seen_action = 1;
        }
        else
        {
            return 0;
        }
    }

    if (!seen_device || !seen_mode || !seen_action)
    {
        return 0;
    }

    strcpy(out->device, "BEERSHEVANATOR");
    strcpy(out->mode, "emergency");
    strcpy(out->action, "self-destruct");

    return 1;
}

const AuthorizationRecord *find_authorization(const AuthorizationRecord *records, size_t count,
                                              int request_id, int unit_id, int required_clearance)
{
    if ((records == NULL && count != 0) || request_id <= 0 || unit_id <= 0 || required_clearance < 0 || required_clearance > 5) {
        return NULL;
    }
    for(const AuthorizationRecord *record = records; record < records + count; ++record) {
        if (!is_valid_record(record)) {
            continue;
        }
        if(record->request_id == request_id && record->unit_id == unit_id && record->clearance >= required_clearance && !record->approved) {
            return record;
        }
    }
    return NULL;
}

int emergency_request_init(EmergencyRequest *dest,
                           const LaunchOptions *options,
                           const AuthorizationRecord *authorization,
                           const int *steps, size_t step_count)
{
    if (dest == NULL || options == NULL ||
        authorization == NULL || steps == NULL ||
        step_count == 0 || step_count > 8)
    {
        return 0;
    }
    if (!emergency_request_is_empty(dest))
    {
        return 0;
    }

    if (options->device[0] == '\0' ||
        memchr(options->device, '\0', M5_DEVICE_CAPACITY) == NULL ||
        memchr(options->mode, '\0', M5_MODE_CAPACITY) == NULL ||
        memchr(options->action, '\0', M5_ACTION_CAPACITY) == NULL)
    {
        return 0;
    }

    size_t device_length = strlen(options->device);
    char *device_copy = malloc(device_length + 1);
    if (device_copy == NULL)
    {
        return 0;
    }
    strcpy(device_copy, options->device);

    int *steps_copy = malloc(step_count * sizeof(int));
    if (steps_copy == NULL)
    {
        free(device_copy);
        return 0;
    }
    memcpy(steps_copy, steps, step_count * sizeof(int));

    dest->request_id = authorization->request_id;
    dest->unit_id = authorization->unit_id;
    dest->authorization = *authorization;

    strcpy(dest->mode, options->mode);
    strcpy(dest->action, options->action);

    dest->device = device_copy;
    dest->steps = steps_copy;
    dest->step_count = step_count;

    return 1;
}


void emergency_request_free(EmergencyRequest *request)
{
    if (request == NULL)
    {
        return;
    }
    free(request->device);
    free(request->steps);
    *request = (EmergencyRequest){0};
}


int validate_emergency_request(const EmergencyRequest *request)
{
    (void)request;
    /* TODO Stage D: validate the complete request without changing it. */
    return 0;
}
