#include <iostream>
#include "VLVector.hpp"


int main()
{
	VLVector<int, 2> vec1;
	for (int i = 0; i < 4; i++)
	{
		vec1.push_back(i);
	}
	VLVector<int, 2> copy_vec;
	copy_vec = vec1;
	int range = vec1.getCapacity();
	for (int i = 0; i < range; i++)
	{
		std::cout << "vec1[i]=" << vec1[i] << std::endl;
		std::cout << "copy_vec[i]=" << copy_vec[i] << std::endl;
	}
	copy_vec.push_back(1000);
	for (int i = 0; i < 10; i++)
	{
		int x = vec1[i];
		int y = copy_vec[i];
		std::cout << "vec1[i]=" << vec1[i] << std::endl;
		std::cout << "copy_vec[i]=" << copy_vec[i] << std::endl;
	}

	return 0;
}