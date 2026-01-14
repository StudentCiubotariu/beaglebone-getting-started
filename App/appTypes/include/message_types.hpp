#pragma once

#include <cstdint>

// --------------------------------------------------
// Signal header
// --------------------------------------------------
#pragma pack(push, 1)

struct SignalHeader
{
    uint8_t  version;
    uint8_t  eSigStatus;
    uint16_t cycleCounter;
    uint16_t measurementCounter;
};

// --------------------------------------------------
// Message container
// --------------------------------------------------
struct Message
{
    SignalHeader header;
    uint8_t payload[1000];
};

#pragma pack(pop)

// --------------------------------------------------
// Compile-time size checks (safety)
// --------------------------------------------------
static_assert(sizeof(SignalHeader) == 6, "SignalHeader size mismatch");
static_assert(sizeof(Message) == 1006, "Message size mismatch");