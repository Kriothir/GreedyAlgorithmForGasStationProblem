

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> 
#include <vector>
using namespace std;
int num_of_stations = 0;
int road_len = 0;

struct radius {
    int left;
    int right;
};


bool sortbysec(const pair<double, double>& a, //https://www.geeksforgeeks.org/sorting-vector-of-pairs-in-c-set-1-sort-by-first-and-second/
    const pair<double, double>& b)
{
    if (a.first != b.first) {
        return b.first > a.first;
    }
    else if (b.first == a.first) {
        return b.second < a.second;

    }
}

int sizes = num_of_stations;
vector<pair<double, double>> acceptedStations;

vector<pair<double, double>> countSuitableStations(vector<pair<double, double>> interval)
{
    int temp = 0;
    //int eliminate = 0;
    int latest_station = 0;
    int checkpoint = 0;
    int start = 0;
    int end = num_of_stations - 1;
    radius pump;

    double curr_second_interval = 0;
    vector<pair<double, double>> errorPair;

    errorPair.push_back(make_pair(-1, -1));
    pair<double, double> current_interval;

    for (int pos = 0; pos < num_of_stations; pos++) {

        if (interval[latest_station].second < road_len) { // Preveri èe je trenutna desna vrednost intervala manjsa od dolzine

            if (interval[latest_station].second < interval[pos].first) { // Preveri èe med dvema intervaloma ni praznega mesta
                cout << "debug2" << endl;// pomeni, da je prazen prostor na intervalu, vrnemo -1

                return errorPair;
            }
            else { 

                if (interval[pos].second > current_interval.second) { // preveri èe ima naslednja postaja veèji doseg v desno stran intervala

                    current_interval = make_pair(interval[pos].first, interval[pos].second);// postavimo novo mejo za naslednjo postajo

                    temp = pos;
                }
                if (pos >= end) {// èe še imamo postaje na voljo
                    if (road_len > interval[pos].second) {
                        //èe ni vec postaj in nismo dosegli dolzine ceste pomeni da so na intervalu prazni prostori vrnemo -1
                        cout << "debug23" << endl;

                        return errorPair;
                    }
                    else {
                        cout << "debug1" << endl; // èe ima postaja veèji desni doseg kot je dolzina, smo prisli do konca
                        sizes--;
                        acceptedStations.push_back(make_pair(interval[pos].first, interval[pos].second));
                        return acceptedStations;

                    }

                }
                else {
                    //if (interval[latest_station].second < interval[temp + 1].first)
                    if (interval[latest_station].second < interval[pos + 1].first) {// èe desna stran trenutne izbrane postaje ne doseže levo stran intervala naslednje postaje, izloèimo naslednjo postajo in se premaknemo na naslednjo
                        sizes--;
                        acceptedStations.push_back(make_pair(interval[latest_station].first, interval[latest_station].second));

                        //latest_station = pos + 1;
                        latest_station = temp;
                    }
                }
             

            }

        }
        else {// Trenutna postaja dosegla konec intervala, konèamo
            cout << "debug3" << endl;

            return acceptedStations;
        }

    }

}
int main(int argc, char** argv)
{
    string line = "";
    string space_delimiter = " ";
    size_t pos = 0;
    vector<string> input{};
    vector<pair<double, double>> interval; //https://www.quora.com/How-can-I-use-multiple-values-with-one-key-in-a-C-STL-map

    if (argc > 1) {
        cout << "argv[1] = " << argv[1] << endl;
    }
    else {
        cout << "No file name entered. Exiting...";
        return -1;
    }

    ifstream infile(argv[1]);
    if (!infile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);
    }
    getline(infile, line);

    line = line + " ";
    {
        while ((pos = line.find(space_delimiter)) != string::npos) {
            input.push_back(line.substr(0, pos));
            line.erase(0, pos + space_delimiter.length());
        }
        road_len = stoi(input.at(0));
        num_of_stations = stoi(input.at(1));
        input.pop_back();
        input.pop_back();

        // cout << road_len << ", " << num_of_stations << endl;

        line = "";
        double first_interval = 0;
        double second_interval = 0;
        for (int i = 1; i <= num_of_stations; i++) {
            getline(infile, line);
            line = line + " ";
            while ((pos = line.find(space_delimiter)) != string::npos) {
                input.push_back(line.substr(0, pos));
                line.erase(0, pos + space_delimiter.length());

            }
            first_interval = stod(input.at(0)) - stod(input.at(1));
            second_interval = stod(input.at(0)) + stod(input.at(1));

            interval.push_back(make_pair(first_interval, second_interval));
            input.pop_back();
            input.pop_back();
            line = "";

        }
    }
    infile.close();
    sort(interval.begin(), interval.end(), sortbysec);
    vector<pair<double, double>> count = countSuitableStations(interval);

    if (count[0].first == -1) {
        cout << "-1" << endl;
    }
    else {
        cout << "Gas stations closed: " << num_of_stations - acceptedStations.size() - 1 << " Gas stations needed: " << acceptedStations.size() + 1 << endl;

    }






}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file