#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int findGCD(const vector<int> &d)
{
    if (d.empty())
    {
        return 0;
    }

    int gcd = d[0];

    for (size_t i = 1; i < d.size(); ++i)
    {
        int b = d[i];
        while (b != 0)
        {
            int temp = b;
            b = gcd % b;
            gcd = temp;
        }
    }

    return gcd;
}

string decryptShiftCipher(const string &str, int shift)
{
    string plaintext = str;

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
    ifstream inputFile("input.txt");
    if (!inputFile)
    {
        cerr << "Error: Unable to open the input file." << endl;
        return 1;
    }

    string input;
    getline(inputFile, input);

    inputFile.close();

    transform(input.begin(), input.end(), input.begin(), ::tolower);

    map<string, vector<int>> bigrams;

    int minBigramLength = 3;
    int maxBigramLength = 8;

    for (int len = minBigramLength; len <= maxBigramLength; len++)
    {
        for (int i = 0; i < input.length() - len + 1; i++)
        {
            string bigram = input.substr(i, len);
            bigrams[bigram].push_back(i);
        }
    }

    vector<int> differences;
    for (const auto &pair : bigrams)
    {
        const vector<int> &indexes = pair.second;

        if (indexes.size() >= 2)
        {
            int diff = indexes[1] - indexes[0];
            differences.push_back(diff);
        }
    }

    vector<int> d;
    for (int diff : differences)
    {
        if (diff <= 20)
            d.push_back(diff);
    }

    cout << "the Differences between the identical strings in the paragraph: ";
    for (int zz : d)
    {
        cout << zz << " , ";
    }
    cout << endl
         << endl;

    int keySize = findGCD(d);
    cout << "So , Possible key length is: " << findGCD(d) << endl;

    vector<string> keyLengthStrings;

    for (size_t i = 0; i < keySize; ++i)
    {
        string indexString;
        for (size_t j = i; j < input.length(); j += keySize)
        {
            indexString.push_back(input[j]);
        }
        keyLengthStrings.push_back(indexString);
    }

    // Output the key length strings
    // cout << "Key length strings:" << endl;
    // for (const string &str : keyLengthStrings)
    // {
    //     cout << str << endl
    //          << endl
    //          << endl;
    // }

    vector<int> shifted_by;
    string theMainKey;

    for (string str : keyLengthStrings)
    {
        double expectedIoC = 0.067;

        int bestShift = -1;
        double closestIoC = 1.0;

        for (int shift = 0; shift <= 25; ++shift)
        {
            string decryptedText = decryptShiftCipher(str, shift);
            double ioc = IOC(decryptedText);

            if (abs(ioc - expectedIoC) < abs(closestIoC - expectedIoC))
            {
                closestIoC = ioc;
                bestShift = shift;
            }
        }

        shifted_by.push_back(bestShift);
        theMainKey.push_back('a' + ((26 - bestShift) % 26));
    }

    for (int i = 0; i < keyLengthStrings.size(); i++)
    {
        // cout << "keyLengthStrings[0] is : " << keyLengthStrings[i][0] << " and the keyLengthStrings[0] - bestShift is  " << keyLengthStrings[i][0] - shifted_by[i] << endl;

        keyLengthStrings[i] = decryptShiftCipher(keyLengthStrings[i], shifted_by[i]);
    }
    cout << endl;
    cout << endl;

    cout << "Key : " << theMainKey << endl
         << endl
         << endl;

    size_t maxLength = 0;
    for (const string &str : keyLengthStrings)
    {
        if (str.length() > maxLength)
        {
            maxLength = str.length();
        }
    }

    cout << "PlainText: \n\n";
    for (size_t i = 0; i < maxLength; ++i)
    {
        for (const string &str : keyLengthStrings)
        {
            if (i < str.length())
            {
                cout << str[i];
            }
            else
            {
                cout << ' ';
            }
        }
    }

    cout << endl
         << endl
         << endl;

    return 0;
}
