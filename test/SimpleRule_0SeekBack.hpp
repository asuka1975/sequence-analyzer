#ifndef SIMPLE_RULE_0SEEKBACK_HPP
#define SIMPLE_RULE_0SEEKBACK_HPP

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

#include "Error.hpp"

struct SimpleRule_0SeekBack : public virtual asuka1975::Rule<char, std::string, Error> {
protected:
    asuka1975::ReadStatus readInternal(const char& item) override {
        if(memory == 'a') {
            return asuka1975::ReadStatus::Reject;
        }
        memory = item;
        if(item == 'a') {
            return asuka1975::ReadStatus::Complete;
        } else if(item == 'b') {
            return asuka1975::ReadStatus::Continue;
        } else {
            return asuka1975::ReadStatus::Reject;
        }
    }

    std::size_t getSeekBackCountInternal() const noexcept override {
        return 0;
    }
public:
    asuka1975::Result<Error, std::string> create() const override {
        if(memory == 'a') {
            return asuka1975::Result<Error, std::string> { std::string { memory } };
        } else {
            return asuka1975::Result<Error, std::string> { getError() };
        }
    }

    Error getError() const noexcept override {
        return Error { 1 };
    }

    void reset() override {
        memory = 0;
    }

    char memory = 0;
};

#endif