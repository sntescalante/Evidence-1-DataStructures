#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

vector<vector<string>> create_vector_from_txt_file(string file_name);
void transform_month_into_digit_and_fix_days(vector<vector<string>> &data_vector);
void get_time_value(vector<vector<string>> &data_vector);
void merge_sort(vector<vector<string>> &data_vector, int low, int high);
void merge(vector<vector<string>> &data_vector, int low, int middle ,int high);
void transform_vector_back(vector<vector<string>> &data_vector);
vector<string> binary_search(vector<vector<string>> &data_vector, string date, int &index);
int date_to_int(string date);
string words_to_numeric_date(string date);
void print_log(vector<vector<string>> &data_vector, int start, int finish);
string ask_user_for_date();


int main() {
    string file = "bitacora.txt";
    //string output_file = "results.txt";
    vector<vector<string>> data = create_vector_from_txt_file(file);
    merge_sort(data, 0, data.size() - 1);

    int starting_date_index = 0;
    int final_date_index = 0;

    cout << "INITIAL DATE" << endl;
    string starting_date = ask_user_for_date(); //Jun 01 02:06:19 (Example)
    cout << endl;

    cout << "Final DATE" << endl;
    string final_date = ask_user_for_date(); //Jun 15 08:28:50 (Example)
    cout << endl;

    vector<string> result1 = binary_search(data, starting_date, starting_date_index);
    vector<string> result2 = binary_search(data, final_date, final_date_index);

    cout << "-------------------------------------------------------------------" << endl;
    cout << "Logs between " << starting_date << " and " << final_date << endl;

    print_log(data, starting_date_index, final_date_index);
    cout << "-------------------------------------------------------------------" << endl;


    return 0;
}

//Function to store the txt file in a vector of 'string' vectors
vector<vector<string>> create_vector_from_txt_file(string file_name) {
    //Empty vector
    vector<vector<string>>  v;
    vector<string> row;
    char c;
    string line;
    ifstream file(file_name);
    //If file was not found
    if (!file.is_open()) {
        cerr << "File couldn't be opened." << endl;
        return v;
    }
    while (file.get(c)) {
        if (c == ' ') {
            row.push_back(line);
            line.clear();
        } else if (c == '\n') {
            row.push_back(line);
            v.push_back(row);
            line.clear();
            row.clear();
        } else {
            line += c;
        }
    }

    if (!line.empty()) {
        row.push_back(line);
        line.clear();
    }
    if (!row.empty()) {
        v.push_back(row);
        row.clear();
    }

    file.close();
    transform_month_into_digit_and_fix_days(v);
    get_time_value(v);
    return v;
}

void transform_month_into_digit_and_fix_days(vector<vector<string>> &data_vector) {
    string june = "06";
    string july = "07";
    string august = "08";
    string september = "09";
    string october = "10";

    for (auto &element : data_vector) {
        if (element[0] == "Jun") {
            element[0] = june;
        } else if (element[0] == "Jul") {
            element[0] = july;
        } else if (element[0] == "Aug") {
            element[0] = august;
        } else if (element[0] == "Sep") {
            element[0] = september;
        } else if (element[0] == "Oct") {
            element[0] = october;
        } else {
            cerr << "Month not in the list!" << endl;
            return;
        }

        //fixing days
        if (element[1].size() == 1) {
            element[1] = "0" + element[1];
        }
    }
}

void get_time_value(vector<vector<string>> &data_vector) {
    vector<vector<string>> new_data;
    string value;
    for (auto &vec : data_vector) {
        value.clear();
        for (auto &element : vec) {
            for (char character : element) {
                if (isdigit(character)) {
                    value += character;
                }
            }
        }
        vec.insert(vec.begin(), value.substr(0,10));
    }

}

void merge_sort(vector<vector<string>> &data_vector, int low, int high) {

    if (low < high) {
        int middle = (low + high) / 2;
        merge_sort(data_vector, low, middle);
        merge_sort(data_vector, middle + 1, high);

        merge(data_vector, low, middle, high);
    }
}

