#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <chrono>
#include<Windows.h>
using namespace std;

struct number {
    char plus = '+';
    unsigned int code;
    unsigned int number;
};

struct FIO {
    string Surname;
    string Name;
    string Otchestvo;
};

struct Data {
    number num;
    FIO fam;
    int line;
};

double sum_time = 0.0;

void WRITE(string file_name, vector<Data> sort_comp) {
    fstream file_out;
    file_out.open(file_name, ios::out);
    for (int i = 0; i < sort_comp.size(); i++) {
        file_out << sort_comp[i].num.code << " " << sort_comp[i].num.number << " " << sort_comp[i].fam.Surname << " " << sort_comp[i].fam.Name << " " <<
            sort_comp[i].fam.Otchestvo << "  " << sort_comp[i].line << endl;
    }
    file_out << "SORTING TIME ==== " << sum_time << " sec. " << endl;
    file_out.close();
}

vector<Data> READ(string file_name, int size) {
    fstream file_in;
    vector<Data> arr_in;
    int line = 0;
    string Line_Data;
    string data_to;
    Data elem;
    file_in.open(file_name, ios::in);
    for (int i = 0; i < size; i++) {
        line++;
        elem.line = line;
        getline(file_in, Line_Data);
        for (int j = 0; j < 3; j++) {
            data_to += Line_Data[j];
        }
        elem.num.code = stoi(data_to);
        data_to = "";
        for (int j = 4; j < 12; j++) {
            data_to += Line_Data[j];
        }
        elem.num.number = stoi(data_to);
        data_to = "";
        int j1 = 13;
        while (Line_Data[j1] != 32) {
            data_to += Line_Data[j1];
            j1++;
        }
        elem.fam.Surname = data_to;
        data_to = "";
        j1++;
        while (Line_Data[j1] != 32) {
            data_to += Line_Data[j1];
            j1++;
        }
        elem.fam.Name = data_to;
        data_to = "";
        j1++;
        while (Line_Data[j1] != '\0') {
            data_to += Line_Data[j1];
            j1++;
        }
        elem.fam.Otchestvo = data_to;
        arr_in.push_back(elem);
        data_to = "";
    }

    return arr_in;

}

bool Num1MoreNum2(number num1, number num2) {
    if (num1.code > num2.code)
        return true;
    if (num1.code == num2.code) {
        if (num1.number > num2.number)
            return true;
        else return false;
    }
    else return false;

}

bool FIO1MoreFIO2(FIO f1, FIO f2) {
    if (f1.Surname > f1.Surname)
        return true;
    if (f1.Surname == f1.Surname) {
        if (f1.Name > f2.Name)
            return true;
        if (f1.Name == f2.Name) {
            if (f1.Otchestvo > f1.Otchestvo)
                return true;
            else return false;
        }
        return false;
    }
    return false;
}

bool Data1MoreData2(Data d1, Data d2) {
    if (Num1MoreNum2(d1.num, d2.num))
        return true;
    if (d1.num.code == d2.num.code && d1.num.number == d2.num.number) {
        if (FIO1MoreFIO2(d1.fam, d2.fam))
            return true;
        else return false;
    }
    return false;
}

vector<Data> SimpleMerg(vector<Data> left, vector<Data> right) {
    int res_mid = 0, left_border = 0, right_border = 0;
    vector<Data> result(left.size() + right.size());

    while (left_border < left.size() && right_border < right.size())
        if (Data1MoreData2(left[left_border], right[right_border]))
            result[res_mid++] = left[left_border++];
        else result[res_mid++] = right[right_border++];

    while (res_mid < result.size())
        if (left_border != left.size())
            result[res_mid++] = left[left_border++];
        else result[res_mid++] = right[right_border++];

    return result;
}

vector<Data> Simple_Sort(vector<Data> arr, int size) {
    if (arr.size() <= 1) return arr;
    vector<Data> left_arr;
    left_arr.insert(left_arr.begin(), arr.begin() + 0, arr.begin() + arr.size() / 2);
    vector<Data> right_arr;
    right_arr.insert(right_arr.begin(), arr.begin() + arr.size() / 2, arr.end());
    return SimpleMerg(Simple_Sort(left_arr, size), Simple_Sort(right_arr, size));
}

void Binary_Sort(vector<Data>& arr, int size) {
    int length = arr.size();

    for (int i = 1; i < length; ++i) {
        Data key = arr[i];
        int start = 0;
        int end = i - 1;
        while (start <= end) {
            int mid = start + (end - start) / 2;
            if (Data1MoreData2(key, arr[mid])) {
                end = mid - 1;
            }
            else {
                start = mid + 1;
                
            }
        }

        cout << "индекс вставки = " << start << "\n";
        for (int j = i - 1; j >= start; --j) {
            arr[j + 1] = arr[j];
        }
        arr[start] = key;
    }
}

int main()
{
    setlocale(LC_ALL, "");
    vector<Data> array_in = READ("input.txt", 10);
    vector<Data> array_binary = array_in;
    int size = array_in.size();

    auto label1 = std::chrono::steady_clock::now();
    Binary_Sort(array_binary, size);
    auto end1 = std::chrono::steady_clock::now();
    sum_time = chrono::duration_cast<chrono::milliseconds>(end1 - label1).count() / CLOCKS_PER_SEC;
    WRITE("binary_output.txt", array_binary);

    auto label2 = std::chrono::steady_clock::now();
    vector<Data> simple = Simple_Sort(array_in, size);
    auto end2 = std::chrono::steady_clock::now();
    sum_time = chrono::duration_cast<chrono::milliseconds>(end2 - label2).count() / CLOCKS_PER_SEC;
    WRITE("simple_output.txt", simple);

    return 0;
}

