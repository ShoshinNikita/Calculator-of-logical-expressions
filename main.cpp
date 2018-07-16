#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <clocale>

using namespace std;


const char NEGATION_SIGN = '!'; // Negation (Инверсия)
const char CONJUNCTION_SIGN = '*'; // Logical conjunction (Конъюнкция)
const char DISJUNCTION_SIGN = '+'; // Logical disjunction (Дизъюнкция)
const char CONDITIONAL_SIGN = '-'; // Material conditional (Импликация)
const char EQUALITY_SIGN = '='; // Logical equality (Эквиваленция)


void show_rules();
string string_parsing(map<char, bool>&, const string&);
vector<string> computation(map<char, bool>, const string&);

///////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "Russian");

	string expression;
	map<char, bool> symbols;
	string new_expression;

	show_rules();
	bool is_end = false;

	while (true) {
		cout << endl << endl;

		// Getting right expression
		while (true) {
			cout << "Enter your expression (for end enter 'exit'): ";
			cin >> expression;
			transform(expression.begin(), expression.end(), expression.begin(), ::toupper);
			if (expression == "EXIT") {
				is_end = true;
				break;
			}

			try {
				new_expression = string_parsing(symbols, expression);
				// If there's no exception
				break;
			}
			catch (exception& e) {
				cout << e.what() << endl << endl;
			}
		}
		// For exit
		if (is_end) {
			break;
		}

		vector<string> results = computation(symbols, new_expression);

		// Output symblos, expression and results
		for (auto i : symbols) {
			cout << i.first << " ";
		}
		cout << '\t' << expression << endl;

		for (auto i : results) {
			cout << i << endl;
		}

		expression.clear();
		symbols.clear();
		new_expression.clear();
	}

	// End of program
	return 0;
}
///////////////////////////////////////

// Continue work on checking correctness of expression
string string_parsing(map<char, bool>& symbols, const string& exp)
{
	stack<char> operations;
	string new_expression;
	stack<char> brackets;

	// Checking string for errors
	if (exp.size() == 1) {
		if (exp[0] == NEGATION_SIGN || exp[0] == DISJUNCTION_SIGN ||
			exp[0] == CONJUNCTION_SIGN || exp[0] == CONDITIONAL_SIGN || 
			exp[0] == EQUALITY_SIGN || exp[0] == '(' || exp[0] == ')') {
			throw exception("Error: bad syntax (1)");
		}
	}
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i] < 65 || exp[i] > 90) {
			if (exp[i] == NEGATION_SIGN || exp[i] == DISJUNCTION_SIGN || exp[i] == CONJUNCTION_SIGN || exp[i] == CONDITIONAL_SIGN || exp[i] == EQUALITY_SIGN || exp[i] == '(' || exp[i] == ')') {
				if (exp[i] == '(') {
					brackets.push('(');
				}
				else if (exp[i] == ')') {
					if (!brackets.empty() && brackets.top() == '(') {
						brackets.pop();
					}
					else {
						throw exception("Error: bad amount of brackets");
					}
				}
				else if (exp[i] == CONJUNCTION_SIGN || exp[i] == DISJUNCTION_SIGN || exp[i] == CONDITIONAL_SIGN || exp[i] == EQUALITY_SIGN) {
					if (! (i > 0 && (exp[i - 1] >= 65 && exp[i - 1] <= 90 || exp[i - 1] == NEGATION_SIGN || exp[i - 1] == ')') &&
						i + 1 < exp.size() && (exp[i + 1] >= 65 && exp[i + 1] <= 90 || exp[i + 1] == NEGATION_SIGN || exp[i + 1] == '('))) {
						throw exception("Error: bad syntax (2)");
					}
				}
				else if (exp[i] == NEGATION_SIGN) {
					if (i > 0) {
						if (! ((exp[i - 1] == CONJUNCTION_SIGN || exp[i - 1] == DISJUNCTION_SIGN || 
							exp[i - 1] == CONDITIONAL_SIGN || exp[i - 1] == EQUALITY_SIGN || exp[i - 1] == '(') &&
							i + 1 < exp.size() && (exp[i + 1] >= 65 && exp[i + 1] <= 90 || exp[i + 1] == '('))) {
							throw exception("Error: bad syntax (3)");
						}
					}
				}
			}
			else {
				throw exception("Error: bad syntax (4)");
			}
		}
		else {
			// Check do letters go in a row
			if (i > 0) {
				if (exp[i - 1] >= 65 && exp[i - 1] <= 90) {
					throw exception("Error: bad syntax (5)");
				}
			}
			if (i + 1 < exp.size()) {
				if (exp[i + 1] >= 65 && exp[i + 1] <= 90) {
					throw exception("Error: bad syntax (6)");
				}
			}
		}
	}
	// Ckeck brackets
	if (!brackets.empty()) {
		throw exception("Error: bad syntax (7)");
	}

	// Creating new expression (Reverse Polish notation)
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i] < 65 || exp[i] > 90) {
			if (exp[i] == '(') {
				operations.push('(');
			}
			else if (exp[i] == ')') {
				while (!operations.empty() && operations.top() != '(') {
					new_expression.push_back(operations.top());
					operations.pop();
				}
				operations.pop();

				if (!operations.empty() && operations.top() == NEGATION_SIGN) {
					new_expression.push_back(NEGATION_SIGN);
					operations.pop();
				}

			}	
			else if (exp[i] == CONJUNCTION_SIGN) {
				operations.push(CONJUNCTION_SIGN);
			}
			else if (exp[i] == DISJUNCTION_SIGN) {
				while (!operations.empty() && operations.top() == CONJUNCTION_SIGN) {
					new_expression.push_back(CONJUNCTION_SIGN);
					operations.pop();
				}
				operations.push(DISJUNCTION_SIGN);
			}
			else if (exp[i] == CONDITIONAL_SIGN) {
				while (!operations.empty() && (operations.top() == CONJUNCTION_SIGN || operations.top() == DISJUNCTION_SIGN)) {
					new_expression.push_back(operations.top());
					operations.pop();
				}
				operations.push(CONDITIONAL_SIGN);
			}
			else if (exp[i] == EQUALITY_SIGN) {
				while (!operations.empty() && (operations.top() == CONJUNCTION_SIGN || operations.top() == DISJUNCTION_SIGN)) {
					new_expression.push_back(operations.top());
					operations.pop();
				}
				operations.push(EQUALITY_SIGN);
			}
			else if (exp[i] == NEGATION_SIGN) {
				operations.push(NEGATION_SIGN);
			}
		}
		else {
			symbols[exp[i]] = false;
			new_expression.push_back(exp[i]);
			if (!operations.empty() && operations.top() == NEGATION_SIGN) {
				new_expression.push_back(NEGATION_SIGN);
				operations.pop();
			}
		}
	}

	while (!operations.empty()) {
		new_expression.push_back(operations.top());
		operations.pop();
	}

	return new_expression;
}