void merge(vector<vector<string>> &data_vector, int low, int middle ,int high) {
    int left_elements = middle - low + 1;
    int right_elements = high - middle;

    vector<vector<string>> left_side(left_elements);
    vector<vector<string>> right_side(right_elements);

    for (int i = 0; i < left_elements; i++) {
        left_side[i] = data_vector[low + i];
    }

    for (int j = 0; j < right_elements; j++) {
        right_side[j] = data_vector[j + middle + 1];
    }

    int i,j,k;
    i = 0, j = 0, k = low;

    while (i < left_elements && j < right_elements) {
        int left_n = stoi(left_side[i][0]);
        int right_n = stoi(right_side[j][0]);
        if (left_n <= right_n) {
            data_vector[k] = left_side[i];
            i++;
            k++;
        } else {
            data_vector[k] = right_side[j];
            j++;
            k++;
        }

    }

    while (i < left_elements) {
        data_vector[k] = left_side[i];
        i++;
        k++;
    }

    while (j < right_elements) {
        data_vector[k] = right_side[j];
        j++;
        k++;
    }
}

void transform_vector_back(vector<vector<string>> &data_vector) {

    string june = "Jun";
    string july = "Jul";
    string august = "Aug";
    string september = "Sep";
    string october = "Oct";

    for (auto &element : data_vector) {
        if (element[1] == "06") {
            element[1] = june;
        } else if (element[1] == "07") {
            element[1] = july;
        } else if (element[1] == "08") {
            element[1] = august;
        } else if (element[1] == "09") {
            element[1] = september;
        } else if (element[1] == "10") {
            element[1] = october;
        } else {
            cerr << "Month not in the list!" << endl;
            return;
        }
    }
}



vector<string> binary_search(vector<vector<string>> &data_vector, string date, int &index) {
    int low = 0;
    int high = data_vector.size() - 1;
    int middle = (high + low) / 2;
    vector<string> v;
    date = words_to_numeric_date(date);
    int wanted_date = date_to_int(date);
    int int_date;

    while (low <= high) {
        int_date = stoi(data_vector[middle][0]);
        if (wanted_date == int_date){
            index = middle;
            return data_vector[middle];
        } else if (wanted_date < int_date) {
            high = middle;

        } else {
            low = middle + 1;
        }

        middle = (high + low) / 2;
    }

    cout << "Date not found!" << endl;

    return v;
}

int date_to_int(string date) {
    string result;
    for (char c : date) {
        if (isdigit(c)) {
            result += c;
        }
    }

    return stoi(result);
}

void print_log(vector<vector<string>> &data_vector, int start, int finish) {
    transform_vector_back(data_vector);
    int count = 0;
    for (int i = start; i < finish + 1; i++) {
        count = 0;
        for (auto element : data_vector[i]) {
            if (count > 0) {
                cout << element << " ";
            }
            count++;
        }
        cout << endl;
    }
}

string words_to_numeric_date(string date) {
    string june = "06";
    string july = "07";
    string august = "08";
    string september = "09";
    string october = "10";
    string word_with_letters;
    string numeric_date;
    for (char c : date) {
        if (c != ' ' && c != ':' && c != ';') {
            word_with_letters += c ;
        }
    }
    string month;
    for (int i = 0; i < 3; i++) {
        month += word_with_letters[i];
    }

    if (month == "Jun") {
        numeric_date += june;
    } else if (month == "Jul") {
        numeric_date += july;
    } else if (month == "Aug") {
        numeric_date += august;
    } else if (month == "Sep") {
        numeric_date += september;
    } else if (month == "Oct") {
        numeric_date += october;
    }

    for (int i = 3; i < word_with_letters.size(); i++) {
        if (isdigit(word_with_letters[i])) {
            numeric_date += word_with_letters[i];
        }
    }
    return numeric_date;
}

string ask_user_for_date() {

    string answer;
    bool valid = false;

    cout << "Enter any valid date with the format [Month] [Day] [Hour]:[Minute]:[Second]" << endl;
    cout << "Example: Jun 01 02:06:19 " << endl;

    do {
        cout << "Date: ";
        getline(cin, answer);

        int digit_count = 0;
        string month;

        for (auto c : answer) {
            if (isdigit(c)) {
                digit_count ++;
            }
        }
        if (digit_count == 8) {
            valid = true;
        } else {
            cout << "Invalid date!" << endl;
        }
    } while (!valid);

    return answer;
}