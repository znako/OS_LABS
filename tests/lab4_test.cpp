#include<gtest/gtest.h>

#include<filesystem>
#include<fstream>
#include<iostream>
#include<memory>

#include<vector>
#include<string>
#include<set>
#include<algorithm>

namespace fs = std::filesystem;

extern "C" {
    #include "parent.h"
}

TEST(FourthLabTest, TestWithIO) 
{
    const char* fileWithInput = "input.txt";
    const char* fileWithOutput1 = "output1.txt";
    const char* fileWithOutput2 = "output2.txt";

    std::vector<std::string> input = {
        "Work",
        "labochka",
        "Work",
        "1234aaron",
        "lorem",
        "ipsum",
        "dolor",
        "sit",
        "amet"
    };

    std::vector<std::string> expectedOutput1 = {
        "1234rn",
        "lrm",
        "psm",
        "dlr",
        "st",
    };
    std::vector<std::string> expectedOutput2 = {
        "Wrk",
        "lbchk",
        "Wrk",
        "mt"
    };

    std::ofstream fout(fileWithInput);

    fout << fileWithOutput1 << '\n';
    fout << fileWithOutput2 << '\n';

    for (const auto &line : input){
        fout << line << '\n';
    }

    fout.close();

    FILE* fin = fopen(fileWithInput, "r");

    ParentRoutine(fin);

    fclose(fin);

    std::ifstream fin1(fileWithOutput1);
    std::ifstream fin2(fileWithOutput2);
    
    ASSERT_TRUE(fin1.good());
    ASSERT_TRUE(fin2.good());

    std::string strRes1;
    std::vector<std::string> vecRes1;
    std::string strRes2;
    std::vector<std::string> vecRes2;

    while (fin1 >> strRes1){
        vecRes1.push_back(std::move(strRes1));
    }
    
    while (fin2 >> strRes2){
        vecRes2.push_back(std::move(strRes2));
    }

    fin1.close();
    fin2.close();

    for (int i = 0; i < (int)vecRes1.size(); ++i){
        EXPECT_EQ(vecRes1[i], expectedOutput1[i]);
    }
    for (int i = 0; i < (int)vecRes2.size(); ++i){
        EXPECT_EQ(vecRes2[i], expectedOutput2[i]);
    }

    auto removeIfExists = [](const char* path) {
        if (fs::exists(path)) {
            fs::remove(path);
        }
    };

    removeIfExists(fileWithInput);
    removeIfExists(fileWithOutput1);
    removeIfExists(fileWithOutput2);
}