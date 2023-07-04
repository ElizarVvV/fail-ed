#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

std::unordered_map<std::string, long double> variables;

int getPrecedence(const std::string& op)
{
    if (op == "^") {
        return 3;
    } else if (op == "*" || op == "/") {
        return 2;
    } else if (op == "+" || op == "-") {
        return 1;
    }
    return 0;
}

long double applyOperator(long double a, long double b, const std::string& op)
{
    if (op == "+") {
        return a + b;
    } else if (op == "-") {
        return a - b;
    } else if (op == "*") {
        return a * b;
    } else if (op == "/") {
        return a / b;
    } else if (op == "^") {
        return std::pow(a, b);
    }
    return 0;
}

long double evaluateExpression(const std::vector<std::string>& tokens)
{
    std::stack<long double> values;
    std::stack<std::string> operators;

    for (const auto& token : tokens)
    {

        if (token == "(")
        {
            operators.push(token);
        }

        else if (token == ")")
        {

            while (!operators.empty() && operators.top() != "(") {
                long double b = values.top();
                values.pop();

                long double a = values.top();
                values.pop();

                std::string op = operators.top();
                operators.pop();

                long double result = applyOperator(a, b, op);
                values.push(result);
            }

            if (!operators.empty() && operators.top() == "(")
            {
                operators.pop();
            }

        }

        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^")
        {

            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(token))
            {
                long double b = values.top();
                values.pop();

                long double a = values.top();
                values.pop();

                std::string op = operators.top();
                operators.pop();

                long double result = applyOperator(a, b, op);
                values.push(result);
            }
            operators.push(token);

        }
        else
        {
            long double value = 0;
            if (token[0] == '[' && token[token.size() - 1] == ']')
            {
                std::string variableName = token.substr(1, token.size() - 2);
                if (variables.count(variableName) > 0) {
                    value = variables[variableName];
                } else {
                    std::cout << "Variable not found: " << variableName << std::endl;
                    return 0;
                }
            }
            else
            {
                if (variables.count(token) > 0) {
                    value = variables[token];
                } else {
                    std::istringstream iss(token);
                    if (!(iss >> value)) {
                        std::cout << "Invalid number or variable: " << token << std::endl;
                        return 0;
                    }
                }
            }

            values.push(value);
        }
    }

    while (!operators.empty())
    {
        long double b = values.top();
        values.pop();

        long double a = values.top();
        values.pop();

        std::string op = operators.top();
        operators.pop();

        long double result = applyOperator(a, b, op);
        values.push(result);
    }

    return values.top();
}

void executeStatement(const std::string& statement)
{
    std::istringstream iss(statement);
    std::string token;
    std::vector<std::string> tokens;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() >= 3 && tokens[1] == "=")
    {
        std::string variableName = tokens[0];
        std::vector<std::string> expressionTokens(tokens.begin() + 2, tokens.end());
        long double value = evaluateExpression(expressionTokens);
        variables[variableName] = value;
    }

    else if (tokens.size() >= 2 && tokens[0] == "print")
    {

        for (size_t i = 1; i < tokens.size(); i++)
        {

            if (tokens[i] == ":")
            {
                continue;
            }

            std::string variableName = tokens[i];

            if (variables.count(variableName) > 0)
            {
                long double value = variables[variableName];
                std::cout << variableName << ": " << value << std::endl;
            }

            else
            {
                std::cout << "Variable not found: " << variableName << std::endl;
            }

        }

    }

    else
    {
        std::cout << "Invalid statement: " << statement << std::endl;
    }
}

void parseLine(const std::string& line)
{

    std::istringstream iss(line);
    std::string statement;

    while (std::getline(iss, statement, ';'))
    {
        executeStatement(statement);
    }

}

void runProgram(const std::string& filePath)
{

    std::ifstream file(filePath);

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line)) {
            parseLine(line);
        }
        file.close();
    }

    else
    {
        std::cout << "Unable to open file: " << filePath << std::endl;
    }
}

int main()
{
    std::string filePath = "C:\\Users\\Administrator\\Music\\program.txt";
    runProgram(filePath);
    return 0;
}
