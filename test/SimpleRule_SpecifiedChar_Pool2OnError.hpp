#ifndef SIMPLE_RULE_SPECIFIED_CHAR_POOL2_ON_ERROR_HPP
#define SIMPLE_RULE_SPECIFIED_CHAR_POOL2_ON_ERROR_HPP

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

#include "Error.hpp"

struct SimpleRule_SpecifiedCharPool2OnError : public virtual asuka1975::Rule<char, std::string, Error> {
    SimpleRule_SpecifiedCharPool2OnError(char target) : target(target) {}

protected:
    asuka1975::ReadStatus readInternal(const char& item) override {
        if(memory == target) {
            return asuka1975::ReadStatus::Reject;
        }
        memory = item;
        if(item == target) {
            return asuka1975::ReadStatus::Complete;
        } else {
            if(errorMemory == 0) {
                errorMemory = 1;
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
};

#endif