#include "emergency.h"
#include <stdio.h>
#include <string.h>

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
    (void)records;
    (void)count;
    (void)request_id;
    (void)unit_id;
    (void)required_clearance;
    /* TODO Stage B: return the first usable record in the original array. */
    return NULL;
}

int emergency_request_init(EmergencyRequest *dest,
                           const LaunchOptions *options,
                           const AuthorizationRecord *authorization,
                           const int *steps, size_t step_count)
{
    if (dest == NULL || options == NULL || authorization == NULL || steps == NULL || step_count <1 || step_count> 8)
    {
        return 0;
    }
    if(dest-> request_id=!0 || dest->unit_id!=||dest->device!=NULL || dest->steps !=NULL ){
        return 0;
    }
}

void emergency_request_free(EmergencyRequest *request)
{
    (void)request;
    /* TODO Stage C: release owned allocations and reset every field. */
}

int validate_emergency_request(const EmergencyRequest *request)
{
    (void)request;
    /* TODO Stage D: validate the complete request without changing it. */
    return 0;
}