vector<string> computation(map<char, bool> symbols, const string& expression)
{
	int iteration_number = [symbols]() {
		int num = 1;
		for (int i = 0; i < symbols.size(); i++) num *= 2;
		return num;
	}();

	// For changing zeros and ones
	string all_symbols = [symbols]() {
		string temp;
		for (auto i : symbols) {
			temp.push_back(i.first);
		}
		return temp;
	}();

	auto dec_to_bin = [](int n, int size) {
		string bin_number;
		while (n != 0) {
			bin_number.append(to_string(n % 2));
			n /= 2;
		}
		reverse(bin_number.begin(), bin_number.end());
		return (string(size - bin_number.size(), '0') + bin_number);
	};

	vector<string> results;
	stack<bool> counting_stack;
	for (int i = 0; i < iteration_number; i++) {
		// Changing zeros and ones
		string bin_number = dec_to_bin(i, all_symbols.size());
		// Change bool values in map according bin_number
		for (int j = 0; j < all_symbols.size(); j++) {
			symbols[all_symbols[j]] = bin_number[j] - '0';
		}
		
		// Main computation
		for (int i = 0; i < expression.size(); i++) {
			if (expression[i] >= 65 && expression[i] <= 90) {
				counting_stack.push(symbols[expression[i]]);
			}
			else if (expression[i] == NEGATION_SIGN) {
				bool a = counting_stack.top();
				counting_stack.pop();
				if (a) a = false;
				else a = true;

				counting_stack.push(a);
			}
			else if (expression[i] == CONJUNCTION_SIGN) {
				bool b = counting_stack.top();
				counting_stack.pop();
				bool a = counting_stack.top();
				counting_stack.pop();

				if (a && b) {
					counting_stack.push(true);
				}
				else {
					counting_stack.push(false);
				}
			}
			else if (expression[i] == DISJUNCTION_SIGN) {
				bool b = counting_stack.top();
				counting_stack.pop();
				bool a = counting_stack.top();
				counting_stack.pop();

				if (a || b) {
					counting_stack.push(true);
				}
				else {
					counting_stack.push(false);
				}
			}
			else if (expression[i] == CONDITIONAL_SIGN) {
				bool b = counting_stack.top();
				counting_stack.pop();
				bool a = counting_stack.top();
				counting_stack.pop();

				if ((a && b) || (!a && !b) || (!a && b)) {
					counting_stack.push(true);
				}
				else {
					counting_stack.push(false);
				}
			}
			else if (expression[i] == EQUALITY_SIGN) {
				bool b = counting_stack.top();
				counting_stack.pop();
				bool a = counting_stack.top();
				counting_stack.pop();

				if ((a && b) || (!a && !b)) {
					counting_stack.push(true);
				}
				else {
					counting_stack.push(false);
				}
			}
		}

		// Creating string for returning
		string temp;
		for (auto i : bin_number) {
			temp.push_back(i);
			temp.push_back(' ');
		}
		temp.push_back('\t');
		temp.push_back(counting_stack.top() ? '1' : '0');
		results.push_back(temp);
		counting_stack.pop();
	}

	return results;
}

void show_rules()
{
	cout << "Negation (Инверсия) - '" << NEGATION_SIGN << "'\n"
		<< "Logical conjunction (Конъюнкция) - '" << CONJUNCTION_SIGN << "'\n"
		<< "Logical disjunction (Дизъюнкция) - '" << DISJUNCTION_SIGN << "'\n"
		<< "Material conditional (Импликация) - '" << CONDITIONAL_SIGN << "'\n"
		<< "Logical equality (Эквиваленция) - '" << EQUALITY_SIGN << "'\n";
}