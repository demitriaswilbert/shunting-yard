#include "Queue.hpp"
#include "Stack.hpp"
#include "linkedlist.hpp"
#include <bits/stdc++.h>

#define USE_BOOST_MP
#ifdef USE_BOOST_MP
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#define BOOST_PREC 1000

using namespace boost::multiprecision;
using cppint = boost::multiprecision::cpp_int;
using cppfloat = number<backends::cpp_bin_float<BOOST_PREC>>;
#else
using cppfloat = double;
using cppint = size_t;
#endif

cppfloat ans = 0;

/**
 * @brief Math Operators
 * 
 */
const std::map<std::string, int> PRECEDENCE = {
    {"+", 2},   {"-", 2},   {"*", 3},    {"/", 3},    {"^", 4},    {"sin", 5},
    {"cos", 5}, {"tan", 5}, {"asin", 5}, {"acos", 5}, {"atan", 5}, {"log", 5},
    {"pow", 5}, {"fac", 5}, {"rad", 5},  {"deg", 5}};

/**
 * @brief Constants
 * 
 */
std::map<std::string, std::string> constants;


void readconst(std::string filename) {
    constants.clear();
    std::ifstream ifs(filename);
    std::string line;
    std::pair<std::string, std::string> linepair;
    size_t index = 0;
    while (std::getline(ifs, line)) {
        if (line == "") continue;
        if ((index & 1U) == 0) {
            linepair.first = line;
        } else {
            linepair.second = line;
            constants.insert(linepair);
        }
        index++;
    }
    ifs.close();
}

/**
 * @brief get stack top with Error Checking
 * 
 * @tparam T 
 * @param stack stack to get the top from
 * @return T top of stack
 */
template <typename T> T stacktop(Stack<T>& stack) {
    if (stack.empty()) throw std::runtime_error("Empty stack!");
    return stack.top();
};

/**
 * @brief Tokenize input expression into operand / operator tokens
 * 
 * @param expr String expression to tokenize
 * @return Queue<std::string> queue of operand / operator tokens
 */
Queue<std::string> tokenize(std::string expr) {
    Queue<std::string> tokens;

    auto checkFunction = [&tokens](const std::string& str, size_t& i) {
        for (const auto& j : PRECEDENCE) {
            if (j.second == 5 && str.substr(i, j.first.length()) == j.first) {
                tokens.push(j.first);
                i += j.first.length();
                return true;
            }
        }
        return false;
    };
    auto checkConstants = [&tokens](const std::string& str, size_t& i) {
        for (const auto& j : constants) {
            if (str.substr(i, j.first.length()) == j.first) {
                tokens.push(j.first);
                i += j.first.length();
                return true;
            }
        }
        return false;
    };

    auto isDigit = [](char v) { return ((v >= '0' && v <= '9') || v == '.'); };

    for (size_t i = 0; i < expr.length();) {
        if (expr[i] == ' ')
            i++;
        else if (expr[i] == ',') {
            i++;
            tokens.push(",");
        } else if (isDigit(expr[i]) ||
                   (expr[i] == '_' && isDigit(expr[i + 1]))) {
            bool min = false;
            if (expr[i] == '_') {
                min = true;
                i++;
            }
            size_t j = i;
            while (j < expr.length() && isDigit(expr[j]))
                j++;
            tokens.push(min ? ("-" + expr.substr(i, j - i))
                            : expr.substr(i, j - i));
            i = j;
        } else if (PRECEDENCE.count(expr.substr(i, 1))) {
            tokens.push(expr.substr(i, 1));
            i++;
        } else if (expr[i] == '(' || expr[i] == ')') {
            tokens.push(expr.substr(i, 1));
            i++;
        } else if (expr.substr(i, 3) == "ans") {
            tokens.push("ans");
            i += 3;
        } else if (!checkConstants(expr, i) && !checkFunction(expr, i)) {
            std::stringstream ss;
            ss << "Invalid Character: " << expr[i] << " Line: " << __LINE__;
            throw std::invalid_argument(ss.str());
        }
    }
    return tokens;
}

/**
 * @brief check if a string is a floating point number
 * 
 * @param str input string
 * @return true 
 * @return false 
 */
