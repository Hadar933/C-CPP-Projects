
#include "RecommenderSystem.h"

bool gFirstLine = true;

// --------------------------------- PARSING FILES ----------------------------------------------//
/**
 * parses each line, skipping spaces and inserting the values to provided vector
 * @param line some line to parse
 * @param vec vector of string to insert line values to
 */
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

/**
 * parses each line, skipping spaces and inserting the values to provided vector
 * @param line some line to parse
 * @param vec vector of ints to insert line values to
 */
void RecommenderSystem::_parseEachLine(const string &line, vector<double> &vec)
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

/**
 * fetches the first word in a line, which is either a userName or a movieName
 * @param line some line to parse
 * @return first word in line
 */
string RecommenderSystem::_popFirstWord(string &line)
{
	string word = line.substr(START, line.find_first_of(" \t"));
	line = line.substr(line.find_first_of(" \t"));
	return word;
}

/**
 * parses one of the files
 * @param filePath - path to file to parse
 * @param isUserRankFile - represents that the file is the user ranks
 * @return
 */
bool RecommenderSystem::_parseFile(const string &filePath, bool isUserRankFile)
{
	std::ifstream file;
	file.open(filePath);
	if (!file.good())
	{
		std::cout << BAD_PATH << filePath << std::endl;
		file.close();
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
			vector<double> tempVec;
			_parseEachLine(line, tempVec);
			isUserRankFile ? _userRankingsMap[key] = tempVec : _movieTraitsMap[key] = tempVec;

		}
	}
	gFirstLine = true;
	return SUCCESS;
}

/**
 * loads the data to our recommendation system
 * @param moviesAttributesFilePath - filepath to movies attributes matrix
 * @param userRanksFilePath - filepath to user ranks matrix
 * @return 0 upon success, 1 otherwise.
 */
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
/**
 *generates a recommendation based on the content
 * @param userName - customer's user name
 * @return string - representing the recommended movie returned by the algorithm
 */
string RecommenderSystem::recommendByContent(const string &userName)
{
	if (_userRankingsMap.find(userName) == _userRankingsMap.end())
	{
		return BAD_USER;
	}
	vector<double> userRanks = _userRankingsMap[userName];
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
	vector<double> prefVec = _generatePrefVec(userRanks);
	// PART III - find the best fitted movie
	return _findResemblance(prefVec, userRanks);
}

/**
 * calculates the average value of all items in vector
 * @param vec some vector of numbers
 * @return
 */
double RecommenderSystem::_getAverage(const vector<double> &vec)
{
	double sum = std::accumulate(vec.begin(), vec.end(), DOUBLE_INDICATOR);
	double size = 0;
	for (auto &element : vec)
	{ // get size by ignoring NA values
		if (element != 0)
		{
			size++;
		}
	}
	return sum / size;
}

/**
 * generates a preferences vector for some user according to the algorithm
 * @param normVec vector of normalized values
 * @return vector of preferences
 */
vector<double> RecommenderSystem::_generatePrefVec(const vector<double> &normVec)
{
	int size = _movieTraitsMap[_movieNames[START]].size();
	vector<double> resVec(size);
	for (vector<double>::size_type i = 0; i != normVec.size(); i++)
	{
		double weight = normVec[i];
		if (weight != 0)
		{
			string movieName = _movieNames[i];
			vector<double> tempVec = _movieTraitsMap[movieName];
			_multByConst(weight, tempVec);
			_addUpVects(resVec, tempVec);
		}
	}
	return resVec;
}

/**
 * for each movie a user hasent watched, calculates similarity, and extracts the movie
 * with the greatest resemblance
 * @param prefVec - preferences vector as calculated in generatePrefVec function
 * @param userRanks - the ranks a user has given to all movies (NA for which he hasnt watched)
 * @return name of the movie with the biggest resemblance
 */
