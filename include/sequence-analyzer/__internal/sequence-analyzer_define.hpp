#ifndef ASUKA1975_SEQUENCE_ANALYZER___INTERNAL_SEQUENCE_ANALYZER_HPP
#define ASUKA1975_SEQUENCE_ANALYZER___INTERNAL_SEQUENCE_ANALYZER_HPP

#include "../sequence-analyzer_declare.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include <bits/ranges_base.h>
#include <iterator>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline SequenceAnalyzer<TItem, TOutput, TError>::SequenceAnalyzer(std::unique_ptr<Rule<TItem, TOutput, TError>> rule) : rule(std::move(rule)) {}

    template <class TItem, class TOutput, class TError>
    template <std::ranges::input_range TAnalyzeeSequence>
        requires std::same_as<std::ranges::range_value_t<TAnalyzeeSequence>, TItem>
    inline Result<TError, TOutput> SequenceAnalyzer<TItem, TOutput, TError>::analyze(const TAnalyzeeSequence& sequence) {
        for(auto iter = std::ranges::begin(sequence); iter != std::ranges::end(sequence); std::advance(iter, 1)) {
            ReadStatus status;
            if(std::next(iter) == std::ranges::end(sequence)) {
                status = rule->readLast(*iter);
            } else {
                status = rule->read(*iter);
            }
            if(status == ReadStatus::Reject) {
                return Result<TError, TOutput> { rule->getError() };
            } else if(status == ReadStatus::Complete && std::next(iter) != std::ranges::end(sequence)) {
                return Result<TError, TOutput> { TError {} };
            }
            auto seekBackCount = rule->getSeekBackCount();
            if(seekBackCount != 0) {
                iter = std::ranges::prev(iter, seekBackCount);
            }
        }
        return rule->create();
    }
}

#endif