bool isfloat(const std::string& str) {
    bool point = false;
    size_t b = 0, j = str.size();
    if (str[b] == '-') b++;
    for (size_t i = b; i < j; i++) {
        char c = str[i];
        if (c == '.' && !point) {
            point = true;
        } else if (!(c >= '0' && c <= '9'))
            return false;
    }
    return j > b;
}

/**
 * @brief convert string to float
 * 
 * @param str string of numbers to convert
 * @return cppfloat floating point number
 */
cppfloat tofloat(const std::string& str) {
    cppfloat f;
    std::istringstream iss(str);
    iss >> f;
    return f;
}

/**
 * @brief apply operation to input operands
 * 
 * @param op operator
 * @param b first operand
 * @param a second operand
 * @return cppfloat result of the operation
 */
cppfloat apply_op(const std::string& op, const cppfloat& b, const cppfloat& a) {
    if (op == "+")
        return a + b;
    else if (op == "-")
        return a - b;
    else if (op == "*")
        return a * b;
    else if (op == "/")
        return a / b;
    else if (op == "^")
        return pow(a, b);
    else {
        std::stringstream ss;
        ss << "Invalid Operator: " << op << " Line: " << __LINE__;
        throw std::invalid_argument(ss.str());
    }
}

/**
 * @brief evaluate the queue of postfix 
 * 
 * @param tokens queue of postfix tokens
 * @return cppfloat result from the evaluation
 */
cppfloat evaluate_postfix(Queue<std::string> tokens) {
    Stack<cppfloat> stack;

    auto checkConstants = [&stack](std::string& str) {
        for (const auto& token : constants) {
            if (str == token.first)
                return true;
        }
        return false;
    };

    for (; tokens.size() > 0; tokens.pop()) {
        std::string token = tokens.front();
        if (isfloat(token)) {
            stack.push(tofloat(token));
        } else if (checkConstants(token)) {
            stack.push(tofloat(constants.at(token)));
        } else if (token == "ans") {
            stack.push(ans);
        } else if (token == "sin") {
            cppfloat tmp = stacktop(stack);
            stack.pop();
            stack.push(sin(tmp));
        } else if (token == "cos") {
            cppfloat tmp = stacktop(stack);
            stack.pop();
            stack.push(cos(tmp));
        } else if (token == "tan") {
            cppfloat tmp = stacktop(stack);
            stack.pop();
            stack.push(tan(tmp));
        } else if (token == "asin") {
            cppfloat tmp = stacktop(stack);
            stack.pop();
            stack.push(asin(tmp));
        } else if (token == "acos") {
            cppfloat tmp = stacktop(stack);
            stack.pop();
            stack.push(acos(tmp));
        } else if (token == "atan") {
            cppfloat tmp = stacktop(stack);
            stack.pop();
            stack.push(atan(tmp));
        } else if (token == "log") {
            cppfloat b = stacktop(stack);
            stack.pop();
            cppfloat a = stacktop(stack);
            stack.pop();
            stack.push(log(a) / log(b));
        } else if (token == "pow") {
            cppfloat b = stacktop(stack);
            stack.pop();
            cppfloat a = stacktop(stack);
            stack.pop();
            stack.push(pow(a, b));
        } else if (token == "rad") {
            cppfloat b = stacktop(stack);
            stack.pop();
            stack.push(b * tofloat(constants.at("pi")) / 180);
        } else if (token == "deg") {
            cppfloat b = stacktop(stack);
            stack.pop();
            stack.push(b * 180 / tofloat(constants.at("pi")));
        } else if (token == "fac") {
// cppint b = stacktop(stack);
#ifdef USE_BOOST_MP
            cppint b = stacktop(stack).convert_to<cppint>();
#else
            cppint b = stacktop(stack);
#endif
            stack.pop();
            cppint res = 1;
            for (; b > 1; b--)
                res *= b;
// stack.push(res);
#ifdef USE_BOOST_MP
            stack.push(res.convert_to<cppfloat>());
#else
            stack.push(res);
#endif
        } else {
            cppfloat b = stacktop(stack);
            stack.pop();
            cppfloat a = stacktop(stack);
            stack.pop();
            stack.push(apply_op(token, b, a));
        }
    }
    return stacktop(stack);
}

/**
 * @brief convert a infix expression string to a queue of postfix expression tokens
 * 
 * @param expr string of infix expression
 * @return Queue<std::string> queue of postfix tokens
 */
