#include "agents.h"

static int is_event_valid(const AccessEvent *event) { /* helper function to validate an AccessEvent */
    if (event == NULL) {
        return 0; /* invalid input */
    }
    if (event->agent_id <= 0) {
        return 0; /* invalid agent_id */
    }
    if (event->required_clearance < 0 || event->required_clearance > 5) {
        return 0; /* invalid required clearance */
    }
    if (event->granted != 0 && event->granted != 1) {
        return 0; /* invalid granted value */
    }
    switch (event->resource_id) {
        case PORT_8080:
        case BGU_CAMERAS:
        case CONTROL_SYSTEM:
        case AIR_SUPPORT:
        case EVIL_MORTY_PRIVATE_COMMS:
        case SERVER_BACKUP:
            break;
        default:
            return 0; /* invalid resource_id */
    }
    if (event->sequence < -1 || event->sequence > 7) {
        return 0; /* invalid sequence number */
    }
    if (event->encoded_symbol < -1 || event->encoded_symbol > 25) {
        return 0; /* invalid encoded symbol */
    }
    return 1; /* valid event */
}


Agent *find_agent(Agent *agents, size_t count, int id)
{
    if (agents == NULL || count == 0) {
        return NULL;
    }
    Agent *p = agents;
    for (size_t i = 0; i < count; i++, p++){
        if (p->id == id)
            return p;
    }
    return NULL;
}

int copy_agents(Agent *dest, size_t capacity, const Agent *source, size_t count)
{   
    if(count==0){
        return 1; /* nothing to copy */
    }
    if(dest == NULL || source==NULL || capacity == 0 ||(capacity < count)) {
        return 0; /* invalid input */
    }
    for (size_t i = 0; i < count; i++) {
        const Agent *src_agent = &source[i];
        Agent *dest_agent = &dest[i];
        *dest_agent = *src_agent; /* copy the entire struct */
    }
    return 1; /* successfully copied */
}

int update_agent(Agent *agent, AgentPatch patch)
{
    if (agent == NULL || patch.agent_id != agent->id) {
        return 0; /* invalid input */
    }
    if(patch.new_clearance < 0 || patch.new_clearance > 5) {
        return 0; /* invalid clearance */
    }
    if(patch.new_active != 0 && patch.new_active != 1) {
        return 0; /* invalid active status */
    }
    Agent copy = *agent;
    copy.clearance = patch.new_clearance;
    copy.active = patch.new_active;
    *agent = copy;
    return 1; /* successfully updated */
}

int apply_patch(Agent *agents, size_t count, const AgentPatch *patch)
{
    if (agents == NULL || patch == NULL || count == 0) {
        return 0; /* invalid input */
    }
    Agent *agent = find_agent(agents, count, patch->agent_id);
    if (agent == NULL) {
        return 0; /* agent not found */
    }
    return update_agent(agent, *patch);
}

int classify_access(const Agent *agents, size_t count, const AccessEvent *event){
    if (is_event_valid(event) == 0) {
        return ACCESS_INVALID; /* invalid input */
    }
    if(agents == NULL) {
        if(count==0){
            if(event->granted == 1){
                return ACCESS_UNAUTHORIZED_GRANTED; /* no agents given, but access granted */
            }
                return ACCESS_DENIED; /* no agents given, and access denied */
        }
        return ACCESS_INVALID; /* invalid input */
    }
    const Agent *found = NULL;
    if(count>0){
        for (const Agent *p = agents; p < agents + count; ++p) {
            if (p->id == event->agent_id) {
                found = p;
                break;
            }
        }
    }
    int authorized = (found != NULL && found->active == 1 && found->clearance >= event->required_clearance);
    if (authorized && event->granted == 1) {
        return ACCESS_OK; /* access granted and authorized */
    }
    if (authorized && !event->granted) {
        return ACCESS_UNEXPECTED_DENIAL; 
    }
    if (!authorized && event->granted) {
        return ACCESS_UNAUTHORIZED_GRANTED;
    }
    return ACCESS_DENIED;
}

/* helper function to filter events for code recovery */
static int filter_event(const Agent *agents, size_t agent_count, const AccessEvent *events){
    if(events == NULL) {
        return 0; /* invalid input */
    }
    if(events->agent_id != 67){ 
        return 0;
    }
    if(events->sequence < 0 || events->sequence > 7){
        return 0;
    }
    if(events->encoded_symbol < 0 || events->encoded_symbol > 25){  
        return 0;
    }
    if (classify_access(agents, agent_count, events) != ACCESS_UNAUTHORIZED_GRANTED) {
        return 0;
    }
    return 1;
}

int recover_code(const Agent *agents, size_t agent_count, const AccessEvent *events, size_t event_count, char *code, size_t capacity)
{
    if (code == NULL) {
        return 0; /* invalid input */
    }
    if(capacity < 9){
        if(capacity > 0){
            code[0] = '\0'; /* not enough capacity for code, return empty string */
        }
        return 0; /* invalid input */
    }
    if ((agents == NULL && agent_count > 0) || (events == NULL && event_count > 0)) {
        code[0] = '\0';
        return 0;
    }
    int seen[8] = {0}; /* track seen sequences */
    char letters[8] = {0}; /* store letters for each sequence */
    for (size_t i = 0; i < event_count; i++) {
        const AccessEvent *event = &events[i];
        if (!filter_event(agents, agent_count, &events[i])) {
            continue;
        }
        int s = event->sequence;
        seen[s]++;
        letters[s] = (char)('A' + ((event->encoded_symbol - s + 26) % 26)); /* decode the symbol to a character */;
    }   
    for(int i = 0; i < 8; i++) {
        if (seen[i] != 1) {
            code[0] = '\0'; /* invalid sequence, return empty string */
            return 0;
        }
    }
    for(int i = 0; i < 8; i++) {
        code[i] = letters[i]; /* construct the code */
    }
    code[8] = '\0'; /* null-terminate the string */
    return 1; /* successfully recovered code */
}