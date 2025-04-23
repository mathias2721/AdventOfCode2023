#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>


int main()
{
    std::string file_name = "input.txt";
    std::ifstream ifs(file_name);
    if(!ifs.is_open())
        return 0;

    enum class State
    {
        START,
        M,
        MU,
        MUL,
        MUL_P,
        FIRST_NUM, 
        COMMA, 
        SECOND_NUM,
        VALID
    };

    State state = State::START;
    std::string num1, num2;

    int result = 0;

    char c;
    while(ifs.get(c))
    {
        switch(state)
        {
            case State::START:
                state = (c == 'm') ? State::M : State::START;
                break;
            case State::M:
                state = (c == 'u') ? State::MU : State::START;
                break;
            case State::MU:
                state = (c == 'l') ? State::MUL : State::START;
                break;
            case State::MUL:
                state = (c == '(') ? State::MUL_P : State::START;
                break;

            case State::MUL_P:
                num1 = c;
                state = (std::isdigit(c)) ? State::FIRST_NUM : State::START;
                break;

            case State::FIRST_NUM:
                if(c == ',' && num1.size()<=3)
                {
                    state = State::COMMA;
                }
                else if(std::isdigit(c) && num1.size()<3)
                {
                    num1 += c;
                    state = State::FIRST_NUM;
                }
                else
                    state = State::START;
                break;

            case State::COMMA:
                num2 = c;
                state = (std::isdigit(c)) ? State::SECOND_NUM : State::START;
                break;

            case State::SECOND_NUM:
                // Valid string
                if(c == ')' && num2.size()<=3)
                {
                    result += std::stoi(num1) * std::stoi(num2);
                    state = State::START;
                }
                else if(std::isdigit(c) && num2.size()<3)
                {
                    num2 += c;
                    state = State::SECOND_NUM;
                }
                else
                    state = State::START;
                break;
        }        
    }

    std::cout << "The result is: " << result << std::endl;
}