string RecommenderSystem::_findResemblance(vector<double> &prefVec, vector<double> &userRanks)
{
	double maxTheta = -DBL_MAX;
	string bestFitMovie = " ";
	for (vector<double>::size_type i = 0; i != userRanks.size(); i++)
	{
		if (userRanks[i] == NA)
		{
			string movieName = _movieNames[i];
			vector<double> movieTraits = _movieTraitsMap[movieName];
			double theta = _compAngle(prefVec, movieTraits);
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
/**
 * generates a prediction for a user's rank to a movie he hasn't seen yet, based
 * on the movies the user has already seen
 * @param movieName - movie to predict a rating for
 * @param userName - name of the customer
 * @param k - number of movies that are most similar to movieName
 * @return -1 if no such user or movie, float predicted rating otherwize
 */
double RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &userName,
												   int k)
{
	if (std::find(_movieNames.begin(), _movieNames.end(), movieName) == _movieNames.end() ||
		_userRankingsMap.find(userName) == _userRankingsMap.end())
	{ // no such movie or no such user name
		return INVALID;
	}
	vector<std::pair<std::pair<string, double>, double> > movieResem; // ((movie:index):resemb
	vector<std::pair<string, double>> watched = _didWatch(const_cast<string &>(userName));
	vector<double> watchedMovieTraits = _movieTraitsMap[movieName];
	// iterate over all watched movies and find resemblance to movieName
	for (auto &movie : watched)
	{
		vector<double> currMovieTraits = _movieTraitsMap[movie.first];
		double resemblance = _compAngle(watchedMovieTraits, currMovieTraits);
		movieResem.emplace_back(movie, resemblance);
	}
	return _forecastRating(movieResem, const_cast<string &>(userName), k);


}

/**
 * a helper function for built in sort, to sort by second element of pair in descending order
 * @param a some pair
 * @param b yet another pair
 * @return boolean value
 */
bool RecommenderSystem::_sortBySecond(const std::pair<std::pair<string, double>, double> &a,
									  const std::pair<std::pair<string, double>, double> &b)
{
	return a.second > b.second;
}

/**
 * forcasts a user rating for some movie using the algorithm provided.
 * @param dataVec - vector of pairs <movieName:resemblance>
 * @param k # of movies that are most similar
 * @return focasted rating of movieName
 */
double
RecommenderSystem::_forecastRating(vector<std::pair<std::pair<string, double>, double> > &dataVec,
								   string &userName, int k)
{
	sort(dataVec.begin(), dataVec.end(), _sortBySecond); //sorting data vec according to second
	// value of pair

	double numerator = 0; // sum_(j=0 to k) [ s_(mj) * r_(xj) ]
	double denominator = 0; // sum_(j=0 to k) [ s_(mj) ]
	for (int j = 0; j < k; j++) // we can assume k < size of vector
	{
		double resemblance = dataVec[j].second;
		string movieName = dataVec[j].first.first;
		int indexOfMovie = dataVec[j].first.second;
		double specificRating = _userRankingsMap[userName].at(indexOfMovie);
		denominator += resemblance;
		numerator += dataVec[j].second * specificRating;
	}
	return numerator / denominator;
}

//-------------------------------------recommend by cf-------------------------------------------//
/**
 * recommends a movie to the user based on the predictMovieScoreForUser algorithm
 * @param userName - name of the customer
 * @param k - number of movies that are most similar to movieName
 * @return string representing the name of the recommended movie
 */
string RecommenderSystem::recommendByCF(const string &userName, int k)
{
	if (_userRankingsMap.find(userName) == _userRankingsMap.end())
	{
		return BAD_USER;
	}
	vector<std::pair<string, double>> results;
	for (auto &movie: _didNotWatch(const_cast<string &>(userName)))
	{
		double resemb = predictMovieScoreForUser(movie, userName, k);
		results.emplace_back(movie, resemb);
	}
	return _getMovieWithMaxResemblance(results);
}

//----------------------------------utility methods----------------------------------------------//
/**
 * multiplies each element of vec by val(some const)
 */
void RecommenderSystem::_multByConst(double val, vector<double> &vec)
{
	for (auto &element : vec)
	{
		element *= val;
	}
}

/**
 * adds other vec to vec (same size vectors)
 */
void RecommenderSystem::_addUpVects(vector<double> &vec, const vector<double> &other)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		vec[i] += other[i];
	}
}

/**
 * computes dot product of two vectors of same length
 * @param vec1
 * @param vec2
 * @return dot product v1*v2
 */
double RecommenderSystem::_dotProduct(const vector<double> &vec1, const vector<double> &vec2)
{
	double product = 0;
	for (size_t i = 0; i < vec1.size(); i++)
	{
		product = product + (vec1[i] * vec2[i]);
	}
	return product;
}

/**
 * calculates a norma of vector: sqrt(v[i]^2+...+v[n]^2)
 * @param vec some vector
 * @return norma
 */
double RecommenderSystem::_norm(vector<double> &vec)
{
	double normSquared = 0;
	for (auto &element : vec)
	{
		normSquared += element * element;
	}
	return sqrt(normSquared);
}

/**
 * calculate the similarity between two vectors, (the argument of arccos)
 * @param vec1
 * @param vec2
 * @return some 0<=value<=1, where 0 means no similarity and 1 means best similarity
 */
double RecommenderSystem::_compAngle(vector<double> &vec1, vector<double> &vec2)
{
	return (_dotProduct(vec1, vec2) * (1 / (_norm(vec1) * _norm(vec2))));
}

vector<std::pair<string, double>> RecommenderSystem::_didWatch(string &username)
{
	vector<std::pair<string, double>> didWatch;
	vector<double> userRanks = _userRankingsMap[username];
	for (size_t i = 0; i < userRanks.size(); i++)
	{
		if (userRanks[i] != 0)
		{
			string movieName = _movieNames[i];
			didWatch.emplace_back(movieName, i); // saving the movie name and the index in movie
			// names for future reach
		}
	}
	return didWatch;

}

/**
 * finds our which movie a username has not watched (all NA values)
 * @param username some customer
 * @return vector of string representing movies the username did watch
 */
vector<string> RecommenderSystem::_didNotWatch(string &username)
{
	vector<string> didntWatch;
	vector<double> userRanks = _userRankingsMap[username];
	for (size_t i = 0; i < userRanks.size(); i++)
	{
		if (userRanks[i] == 0)
		{
			string movieName = _movieNames[i];
			didntWatch.push_back(movieName);
		}
	}
	return didntWatch;
}

/**
 * iterates over a vector of pairs, find the largest resemblance (Vec.second)
 * and returns the corresponding movie name (vec.first)
 * @param data some vector with pairs elements (moviename:resemblance)
 * @return movie with the max resemblance
 */
string RecommenderSystem::_getMovieWithMaxResemblance(vector<std::pair<string, double>> &data)
{
	double maxResemb = 0;
	string outputMovie = " ";
	for (auto &pair: data)
	{
		if (pair.second > maxResemb)
		{
			maxResemb = pair.second;
			outputMovie = pair.first;
		}
	}
	return outputMovie;
}

int main()
{
	string moviePath = "movies_big.txt";
	string ranksPath = "ranks_big.txt";
	RecommenderSystem obj;
	obj.loadData(moviePath, ranksPath);
	std::cout << obj.recommendByContent("Molly") << std::endl;
	//TODO theres an extra row in output file, when compared to cmp files. da fuck?

}

