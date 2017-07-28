#include <cassert>
#include <numeric>
#include <chrono>

#include "strategy.hpp"
#include "mystrategy.hpp"
#include "world.hpp"
#include "writer.hpp"

struct Profiler {
	// Number of iterations (time steps)
	// This will be I ~ 1e6
	static const unsigned I = 10000;
	
	// W[i] = average number of people waiting at time i
	vector<double> W;
	// w = average over time
	double w;

	Profiler(World& wrd, Strategy& nav) : W(), w(0)
	{
		assert((0 < I) & (I <= 1e9));
		
		wrd.rewind();
		assert(wrd.i == 0);
		
		// Main loop
		while(wrd.i < I) {
			// Make read-only references
			const unsigned b = wrd.b;
			const Targets& B = wrd.B;
			const vector<Targets>& Q = wrd.Q;
			wrd.move(nav.step(b, B, Q));
			W.push_back(wrd.get_w());
		}
		
		assert(!W.empty());
		w = mean(W);
        //writeToFile("data/occupancy_" + nav.name + ".dat",W);
	}
};

#include <algorithm>
#include <memory>
//
int main() {

	unsigned C = 10;
	unsigned N = 20;
	
	// http://codereview.stackexchange.com/a/109518
	RNG.discard(700000);

	typedef std::shared_ptr<Strategy> P2S;
	vector<P2S> NAV;
	NAV.push_back(P2S(new AI_MY(C,N)));
	NAV.push_back(P2S(new AI_CLOCK(C,N)));
	NAV.push_back(P2S(new AI_GREEDY(C,N)));
	
	// Next rank to be assigned
	unsigned rank = NAV.size();
	
	// Ranks
	vector<unsigned> R(rank, 0);
	// Score histories
	vector< vector<double> > S(rank);
	
	// Perform the competition
	while (rank) {
		cout << "Number of competitors: " << rank << endl;
		
		// Create a rewindable world
		World wrd(C, N);
		
		// Helper struct
		struct Score {
			unsigned n; // Contestant number
			double v;   // Score value
			Score(unsigned n, double v) : n(n), v(v) { }
			operator double() const { return v; }
		};
		
		// Navigator scores for this round
		// (nonnegative; max score loses)
		vector<Score> K;
		
		for (unsigned n = 0; n < NAV.size(); ++n) {
			if (R[n]) continue;
			Strategy& nav = *NAV[n];
		
			cout << " - Profiling " << nav.name << endl;
            
            auto start = std::chrono::high_resolution_clock::now();
			// Profile the navigator on the world
			Profiler report(wrd, nav);
            auto stop = std::chrono::high_resolution_clock::now();
			// Score = average number of people waiting
			double score = report.w;
			// Record score
			K.push_back( Score(n, score) );
			cout << "   *Score for this round: " << score << endl;
            
            cout << "   *Time taken this round:" << static_cast<std::chrono::duration<double> >(stop-start).count() << endl;
		}
		
		assert(!K.empty());
		
		// Find the maximum score
		double max_score = *std::max_element(K.begin(), K.end());
		
		// Rank the losers of this round
		for (auto i : K) {
			// Append the score of the strategy to its history
			S[i.n].push_back(i.v);
			// Take the running average as the score
			S[i.n].back() = mean(S[i.n]);
			
			// Filter losers
			if (i.v < max_score) continue;
			
			// Assign rank to the n-th strategy
			assert(is_zero(R[i.n]));
			R[i.n] = rank;
		}
		
		rank = std::count_if(R.begin(), R.end(), is_zero);
	}
	
	
	cout << "Final ranking:" << endl;
	{
		for (unsigned rank = 1; rank <= R.size(); ++rank) {
			cout << "Rank " << rank << endl;
			for (unsigned n = 0; n != R.size(); ++n) {
				if (R[n] != rank) continue;
				cout << "   " << (NAV[n]->name) << endl;
			}
		}
	}

	// The history of scores of n-th competitor 
	// is available here as S[n]
	cout << "Running average score history:" << endl;
	for (unsigned n = 0; n != S.size(); ++n)
		cout << "   Contestant #" << n << ": " << S[n] << endl;

	return EXIT_SUCCESS;
}
