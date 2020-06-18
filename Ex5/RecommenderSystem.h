//
// Created by hadar on 18-Jun-20.
//

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

#include <string>
#include <iostream>
#include <vector>

#define SUCCESS 0
#define FAILURE 1
#define INVALID -1
#define BAD_PATH "Unable to open file "
#define BAD_USER "USER NOT FOUND"

using std::string;
using std::cerr;
using std::endl;

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
	 * parses one of the files
	 * @param filePath - path to file to parse
	 * @param isUserRank - represents that the file is the user ranks
	 * @return
	 */
	bool parseFile(const string &filePath, bool isUserRank);

	bool parseFeatures(const string &moviesAttributesFilePath);

	/**
	 *generates a recommendation based on the content
	 * @param username - customer's user name
	 * @return string - representing the recommended movie returned by the algorithm
	 */
	std::string recommendByContent(char *username);

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
	std::string recommendByCF(string userName, int k);

private:
	/**
	 * returns a file's size (assume file exists)
	 * @param file some file objecy
	 * @return file's size
	 */
	static int _getFileSize(std::ifstream &file);

	static void parseEachLine(string &line, std::vector<string> &vec);

	std::vector<string> _movieNamesVec;

};


#endif //EX5_RECOMMENDERSYSTEM_H
