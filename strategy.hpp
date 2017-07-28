#pragma once

#include "helper.hpp"

// Base class for navigation strategies
#include <string>
#include <cassert>
struct Strategy {
	string name;
	Strategy(string name) : name(name) { }
	
	// The following function should be overloaded in your strategy
	// Do not change the signature of this function
	virtual
	Response step(unsigned b, const Targets& B, const vector<Targets>& Q)
	{ 
		return Response();
	};
};

// STRATEGY: "Always go in the same direction"
#include <numeric>
struct AI_CLOCK : public Strategy {
	unsigned C;
	unsigned N;
	
	AI_CLOCK(unsigned C, unsigned N) : Strategy("Clock"), C(C), N(N)
	{
	}
	
	Response step(unsigned b, const Targets& B, const vector<Targets>& Q)
	{
		// Number of passengers to board
		unsigned n = min(Q[b].size(), C - B.size());
		
		// Passenger selection from Q[b]
		// Take passengers number 0, 1, .., n-1
		Targets M(n);
		std::iota(M.begin(), M.end(), 0);
		
		// Always go in the same direction
		int s = +1;
		
		return Response(M, s);
	}
};


// STRATEGY: Modestly greedy
#include <numeric>
struct AI_GREEDY : public Strategy {
	const char* name;
	unsigned C;
	unsigned N;
	int s;
	
	AI_GREEDY(unsigned C, unsigned N) : Strategy("Modestly greedy"), C(C), N(N)
	{
		// Default direction
		s = +1;
	}
	
	Response step(unsigned b, const Targets& B, const vector<Targets>& Q)
	{
		// Number of passengers to board
		unsigned n = min(Q[b].size(), C - B.size());
		// Passenger selection from Q[b]
		// Take passengers number 0, 1, .., n-1
		Targets M(n);
		std::iota(M.begin(), M.end(), 0);
		
		// No passengers? Continue as before
		if (B.empty() & M.empty()) return Response(M, s);

		// Next passenger's destination
		int t;
		if (!B.empty()) t = B[0]; else t = Q[b][M[0]];

		// Destination relative to the current position
		t = N - 2*((t+N-b) % N);

		// Move towards that destination (modify default direction)
		s = (t > 0 ? +1 : -1);
		
		return Response(M, s);
	}
};
