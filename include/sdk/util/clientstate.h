#ifndef SDK_CLIENTSTATE_H
#define SDK_CLIENTSTATE_H
#pragma once

#include "../common.h"

#ifndef INetChannel
class INetChannel;
#endif

// this + 96 = m_NetChannel ?
// this + 100 = m_nSignonState ?
// this + 104 = m_nDeltaTick
// this + 68212 = lastoutgoingcommand
// this + 68216 = chokedcommands
// this + 68220 = last_command_ack

#define CLIENTSTATE_OFFSET_NETCHANNEL 0x60
#define CLIENTSTATE_OFFSET_SIGNONSTATE 0x64
#define CLIENTSTATE_OFFSET_DELTATICK 0x68
#define CLIENTSTATE_OFFSET_LASTOUTGOINGCOMMAND 0x10A74
#define CLIENTSTATE_OFFSET_CHOKEDCOMMANDS 0x10A78
#define CLIENTSTATE_OFFSET_LASTCOMMANDACK 0x10A7C

class CClientState
{
public:
	// CBaseClientState
	INetChannel* m_NetChannel = (INetChannel*)((DWORD*)this + CLIENTSTATE_OFFSET_NETCHANNEL);
	int m_nSignonState = *(int*)((DWORD*)this + CLIENTSTATE_OFFSET_SIGNONSTATE);
	int m_nDeltaTick = *(int*)((DWORD*)this + CLIENTSTATE_OFFSET_DELTATICK);

	// CClientState
	int lastoutgoingcommand = *(int*)((DWORD*)this + CLIENTSTATE_OFFSET_LASTOUTGOINGCOMMAND);
	int chokedcommands = *(int*)((DWORD*)this + CLIENTSTATE_OFFSET_CHOKEDCOMMANDS);
	int last_command_ack = *(int*)((DWORD*)this + CLIENTSTATE_OFFSET_LASTCOMMANDACK);
};

#endif