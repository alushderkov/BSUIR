#include <iostream>
#include <locale.h>
#include <string>
#include <set>
#include <limits>
#include <fstream>

using namespace std;

enum TCharType {
    S,
    A
};

enum TState {
    State0 = 0,
    State1 = 1,
    State2 = 2,
    State3 = 3,
    State4 = 4
};

const TState Transitions[5][2] = {
    {State0, State0},
    {State0, State2},
    {State2, State3},
    {State0, State4},
    {State2, State3}
};

const bool IsFinalState[5] = {
    false, false, false, true, false
};

int GetCharType(char C) {
    if (C == '\'') {
        return 1;
    }
    else {
        return 0;
    }
};

bool CheckString(const std::string Line) {
    TState State = State1;
    std::size_t size = Line.size();
    for (int i = 0; i <= size-1; ++i) {
        State = Transitions[State][GetCharType(Line[i])];
    };
    return IsFinalState[State];
};

int main()
{
    string Line;
    setlocale(LC_ALL, "Russian");
    string FilePath = "D:\\Учёба\\КТ\\LR1.txt";
    ifstream MyFile(FilePath);
        do {
        std::cout << "Нажмите 0 для ввода с клавиатуры или 1 для чтения из файла:" << std::endl;
        bool Input;
        bool ValInput = 0;
        while (!ValInput) {
            std::cin >> Input;

            if (std::cin.fail()) {
                std::cout << "ОШИБКА! Введите 0 или 1" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                ValInput = 1;
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!Input) {
            std::cout << "Введите строку или stop, если хотите закончить:" << std::endl;
            getline(cin, Line);
        }

        if (Input) {
            if (!MyFile.is_open()) {
                std::cout << "Не удается открыть файл!" << std::endl;
            }
            if (!MyFile.eof()) {
                std::getline(MyFile, Line);
                std::cout << Line;
            }
            else {
                std::cout << std::endl << "Файл закончился!" << std::endl;
                Line = "stop";
            }
        }

        std::cout << std::endl;
            if (Line != "stop") {
                if (CheckString(Line)) {
                std:cout << "Введённая строка является строковым литералом в языке Pascal" << std::endl << std::endl;
                }
                else {
                    std::cout << "Введённая строка НЕ является строковым литералом в Pascal" << std::endl << std::endl;
                }

                std::cout << "Подстроки-литералы:" << std::endl;

                int counterS = 0;

                std::ofstream outFile("output.txt");

                std::size_t size = Line.size();
                //bool flag_Ap = 0;
                bool flag_N = 0;
                std::set <std::string> tempSet;
                tempSet.insert(Line);
                for (int i = 0; i <= size - 1; ++i) {
                    for (int k = size - i; k >= 3; --k) {
                        for (int j = 2; j <= size - i - k + 3; ++j) {
                            std::string subLine = Line.substr(i, j);
                            if (CheckString(subLine)) {
                                if (tempSet.find(subLine) == tempSet.end()) {
                                    std::cout << subLine << std::endl;
                                    tempSet.insert(subLine);
                                    flag_N = 1;
                                    ++counterS;
                                }
                            }
                        }
                    }
                }
                outFile << counterS;    
                outFile.close(); 

                if (!flag_N) {
                    std::cout << "Среди подстрок литералов не обнаружено" << std::endl << std::endl;
                }
                else { std::cout << std::endl; };

            }
        } while (Line != "stop");
        MyFile.close();
    return 0;
}