#include "Error.hpp"
#include "SimpleRule_0SeekBack.hpp"
#include "SimpleRule_1SeekBack.hpp"
#include "sequence-analyzer/sequence-analyzer.hpp"
#include <iostream>

#include <gtest/gtest.h>
#include <memory>

namespace {
class RuleSimpleTest : public ::testing::Test {
protected:
    void SetUp() override {
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::make_unique<SimpleRule_0SeekBack>());
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

class RuleSimple1Test : public ::testing::Test {
protected:
    void SetUp() override {
        analyzer = std::make_unique<asuka1975::SequenceAnalyzer<char, std::string, Error>>(std::make_unique<SimpleRule_1SeekBack>());
    }

    std::unique_ptr<asuka1975::SequenceAnalyzer<char, std::string, Error>> analyzer;
};

TEST_F(RuleSimpleTest, Complete_0SeekBackCase) {
    std::string s = "a";
    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(RuleSimpleTest, ContinueAndComplete_0SeekBackCase) {
    std::string s = "bba";
    auto result = analyzer->analyze(s);

    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ("a", result.get());
}

TEST_F(RuleSimpleTest, ContinueInTheMiddle_0SeekBackCase) {
    std::string s = "bb";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(RuleSimpleTest, Reject_0SeekBackCase) {
    std::string s = "c";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(RuleSimpleTest, ContinueAndReject_0SeekBackCase) {
    std::string s = "bbc";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(RuleSimpleTest, CompleteAndReject_0SeekBackCase) {
    std::string s = "ac";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(RuleSimpleTest, CompleteAndComplete_0SeekBackCase) {
    std::string s = "aa";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

TEST_F(RuleSimpleTest, RejectAndComplete_0SeekBackCase) {
    std::string s = "ca";
    auto result = analyzer->analyze(s);

    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(1, result.getError().code);
}

}
