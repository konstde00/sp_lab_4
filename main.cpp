#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <sstream>

using namespace std;

const string FILE_NAME = "../input.txt";



// Utility methods:
fstream open_grammar_file();

vector<pair<char, string> > parse_grammar_file(fstream& grammar_file);

set<char> get_terms(const vector<pair<char, string> >& grammar);

set<char> get_non_terms(const vector<pair<char, string> >& grammar);

set<char> get_epsilon_non_terms(const vector<pair<char, string> >& grammar);

void build_parsing_table(const vector<pair<char, string> >& grammar,
                         const set<char>& non_terms,
                         const set<char>& terms,
                         map<char, set<char>> firsts,
                         map< char, set<char> > follows,
                         int parse_table[100][100]);

map< char, set<char> > get_firsts_list(const vector<pair<char, string> >& grammar, const set<char>& non_terms);

map< char, set<char> > get_follows_list(const vector<pair<char, string> >& grammar, const map<char, set<char> >& firsts, const set<char>& non_terms);

void print_starting_operations(string input_string,
                               const vector<pair<char, string> >& grammar,
                               const set<char>& terms, const set<char>& non_terms,
                               int parse_table[100][100]);


int main()
{

    cout << "Input string: ";
    string input_string;
    cin >> input_string;

    fstream grammar_file = open_grammar_file();

    vector<pair<char, string> > grammar = parse_grammar_file(grammar_file);

    set<char> terms = get_terms(grammar);
    set<char> non_terms = get_non_terms(grammar);


    set<char> eps_non_terms = get_epsilon_non_terms(grammar);

    map<char, set<char> > firsts = get_firsts_list(grammar, non_terms);

    map< char, set<char> > follows = get_follows_list(grammar, firsts, non_terms);

    int parse_table[100][100];
    build_parsing_table(grammar, non_terms, terms, firsts, follows, parse_table);

    print_starting_operations(input_string, grammar, terms, non_terms, parse_table);

    return 0;
}

fstream open_grammar_file()
{

    fstream grammar_file;

    grammar_file.open(FILE_NAME, ios::in);

    if (grammar_file.fail()) {

        throw invalid_argument("Error in opening grammar file");
    }

    return grammar_file;
}

vector<pair<char, string>> parse_grammar_file(fstream& grammar_file)
{
    vector<pair<char, string>> productions;

    int count = 0;

    string line;
    while (getline(grammar_file, line)) {
        istringstream iss(line);
        char non_terminal;
        string production;

        if (!(iss >> non_terminal)) {
            throw runtime_error("Error reading non-terminal from the grammar file.");
        }

        // ignore '->'
        iss.ignore(2);

        if (!(iss >> production)) {
            throw runtime_error("Error reading production from the grammar file.");
        }

        productions.emplace_back(non_terminal, production);
    }

    cout << "\n";
    cout << "Grammar read from file: \n";
    for (auto & production : productions) {

        cout << ++count << ".  " << production.first << " -> " << production.second << "\n";
    }

    return productions;
}

set<char> get_terms(const vector<pair<char, string> >& grammar)
{

    set<char> terms;

    for (const auto & i : grammar) {
        for (char ch : i.second) {
            if (!isupper(ch)) {
                terms.insert(ch);
            }
        }
    }

    cout << "\n";
    cout << "The terminals in the grammar are: ";

    for (char term : terms) {
        cout << term << " ";
    }

    cout << "\n\n";

    terms.erase('e');
    terms.insert('$');

    return terms;
}

set<char> get_non_terms(const vector<pair<char, string> >& grammar)
{

    set<char> non_terms;

    for (const auto & i : grammar)
    {
        non_terms.insert(i.first);
    }

    cout << "The non terminals in the grammar are: ";

    for (char non_term : non_terms)
    {
        cout << non_term << " ";
    }
    cout << "\n";

    return non_terms;
}

