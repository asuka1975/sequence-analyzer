#ifndef ASUKA1975_SEQUENCE_ANALYZER___INTERNAL_SEQUENCE_ANALYZER_HPP
#define ASUKA1975_SEQUENCE_ANALYZER___INTERNAL_SEQUENCE_ANALYZER_HPP

#include "../sequence-analyzer_declare.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline SequenceAnalyzer<TItem, TOutput, TError>::SequenceAnalyzer(std::unique_ptr<Rule<TItem, TOutput, TError>> rule) : rule(rule) {}

    template <class TItem, class TOutput, class TError>
    template <std::ranges::input_range TAnalyzeeSequence>
        requires std::same_as<std::ranges::range_value_t<TAnalyzeeSequence>, TItem>
    inline Result<TError, TOutput> SequenceAnalyzer<TItem, TOutput, TError>::analyze(const TAnalyzeeSequence& sequence) {
        for(const TItem& item : sequence) {
            ReadStatus status = rule->read(item);
            if(status == ReadStatus::Reject) {
                return Result<TError, TOutput> { rule->getError() };
            }
        }
        return rule->create();
    }
}

#endif