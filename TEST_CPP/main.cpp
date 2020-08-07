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
	for (int i = 0; i < 16; ++i) {
		VL1.push_back(i);
		VL2.push_back(i);
		v1.push_back(i);
	}

	/*2 - push back and pop*/
	assertm(VL1.size() == v1.size(), "failed test 2.2\n");
	assertm(VL2.size() == VL1.size(), "failed test 2.3\n");
	assertm(VL1.capacity() == DEFAULT_CAP, "failed test 2.4\n"); //should not increase capacity.
	/*check at() method one by one*/
	for (int j = 0; j < VL1.size(); ++j) {
		assertm(VL1.at(j) == v1.at(j), "failed test 2.5\n");
	}

	/*check if size is updated correctly when removing*/
	for (int k = 0; k < 4; ++k) {
		VL1.pop_back();
		v1.pop_back();
		assertm(v1.size() == VL1.size(), "failed test 2.6\n");
	}
	std::cout << "******************************************" << std::endl;
	std::cout << "*****Pushback and PopBack test passed*****" << std::endl;
	std::cout << "******************************************" << std::endl;

	/*3 - operators*/

	//check operator[]
	for (int j = 0; j < VL1.size(); ++j) {
		assertm(VL1[j] == v1[j], "failed test 3.0\n");
	}

	/*change values*/
	for (int & l : VL1) {
		l += 3;
	}

	/*should be different now*/
	for (int j = 1; j < VL1.size(); ++j) {
		assertm(VL1[j] != v1[j], "failed test 3.1\n");
	}
	assertm(VL1 != VL2, "failed test 3.2\n");
	VL1.clear();
	assertm(VL1.empty(),"failed test 3.3\n");
	/*test assignment operator=*/
	VL1 = VL2;
	assertm(VL1 == VL2, "failed test 3.4\n");

	/*test copy constructor*/
	VLVector<int> VL3 = VL1;
	assertm(VL1 == VL3, "failed test 3.5\n");

	/*test both erase and insert methods*/
	for (int m = 0; m < VL3.size(); ++m) {
		VL1.erase(VL1.begin()); //erases whole vector
	}
	assertm(VL1.empty(), "failed test 3.6\n");

	/*creates an dynamically allocated array*/
	for (int n = 0; n < 17; ++n) {
		VL1.push_back(n);
	}

	/*print size, capacity and the vec itself*/
	std::cout << "Size = " << VL1.size() << " ,Capacity = " << VL1.capacity() << " ,The vector itself: " << std::endl;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "****************************" << std::endl;

	/*erase 3 elements from the middle of it*/
	auto first = &(VL1[3]);
	auto last = &(VL1[6]);
	auto  it3 = VL1.erase(first, last); // erase 3 items
	std::cout << "Size = " << VL1.size() << " ,Capacity = " << VL1.capacity() << " ,The vector itself: " << std::endl;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "****************************" << std::endl;
	std::cout << *it3 << std::endl;
	assertm(*it3 == 6, "failed test 3.7\n" );
	assertm(VL1.size() == 14, "failed test 3.8\n");
	VL1.erase(VL1.begin(), VL1.end());
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "****************************" << std::endl;
	assertm(VL1.empty(),"failed test 3.9\n");
	VL1 = VL3;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "****************************" << std::endl;

	VL1.insert(VL1.begin(), 36);
	assertm(VL1[0] == 36, "failed test 3.10\n");
	assertm(VL1.size() == 17, "failed test 3.11\n");
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "****************************" << std::endl;

	/*add in the middle and check size*/
	auto itr = &(VL1[6]);
	auto itr1 = VL1.insert(itr, 15);
	assertm(VL1[6] == 15, "failed test 3.12\n");
	assertm(*itr1 == 15, "failed test 3.13\n");
	assertm(VL1.size() == 18, "failed test 3.13");
	std::cout << "Size = " << VL1.size() << " ,Capacity = " << VL1.capacity() << " ,The vector itself: " << std::endl;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "*******************************" << std::endl;

	/*add in the middle again and check size again*/
	VL1.insert(itr1, 14);
	assertm(VL1[6] == 14, "failed test 3.14\n");
	assertm(*itr1 == 14, "failed test 3.15\n");
	assertm(VL1.size() == 19, "failed test 3.16\n");
	std::cout << "Size = " << VL1.size() << " ,Capacity = " << VL1.capacity() << " ,The vector itself: " << std::endl;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "*******************************" << std::endl;

	/*add 5 items in a row to beginning*/
	auto it = &(VL1[0]);
	for (int k = 4; k >= 0; --k) {
		VL1.insert(it, k);
	}

	assertm(VL1.size() == 24, "failed test 3.17\n");
	for (int i1 = 0; i1 < 5; ++i1) {
		assertm(VL1[i1] == i1,"failed test 3.18\n");
	}

	std::cout << "Size = " << VL1.size() << " ,Capacity = " << VL1.capacity() << " ,The vector itself: " << std::endl;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "*******************************" << std::endl;

	/*test insert of multiple items*/
	VL1 = VL3; // VL1 now holds 16 items 0,1,2 ... 15.
	v1.clear();
	for (int k1 = 0; k1 < 5; ++k1) { // push 5 zeros.
		v1.push_back(0);
	}
	auto iter1 = v1.begin();
	auto iter2 = v1.end();
	auto pos = &(VL1[3]);
	std::cout <<"The size before insertion :" << VL1.size() << std::endl;
	VL1.insert(pos, iter1, iter2);
	std::cout <<"The size after insertion: " << VL1.size() << std::endl;
	std::cout << "Size = " << VL1.size() << " ,Capacity = " << VL1.capacity() << " ,The vector itself: " << std::endl;
	for (int n : VL1) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	std::cout << "*******************************" << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "*****operators, insert and erase tests passed*****" << std::endl;
	std::cout << "**************************************************" << std::endl;

}
