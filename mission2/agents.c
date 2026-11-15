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
    if (agents==NULL && count == 0) {
        return ACCESS_DENIED; /* no agents given */
    }
    Agent *agents_copy;
    if(!copy_agents(agents_copy, count, agents, count)){
        return ACCESS_INVALID; /* failed to copy agents */
    }
    Agent *agent = find_agent(agents_copy, count, event->agent_id);
    if (agent== NULL) {
        return ACCESS_INVALID; /* agent not found */
    }
    if (event->granted == 1 && event->required_clearance > agent->clearance) {
        return ACCESS_UNAUTHORIZED_GRANTED; /* access granted but not authorized */
    }
    if (event->granted == 0 && event->required_clearance <= agent->clearance) {
        return ACCESS_UNEXPECTED_DENIAL; /* access denied but should have been granted */
    }
    if(event->granted == 0 && event->required_clearance > agent->clearance) {
        return ACCESS_DENIED; /* access denied and not authorized */
    }
    return ACCESS_OK;
}

int recover_code(const Agent *agents, size_t agent_count, const AccessEvent *events, size_t event_count, char *code, size_t capacity)
{
    (void)agents;
    (void)agent_count;
    (void)events;
    (void)event_count;
    if (code != NULL && capacity > 0)
        code[0] = '\0';
        
    return 0; /* TODO: Stage D */
}