set<char> get_epsilon_non_terms(const vector<pair<char, string>>& grammar) {
    set<char> epsilon_non_terms;

    for (const auto& i : grammar)
    {
        const char non_term = i.first;
        const string& production = i.second;

        if (production == "e")
        {
            epsilon_non_terms.insert(non_term);
        }
    }

    cout << "The epsilon non-terminals in the grammar are: ";
    for (char epsilon_non_term : epsilon_non_terms) {
        cout << epsilon_non_term << " ";
    }
    cout << "\n";

    return epsilon_non_terms;
}


void find_first(vector< pair<char, string> > gram, map< char, set<char> >& firsts, char non_term)
{

    for (auto it = gram.begin(); it != gram.end(); ++it)
    {
        if (it->first != non_term)
        {
            continue;
        }

        string rhs = it->second;
        for (auto ch = rhs.begin(); ch != rhs.end(); ++ch)
        {
            if (!isupper(*ch))
            {
                firsts[non_term].insert(*ch);
                break;
            }
            else
            {
                if (firsts[*ch].empty())
                {
                    find_first(gram, firsts, *ch);
                }
                if (firsts[*ch].find('e') == firsts[*ch].end())
                {
                    firsts[non_term].insert(firsts[*ch].begin(), firsts[*ch].end());
                    break;
                }

                set<char> firsts_copy(firsts[*ch].begin(), firsts[*ch].end());

                if (ch + 1 != rhs.end())
                {
                    firsts_copy.erase('e');
                }

                firsts[non_term].insert(firsts_copy.begin(), firsts_copy.end());
            }
        }

    }
}

void find_follow(vector< pair<char, string> > gram, map< char, set<char> >& follows, map< char, set<char> > firsts, char non_term)
{

    for (auto it = gram.begin(); it != gram.end(); ++it)
    {
        try
        {
            bool finished = true;
            auto ch = it->second.begin();
            for (; ch != it->second.end(); ++ch)
            {

                if (*ch == non_term)
                {
                    finished = false;
                    break;
                }
            }
            if (ch != it->second.end())
            {
                ++ch;
            }

            for (; ch != it->second.end() && !finished; ++ch)
            {
                if (!isupper(*ch))
                {
                    follows[non_term].insert(*ch);
                    finished = true;
                    break;
                }

                set<char> firsts_copy(firsts[*ch]);
                if (firsts_copy.find('e') == firsts_copy.end())
                {
                    follows[non_term].insert(firsts_copy.begin(), firsts_copy.end());
                    finished = true;
                    break;
                }
                firsts_copy.erase('e');
                follows[non_term].insert(firsts_copy.begin(), firsts_copy.end());

            }


            if (ch == it->second.end() && !finished)
            {
                if (follows[it->first].empty())
                {
                    find_follow(gram, follows, firsts, it->first);
                }
                follows[non_term].insert(follows[it->first].begin(), follows[it->first].end());
            }
        }
        catch (const char* error) {
        }
    }

}

map< char, set<char> > get_firsts_list(const vector<pair<char, string> >& grammar, const set<char>& non_terms)
{
    map< char, set<char> > firsts;
    for (char non_term : non_terms)
    {
        if (firsts[non_term].empty())
        {
            find_first(grammar, firsts, non_term);
        }
    }

    cout << "\n";
    cout << "First_1(): \n";

    for (auto & first : firsts)
    {
        cout << first.first << " : ";
        for (char firsts_it : first.second)
        {
            cout << firsts_it << " ";
        }
        cout << "\n";
    }

    return firsts;
}

