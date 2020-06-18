#include <fstream>
#include <filesystem>
#include "RecommenderSystem.h"

bool gFirstLine = true;


bool
RecommenderSystem::loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath)
{
}

bool RecommenderSystem::parseFile(const string &filePath, bool isUserRank)
{
	std::ifstream file;
	file.open(filePath);
	int fileSize = _getFileSize(file);
	string line;
	while (std::getline(file, line))
	{
		if (gFirstLine && isUserRank)
		{ // first line represents movies names (only in user ranks file)
			parseEachLine(line,_movieNamesVec);
			gFirstLine = false;
		}
		else
		{
		}

	}
}


int RecommenderSystem::_getFileSize(std::ifstream &file)
{
	file.seekg(0, std::ios::end);
	std::streampos length = file.tellg();
	file.seekg(0, std::ios::beg);
	return length;

}

void RecommenderSystem::parseEachLine(string &line, std::vector<string> &vec)
{
	std::istringstream iss(line);
	do
	{
		// reading the word
		string word;
		iss >> word;
		//adding the word to a provided vector
		vec.push_back(word);
	} while (iss); // repeat for all words in line

}

int main()
{
	RecommenderSystem obj;
	obj.parseFile("ranks_matrix.txt",true);
}