Queue<std::string> shunting_yard(std::string expr) {
    
    readconst("constants.txt");
    Queue<std::string> output;
    Stack<std::string> stack;
    Queue<std::string> tokens = tokenize(expr);
    auto isConst = [](const std::string& str) {
        if (str == "ans") return true;
        for (const auto& token : constants) {
            if (str == token.first) return true;
        }
        return false;
    };
    for (; tokens.size() > 0; tokens.pop()) {
        std::string token = tokens.front();
        if (isfloat(token) || isConst(token)) { // operand
            output.push(token);
        } else if (token == "(") {
            stack.push(token);
        } else if (token == ",") {
            while (stack.size() > 0 && stacktop(stack) != "(") {
                output.push(stacktop(stack));
                stack.pop();
            }
        } else if (token == ")") {
            while (stacktop(stack) != "(") {
                output.push(stacktop(stack));
                stack.pop();
            }
            stack.pop();
            if (stack.size() > 0) {
                if (PRECEDENCE.count(stacktop(stack)) &&
                    PRECEDENCE.at(stacktop(stack)) == 5) {
                    output.push(stacktop(stack));
                    stack.pop();
                }
            }
        } else if (PRECEDENCE.count(token)) { // operator & function
            if (PRECEDENCE.at(token) != 5) {
                // operator
                while ((stack.size() > 0 &&
                        PRECEDENCE.count(stacktop(stack)) &&
                        PRECEDENCE.at(token) <= PRECEDENCE.at(stacktop(stack))) &&
                       token != "^") {
                    output.push(stacktop(stack));
                    stack.pop();
                }
            }
            stack.push(token);
        } else {
            std::stringstream ss;
            ss << "Invalid Token: " << token << " Line: " << __LINE__;
            throw std::invalid_argument(ss.str());
        }
    }
    while (stack.size() > 0) {
        output.push(stacktop(stack));
        stack.pop();
    }
    return output;
}


int main() {
    std::vector<std::string> expressions = {
        "pow(sin(0.5), 2) + pow(cos(0.5), 2)",
        "sin(0.5)^2 + cos(0.5)^2",
        "log(pow(2, 32), 2)",
        "2^2^3",
    };
    for (auto expr : expressions) {
        std::cout << "Expr: " << expr << std::endl;
        std::chrono::high_resolution_clock::time_point t1 = 
            std::chrono::high_resolution_clock::now();
        Queue<std::string> postfix = shunting_yard(expr), tmp;
        tmp = postfix;
        std::cout << "    Postfix : ";
        for (; tmp.size(); tmp.pop()) {
            std::string& j = tmp.front();
            std::cout << j << " ";
        }
        std::cout << std::endl;
        cppfloat result = evaluate_postfix(postfix);
        std::chrono::duration<cppfloat> time_span =
            std::chrono::duration_cast<std::chrono::duration<cppfloat>>(
                std::chrono::high_resolution_clock::now() - t1);
        std::cout << "    Result: " << std::setprecision(BOOST_PREC) << result
                  << std::endl;
        ans = result;
        std::cout << "    Took " << std::setprecision(BOOST_PREC) << time_span.count() << " seconds."
                  << std::endl;
    }
    std::cout << "Input Expression: " << std::endl;
    std::string expr;
    while (std::getline(std::cin, expr)) {
        try {
            Queue<std::string> postfix = shunting_yard(expr);
            std::chrono::high_resolution_clock::time_point t1 = 
                std::chrono::high_resolution_clock::now();
            Queue<std::string> tmp = postfix;
            std::cout << "    Postfix : ";
            for (; tmp.size(); tmp.pop()) {
                std::string& j = tmp.front();
                std::cout << j << " ";
            }
            std::cout << std::endl;
            cppfloat result = evaluate_postfix(postfix);
            std::chrono::duration<cppfloat> time_span =
                std::chrono::duration_cast<std::chrono::duration<cppfloat>>(
                    std::chrono::high_resolution_clock::now() - t1);
            std::cout << "    Result: " << std::setprecision(BOOST_PREC) << result
                      << std::endl;
            ans = result;
            std::cout << "    Took " << std::setprecision(BOOST_PREC) << time_span.count() << " seconds."
                  << std::endl;
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        std::cout << "Input Expression: " << std::endl;
    }
}