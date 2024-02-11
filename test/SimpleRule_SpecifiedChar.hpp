#ifndef SIMPLE_RULE_SPECIFIED_CHAR_HPP
#define SIMPLE_RULE_SPECIFIED_CHAR_HPP

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

#include "Error.hpp"

struct SimpleRule_SpecifiedChar : public virtual asuka1975::Rule<char, std::string, Error> {
    SimpleRule_SpecifiedChar(char target) : target(target) {}

    asuka1975::ReadStatus read(const char& item) override {
        if(memory == target) {
            return asuka1975::ReadStatus::Reject;
        }
        memory = item;
        if(item == target) {
            return asuka1975::ReadStatus::Complete;
        } else {
            return asuka1975::ReadStatus::Reject;
        }
    }

    asuka1975::Result<Error, std::string> create() const override {
        if(memory == target) {
            return asuka1975::Result<Error, std::string> { std::string { memory } };
        } else {
            return asuka1975::Result<Error, std::string> { getError() };
        }
    }

    Error getError() const noexcept override {
        return Error { 1 };
    }

    std::size_t getSeekBackCount() const noexcept override {
        return 0;
    }

    void reset() override {
        memory = 0;
    }

    char memory = 0;
    const char target;
};

#endif