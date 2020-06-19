
#include "RecommenderSystem.h"

bool gFirstLine = true;

// --------------------------------- PARSING FILES ----------------------------------------------//

void RecommenderSystem::_parseStringLine(const string &line, vector<string> &vec)
{
	std::istringstream iss(line);
	do
	{// reading the word
		string word;
		iss >> word;
		//adding the word to a provided vector
		if (!word.empty())
		{
			vec.push_back(word);
		}
	} while (iss); // repeat for all words in line
}


void RecommenderSystem::_parseEachLine(const string &line, vector<int> &vec)
{
	std::istringstream iss(line);
	do
	{// reading the word
		string word;
		iss >> word;
		//adding the word to a provided vector
		if (!word.empty())
		{
			word == "NA" ? vec.push_back(NA) : vec.push_back(std::stoi(word)); // NA will 0
		}
	} while (iss); // repeat for all words in line

}

string RecommenderSystem::_popFirstWord(string &line)
{
	string word = line.substr(START, line.find_first_of(" \t"));
	line = line.substr(line.find_first_of(" \t"));
	return word;
}

bool RecommenderSystem::_parseFile(const string &filePath, bool isUserRankFile)
{
	std::ifstream file;
	file.open(filePath);
	if (!file.good())
	{
		return INVALID;
	}
	string line;
	while (std::getline(file, line))
	{
		if (gFirstLine && isUserRankFile)
		{ // first line represents movies names (only in user ranks file)
			_parseStringLine(line, _movieNames);
			gFirstLine = false;
		}
		else
		{ // each of the following lines will be added as key:value
			string key = _popFirstWord(line); //movieName of userName
			vector<int> tempVec;
			_parseEachLine(line, tempVec);
			isUserRankFile ? _userRankingsMap[key] = tempVec : _movieTraitsMap[key] = tempVec;
		}
	}
	return SUCCESS;
}

bool
RecommenderSystem::loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath)
{
	bool cond1 = _parseFile(moviesAttributesFilePath, false);
	bool cond2 = _parseFile(userRanksFilePath, true);
	if (cond1 == SUCCESS && cond2 == SUCCESS)
	{
		return SUCCESS;
	}
	return FAILURE;
}

//-------------------------------recommend by content--------------------------------------------//

string RecommenderSystem::recommendByContent(const string &username)
{
	//TODO if username not found - return USER NOT FOUND
	vector<int> userRanks = _userRankingsMap[username];
	// PART I - normalize
	double avg = _getAverage(userRanks);
	for (auto &element : userRanks)
	{
		if (element != 0)
		{
			element -= avg;
		}
	}
	//PART II - get preference vector
	vector<int> prefVec = generatePrefVec(userRanks);
	// PART III - find the best fitted movie
	return findResemblance(prefVec, userRanks);

}

double RecommenderSystem::_getAverage(const vector<int> &vec)
{
	double sum = std::accumulate(vec.begin(), vec.end(), START);
	int size = 0;
	for (auto &element : vec)
	{ // get size by ignoring NA values
		if (element != 0)
		{
			size++;
		}
	}
	return sum / size;
}

vector<int> RecommenderSystem::generatePrefVec(const vector<int> &normVec)
{
	vector<int> resVec(normVec.size() - 1);
	for (vector<int>::size_type i = 0; i != normVec.size(); i++)
	{
		int weight = normVec[i];
		string movieName = _movieNames[i];
		vector<int> tempVec = _movieTraitsMap[movieName];
		_multByConst(weight, tempVec);
		_addUpVects(resVec, tempVec);
	}
	return resVec;
}

string RecommenderSystem::findResemblance(vector<int> &prefVec, vector<int> &userRanks)
{
	double maxTheta = 0;
	string bestFitMovie = " ";
	for (vector<int>::size_type i = 0; i != userRanks.size(); i++)
	{
		if (userRanks[i] == NA)
		{
			string movieName = _movieNames[i];
			vector<int> movieTraits = _movieTraitsMap[movieName];
			double theta = compAngle(prefVec, movieTraits);
			if (theta > maxTheta)
			{
				maxTheta = theta;
				bestFitMovie = movieName;
			}
		}
	}
	return bestFitMovie;
}

//-------------------------------predict movie score for user -----------------------------------//

double RecommenderSystem::predictMovieScoreForUser(string &movieName, string &userName, int k)
{
	if (std::find(_movieNames.begin(), _movieNames.end(), movieName) == _movieNames.end() ||
		_userRankingsMap.find(userName) == _userRankingsMap.end())
	{ // no such movie or no such user name
		return INVALID;
	}
	vector< std::pair< string,double> > movieResem;
	vector<string> watched = didWatch(userName);
	vector<int> watchedMovieTraits = _movieTraitsMap[movieName];
	// iterate over all watched movies and find resemblance to movieName
	for (auto &movie : watched)
	{
		vector<int>  currMovieTraits = _movieTraitsMap[movie];
		double resemblance = compAngle(watchedMovieTraits,currMovieTraits);
		movieResem.emplace_back(movie, resemblance);
	}
	forecastRating(movieResem,k);


}
double RecommenderSystem::forecastRating(vector<std::pair<string,double>> &dataVec, int k)
{
	//TODO - after finding all resemblances to some movieName in the prev function,
	// we've added it all to a vector of pairs <moviename,resemblance>. the next thing that
	// needs to be done is to sort it and iterate over k elements accurding to the sum alrogithm
	// provided on page 6 (stage 2)

}










//----------------------------------utility methods----------------------------------------------//

void RecommenderSystem::_multByConst(int val, vector<int> &vec)
{
	for (auto &element : vec)
	{
		element *= val;
	}
}

void RecommenderSystem::_addUpVects(vector<int> &vec, const vector<int> &other)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		vec[i] += other[i];
	}
}

int RecommenderSystem::dotProduct(vector<int> &vec1, const vector<int> &vec2)
{
	int product = 0;
	for (size_t i = 0; i < vec1.size(); i++)
	{
		product += vec1[i] * vec2[i];
	}
	return product;

}

double RecommenderSystem::norm(vector<int> &vec)
{
	double normSquared = 0;
	for (auto &element : vec)
	{
		normSquared += element * element;
	}
	return sqrt(normSquared);
}

double RecommenderSystem::compAngle(vector<int> &vec1, vector<int> &vec2)
{
	return (dotProduct(vec1, vec2) / (norm(vec1) * norm(vec2)));
}

vector<string> RecommenderSystem::didWatch(string &username)
{
	vector<string> didWatch;
	vector<int> userRanks = _userRankingsMap[username];
	for (size_t i = 0; i < userRanks.size(); i++)
	{
		if (userRanks[i] != 0)
		{
			string movieName = _movieNames[i];
			didWatch.push_back(movieName);
		}
	}
	return didWatch;

}

vector<string> RecommenderSystem::didNotWatch(string &username)
{
	vector<string> didNotWatch;
	vector<int> userRanks = _userRankingsMap[username];
	for (size_t i = 0; i < userRanks.size(); i++)
	{
		if (userRanks[i] == 0)
		{
			string movieName = _movieNames[i];
			didNotWatch.push_back(movieName);
		}
	}
	return didNotWatch;
}




int main()
{
	RecommenderSystem obj;
	obj.loadData("movies_features.txt", "ranks_matrix.txt");
	obj.recommendByContent("Sofia");


}