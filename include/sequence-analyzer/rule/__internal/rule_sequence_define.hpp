#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_SEQUENCE_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_SEQUENCE_DEFINE_HPP

#include "../rule_sequence_declare.hpp"

#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"
#include <iterator>
#include <memory>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline RuleSequence<TItem, TOutput, TError>::RuleSequence(std::unique_ptr<Rule<TItem, TOutput, TError>> rule, std::unique_ptr<SequenceBuilder<TOutput, TError>> builder) : rule(rule), builder(builder) {}

    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleSequence<TItem, TOutput, TError>::read(const TItem& item) {
        history.push_back(item);
        ReadStatus status = rule->read(item);
        if(status == ReadStatus::Complete) {
            builder->add(rule->create());
            auto seekBackCount = rule->getSeekBackCount();
            rule->reset();

            std::list<TItem> extractedHistory;
            extractedHistory.splice(extractedHistory.end(), std::move(history), std::prev(history.end(), seekBackCount), history.end());
            while(!extractedHistory.empty()) {
                auto status = this->read(extractedHistory.front());
                extractedHistory.pop_front();
                if(status == ReadStatus::Complete || status == ReadStatus::Reject) {
                    unreadedOnCompleted = extractedHistory.size();
                    return status;
                } else {
                    auto seekBackCount = rule->getSeekBackCount();
                    extractedHistory.splice(extractedHistory.begin(), std::move(history), std::prev(history.end(), seekBackCount), history.end());
                }
            }
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
    inline std::size_t RuleSequence<TItem, TOutput, TError>::getSeekBackCount() const noexcept {
        return rule->getSeekBackCount() + unreadedOnCompleted;
    }

    template <class TItem, class TOutput, class TError>
    inline void RuleSequence<TItem, TOutput, TError>::reset() {
        unreadedOnCompleted = 0;
        history.clear();
        rule->reset();
        builder->reset();
    }
}

#endif