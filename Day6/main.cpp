#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

enum class MAP_TYPE
{
    NEUTRAL,
    VISITED,
    OBSTACLE,
};

class Guard
{
    public:
    enum class DIR
    {
        UP, RIGHT, DOWN, LEFT
    };
    
    public:
    Guard() = default;
    Guard(int r, int c): row(r), col(c){}

    bool visit_next(std::vector<std::vector<MAP_TYPE>>& map);
    std::pair<int, int> next_tile();
    int getCount() {return count;}
    int getRow() {return row;}
    int getCol() {return col;}
    DIR getDir() {return direction;}

    private:
    int row, col;
    DIR direction = DIR::UP; 
    int count = 1;
};

std::pair<int, int> Guard::next_tile()
{
    std::pair<int, int> next;
    switch(direction)
    {
        case DIR::UP:
            next = {row - 1, col};
            break;
        case DIR::RIGHT:
            next = {row, col +1};
            break;
        case DIR::DOWN:
            next = {row+1, col};
            break;
        case DIR::LEFT:
            next = {row, col - 1};
            break;
    }
    return next;
}

bool Guard::visit_next(std::vector<std::vector<MAP_TYPE>>& map)
{
    std::pair<int, int> next = next_tile();

    // Guard out of bounds
    if (next.first >= map.size() || next.first < 0 || next.second >= map[0].size() || next.second < 0)
        return false;

    else
    {
        // Next is an obstacle
        if(map[next.first][next.second] == MAP_TYPE::OBSTACLE)
        {
            direction = (Guard::DIR)(((int)direction + 1)%4); //rotate 90°
        }
        else if (map[next.first][next.second] == MAP_TYPE::NEUTRAL)
        {
            row = next.first;
            col = next.second;
            map[row][col] = MAP_TYPE::VISITED;
            count++;
        }
        else if (map[next.first][next.second] == MAP_TYPE::VISITED)
        {
            row = next.first;
            col = next.second;
        }
        return true;
    }

}

void print_map(std::vector<std::vector<MAP_TYPE>>& map);

void parse(const char* str, std::vector<std::vector<MAP_TYPE>>& map, Guard& guard)
{
    std::ifstream ifs(str);
    if(!ifs.is_open())
        return;

    std::string line;
    int row = 0;
    while(std::getline(ifs, line))
    {
        std::remove_if(line.begin(), line.end(), [](const char& c){return std::isblank(c);});

        std::vector<MAP_TYPE> map_line;
        map_line.reserve(line.size());

        for(int col=0 ; col<line.size(); col++)
        {
            switch(line[col])
            {
                case '.':
                    map_line.push_back(MAP_TYPE::NEUTRAL);
                    break;
                case '#': 
                    map_line.push_back(MAP_TYPE::OBSTACLE);
                    break;
                case '^': 
                    map_line.push_back(MAP_TYPE::VISITED);
                    guard = Guard(row, col);
                    break;
                
            }
        }

        map.push_back(map_line);

        row++;
    }

}


int main()
{
    std::vector<std::vector<MAP_TYPE>> map;
    Guard guard;
    parse("input.txt", map, guard);

    // Part 1
    while(guard.visit_next(map));
    std::cout << "The guard will visit " << guard.getCount() << " different tiles" << std::endl;


    // Part 2
    map.erase(map.begin(), map.end());
    parse("input.txt", map, guard);

    std::set<std::tuple<int, int>> path;
    
    std::set<std::pair<int, int>> add_obstacles;
    do
    {
        auto map_cpy = map;
        Guard loop_guard = guard;
        path.insert({guard.getRow(), guard.getCol()});

        auto[next_row, next_col] = loop_guard.next_tile();

        //The guard can t put obstacle go somwhere he already went (othwerise he won t beh there)
        if (next_row < map_cpy.size() && next_row >= 0 && next_col < map_cpy[0].size() && next_col >= 0 && map_cpy[next_row][next_col] != MAP_TYPE::OBSTACLE && path.find({next_row, next_col}) == path.end())
        {
            map_cpy[next_row][next_col] = MAP_TYPE::OBSTACLE;
        }
        else
        {
            continue;
        }

        std::set<std::tuple<int, int, Guard::DIR>> already_checked;

        while(loop_guard.visit_next(map_cpy))
        {
            std::tuple<int, int, Guard::DIR> current_state = {loop_guard.getRow(), loop_guard.getCol(), loop_guard.getDir()};
            

            if (already_checked.find(current_state) != already_checked.end())
            {
                add_obstacles.insert({next_row, next_col});
                break;
            }
            else
            {
                already_checked.insert(current_state);
            }
        }
    }
    while(guard.visit_next(map));

    std::cout << "You can put " << add_obstacles.size() <<" different obstacles to have infinite guard loop" << std::endl;


    return 0;
}


void print_map(std::vector<std::vector<MAP_TYPE>>& map)
{
    for(int i = 0; i< map.size(); i++)
    {
        for (int j = 0; j<map[0].size(); j++)
        {
            switch(map[i][j])
            {
                case MAP_TYPE::NEUTRAL:
                    std::cout << '.';
                    break;
                case MAP_TYPE::VISITED:
                    std::cout << 'X';
                    break;
                case MAP_TYPE::OBSTACLE:
                    std::cout << '#';
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}