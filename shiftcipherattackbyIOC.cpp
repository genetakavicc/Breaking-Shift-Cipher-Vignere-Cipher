#include <iostream>
#include <bits/stdc++.h>
using namespace std;

string decryptShiftCipher(const string &input, int shift)
{
    string plaintext = input;

    for (char &c : plaintext)
        c = 'a' + ((c - 'a' + shift) % 26);
    // cout << "Plain text is: " << plaintext << endl;

    return plaintext;
}

double IOC(string &decryptedText)
{
    map<char, int> letter_freq;
    double count = 0;
    for (char c : decryptedText)
    {
        if (c >= 'a' && c <= 'z')
        {
            letter_freq[c]++;
            count++;
        }
    }
    map<char, double> letter_prob;

    for (auto &ptr : letter_freq)
    {
        letter_prob[ptr.first] = (double)(ptr.second / count);
    }

    vector<double> expectedFrequencies = {
        0.0817, 0.0149, 0.0278, 0.0425, 0.1270, 0.0223, 0.0202, 0.0609,
        0.0697, 0.0015, 0.0077, 0.0403, 0.0241, 0.0675, 0.0751, 0.0193,
        0.0009, 0.0599, 0.0633, 0.0906, 0.0276, 0.0098, 0.0236, 0.0015,
        0.0197, 0.0007};

    int z = 0;
    double ioc = 0.0;
    for (char i = 'a'; i <= 'z'; i++)
    {
        if (letter_freq[i] == 0)
        {
            z++;
            continue;
        }
        ioc = ioc + (letter_freq[i] / count) * (expectedFrequencies[z++]);
    }

    return ioc;
}

int main()
{
    ifstream inputFile("input_for_shift_cipher.txt");
    if (!inputFile)
    {
        cerr << "Error: Unable to open the input file." << endl;
        return 1;
    }

    string input;
    getline(inputFile, input);

    inputFile.close();
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    double expectedIoC = 0.067;

    int bestShift = -1;
    double closestIoC = 1.0;

    for (int shift = 0; shift <= 25; ++shift)
    {
        string decryptedText = decryptShiftCipher(input, shift);
        double ioc = IOC(decryptedText);

        if (abs(ioc - expectedIoC) < abs(closestIoC - expectedIoC))
        {
            closestIoC = ioc;
            bestShift = shift;
        }
    }

    cout << "____________________________________________________________________________________________" << endl;
    cout << endl
         << endl;
    cout << "Key : " << bestShift << endl
         << endl;
    cout << "Plaintext: " << endl;
    cout << decryptShiftCipher(input, bestShift) << endl;
    cout << "_____________________________________________________________________________________________" << endl;
    ;

    return 0;
}