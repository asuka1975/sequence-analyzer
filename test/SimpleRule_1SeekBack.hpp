#ifndef SIMPLE_RULE_1SEEKBACK_HPP
#define SIMPLE_RULE_1SEEKBACK_HPP

#include "Error.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"
#include <string>

struct SimpleRule_1SeekBack : public virtual asuka1975::Rule<char, std::string, Error> {
protected:
    asuka1975::ReadStatus readInternal(const char& item) override {
        if(status != asuka1975::ReadStatus::Continue) {
            status = asuka1975::ReadStatus::Reject;
            return status;
        }
        readCount++;
        if('a' <= item && item <= 'z') {
            memory += item;
            status = asuka1975::ReadStatus::Continue;
        } else if(item == '+') {
            status = asuka1975::ReadStatus::Complete;
        } else {
            status = asuka1975::ReadStatus::Reject;
        }

        return status;
    }

    std::size_t getSeekBackCountInternal() const noexcept override {
        if(status == asuka1975::ReadStatus::Complete) {
            return 1;
        } else if (status == asuka1975::ReadStatus::Continue) {
            return 0;
        } else {
            return readCount;
        }
    }

    void resetInternal() override {
        memory = "";
        readCount = 0;
    }
public:
    asuka1975::Result<Error, std::string> create() const override {
        if(status == asuka1975::ReadStatus::Complete) {
            return asuka1975::Result<Error, std::string> { memory };
        } else {
            return asuka1975::Result<Error, std::string> { getError() };
        }
    }

    Error getError() const noexcept override {
        return Error { 1 };
    }

    std::string memory = "";
    std::size_t readCount = 0;
    asuka1975::ReadStatus status;
};

#endif