#include <iostream>
#include <vector>
#include <string>
#include <fstream>

enum class Token
{
    None,
    INSTRUCTION, // series of alphabetics or ' or (
    NUMBER, // series of digit
    CLOSE_PAR, // )
    COMMA, // ,
};

bool suffix_is(const std::string& str, const std::string& suffix);
std::vector<std::pair<std::string, Token>> TokenizeFile(const std::string& file);

int main()
{
    auto tokens = TokenizeFile("input.txt");

    enum class State
    {
        START,

        DO,
        DONT,
        MUL,

        NUM1,
        COMMA,
        NUM2,
    };

    State state = State::START;
    bool mul_enabled = true;

    int num1, num2;
    long result = 0;

    int i = 0;
    while (i < tokens.size())
    {
        const auto& [str, token] = tokens[i];
        switch(state)
        {
            case State::START:
                if (token==Token::INSTRUCTION)
                {
                    if (suffix_is(str,"do("))
                        state = State::DO;
                    else if (suffix_is(str, "don't("))
                        state = State::DONT;
                        
                    else if (suffix_is(str, "mul(") && mul_enabled)
                        state = State::MUL;
                }
                i++;
                break;

            case State::DO:
                if (token == Token::CLOSE_PAR)
                    mul_enabled = true;
                state = State::START;
                i++;
                break;

            case State::DONT:
                if (token == Token::CLOSE_PAR)
                    mul_enabled = false;
                state = State::START;
                i++;
                break;

            case State::MUL:
                if (token == Token::NUMBER && str.size() <=3)
                {
                    num1 = std::stoi(str);
                    state = State::NUM1;
                    i++;
                }
                else
                    state = State::START;
                break;

            case State::NUM1:
                if (token == Token::COMMA)
                {
                    state = State::COMMA;
                    i++;
                }
                else
                    state = State::START;
                break;

            case State::COMMA:
                if (token == Token::NUMBER && str.size() <=3)
                {
                    num2 = std::stoi(str);
                    state = State::NUM2;
                    i++;
                }
                else
                    state = State::START;
                break;

            case State::NUM2:
                if (token == Token::CLOSE_PAR)
                {
                    if (mul_enabled)
                        result += num1 * num2;
                    i++;
                }
                state = State::START;
                break;
        }
    }

    std::cout << "The Result is: " << result << std::endl;

 
        
}

//------------HELPER FUNCTION----------------

bool suffix_is(const std::string& str, const std::string& suffix) {
    auto b =  str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    return b;
}

std::vector<std::pair<std::string, Token>> TokenizeFile(const std::string& file)
{
    std::vector<std::pair<std::string, Token>> tokens;

    Token prev_token = Token::None;

    std::ifstream ifs(file);
    if(!ifs.is_open())
        return tokens;

    char c;
    while(ifs.get(c))
    {
        if( c == ',')
        {
            tokens.push_back( {std::string(1,c), Token::COMMA});
            prev_token = Token::COMMA;
        }
        else if (c == ')')
        {
            tokens.push_back( {std::string(1,c), Token::CLOSE_PAR});
            prev_token = Token::CLOSE_PAR;
        }

        else if ( std::isalpha(c) || c=='\'' || c=='(')
        {
            if (prev_token == Token::INSTRUCTION)
            {
                tokens.back().first.push_back(c);
            }
            else
            {
                tokens.push_back( {std::string(1,c), Token::INSTRUCTION});
                prev_token = Token::INSTRUCTION;
            }
        }

        else if( std::isdigit(c))
        {
            if (prev_token == Token::NUMBER)
            {
                tokens.back().first.push_back(c);
            }
            else
            {
                tokens.push_back( {std::string(1,c), Token::NUMBER});
                prev_token = Token::NUMBER;
            }
        }
        else 
        {
            prev_token = Token::None;
        }
    }
    return tokens;
}