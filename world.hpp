#pragma once

#include "helper.hpp"
#include "strategy.hpp"

// This class simulates the system step-by-step.
// Do not modify it.
#include <cassert>
#include <algorithm>
struct World {
	// a = location, b = destination of passenger
	struct News {
		unsigned a, b;
		News(unsigned a, unsigned b) : a(a), b(b) { }
	};

	unsigned C;        // Bus capacity
	unsigned N;        // Number of stations
	unsigned b;        // Bus position
	Targets B;         // Bus passengers' destinations
	vector<Targets> Q; // Queues at stations
	unsigned i;        // Iteration number (i.e. time)
	vector<News> NEWS; // World trajectory record
	
	World(unsigned C, unsigned N) : C(C), N(N)
	{
		rewind();
	}

	void rewind()
	{
		b = 0;
		B = Targets();
		Q = vector<Targets>(N);
		i = 0;
	}
	
	News news() {
		while (NEWS.size() <= i) {
			unsigned a = randint(0, N-1);
			unsigned b = (a + randint(1, N-1)) % N;
			NEWS.push_back(News(a,b));
		}
		
		assert((i < NEWS.size()));  //removed (0 <= i) & //always true
		return NEWS[i];
	}
	
	void move(Response res)
	{
		// Passengers mount (in the given order)
		{
			for (auto i : res.M)
				B.push_back(Q[b][i]);
		}
        
		
		// Remove them from the queue
		{
			// Sort in decreasing order
			Targets M = res.M;
			std::sort(M.rbegin(), M.rend()); 
			
			for (auto i : M)
				Q[b].erase(Q[b].begin() + i);
		}
		
		// Advance bus
		b = (b + (N + res.s)) % N;
		
		// Passengers unmount
		B.erase(std::remove(B.begin(), B.end(), b), B.end());
		
		// Advance time
		this->i ++;
		
		// New person arrives at "a" with destination "b"
		News news = this->news();
		Q[news.a].push_back(news.b);
	}
	
	// Number of people waiting averaged over stations
	double get_w()
	{
		std::vector<unsigned> sizes;
		for (auto q : Q)
			sizes.push_back(q.size());
		
		return mean(sizes);
	}
};
