#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_HPP

#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"
#include <memory>
namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline RuleSequence<TItem, TOutput, TError>::RuleSequence(std::unique_ptr<Rule<TItem, TOutput, TError>> rule, std::unique_ptr<SequenceBuilder<TOutput, TError>> builder) : rule(rule), builder(builder) {}

    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleSequence<TItem, TOutput, TError>::read(const TItem& item) {
        ReadStatus status = rule->read(item);
        if(status == ReadStatus::CompleteAccept || status == ReadStatus::CompleteIgnore) {
            builder->add(rule->create());
        }
        if(status == ReadStatus::Reject) {
            return status;
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
}

#endif