map< char, set<char> > get_follows_list(const vector<pair<char, string> >& grammar, const map<char, set<char> >& firsts, const set<char>& non_terms) {

    map< char, set<char> > follows;
    char start_var = grammar.begin()->first;
    follows[start_var].insert('e');
    find_follow(grammar, follows, firsts, start_var);

    for (char non_term : non_terms)
    {
        if (follows[non_term].empty())
        {
            find_follow(grammar, follows, firsts, non_term);
        }
    }

    cout << "\nFollow_1(): \n";
    for (auto & follow : follows)
    {
        cout << follow.first << " : ";
        for (char follows_it : follow.second)
        {
            cout << follows_it << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    return follows;
}

void build_parsing_table(const vector<pair<char, string> >& grammar,
                         const set<char>& non_terms,
                         const set<char>& terms,
                         map<char, set<char>> firsts,
                         map< char, set<char> > follows,
                         int parse_table[100][100])
{

    fill(&parse_table[0][0], &parse_table[0][0] + 10000 / sizeof(parse_table[0][0]), -1);
    for (auto prod = grammar.begin(); prod != grammar.end(); ++prod) {
        string rhs = prod->second;
        set<char> next_list;
        bool finished = false;
        for (char& rh : rhs) {
            if (!isupper(rh)) {
                if (rh != 'e') {
                    next_list.insert(rh);
                    finished = true;
                    break;
                }
                continue;
            }

            set<char> firsts_copy(firsts[rh].begin(), firsts[rh].end());
            if (firsts_copy.find('e') == firsts_copy.end()) {
                next_list.insert(firsts_copy.begin(), firsts_copy.end());
                finished = true;
                break;
            }
            firsts_copy.erase('e');
            next_list.insert(firsts_copy.begin(), firsts_copy.end());
        }
        if (!finished) {
            next_list.insert(follows[prod->first].begin(), follows[prod->first].end());
        }

        for (char ch : next_list) {
            int row = distance(non_terms.begin(), non_terms.find(prod->first));
            int col = distance(terms.begin(), terms.find(ch));
            int prod_num = distance(grammar.begin(), prod);
            if (parse_table[row][col] != -1) {
                cout << "Collision at [" << row << "][" << col << "] for production " << prod_num << "\n";
                continue;
            }
            parse_table[row][col] = prod_num;
        }

    }
    cout << "Parsing Table: \n";
    cout << "   ";
    for (auto i = terms.begin(); i != terms.end(); ++i) {
        cout << *i << " ";
    }
    cout << "\n";
    for (auto row = non_terms.begin(); row != non_terms.end(); ++row) {
        cout << *row << "  ";
        for (int col = 0; col < terms.size(); ++col) {
            int row_num = distance(non_terms.begin(), row);
            if (parse_table[row_num][col] == -1) {
                cout << "- ";
                continue;
            }
            cout << parse_table[row_num][col] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}


void print_starting_operations(string input_string,
                               const vector<pair<char, string> >& grammar,
                               const set<char>& terms, const set<char>& non_terms,
                               int parse_table[100][100])
{

    input_string.push_back('$');
    stack<char> st;
    st.push('$');
    st.push('S');

    for (char & ch : input_string) {
        if (terms.find(ch) == terms.end()) {
            throw invalid_argument("Input string is invalid");
        }
    }

    bool accepted = true;
    cout << "Starting operations: " << endl;
    while (!st.empty() && !input_string.empty()) {
        if (input_string[0] == st.top()) {
            st.pop();
            input_string.erase(0, 1);
        }
        else if (!isupper(st.top())) {
            cout << "Unmatched terminal found\n";
            accepted = false;
            break;
        }
        else {
            char stack_top = st.top();
            int row = distance(non_terms.begin(), non_terms.find(stack_top));
            int col = distance(terms.begin(), terms.find(input_string[0]));
            int prod_num = parse_table[row][col];

            if (prod_num == -1) {
                cout << "No production found in parse table\n";
                accepted = false;
                break;
            }

            st.pop();
            string rhs = grammar[prod_num].second;
            cout << prod_num + 1 << ", ";
            if (rhs[0] == 'e') {
                continue;
            }
            for (auto ch = rhs.rbegin(); ch != rhs.rend(); ++ch) {
                st.push(*ch);
            }
        }
    }


    if (accepted) {
        cout << "Input string is accepted\n";
    }
    else {
        cout << "Input string is rejected\n";
    }
}