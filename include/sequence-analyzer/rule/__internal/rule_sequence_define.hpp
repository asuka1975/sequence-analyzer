#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_SEQUENCE_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_SEQUENCE_DEFINE_HPP

#include "../rule_sequence_declare.hpp"

#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"
#include <iterator>
#include <memory>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline RuleSequence<TItem, TOutput, TError>::RuleSequence(std::unique_ptr<Rule<TItem, TOutput, TError>> rule, std::unique_ptr<SequenceBuilder<TOutput, TError>> builder) : rule(std::move(rule)), builder(std::move(builder)) {}

    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleSequence<TItem, TOutput, TError>::readInternal(const TItem& item) {
        ReadStatus status = rule->read(item);
        seekBackCount = rule->getSeekBackCount();
        if(status == ReadStatus::Complete) {
            auto result = rule->create();
            if(result.hasValue()) {
                builder->add(result.get());
            } else {
                return ReadStatus::Reject;
            }
            rule->reset();
        }
        if(status == ReadStatus::Reject) {
            if(builder->ready()) {
                return ReadStatus::Complete;
            } else {
                return status;
            }
        }
        return ReadStatus::Continue;
    }

    template <class TItem, class TOutput, class TError>
    inline Result<TError, TOutput> RuleSequence<TItem, TOutput, TError>::create() const {
        return builder->create();
    }

    template <class TItem, class TOutput, class TError>
    inline TError RuleSequence<TItem, TOutput, TError>::getError() const noexcept {
        return rule->getError();
    }

    template <class TItem, class TOutput, class TError>
    inline std::size_t RuleSequence<TItem, TOutput, TError>::getSeekBackCountInternal() const noexcept {
        return seekBackCount;
    }

    template <class TItem, class TOutput, class TError>
    inline void RuleSequence<TItem, TOutput, TError>::reset() {
        seekBackCount = 0;
        rule->reset();
        builder->reset();
    }
}

#endif