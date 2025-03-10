#include <algorithm>
#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#define NUMBER_OF_MEDALS 3

typedef size_t medal_t;
typedef size_t line_t;
typedef size_t value_of_medal_t;
typedef size_t score_t;
typedef std:: pair<size_t, std:: string> score_and_name_t;

using namespace std;

// Compares pairs by first element in descending order and if they're equal by second element lexicographically.
bool custom_comparator(const score_and_name_t &pair1,const score_and_name_t &pair2) {
    return pair1.first > pair2.first || (pair1.first == pair2.first && pair1.second < pair2.second);
}

void printing_error(size_t number_of_line) {
    cerr << "ERROR" << " " << number_of_line << endl;
}

int main() {
    // Map with name of country and an array with amount of each type of medal.
    unordered_map<string, array<medal_t, NUMBER_OF_MEDALS>> classification;
    string line;
    line_t number_of_line = 1;

    // Regexes checking if input is correct. They work only when NUMBER_OF_MEDALS is equal to three.
    const regex pattern_adding("([A-Z][a-zA-Z ]*[a-zA-Z]) ([0-3])");
    const regex pattern_disqualification("-([A-Z][a-zA-Z ]*[a-zA-Z]) ([1-3])");
    const regex pattern_print_results("=([1-9]\\d{0,5}) ([1-9]\\d{0,5}) ([1-9]\\d{0,5})");

    while (getline(cin, line)) {
        smatch splitted_line;

        if (regex_match(line, splitted_line, pattern_adding)) {
            string name_of_country = splitted_line[1];
            size_t which_medal = stoul(splitted_line[2]);

            classification.insert({name_of_country, {}});
            if (which_medal != 0) {
                classification[name_of_country][which_medal - 1]++;
            }
        }
        else if (regex_match(line, splitted_line, pattern_disqualification)) {
            string name_of_country = splitted_line[1];
            size_t which_medal = stoul(splitted_line[2]);

            if (!classification.contains(name_of_country)) {
                printing_error(number_of_line);
            }
            else if (classification[name_of_country][which_medal - 1] > 0) {
                classification[name_of_country][which_medal - 1]--;
            }
            else {
                printing_error(number_of_line);
            }
        }
        else if (regex_match(line, splitted_line, pattern_print_results)) {
            vector<value_of_medal_t> value;

            for (size_t i = 1; i < NUMBER_OF_MEDALS + 1; i++) {
                value.push_back(stoul(splitted_line[i]));
            }

            // Vector with score of each country and it's name.
            vector<score_and_name_t> ranking;
            
            // Calculating score of each country and making vector of pairs with score and name of country.
            for (auto &countries: classification) {
                string name = countries.first;
                size_t score = 0;

                for (size_t i = 0; i < NUMBER_OF_MEDALS; i++) {
                    score += value[i] * countries.second[i];
                }
                ranking.push_back({score, name});
            }

            sort(ranking.begin(), ranking.end(), custom_comparator);

            size_t place = 1;
            for (size_t i = 0; i < ranking.size(); i++) {
                // If country has lower score than previous one it's not placed ex aequo so it's position is changed.
                if (i != 0 && ranking[i-1].first != ranking[i].first) {
                    place = i + 1;
                }
                cout << place << ". " << ranking[i].second << endl;
            }
        }
        else {
            printing_error(number_of_line);
        }
        number_of_line++;
    }
    return 0;
}
