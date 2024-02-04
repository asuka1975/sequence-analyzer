#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_HPP

#include <memory>

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    struct Rule {
        virtual ReadStatus read(const TItem& item) = 0;
        virtual Result<TError, TOutput> create() const = 0;
        virtual TError getError() const noexcept = 0;
    };
}

#endif