#include "NetWork.h"
#include <chrono>

#include <cstddef>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>
#include <iomanip>

struct csv_whitespace : std::ctype<char>
{
    static const mask* make_table()
    {
        // make a copy of the "C" locale table
        static std::vector<mask> v(classic_table(), classic_table() + table_size);
        v[','] |= space; // comma will be classified as whitespace
        v[' '] &= ~space; // space will not be classified as whitespace
        return &v[0];
    }

    csv_whitespace(std::size_t refs = 0) : ctype(make_table(), false, refs) {}
};

struct data_info {
    double* pixels;
    int digit;
};

void draw_pix(int num, data_info* data, int w = 28, int h = 28)
{
    double* pix = data[num].pixels;
    std::cout << "pic=" << num << " is " << data[num].digit << std::endl;
    for (int i = 0, k = 0; i < w; ++i)
    {
        for (int j = 0; j < h; ++j, ++k)
        {
            std::cout << (pix[k] ? '*' : ' ');
        }
        std::cout << std::endl;
    }
}

data_NetWork ReadDataNetWork(string path) {
    data_NetWork data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file" << path << endl;
        system("pause");
    }
    else
        cout << path << "loading... \n";
    string tmp;
    int L;
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "NetWork") {
            fin >> L;
            data.L = L;
            data.size = new int[L];
            for (int i = 0; i < L; i++) {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}

data_info* ReadData(string path, const data_NetWork& data_NW, int& examples) {
    int numPix = 28 * 28;
    cout << "Examples expected=" << examples << endl;
    //
    data_info* data;
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file" << path << endl;
        system("pause");
    }
    else
        cout << path << " loading... \n";
    string tmp, line;
    fin >> line;
    std::istringstream ss(line);
    ss.imbue(std::locale(ss.getloc(), new csv_whitespace));
    ss >> tmp;
    if (tmp == "label") {
        data = new data_info[examples];
        for (int i = 0; i < examples; ++i) {
            data[i].pixels = new double[numPix];
        }
        for (int i = 0; i < examples; ++i) {
            fin >> line;
            std::istringstream ss(line);
            ss.imbue(std::locale(ss.getloc(), new csv_whitespace));
            ss >> data[i].digit;
            for (int j = 0; j < numPix; ++j) {
                ss >> data[i].pixels[j];
                data[i].pixels[j] = (data[i].pixels[j]) / 256;
            }
            if (!fin) {
                std::cout << "examples less than expected, " << i + 1 << std::endl;
                break;
            }
        }
        fin.close();
        cout << " file loaded... \n";
        const auto test_draw = true;
        if (test_draw)
        {
            cout << "some random test draw" << std::endl;
            int d_ind[] = { 0,99,999,9999,59999 };
            for (auto ind : d_ind)
            {
                if (ind >= examples) break;
                draw_pix(ind, data);
            }
        }
        return data;
    }
    else {
        cout << "Error loading:" << path << endl;
        fin.close();
        return nullptr;
    }
}


int main()
{
    NetWork NW{};
    data_NetWork NW_config;
    data_info* data;
    double ra = 0, right, predict, maxra = 0;
    int epoch = 0;
    bool study, repeat = true;
    chrono::duration<double> time;

    NW_config = ReadDataNetWork("Config.txt");
    NW.Init(NW_config);
    NW.PrintConfig();

    while (repeat) {
        cout << "STUDY? (1/0)" << endl;
        cin >> study;
        if (study) {
            int examples = 60000; //можно задать любым, <= чем в файле, тогда он будет использовать только такое количество образцов
            data = ReadData("mnist_train.csv", NW_config, examples);
            auto begin = chrono::steady_clock::now();
            while (ra / examples * 100 < 100) {
                ra = 0;
                auto t1 = chrono::steady_clock::now();
                for (int i = 0; i < examples; ++i) {
                    NW.SetInput(data[i].pixels);
                    right = data[i].digit;
                    predict = NW.ForwardFeed();
                    if (predict != right) {
                        NW.BackPropogation(right);
                        NW.WeightsUpdater(0.15 * exp(-epoch / 20.));
                    }
                    else
                        ra++;
                }
                auto t2 = chrono::steady_clock::now();
                time = t2 - t1;
                if (ra > maxra) maxra = ra;
                cout <<"EPOCH:"<< epoch
                    << "\t "<<"ACCURACY:" << ra / examples * 100 << "\t" << "MAX ACCURACY : " << maxra / examples * 100 << "\t" << "TIME:" << time.count() << std::endl;
                ++epoch;
                if (epoch == 20)
                    break;
            }
            auto end = chrono::steady_clock::now();
            time = end - begin;
            cout << "TIME:" << time.count() / 60. << "min" << endl;
            NW.SaveWeights();
        }
        else
            NW.Readweights();
        cout << "Test? (1/0)\n";
        bool test_flag;
        cin >> test_flag;
        if (test_flag) {
            int ex_tests = 10000;
            data_info* data_test;
            data_test = ReadData("mnist_test.csv", NW_config, ex_tests);
            ra = 0;
            for (int i = 0; i < ex_tests; ++i) {
                NW.SetInput(data_test[i].pixels);
                predict = NW.ForwardFeed();
                right = data_test[i].digit;
                if (right == predict)
                    ra++;
            }
            cout << "RA" << ra / ex_tests * 100 << endl;
        }
        cout << "Repeat? (1/0)\n";
        cin >> repeat;
    }
    system("pause");
    return 0;
}