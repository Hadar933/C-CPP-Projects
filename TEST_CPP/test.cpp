//
// Created by nirga on 06-Aug-20.
//
#include "VLVector.hpp"
#include <vector>
#include <random>
#include <fstream>
#include <iostream>

#define MAX_SIZE 500
#define POPS 490


using namespace std;

size_t DEFAULT_STATIC_CAPACITY = DEFAULT_SIZE;
const int range_from  = 0;
const int range_to    = 1000;
std::random_device                  rand_dev;
std::mt19937                        generator(rand_dev());
std::uniform_int_distribution<int>  distr(range_from, range_to);

void printVectors(VLVector<int> &nir, vector<int> &stl, ofstream &out, ofstream &cmp)
{
	out << "vector: " << endl;
	for (auto item : nir)
	{
		out << item << '\t';
	}
	cmp << "vector: " << endl;
	for (auto item : stl)
	{
		cmp << item << '\t';
	}
	out << endl;
	cmp << endl;
}

void intTest()
{
	ofstream out("../test\\out\\int.out"), cmp("../test\\cmp\\int.cmp");
	VLVector<int> nir;
	vector<int> stl, extra;
	string extraS;
	out << "is Empty? " << nir.empty() << endl;
	cmp << "is Empty? " << stl.empty() << endl;
	try
	{
		nir.at(3);
		out << "wrong index wasn't thrown" << endl;
	} catch (exception &e)
	{}
	if (nir.capacity() != DEFAULT_STATIC_CAPACITY)
	{ out << "wrong static capacity" << endl; }
	cout << "####int####\ntesting push_back and pop_back and indexing" << endl;
	for (int i = 1; i < MAX_SIZE; ++i)
	{
		int n = distr(generator);
		nir.push_back(n);
		stl.push_back(n);
		if (i % 100 == 0)
		{
			int indexes[5];
			out << "size after " << i << " insertions: " << nir.size() << std::endl;
			cmp << "size after " << i << " insertions: " << stl.size() << std::endl;
			for (int j = 0; j < 5; ++j)
			{
				indexes[j] = distr(generator) % i;
			}
			char buff[100];
			snprintf(buff, 100, "values at %d, %d, %d, %d, %d are %d, %d, %d, %d, %d\n",
					 indexes[0], indexes[1], indexes[2], indexes[3], indexes[4],
					 nir[indexes[0]], nir.at(indexes[1]), nir[indexes[2]],
					 nir.at(indexes[3]), nir[indexes[4]]);
			out << buff;
			snprintf(buff, 100, "values at %d, %d, %d, %d, %d are %d, %d, %d, %d, %d\n",
					 indexes[0], indexes[1], indexes[2], indexes[3], indexes[4],
					 stl[indexes[0]], stl.at(indexes[1]), stl[indexes[2]],
					 stl.at(indexes[3]), stl[indexes[4]]);
			cmp << buff;
		}
		if (i % 50 == 0)
		{
			extra.push_back(n * 3);
			extraS += to_string(extra[extra.size() - 1]) += '\t';
		}
	}

	for (int k = 0; k < POPS; ++k)
	{
		nir.pop_back();
		stl.pop_back();
	}
	out << "size after 490 pop_backs: " << nir.size() << std::endl;
	cmp << "size after 490 pop_backs: " << stl.size() << std::endl;
	printVectors(nir, stl, out, cmp);
	cout << "testing erase and insert, each one causes heap<>stack transforms" << endl;
	out << "insert vector " << extraS << "at position " << nir.size() - 2 << endl;
	out << "output reference to " << *nir.insert(nir.end() - 2, extra.begin(), extra.end()) << endl;
	cmp << "insert vector " << extraS << "at position " << stl.size() - 2 << endl;
	cmp << "output reference to " << *stl.insert(stl.end() - 2, extra.begin(), extra.end()) << endl;
	printVectors(nir, stl, out, cmp);
	out << "size after inserts: " << nir.size() << endl;
	cmp << "size after inserts: " << stl.size() << endl;
	if (nir.capacity() != 3 * (9 + extra.size()) / 2)
	{ out << "wrong capC" << endl; }
	out << "erase many: " << *nir.erase(nir.begin() + 3, nir.begin() + 5) << endl;
	cmp << "erase many: " << *stl.erase(stl.begin() + 3, stl.begin() + 5) << endl;
	out << "size after erases: " << nir.size() << endl;
	cmp << "size after erases: " << stl.size() << endl;
	printVectors(nir, stl, out, cmp);
	out << "insert 46 at " << nir.size() - 3 << " ref: " << *nir.insert(nir.end() - 3, 46) << endl;
	cmp << "insert 46 at " << stl.size() - 3 << " ref: " << *stl.insert(stl.end() - 3, 46) << endl;
	printVectors(nir, stl, out, cmp);
	out << "erase from " << nir.size() - 5 << " ref: " << *nir.erase(nir.end() - 5) << endl;
	cmp << "erase from " << stl.size() - 5 << " ref: " << *stl.erase(stl.end() - 5) << endl;
	printVectors(nir, stl, out, cmp);
	cmp.close();
	VLVector<int> nir2 = nir;
	for (int l = 0; l < nir.size(); ++l)
	{
		if (nir2[l] != nir[l])
		{
			out << "bad assignment operator" << endl;
		}
	}
	VLVector<int> nir3(stl.begin(), stl.end());
	for (int l = 0; l < nir3.size(); ++l)
	{
		if (nir3[l] != stl[l])
		{
			out << "bad iterator constructor" << endl;
		}
	}
	out.close();
}

