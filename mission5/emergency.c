#include "emergency.h"
#include <stdio.h>
#include <string.h>

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

static int is_valid_auth(const EmergencyRequest *request)
{
    if(request->authorization.request_id <= 0 || request->authorization.unit_id <= 0 || request->authorization.approver_id <= 0){
        return 0;
    }
    if(request->authorization.request_id != request->request_id){
        return 0;
    }
    if(request->authorization.unit_id != request->unit_id){
        return 0;
    }
    return 1;
}

static int is_auth_approved(const EmergencyRequest *request){
    if((request->authorization.approved != 1) || (request->authorization.clearance < M5_ADMIN_CLEARANCE) || (request->authorization.clearance > 5)){
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
    if (dest == NULL || options == NULL || authorization == NULL || steps == NULL || step_count <1 || step_count> 8)
    {
        return 0;
    }
    if(dest-> request_id=!0 || dest->unit_id!=0||dest->device!=NULL || dest->steps !=NULL ){
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
    if(request==NULL){
        return 0;
    }
    if(request->device==NULL || strcmp(request->device, "BEERSHEVANATOR") != 0){
        return 0;
    }
    if((strcmp(request->mode, "emergency") != 0) || (strcmp(request->action, "self-destruct") != 0)){
        return 0;
    }
    if(!is_valid_auth(request) || !is_auth_approved(request)){
        return 0;
    }
    if(request->steps == NULL || request->step_count != 3){
        return 0;
    }
    if(request->steps[0] != STEP_PREPARE_EMERGENCY || request->steps[1] != STEP_REQUEST_AUTHORIZATION || request->steps[2] != STEP_EXECUTE_COMMAND){
        return 0;
    }
    return 1;
}

