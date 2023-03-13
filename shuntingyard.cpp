#include <bits/stdc++.h>
#include "linkedlist.hpp"
#include "Queue.hpp"
#include "Stack.hpp"


#define USE_BOOST_MP
#ifdef USE_BOOST_MP
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using cppint = boost::multiprecision::cpp_int;
using cppfloat = number<backends::cpp_bin_float<200>>;
#else
using cppfloat = double;
using cppint = size_t;
#endif

std::map<std::string, int> PRECEDENCE = {
    {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"^", 4}, {"sin", 5}, {"cos", 5}, {"tan", 5}, {"asin", 5}, {"acos", 5}, {"atan", 5}, {"log", 5}, {"pow", 5}, {"fac", 5}, {"rad", 5}, {"deg", 5}};

std::map<std::string, std::string> constants = {
    {"pi", "3."
           "1415926535897932384626433832795028841971693993751058209749445923078"
           "1640628620899862803482534211706798214808651328230664709384460955058"
           "2231725359408128481117450284102701938521105559644622948954930381964"
           "4288109756659334461284756482337867831652712019091"},
    {"e", "2."
          "71828182845904523536028747135266249775724709369995957496696762772407"
          "66303535475945713821785251664274274663919320030599218174135966290435"
          "72900334295260595630738132328627943490763233829880753195251019011573"
          "8341879307021540891499348841675092447614606680"}};

std::vector<std::string> functions_used = {};
std::vector<std::string> constants_used = {};

// implements error checking
template <typename T> 
T stacktop(Stack<T>& stack) {
    if (stack.empty())
        throw std::runtime_error("Empty stack!");
    return stack.top();
};

/**
 * @brief initializes functions_used and constants_used
 *
 * @return
 */
void init() {
    // function vector
    for (auto& i : PRECEDENCE) {
        if (i.second == 5)
            functions_used.push_back(i.first);
    }
    std::sort(functions_used.begin(), functions_used.end(),
              [](const std::string& a, const std::string& b) {
                  return a.length() < b.length();
              });

    // constant vector
    for (auto& i : constants) {
        constants_used.push_back(i.first);
    }
    std::sort(constants_used.begin(), constants_used.end(),
              [](const std::string& a,
                 const std::string& b) {
                  return a.length() < b.length();
              });
}

Queue<std::string> tokenize(std::string expr) {
    Queue<std::string> tokens;

    auto checkFunction = [&tokens](const std::string& str, size_t& i) {
        for (const auto& j : functions_used) {
            if (str.substr(i, j.length()) == j) {
                tokens.push(j);
                i += j.length();
                return true;
            }
        }
        return false;
    };
    auto checkConstants = [&tokens](const std::string& str, size_t& i) {
        for (const auto& j : constants_used) {
            if (str.substr(i, j.length()) == j) {
                tokens.push(j);
                i += j.length();
                return true;
            }
        }
        return false;
    };
    
    auto isDigit = [](char v) {
        return ((v >= '0' && v <= '9') || v == '.');
    };

    for (size_t i = 0; i < expr.length();) {
        if (expr[i] == ' ')
            i++;
        else if (expr[i] == ',') {
            i++;
            tokens.push(",");
        } else if (isDigit(expr[i]) || (expr[i] == '_' && isDigit(expr[i + 1]))) {
            bool min = false;
            if (expr[i] == '_') {
                min = true;
                i++;
            }
            size_t j = i;
            while (j < expr.length() && isDigit(expr[j]))
                j++;
            tokens.push(min? ("-" + expr.substr(i, j-i)) : expr.substr(i, j - i));
            i = j;
        } else if (PRECEDENCE.find(expr.substr(i, 1)) != PRECEDENCE.end()) {
            tokens.push(expr.substr(i, 1));
            i++;
        } else if (expr[i] == '(' || expr[i] == ')') {
            tokens.push(expr.substr(i, 1));
            i++;
        } else if (!checkConstants(expr, i) && !checkFunction(expr, i)) {
            std::stringstream ss;
            ss << "Invalid Character: " << expr[i] << " Line: " << __LINE__;
            throw std::invalid_argument(ss.str());
        }
    }
    return tokens;
}