void printStringVectors(VLVector<string> &nir, vector<string> &stl, ofstream &out, ofstream &cmp)
{
	out << "vector: " << endl;
	for (auto const &item : nir)
	{
		out << item << '\t';
	}
	cmp << "vector: " << endl;
	for (auto const &item : stl)
	{
		cmp << item << '\t';
	}
	out << endl;
	cmp << endl;
}

void stringTest()
{
	ofstream out("../test\\out\\string.out"), cmp("../test\\cmp\\string.cmp");
	string line, extraS, temp;
	VLVector<string> nir;
	vector<string> stl, extra;
	out << "is Empty? " << nir.empty() << endl;
	cmp << "is Empty? " << stl.empty() << endl;
	try {nir.at(3); out << "wrong index error wasn't thrown" << endl;} catch (exception& e) {}
	if (nir.capacity() != DEFAULT_STATIC_CAPACITY) {out << "wrong static capacity" << endl;}
	cout << "####string####\ntesting push_back and pop_back and indexing" << endl;
	ifstream stringFile("../test\\in\\string.in");
	int count = 1;
	while (getline(stringFile, line))
	{
		line.pop_back();  //removing /r
		nir.push_back(temp);
		stl.push_back(line);
		if (count % 100 == 0)
		{
			int indexes[5];
			out << "size after " << count << " insertions: " << nir.size() << std::endl;
			cmp << "size after " << count << " insertions: " << stl.size() << std::endl;
			for (int j = 0; j < 5; ++j)
			{
				indexes[j] = distr(generator) % count;
			}
			char buff[50];
			snprintf(buff, 50, "values at %d, %d, %d, %d, %d are (concatenated) ",
					 indexes[0], indexes[1], indexes[2], indexes[3], indexes[4]);
			out << buff << nir[indexes[0]] << nir.at(indexes[1]) << nir[indexes[2]] <<
					 nir.at(indexes[3]) << nir[indexes[4]] << endl;
			cmp << buff << stl[indexes[0]] << stl.at(indexes[1]) << stl[indexes[2]] <<
				stl.at(indexes[3]) << stl[indexes[4]] << endl;
		}
		if (count % 50 == 0)
		{
			extra.push_back(line + "##");
			extraS += extra[extra.size() - 1] += '\t';
		}
		count++;
	}

	for (int k = 0; k < POPS; ++k)
	{
		nir.pop_back();
		stl.pop_back();
	}
	out << "size after 490 pop_backs: " << nir.size() << std::endl;
	cmp << "size after 490 pop_backs: " << stl.size() << std::endl;
	printStringVectors(nir, stl, out, cmp);
	cout << "testing erase and insert, each one causes heap<>stack transforms" << endl;
	out << "insert vector " << extraS << "at position " << nir.size() - 2 << endl;
	out << "output reference to " << *nir.insert(nir.end() -2,extra.begin(), extra.end()) << endl;
	cmp << "insert vector " << extraS << "at position " << stl.size() - 2 << endl;
	cmp << "output reference to " << *stl.insert(stl.end() -2,extra.begin(), extra.end()) << endl;
	printStringVectors(nir, stl, out, cmp);
	out << "size after inserts: " << nir.size() << endl;
	cmp << "size after inserts: " << stl.size() << endl;
	if (nir.capacity() != 3 * (10 + extra.size()) / 2) {out << "wrong capC" << endl;}
	out << "erase many: " << *nir.erase(nir.begin() + 3, nir.begin() + 5) << endl;
	cmp << "erase many: " << *stl.erase(stl.begin() + 3, stl.begin() + 5) << endl;
	out << "size after erases: " << nir.size() << endl;
	cmp << "size after erases: " << stl.size() << endl;
	printStringVectors(nir, stl, out, cmp);
	out << "insert PO at " << nir.size() - 3 << " ref: " << *nir.insert(nir.end() - 3, "PO") << endl;
	cmp << "insert PO at " << stl.size() - 3 << " ref: " << *stl.insert(stl.end() - 3, "PO") << endl;
	printStringVectors(nir, stl, out, cmp);
	out << "erase from " << nir.size() - 5 << " ref: "<< *nir.erase(nir.end() - 5) << endl;
	cmp << "erase from " << stl.size() - 5 << " ref: "<< *stl.erase(stl.end() - 5) << endl;
	printStringVectors(nir, stl, out, cmp);
	out.close();
	cmp.close();
}

