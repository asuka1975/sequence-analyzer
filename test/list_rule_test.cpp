#include "Error.hpp"
#include "SimpleRule_0SeekBack.hpp"
#include "result/result.hpp"
#include "sequence-analyzer/delivable/builder/list_builder.hpp"
#include "sequence-analyzer/sequence-analyzer.hpp"
#include <iostream>

#include <gtest/gtest.h>
#include <memory>

#include "sequence-analyzer/rule/rule.hpp"
#include "sequence-analyzer/rule/rule_list.hpp"


namespace {

struct SimpleBuilder;

class RuleSimpleTest1 : public ::testing::Test {
protected:
    void SetUp() override {
        std::list<std::unique_ptr<asuka1975::Rule<char, std::string, Error>>> rules;
        rules.push_back(std::make_unique<SimpleRule_0SeekBack>());
        rules.push_back(std::make_unique<SimpleRule_0SeekBack>());
        std::unique_ptr<asuka1975::Rule<char, std::string, Error>> rule = std::make_unique<asuka1975::RuleList<char, std::string, Error>>(std::move(rules), std::make_unique<SimpleBuilder>());
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::move(rule));
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

struct SimpleBuilder : public virtual asuka1975::ListBuilder<std::string, Error> {
    void add(std::string output) override {
        value = output;
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

TEST_F(RuleSimpleTest1, TwoRule_0SeekBackCase) {
    std::string s = "aa";
    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(RuleSimpleTest1, CompleteAndReject_0SeekBackCase) {
    std::string s = "ac";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(RuleSimpleTest1, RejectAndComplete_0SeekBackCase) {
    std::string s = "ca";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

}