#include <iostream>
#include <unordered_map>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


std::unordered_map<int, std::set<int>> rules;
std::vector<std::vector<int>> pages;

void parse(const char* file_name)
{
    std::ifstream ifs(file_name);
    std::string line;
    std::getline(ifs, line);

    while(!line.empty())
    {
        int left = std::stoi(line.substr(0, line.find("|")));
        int right = std::stoi(line.substr(line.find("|")+1));

        rules[left].insert(right);

        std::getline(ifs, line);
    }

    while( std::getline(ifs, line) )
    {
        pages.push_back(std::vector<int>());
        std::stringstream ss(line);
        for (std::string num; std::getline(ss, num, ',');)
        {
            pages.back().push_back(std::stoi(num));
        }
    }
}

bool check(int num, std::vector<int>::iterator beg,const std::vector<int>::iterator& end)
{
    if (beg == end)
        return true;
    else
    {
        for (auto it = beg; it!=end; it++)
        {
            // num is not in the rules of *beg --> perfect
            if(rules[*it].find(num) != rules[*it].end())
            {
                return false;
            }
        }
        
        num = *beg;
        return check(num, ++beg, end);
    }
}

void reOrder(std::vector<int>& line)
{
    std::vector<int> res;

    auto sorting = [&](int a, int b){
        return (rules[b].find(a) == rules[b].end());
    };

    std::sort(line.begin(), line.end(), sorting);

}

int main()
{
    parse("input.txt");
    int sum = 0;

    std::vector<std::vector<int>> incorrects;
    for (auto& line : pages)
    {
        auto beg = line.begin();
        if(check(*beg, beg+1, line.end()))
        {
            sum += *(beg + (std::distance(beg, line.end()) / 2));
        }
        else
        {
            incorrects.push_back(line);
        }
    }

    int bad_sum = 0;
    for (auto& line: incorrects)
    {
        reOrder(line);
        bad_sum += line[line.size()/2];
    }

    std::cout << "Part 1 : " << sum << std::endl;
    std::cout << "Part 2 : " << bad_sum << std::endl;
    return 0;
}