void stringTest2()
{
	ofstream out("../test\\out\\string.out"), cmp("../test\\cmp\\string.cmp");
	string extraS, *source = new string[17] {"nir", "is", "the", "writer", "of", "this",
											 "tester",
										 "and", "have", "to", "make", "up", "seventeen",
										 "strings", "for", "the", "array"};
	string *toExtra = new string [5] {"this#", "is#", "for#", "extra#", "array#"};
	VLVector<string> nir;
	vector<string> stl, extra;
	out << "is Empty? " << nir.empty() << endl;
	cmp << "is Empty? " << stl.empty() << endl;
	try {nir.at(3); out << "wrong index error wasn't thrown" << endl;} catch (exception& e) {}
	if (nir.capacity() != DEFAULT_STATIC_CAPACITY) {out << "wrong static capacity" << endl;}
	cout << "####string####\ntesting push_back and pop_back and indexing" << endl;
	for (int i = 0; i < 17; i++)
	{
		nir.push_back(source[i]);
		stl.push_back(source[i]);
	}
	int indexes[5];
	out << "size after " << 17 << " insertions: " << nir.size() << std::endl;
	cmp << "size after " << 17 << " insertions: " << stl.size() << std::endl;
	for (int j = 0; j < 5; ++j)
	{
		indexes[j] = distr(generator) % 17;
	}
	char buff[50];
	snprintf(buff, 50, "values at %d, %d, %d, %d, %d are (concatenated) ",
			 indexes[0], indexes[1], indexes[2], indexes[3], indexes[4]);
	out << buff << nir[indexes[0]] << nir.at(indexes[1]) << nir[indexes[2]] <<
		nir.at(indexes[3]) << nir[indexes[4]] << endl;
	cmp << buff << stl[indexes[0]] << stl.at(indexes[1]) << stl[indexes[2]] <<
		stl.at(indexes[3]) << stl[indexes[4]] << endl;
	for (int i = 0; i < 5; i++)
	{
		extra.push_back(toExtra[i]);
		extraS += extra[extra.size() - 1] += '\t';
	}
	for (int k = 0; k < 4; ++k)
	{
		nir.pop_back();
		stl.pop_back();
	}
	out << "size after 4 pop_backs: " << nir.size() << std::endl;
	cmp << "size after 4 pop_backs: " << stl.size() << std::endl;
	printStringVectors(nir, stl, out, cmp);
	cout << "testing erase and insert, each one causes heap<>stack transforms" << endl;
	out << "insert vector " << extraS << "at position " << nir.size() - 2 << endl;
	out << "output reference to " << *nir.insert(nir.end() -2,extra.begin(), extra.end()) << endl;
	cmp << "insert vector " << extraS << "at position " << stl.size() - 2 << endl;
	cmp << "output reference to " << *stl.insert(stl.end() -2,extra.begin(), extra.end()) << endl;
	printStringVectors(nir, stl, out, cmp);
	out << "size after inserts: " << nir.size() << endl;
	cmp << "size after inserts: " << stl.size() << endl;
	if (nir.capacity() != 3 * (13 + extra.size()) / 2) {out << "wrong capC" << endl;}
	out << "erase many: " << *nir.erase(nir.begin() + 3, nir.begin() + 5) << endl;
	cmp << "erase many: " << *stl.erase(stl.begin() + 3, stl.begin() + 5) << endl;
	out << "size after erases: " << nir.size() << endl;
	cmp << "size after erases: " << stl.size() << endl;
	printStringVectors(nir, stl, out, cmp);
	out << "insert PO at " << nir.size() - 3 << " ref: " << *nir.insert(nir.end() - 3, "PO") << endl;
	cmp << "insert PO at " << stl.size() - 3 << " ref: " << *stl.insert(stl.end() - 3, "PO") << endl;
	printStringVectors(nir, stl, out, cmp);
	out << "erase from " << nir.size() - 5 << " ref: "<< *nir.erase(nir.end() - 5) << endl;
	cmp << "erase from " << stl.size() - 5 << " ref: "<< *stl.erase(stl.end() - 5) << endl;
	printStringVectors(nir, stl, out, cmp);
	out.close();
	cmp.close();
	delete[] source, extra;
}

int main()
{
	intTest();
	stringTest2();
}
