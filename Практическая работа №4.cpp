#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstring>
#include <cctype>
#include <vector>
using namespace std;
const int maxWords = 50;
const int maxLength = 11;
const int maxInputSize = 1000;

void RemoveExtraSpaces(char* str)
{
    int n = strlen(str);
    int j = 0; 
    int count = 0;

    for (int i = 0; i < n; i++) 
    {
        if (str[i] != ' ' || (i > 0 && str[i - 1] != ' '))
        {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}

void RemoveExtraPunctuation(char* str)
{
    int n = strlen(str);
    int j = 0;

    for (int i = 0; i < n; i++)
    {
        if (!ispunct(str[i]) || (i > 0 && !ispunct(str[i - 1])))
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void FixLetterCase(char* str)
{
    int n = strlen(str);
    for (int i = 1; i < n; i++)
    {
        if (str[i - 1] != ' ' && isupper(str[i]))
            str[i] = tolower(str[i]);
    }
}

void EditText(char* text) 
{
    RemoveExtraSpaces(text);
    RemoveExtraPunctuation(text);
    FixLetterCase(text);
}

int TextToWords(const char* text, char(&words)[maxWords][maxLength])
{
    int wordCount = 0;
    char text1[maxInputSize];
    strcpy(text1, text);

    char* token = strtok(text1, " .,:;-?!()[]{}<>|/+=*&^%$#@~`№");
    while (token != nullptr && wordCount < maxWords)
    {
        strcpy(words[wordCount], token);
        wordCount++;
        token = strtok(nullptr, " .,:;-?!()[]{}<>|/+=*&^%$#@~`№");
    }

    return wordCount;
}

void ChangeRegister(char (&words)[maxWords][maxLength], int count)
{
    for (int i = 0; i < count; i++)
        words[i][0] = toupper(words[i][0]);
}

int LinearSearch(char* text, char* pattern)
{
    int n = strlen(text); 
    int m = strlen(pattern); 

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j])
                break;
        }

        if (j == m)
            return i;
    }

    return -1; 
}

vector<int> Suffixes(const char* pattern) {
    int length = strlen(pattern);
    vector<int> suffix(length, 0);

    int g, f;
    g = f = length - 1;
    for (int i = length - 2; i >= 0; --i) 
    {
        if (i > g && suffix[i + length - 1 - f] < i - g) 
            suffix[i] = suffix[i + length - 1 - f];
        else 
        {
            if (i < g) 
                g = i;
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + length - 1 - f])
                --g;
            suffix[i] = f - g;
        }
    }

    return suffix;
}

vector<int> boyerMoore(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    std::vector<int> occurrences;

    std::vector<int> shift(256, m);
    for (int i = 0; i < m - 1; ++i) {
        shift[pattern[i]] = m - i - 1;
    }

    std::vector<int> suffix = Suffixes(pattern);

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[i + j]) {
            --j;
        }
        if (j < 0) {
            occurrences.push_back(i);
            i += shift[text[i + m]];
        }
        else {
            int x = suffix[j];
            int y = j - x;
            int z = m - 1 - j;
            i += max(shift[text[i + j]] - y, z);
        }
    }

    return occurrences;
}

int main()
{
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char text[maxInputSize];
    char words[maxWords][maxLength];

    cout << "Введите последовательность слов, заканчивающуюся точкой:\n";
    cin.getline(text, maxInputSize);

    EditText(text);

    cout << "Отредактированный текст:\n";
    cout << text;
        
    int count = TextToWords(text, words);

    cout << endl << "Количество слов: " << count << endl;
    cout << "Слова в обратном порядке: " << endl;
    for (int i = count - 1; i >= 0; --i)
        cout << words[i] << " ";
    cout << endl;

    ChangeRegister(words, count);

    for (int i = 0; i < count; i++)
        cout << words[i] << " ";
    cout << endl;

    char pattern1[maxLength];
    char pattern2[maxLength];
    
    cout << "Алгоритм линейного поиска\nВведите подстроку: ";
    cin.get(pattern1, maxLength);

    int result = LinearSearch(text, pattern1);

    if (result == -1)
        cout << "Подстрока не найдена" << endl;
    else
        cout << "Подстрока найдена в позиции: " << result << endl;

    cout << "Алгоритм Бойера-Мура\nВведите подстроку: ";
    cin >> pattern2;

    vector<int> occurrences = boyerMoore(text, pattern2);

    if (!occurrences.empty()) 
    {
        cout << "Подстрока найдена на позициях: ";
        for (int i = 0; i < occurrences.size(); ++i) 
            cout << occurrences[i] << " ";
    }
    else 
        cout << "Подстрока не найдена";

    return 0;
}
