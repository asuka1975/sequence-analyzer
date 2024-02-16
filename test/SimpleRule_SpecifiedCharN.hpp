#ifndef SIMPLE_RULE_SPECIFIED_CHARN_HPP
#define SIMPLE_RULE_SPECIFIED_CHARN_HPP

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

#include "Error.hpp"

struct SimpleRule_SpecifiedCharN : public virtual asuka1975::Rule<char, std::string, Error> {
    SimpleRule_SpecifiedCharN(char target, int n) : target(target), number(n) {}

protected:
    asuka1975::ReadStatus readInternal(const char& item) override {
        if(memory.size() == number) {
            return asuka1975::ReadStatus::Reject;
        }
        if(item == target) {
            memory += item;
            if(memory.size() < number) {
                return asuka1975::ReadStatus::Continue;
            }
            return asuka1975::ReadStatus::Complete;
        } else {
            return asuka1975::ReadStatus::Reject;
        }
    }

    std::size_t getSeekBackCountInternal() const noexcept override {
        return 0;
    }

    void resetInternal() override {
        memory = "";
    }
public:
    asuka1975::Result<Error, std::string> create() const override {
        if(memory.size() == number) {
            return asuka1975::Result<Error, std::string> { memory };
        } else {
            return asuka1975::Result<Error, std::string> { getError() };
        }
    }

    Error getError() const noexcept override {
        return Error { 1 };
    }

    std::string memory = "";
    const char target;
    const int number; 
};

#endif