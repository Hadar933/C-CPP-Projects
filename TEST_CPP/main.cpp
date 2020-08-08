#include <iostream>
#include <vector>
#include "VLVector.hpp"
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))
#define DEFAULT_CAP 16
#define EMPTY 0
#define INCREASE_FACTOR 3

using std::vector;

int getNewSize(int s)
{
	return floorf(INCREASE_FACTOR * (s + 1) * 0.5);
}

int main() {
	VLVector<int,2> vec1;
	VLVector<int,2> vec2;
	vec1.push_back(28);
	vec1.push_back(179);
	vec1.push_back(266);
	vec1.push_back(556);
	vec1.push_back(618);
	vec1.push_back(655);
	vec1.push_back(780);
	vec1.push_back(726);
	vec1.push_back(801);
	std::cout << "\nbefore insertion" << std::endl;
	for(int i=0;i<vec1.size();i++)
	{
		std::cout<< vec1[i] << " ";
	}
	vec2.push_back(180);
	vec2.push_back(2604);
	vec2.push_back(909);
	vec2.push_back(2583);
	vec2.push_back(624);
	vec2.push_back(2253);
	vec2.push_back(2379);
	vec2.push_back(2310);
	vec2.push_back(2292);
	std::cout << "\nvector to insert" << std::endl;
	for(int i=0;i<vec2.size();i++)
	{
		std::cout<< vec2[i] << " ";
	}
	*vec1.insert(vec1.end()-2,vec2.begin(),vec2.end());

	std::cout << "\nafter insertion" << std::endl;
	for(int i=0;i<vec1.size();i++)
	{
		std::cout<< vec1[i] << " ";
	}


}
