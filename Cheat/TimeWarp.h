#pragma once
#include "../Core/stdafx.h"
#include "../SDK/csgostructs.h"
#include "../Core/singleton.h"
#include <deque>

#define NUM_OF_TICKS 12

struct StoredData
{
	float simtime;
	Vector hitboxPos;
};

struct CIncomingSequence
{
	CIncomingSequence::CIncomingSequence(int instate, int outstate, int seqnr, float time)
	{
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};

class TimeWarp : public Singleton<TimeWarp>
{
	int nLatestTick;
	StoredData TimeWarpData[64][NUM_OF_TICKS];
public:
	void CreateMove(CUserCmd* cmd);
	void AddLatencyToNetchan(INetChannel * netchan, float Latency);
	void UpdateIncomingSequences();
	void ClearIncomingSequences();
};
