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
const std::map<std::string, std::string> constants = {
    {"pi", "3."
           "1415926535897932384626433832795028841971693993751058209749445923078164062"
           "8620899862803482534211706798214808651328230664709384460955058223172535940"
           "8128481117450284102701938521105559644622948954930381964428810975665933446"
           "1284756482337867831652712019091456485669234603486104543266482133936072602"
           "4914127372458700660631558817488152092096282925409171536436789259036001133"
           "0530548820466521384146951941511609433057270365759591953092186117381932611"
           "7931051185480744623799627495673518857527248912279381830119491298336733624"
           "4065664308602139494639522473719070217986094370277053921717629317675238467"
           "4818467669405132000568127145263560827785771342757789609173637178721468440"
           "9012249534301465495853710507922796892589235420199561121290219608640344181"
           "5981362977477130996051870721134999999837297804995105973173281609631859502"
           "4459455346908302642522308253344685035261931188171010003137838752886587533"
           "2083814206171776691473035982534904287554687311595628638823537875937519577"
           "8185778053217122680661300192787661119590921642019893809525720106548586327"
           "8865936153381827968230301952035301852968995773622599413891249721775283479"
           "1315155748572424541506959508295331168617278558890750983817546374649393192"
           "5506040092770167113900984882401285836160356370766010471018194295559619894"
           "6767837449448255379774726847104047534646208046684259069491293313677028989"
           "1521047521620569660240580381501935112533824300355876402474964732639141992"
           "7260426992279678235478163600934172164121992458631503028618297455570674983"
           "850549458858692699569092721079750930295"},
    {"e", "2."
          "7182818284590452353602874713526624977572470936999595749669676277240766303"
          "5354759457138217852516642742746639193200305992181741359662904357290033429"
          "5260595630738132328627943490763233829880753195251019011573834187930702154"
          "0891499348841675092447614606680822648001684774118537423454424371075390777"
          "4499206955170276183860626133138458300075204493382656029760673711320070932"
          "8709127443747047230696977209310141692836819025515108657463772111252389784"
          "4250569536967707854499699679468644549059879316368892300987931277361782154"
          "2499922957635148220826989519366803318252886939849646510582093923982948879"
          "3320362509443117301238197068416140397019837679320683282376464804295311802"
          "3287825098194558153017567173613320698112509961818815930416903515988885193"
          "4580727386673858942287922849989208680582574927961048419844436346324496848"
          "7560233624827041978623209002160990235304369941849146314093431738143640546"
          "2531520961836908887070167683964243781405927145635490613031072085103837505"
          "1011574770417189861068739696552126715468895703503540212340784981933432106"
          "8170121005627880235193033224745015853904730419957777093503660416997329725"
          "0886876966403555707162268447162560798826517871341951246652010305921236677"
          "1943252786753985589448969709640975459185695638023637016211204774272283648"
          "9613422516445078182442352948636372141740238893441247963574370263755294448"
          "3379980161254922785092577825620926226483262779333865664816277251640191059"
          "0049164499828931505660472580277863186415519565324425869829469593080191529"
          "872117255634754639644791014590409058629"}};

std::vector<std::string> functions_used = {};
std::vector<std::string> constants_used = {};

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
 * @brief initializes functions_used and constants_used
 *
 * @return void
 */
void init() {
    // function vector
    for (auto& i : PRECEDENCE) {
        if (i.second == 5) functions_used.push_back(i.first);
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
              [](const std::string& a, const std::string& b) {
                  return a.length() < b.length();
              });
}

/**
 * @brief Tokenize input expression into operand / operator tokens
 * 
 * @param expr String expression to tokenize
 * @return Queue<std::string> queue of operand / operator tokens
 */
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
        auto v = std::find(constants_used.begin(), constants_used.end(), str);
        if (v == constants_used.end()) return false;
        stack.push(tofloat(constants.find(*v)->second));
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
            stack.push(b * tofloat(constants.find("pi")->second) / 180);
        } else if (token == "deg") {
            cppfloat b = stacktop(stack);
            stack.pop();
            stack.push(b * 180 / tofloat(constants.find("pi")->second));
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
                if (PRECEDENCE.find(stacktop(stack)) != PRECEDENCE.end() &&
                    PRECEDENCE.find(stacktop(stack))->second == 5) {
                    output.push(stacktop(stack));
                    stack.pop();
                }
            }
        } else if (PRECEDENCE.find(token) !=
                   PRECEDENCE.end()) { // operator & function
            if (PRECEDENCE.find(token)->second != 5) {
                // operator
                while ((stack.size() > 0 &&
                        PRECEDENCE.find(stacktop(stack)) != PRECEDENCE.end() &&
                        PRECEDENCE.find(token)->second <=
                            PRECEDENCE.find(stacktop(stack))->second) &&
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
    init();
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
            
            std::cout << "    Took " << std::setprecision(BOOST_PREC) << time_span.count() << " seconds."
                  << std::endl;
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        std::cout << "Input Expression: " << std::endl;
    }
}