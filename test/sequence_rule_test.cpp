#include "Error.hpp"
#include "LongMatchRule_0SeekBack.hpp"
#include "SimpleRule_0SeekBack.hpp"
#include "SimpleRule_1SeekBack.hpp"
#include "SimpleRule_SpecifiedChar.hpp"
#include "result/result.hpp"
#include "sequence-analyzer/delivable/builder/list_builder.hpp"
#include "sequence-analyzer/delivable/builder/sequence_builder.hpp"
#include "sequence-analyzer/rule/rule_list_declare.hpp"
#include "sequence-analyzer/rule/rule_sequence_declare.hpp"
#include "sequence-analyzer/sequence-analyzer.hpp"
#include "sequence-analyzer/rule/rule_sequence.hpp"
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

class SimpleSequenceRule_NoSeekBackTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> rule = std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::make_unique<SimpleRule_0SeekBack>(), std::make_unique<SimpleSequenceBuilder>());
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::move(rule));
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

class PlusEndRule_NoSeekBackTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> rules;
        rules.push_back(std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::make_unique<SimpleRule_SpecifiedChar>('a'), std::make_unique<SimpleSequenceBuilder>()));
        rules.push_back(std::make_unique<SimpleRule_SpecifiedChar>('+'));
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> rule = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(rules), std::make_unique<SimpleListBuilder>());
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::move(rule));
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

class PlusEndRule_Nested_NoSeekBackTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> aSequence1 = std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::make_unique<SimpleRule_SpecifiedChar>('a'), std::make_unique<SimpleSequenceBuilder>());
        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> aSequenceEndPlusRules;
        aSequenceEndPlusRules.push_back(std::move(aSequence1));
        aSequenceEndPlusRules.push_back(std::make_unique<SimpleRule_SpecifiedChar>('+'));
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> aSequenceEndPlus = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(aSequenceEndPlusRules), std::make_unique<SimpleListBuilder>());
        

        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> aSequence2 = std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::make_unique<SimpleRule_SpecifiedChar>('a'), std::make_unique<SimpleSequenceBuilder>());
        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> aSequenceEndSemicolonRules;
        aSequenceEndSemicolonRules.push_back(std::move(aSequence2));
        aSequenceEndSemicolonRules.push_back(std::make_unique<SimpleRule_SpecifiedChar>(';'));
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> aSequenceEndSemicolon = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(aSequenceEndSemicolonRules), std::make_unique<SimpleListBuilder>());

        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> rules;
        rules.push_back(std::make_unique<asuka1975::RuleSequence<char, std::string, Error>>(std::move(aSequenceEndPlus), std::make_unique<SimpleSequenceBuilder>()));
        rules.push_back(std::move(aSequenceEndSemicolon));

        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> rule = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(rules), std::make_unique<SimpleListBuilder>());
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::move(rule));
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

TEST_F(SimpleSequenceRule_NoSeekBackTest, NormalCase_1Length_And_2Length_Target_length1) {
    std::string s = "a";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(SimpleSequenceRule_NoSeekBackTest, NormalCase_1Length_And_2Length_Target_length2) {
    std::string s = "aa";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("aa", result.get());
}

TEST_F(SimpleSequenceRule_NoSeekBackTest, NormalCase_1Length_And_2Length_LongRuleReject) {
    std::string s = "ba";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(SimpleSequenceRule_NoSeekBackTest, ErrorCase1_1Length_And_2Length) {
    std::string s = "c";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(SimpleSequenceRule_NoSeekBackTest, ErrorCase2_1Length_And_2Length) {
    std::string s = "cc";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(PlusEndRule_NoSeekBackTest, NormalCase) {
    std::string s = "aa+";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("aa+", result.get());
}

TEST_F(PlusEndRule_NoSeekBackTest, ErrorCase1) {
    std::string s = "aa;";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(PlusEndRule_NoSeekBackTest, ErrorCase2) {
    std::string s = "ab;";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(PlusEndRule_NoSeekBackTest, ErrorCase3) {
    std::string s = "ab+";

    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(PlusEndRule_Nested_NoSeekBackTest, NormalCase1) {
    std::string s = "aa+a+aaaa;";

    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("aa+a+aaaa;", result.get());
}

}