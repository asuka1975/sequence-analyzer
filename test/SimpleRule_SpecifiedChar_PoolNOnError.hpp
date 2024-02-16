#ifndef SIMPLE_RULE_SPECIFIED_CHAR_POOLN_ON_ERROR_HPP
#define SIMPLE_RULE_SPECIFIED_CHAR_POOLN_ON_ERROR_HPP

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

#include "Error.hpp"

struct SimpleRule_SpecifiedCharPoolNOnError : public virtual asuka1975::Rule<char, std::string, Error> {
    SimpleRule_SpecifiedCharPoolNOnError(char target, int n) : target(target), number(n) {}

protected:
    asuka1975::ReadStatus readInternal(const char& item) override {
        if(memory == target) {
            return asuka1975::ReadStatus::Reject;
        }
        memory = item;
        if(item == target) {
            return asuka1975::ReadStatus::Complete;
        } else {
            if(errorMemory < number - 1) {
                errorMemory++;
                return asuka1975::ReadStatus::Continue;
            }
            return asuka1975::ReadStatus::Reject;
        }
    }

    std::size_t getSeekBackCountInternal() const noexcept override {
        return 0;
    }

    void resetInternal() override {
        memory = 0;
        errorMemory = 0;
    }
public:
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

    char memory = 0;
    char errorMemory = 0;
    const char target;
    const int number;
};

#endif