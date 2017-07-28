#pragma once

#include <iostream>
using namespace std;

// Vector of passenger destinations
#include <vector>
typedef vector<unsigned> Targets;

// Packs the response of a navigation strategy
struct Response {
	Targets M;
	int s;
	Response() : M(), s(0) { }
	Response(Targets M, int s) : M(M), s(s) { }
};

// Print a vector's content, e.g. cout << response.M << endl;
template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T>& v)
{
	os << "[ ";
	for (const auto& i : v) os << i << ' '; 
	os << "]";
	return os;
}


// Function to compute the numerical average of a vector
#include <numeric>
#include <cassert>
template<class V>
double mean(const V& v) {
	assert(!v.empty());
	double sum = std::accumulate(v.begin(), v.end(), 0.0);
	return sum / v.size();
}

// Random integers in the range [A, B]
#include <random>
std::default_random_engine RNG(std::random_device{}());
unsigned randint(unsigned A, unsigned B) {
	std::uniform_int_distribution<unsigned> D(A, B);
	return D(RNG);
}

// Helper function for checking rank
bool is_zero(unsigned r) { return (r == 0); }
