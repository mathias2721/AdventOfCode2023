#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// Parsing 2 columns of int into 2 vector
std::pair<std::vector<int>, std::vector<int>> ParseFile(const std::string& filename);

// Question 1
int Distance(const std::vector<int>& list1, const std::vector<int>& list2);
// Question 2
int SimilarityScore(const std::vector<int>& list1, const std::vector<int>& list2);

int main()
{
    std::string filename = "input.txt";

    auto [list1, list2] = ParseFile(filename);

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    std::cout << "Distance : " << Distance(list1, list2) << std::endl;
    std::cout << "Similarity Score: " << SimilarityScore(list1, list2) << std::endl;
}

// Question 1
int Distance(const std::vector<int>& list1, const std::vector<int>& list2)
{
    int distance = 0;

    for (int i = 0; i<list1.size(); i++)
    {
        distance+= std::abs(list1[i] - list2[i]);
    }
    return distance;
}

// Question 2
int SimilarityScore(const std::vector<int>& list1, const std::vector<int>& list2)
{
    int score = 0;

    const auto set1 = std::set<int>(list1.begin(), list1.end());

    auto it1 = set1.cbegin();
    auto it2 = list2.cbegin();

    while(it1 != set1.cend() && it2 != list2.cend())
    {
        if (*it1 == *it2)
        {
            score+= *it2;
            it2++;
        }
        else if (*it1 > *it2)
            it2++;
        else
            it1++;
    }

    return score;
}


// Parsing
std::pair<std::vector<int>, std::vector<int>> ParseFile(const std::string& filename)
{
    std::ifstream ifs(filename);
    std::vector<int> l1, l2;

    for(std::string line; std::getline(ifs, line);)
    {
        std::stringstream ss(line);
        int v1, v2;
        ss >> v1 >> v2;
        l1.push_back(v1);
        l2.push_back(v2);
    }
    return { l1, l2 };
}