// check if a sequence of char is a float type
bool isfloat(const std::string& str) {
    bool point = false;
    size_t b=0, j=str.size();
    if (str[b] == '-') b++;
    for (size_t i=b; i<j; i++) {
        char c = str[i];
        if (c == '.' && !point) {
            point = true;
        } else if (!(c >= '0' && c <= '9'))
            return false;
    }
    return j > b;
}

cppfloat tofloat(const std::string& str) {
    cppfloat f;
    std::istringstream iss(str);
    iss >> f;
    return f;
}

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

cppfloat evaluate_postfix(Queue<std::string> tokens) {
    Stack<cppfloat> stack;

    auto checkConstants = [&stack](std::string& str) {
        auto v = std::find(constants_used.begin(), constants_used.end(), str);
        if (v == constants_used.end())
            return false;
        stack.push(tofloat(constants[*v]));
        return true;
    };

    for (; tokens.size() > 0; tokens.pop()) {
        std::string token = tokens.front();
        if (isfloat(token)) {
            stack.push(tofloat(token));
        } else if (checkConstants(token)) {
            ;
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
            stack.push(b * tofloat(constants["pi"]) / 180);
        } else if (token == "deg") {
            cppfloat b = stacktop(stack);
            stack.pop();
            stack.push(b * 180 / tofloat(constants["pi"]));
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

Queue<std::string> shunting_yard(std::string expr) {
    Queue<std::string> output;
    Stack<std::string> stack;
    Queue<std::string> tokens = tokenize(expr);
    auto isConst = [](const std::string& str) {
        return std::find(constants_used.begin(), constants_used.end(), str) !=
               constants_used.end();
    };
    for (; tokens.size() > 0; tokens.pop()) {
        std::string token = tokens.front();
        if (isfloat(token) || isConst(token)) { // operand
            output.push(token);
        } else if (PRECEDENCE.find(token) != PRECEDENCE.end()) { // operator
            while ((stack.size() > 0 &&
                    PRECEDENCE.find(stacktop(stack)) != PRECEDENCE.end() &&
                    PRECEDENCE[token] <= PRECEDENCE[stacktop(stack)]) &&
                   token != "^") {
                output.push(stacktop(stack));
                stack.pop();
            }
            stack.push(token);
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
                if (PRECEDENCE[stack.top()] == 5) {
                    output.push(stack.top());
                    stack.pop();
                }
            }
        } else if (PRECEDENCE[token] == 5) {
            stack.push(token);
        } else {
            std::stringstream ss;
            ss << "Invalid Token: " << token << " Line: " << __LINE__;
            throw std::invalid_argument(ss.str());
        }
    }
    while (stack.size() > 0) {
        output.push(stack.top());
        stack.pop();
    }
    return output;
}

int main() {
    init();
    std::vector<std::string> expressions = {
        "pow(sin(0.5), 2) + pow(cos(0.5), 2)",
        "sin(0.5)^2 + cos(0.5)^2",
        "log(pow(2, 32), 2)",
        "2^2^3",
    };
    for (auto expr : expressions) {
        std::cout << "Expr: " << expr << std::endl;
        Queue<std::string> postfix = shunting_yard(expr), tmp;
        tmp = postfix;
        std::cout << "    Postfix : ";
        for (; tmp.size(); tmp.pop()) {
            std::string& j = tmp.front();
            std::cout << j << " ";
        }
        std::cout << std::endl;
        cppfloat result = evaluate_postfix(postfix);
        std::cout << "    Result: " << std::setprecision(200) << result
                  << std::endl;
    }
    std::cout << "Input Expression: " << std::endl;
    std::string expr;
    while (std::getline(std::cin, expr)) {
        try {
            Queue<std::string> postfix = shunting_yard(expr);
            Queue<std::string> tmp = postfix;
            std::cout << "    Postfix : ";
            for (; tmp.size(); tmp.pop()) {
                std::string& j = tmp.front();
                std::cout << j << " ";
            }
            std::cout << std::endl;
            cppfloat result = evaluate_postfix(postfix);
            std::cout << "    Result: " << std::setprecision(200) << result
                      << std::endl;
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        std::cout << "Input Expression: " << std::endl;
    }
}