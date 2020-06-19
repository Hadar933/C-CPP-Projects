//
// Created by hadar on 18-Jun-20.
//

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include <numeric>
#include <algorithm>
#include <cmath>

#define SUCCESS 0
#define FAILURE 1
#define INVALID (-1)
#define BAD_PATH "Unable to open file "
#define BAD_USER "USER NOT FOUND"
#define NA 0
#define START 0

using std::string;
using std::cerr;
using std::endl;
using std::vector;

/**
 *based on movie attributes file and a user ranks file, the class is responsible
 * of generating relevant recommendations to the user, in multiple ways.
 */
class RecommenderSystem
{
public:
	/**
	 * loads the data to our recommendation system
	 * @param moviesAttributesFilePath - filepath to movies attributes matrix
	 * @param userRanksFilePath - filepath to user ranks matrix
	 * @return 0 upon success, 1 otherwise.
	 */
	static bool loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath);


	/**
	 *generates a recommendation based on the content
	 * @param username - customer's user name
	 * @return string - representing the recommended movie returned by the algorithm
	 */
	string recommendByContent(const string &username);

	/**
	 * generates a prediction for a user's rank to a movie he hasn't seen yet, based
	 * on the movies the user has already seen
	 * @param movieName - movie to predict a rating for
	 * @param userName - name of the customer
	 * @param k - number of movies that are most similar to movieName
	 * @return -1 if no such user or movie, float predicted rating otherwize
	 */
	double predictMovieScoreForUser(string movieName, string userName, int k);

	/**
	 * recommends a movie to the user based on the predictMovieScoreForUser algorithm
	 * @param userName - name of the customer
	 * @param k - number of movies that are most similar to movieName
	 * @return string representing the name of the recommended movie
	 */
	string recommendByCF(string userName, int k);


private:
	inline static vector<string> _movieNamesVec;
	inline static std::map<string, vector<int>> _movieTraitsMap; // movieName:traits vec(scary,
	// funny etc..)
	inline static std::map<string, vector<int>> _userRankingsMap;
	// userName:vector of (movie:rank) pairs

	/**
	 * parses one of the files
	 * @param filePath - path to file to parse
	 * @param isUserRankFile - represents that the file is the user ranks
	 * @return
	 */
	static bool _parseFile(const string &filePath, bool isUserRankFile);

	/**
	 * parses each line, skipping spaces and inserting the values to provided vector
	 * @param line some line to parse
	 * @param vec vector of ints to insert line values to
	 */
	static void _parseEachLine(const string &line, vector<int> &vec);

	/**
	 * parses each line, skipping spaces and inserting the values to provided vector
	 * @param line some line to parse
	 * @param vec vector of string to insert line values to
	 */
	static void _parseStringLine(const string &line, vector<string> &vec);

	/**
	 * fetches the first word in a line, which is either a userName or a movieName
	 * @param line some line to parse
	 * @return first word in line
	 */
	static string _popFirstWord(string &line);

	/**
	 * calculates the average value of all items in vector
	 * @param vec some vector of numbers
	 * @return
	 */
	static double _getAverage(const vector<int> &vec);

	static vector<int> generatePrefVec(const vector<int> &normVec);

	/**
	 * multiplies each element of vec by val(some const)
	 */
	static void _multByConst(int val, vector<int> &vec);

	/**
	 * adds other vec to vec (same size vectors)
	 */
	static void _addUpVects(vector<int> &vec, const vector<int> &other);

	/**
	 * computes dot product of two vectors of same length
	 * @param vec1
	 * @param vec2
	 * @return dot product v1*v2
	 */
	static int dotProduct(vector<int> &vec1, const vector<int> &vec2);

	/**
	 * calculates a norma of vector: sqrt(v[i]^2+...+v[n]^2)
	 * @param vec some vector
	 * @return norma
	 */
	static double norm(vector<int> &vec);

	static double compAngle(vector<int> &vec1, vector<int> &vec2);

	static string findResemblance(vector<int> &prefVec, vector<int> &userRanks);


};


#endif //EX5_RECOMMENDERSYSTEM_H
