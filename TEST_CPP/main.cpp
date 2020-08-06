#include <iostream>
#include <vector>
#include "VLVector.hpp"
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))
int DEFAULT_CAP = DEFAULT_SIZE;
int INCREASE_FACTOR = 3;

using std::vector;

int getNewSize(int s)
{
	return floorf(INCREASE_FACTOR * (s + 1) * 0.5);
}

int main()
{

	int s = 16;
	/*testing VlVector against Vector*/
	VLVector<int> VL1;
	vector<int> v1;
	/*1 - basic methods*/
	assertm((VL1.empty()), "failed test 1.0\n");
	assertm(VL1.size() == v1.size(), "failed test 1.1\n");
	assertm(VL1.capacity() == DEFAULT_CAP, "failed test 1.2\n");

	/*add 16 numbers into the array*/
	VLVector<int> VL2;
	for (int i = 0; i < 16; ++i)
	{
		VL1.push_back(i);
		VL2.push_back(i);
		v1.push_back(i);
	}

	/*2 - push back and pop*/
	assertm(VL1.size() == v1.size(), "failed test 2.2\n");
	assertm(VL2.size() == VL1.size(), "failed test 2.3\n");
	assertm(VL1.capacity() == DEFAULT_CAP, "failed test 2.4\n"); //should not increase capacity.
	/*check at() method one by one*/
	for (int j = 0; j < VL1.size(); ++j)
	{
		assertm(VL1.at(j) == v1.at(j), "failed test 2.5\n");
	}

	/*check if size is updated correctly when removing*/
	for (int k = 0; k < 4; ++k)
	{
		VL1.pop_back();
		v1.pop_back();
		assertm(v1.size() == VL1.size(), "failed test 2.6\n");
	}
	std::cout << "******************************************" << std::endl;
	std::cout << "*****Pushback and PopBack test passed*****" << std::endl;
	std::cout << "******************************************" << std::endl;

	/*3 - operators*/

	//check operator[]
	for (int j = 0; j < VL1.size(); ++j)
	{
		assertm(VL1[j] == v1[j], "failed test 3.0\n");
	}

	/*change values*/
	for (int l = 0; l < VL1.size(); ++l)
	{
		VL1[l] += 3;
	}

	/*should be different now*/
	for (int j = 1; j < VL1.size(); ++j)
	{
		assertm(VL1[j] != v1[j], "failed test 3.1\n");
	}
	assertm(VL1 != VL2, "failed test 3.2\n");
	VL1.clear();
	assertm(VL1.empty(), "failed test 3.3\n");
	/*test assignment operator=*/
	VL1 = VL2;
	assertm(VL1 == VL2, "failed test 3.4\n");

	/*test copy constructor*/
	VLVector<int> VL3;
	VL3 = VL1;
	assertm(VL1 == VL3, "failed test 3.5\n");

	/*test both erase and insert methods*/
	for (int m = 0; m < VL3.size(); ++m)
	{
		VL1.erase(VL1.begin()); //erases whole vector
	}
	assertm(VL1.empty(), "failed test 3.6\n");
	for (int n = 0; n < 17; ++n)
	{
		VL1.push_back(n);
	}
//	for (int n = 0; n < VL1.size(); ++n)
//	{
//		std::cout << VL1[n] << std::endl;
//	}
	auto first = &(VL1[3]);
	auto last = &(VL1[6]);
	auto it3 = VL1.erase(first, last);
//    for (int n = 0; n < VL1.size(); ++n) {
//        std::cout << VL1[n] << std::endl;
//    }
	//std::cout << *it3 << std::endl;
	assertm(*it3 == 6, "failed test 3.7\n");

	assertm(VL1.size() == 14, "failed test 3.8\n");
	VL1.erase(VL1.begin(), VL1.end());
	assertm(VL1.empty(),"failed test 3.9\n");
	VL1 = VL3;

//    VL1.pop_back();
//    VL1.pop_back();
	VL1.insert(VL1.begin(), 36);
	assertm(VL1[0] == 36, "failed test 3.10\n");
	assertm(VL1.size() == 17, "failed test 3.11\n");
//    for (int n = 0; n < VL1.size(); ++n) {
//        std::cout << VL1[n] << std::endl;
//    }
	auto itr = &(VL1[6]);
	auto itr1 = VL1.insert(itr, 15);
	assertm(VL1[6] == 15, "failed test 3.12\n");
//	    for (int n = 0; n < VL1.size(); ++n) {
//        std::cout << VL1[n] << std::endl;
//    }
	assertm(*itr1 == 15, "failed test 3.13\n");
	assertm(VL1.size() == 18, "failed test 3.13");
	VL1.insert(itr1, 14);
	for (int i1 = 0; i1 < 18; ++i1) {
		auto tmpItr = &(VL1[i1]);
		int val = i1 * 2;
		VL1.insert(tmpItr, val);
	}
	assertm(VL1.size() == 37, "failed test 3.14\n");
	VL1.erase(VL1.begin(), VL1.end());
	assertm(VL1.empty(), "failed test 3.15\n");
	assertm(VL1.capacity() == 16, "failed test 3.16");

	std::cout << "**************************************************" << std::endl;
	std::cout << "*****operators, insert and erase tests passed*****" << std::endl;
	std::cout << "**************************************************" << std::endl;


}
