#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
struct Data
{
    int timestamp;
    string variable;
    bool value;
};
bool accandcompsimfile(string pathname1, string pathname2)
{
    ifstream inFile1(pathname1), inFile2(pathname2);
    string line1, line2;
    vector<string> contentfile1, contentfile2;

    while (getline(inFile1, line1))
    {
        contentfile1.push_back(line1);
    }

    while (getline(inFile2, line2))
    {
        contentfile2.push_back(line2);
    }

    if (contentfile1.size() != contentfile2.size())
    {
        return false;
    }

    for (int i = 0; i < contentfile1.size(); ++i)
    {
        if (contentfile1[i] != contentfile2[i])
        {
            return false;
        }
    }

    return true;
}
void output(const string &variable, bool value, int TimeStamp)
{
    cout << "Variable: " << variable << ", Value: " << value << ", Timestamp: " << TimeStamp << endl;
}
int main()
{
    ifstream inFile;
    inFile.open("circ_3Var.stim");
    string line;
    vector<Data> dataVector;
    dataVector.push_back({0, "A", 0});
    dataVector.push_back({0, "B", 0});
    dataVector.push_back({0, "C", 0});

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string parcer;

        Data data;

        getline(ss, parcer, ',');
        data.timestamp = stoi(parcer);

        getline(ss, parcer, ',');
        data.variable = parcer;

        getline(ss, parcer, ',');
        data.value = stoi(parcer);

        dataVector.push_back(data);
    }
    for (int i = 0; i < dataVector.size(); i++)
    {
        output(dataVector[i].variable, dataVector[i].value, dataVector[i].timestamp);
    }
    return 0;
}