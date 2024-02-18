#include "Error.hpp"
#include "LongMatchRule_0SeekBack.hpp"
#include "SimpleRule_0SeekBack.hpp"
#include "SimpleRule_1SeekBack.hpp"
#include "SimpleRule_SpecifiedChar.hpp"
#include "SimpleRule_SpecifiedCharN.hpp"
#include "SimpleRule_SpecifiedChar_PoolNOnError.hpp"
#include "result/result.hpp"
#include "sequence-analyzer/delivable/builder/list_builder.hpp"
#include "sequence-analyzer/delivable/builder/sequence_builder.hpp"
#include "sequence-analyzer/rule/rule_candidates.hpp"
#include "sequence-analyzer/rule/rule_list.hpp"
#include "sequence-analyzer/rule/rule_list_declare.hpp"
#include "sequence-analyzer/rule/rule_sequence.hpp"
#include "sequence-analyzer/sequence-analyzer.hpp"
#include "sequence-analyzer/rule/rule_candidates.hpp"
#include <iostream>

#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "sequence-analyzer/rule/rule.hpp"
#include "sequence-analyzer/rule/rule_list.hpp"

namespace {


struct SimpleListBuilder : public virtual asuka1975::ListBuilder<std::string, Error> {
    void add(std::string output) override {
        value += output;
    }

    asuka1975::Result<Error, std::string> create() override {
        if(value.empty()) {
            return asuka1975::Result<Error, std::string> { Error { 1 } };
        } else {
            return asuka1975::Result<Error, std::string> { value };
        }
    }

    bool ready() const noexcept override {
        return !value.empty();
    }

    void reset() override {
        value = "";
    }

    std::string value = "";
};

class SimpleSequenceBuilder : public virtual asuka1975::SequenceBuilder<std::string, Error> {
public:
    void add(std::string output) override {
        memory += output;
    }

    asuka1975::Result<Error, std::string> create() override {
        if(memory.empty()) {
            return asuka1975::Result<Error, std::string> { Error { 1 } };
        } else {
            return asuka1975::Result<Error, std::string> { memory };
        }
    }

    bool ready() const noexcept override {
        return !memory.empty();
    }

    void reset() override {
        memory = "";
    }
private:
    std::string memory;
};

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

class TwoCandidatesRule_SeekBackTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> candidate1List;
        candidate1List.push_back(std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::make_unique<SimpleRule_SpecifiedCharPoolNOnError>('a', 2), std::make_unique<SimpleSequenceBuilder>()));
        candidate1List.push_back(std::make_unique<SimpleRule_SpecifiedCharN>('+', 2));
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> candidate1 = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(candidate1List), std::make_unique<SimpleListBuilder>());

        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> candidate2List;
        candidate1List.push_back(std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::make_unique<SimpleRule_SpecifiedCharPoolNOnError>('a', 5), std::make_unique<SimpleSequenceBuilder>()));
        candidate1List.push_back(std::make_unique<SimpleRule_SpecifiedCharN>('+', 5));
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> candidate2 = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(candidate1List), std::make_unique<SimpleListBuilder>());

        
        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> candidatesList;
        candidatesList.push_back(std::move(candidate1));
        candidatesList.push_back(std::move(candidate2));
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> candidates = std::make_unique<asuka1975::RuleCandidates<char, std::string, Error>>(std::move(candidatesList));

        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> ruleList;
        ruleList.push_back(std::move(candidates));
        ruleList.push_back(std::make_unique<SimpleRule_SpecifiedCharN>(';', 2));

        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> rule = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(ruleList), std::make_unique<SimpleListBuilder>());
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

TEST_F(TwoCandidatesRule_NoSeekBackTest, NormalCase_1Length_And_2Length_LongRuleReject) {
    std::string s = "ba";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(TwoCandidatesRule_NoSeekBackTest, ErrorCase1_1Length_And_2Length) {
    std::string s = "c";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(TwoCandidatesRule_NoSeekBackTest, ErrorCase2_1Length_And_2Length) {
    std::string s = "cc";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(TwoCandidatesRule_SeekBackTest, NormalCase1) {
    std::string s = "aa++;;";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("aa++;;", result.get());
}

TEST_F(TwoCandidatesRule_SeekBackTest, NormalCase2) {
    std::string s = "aa+++++;;";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("aa+++++;;", result.get());
}

}