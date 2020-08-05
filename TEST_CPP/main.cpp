#include <iostream>
#include "VLVector.hpp"


int main()
{
	printf("checking push back func:\n");
	VLVector<int,2> vlv;
	vlv.push_back(4);
	vlv.push_back(7);
	vlv.push_back(1);
	vlv.push_back(20);
	vlv.push_back(23);
	vlv.push_back(234);
	vlv.push_back(213);
	vlv.push_back(31);
	printf("Pushing:\n");
	for (int i=0;i<vlv.getCurrSize();i++)
	{
		printf("item %d is %d\n",i,vlv[i]);
	}
	printf("Added succesfully\n\n");
	printf("copy ctor:\n");
	VLVector<int,2> copy_vec = vlv;
	for (int i=0;i<copy_vec.getCurrSize();i++)
	{
		printf("item %d is %d\n",i,copy_vec[i]);
	}

	printf("\nchecking empty func:\n");
	VLVector<int> empty_vec;
	printf("vlv(not empty) is empty = %d\n",vlv.empty());
	printf("empty_vec is empty = %d\n", empty_vec.empty());

	printf("\nchecking at func:\n");
	std::cout<< "item in vector:" << vlv.at(3) << std::endl;
	vlv.at(100);
	std::cout<<vlv.at(5)<<std::endl;

	return 0;
}
