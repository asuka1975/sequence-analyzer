#include "Error.hpp"
#include "LongMatchRule_0SeekBack.hpp"
#include "SimpleRule_0SeekBack.hpp"
#include "SimpleRule_1SeekBack.hpp"
#include "SimpleRule_SpecifiedChar.hpp"
#include "result/result.hpp"
#include "sequence-analyzer/delivable/builder/list_builder.hpp"
#include "sequence-analyzer/sequence-analyzer.hpp"
#include "sequence-analyzer/rule/rule_candidates.hpp"
#include <iostream>

#include <gtest/gtest.h>
#include <memory>

#include "sequence-analyzer/rule/rule.hpp"
#include "sequence-analyzer/rule/rule_list.hpp"

namespace {

class TwoCandidatesRule_NoSeekBackTest : public ::testing::Test {
protected:
    void SetUp() override {

        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> rules;
        rules.push_back(std::make_unique<SimpleRule_0SeekBack>());
        rules.push_back(std::make_unique<LongMatchRule_0SeekBack>());
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> rule = std::make_unique<asuka1975::RuleCandidates<char, std::string, Error>>(std::move(rules));
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::move(rule));
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

TEST_F(TwoCandidatesRule_NoSeekBackTest, NormalCase_1Length_And_2Length_Target_length1) {
    std::string s = "a";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(TwoCandidatesRule_NoSeekBackTest, NormalCase_1Length_And_2Length_Target_length2) {
    std::string s = "aa";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("aa", result.get());
}

}