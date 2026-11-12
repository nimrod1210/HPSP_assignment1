#include "mission2_data.h"

const Agent original_agents[] = {
    {2892, "DOOFENSHMIRTZ", "INVENTOR", 1, 1},
    {682, "EVIL_MORTY", "STRATEGIST", 5, 1},
    {7148, "PLANKTON", "CYBER_OPERATOR", 3, 1},
    {381, "GRETA_THUNBERG", "MARITIME_SUPPORT", 1, 0},
    {2861, "MRS_NORA_BEADY", "CAMPUS_SURVEILLANCE", 1, 0},
    {3793, "BOMBARDIRO_CROCODILO", "IRANIAN_AIR_SUPPORT", 4, 0},
    {5162, "SECURITY_OFFICER", "SECURITY", 5, 1},
    {6255, "TECH_SUPPORT", "TECHNICAL_SUPPORT", 4, 1},
    {2147, "LOGISTICS_STAFF", "LOGISTICS", 2, 1},
    {8513, "MAINTENANCE_STAFF", "MAINTENANCE", 3, 1}
};
const size_t original_agent_count =
    sizeof original_agents / sizeof original_agents[0];

const AgentPatch verified_patches[] = {
    {2892, 4, 1},
    {381, 1, 1},
    {2861, 2, 1},
    {3793, 4, 1}
};
const size_t verified_patch_count =
    sizeof verified_patches / sizeof verified_patches[0];

const AccessEvent access_events[] = {
    {381, PORT_8080, 5, 0, -1, -1},
    {381, PORT_8080, 5, 0, -1, -1},
    {90001, PORT_8080, 1, 0, 1, 4},
    {67, BGU_CAMERAS, 3, 1, 3, 2},
    {7148, CONTROL_SYSTEM, 2, 0, 5, 7},
    {67, CONTROL_SYSTEM, 5, 1, 5, 13},
    {381, PORT_8080, 5, 0, -1, -1},
    {67, BGU_CAMERAS, 3, 1, 0, 0},
    {381, PORT_8080, 5, 0, -1, -1},
    {67, SERVER_BACKUP, 4, 1, 7, 0},
    {67, EVIL_MORTY_PRIVATE_COMMS, 5, 1, 2, 10},
    {381, PORT_8080, 5, 0, -1, -1},
    {381, PORT_8080, 5, 0, -1, -1},
    {67, AIR_SUPPORT, 4, 1, 4, 15},
    {2861, BGU_CAMERAS, 3, 0, -1, -1},
    {67, CONTROL_SYSTEM, 4, 1, 1, 21},
    {67, EVIL_MORTY_PRIVATE_COMMS, 3, 1, 6, 5},
    {3793, AIR_SUPPORT, 4, 1, -1, -1},
    {682, EVIL_MORTY_PRIVATE_COMMS, 5, 1, -1, -1},
    {381, PORT_8080, 5, 0, -1, -1},
    {2892, CONTROL_SYSTEM, 4, 1, 3, 12}
};
const size_t access_event_count =
    sizeof access_events / sizeof access_events[0];
