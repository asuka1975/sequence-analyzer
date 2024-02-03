#ifndef ASUKA1975_SEQUENCE_ANALYZER_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_HPP

#include <concepts>
#include <ranges>

#include <result/result.hpp>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    class SequenceAnalyzer {
    public:
        template <std::ranges::input_range TAnalyzeeSequence>
            requires std::same_as<std::ranges::range_value_t<TAnalyzeeSequence>, TItem>
        Result<TError, TOutput> analyze(const TAnalyzeeSequence& sequence);
    };
}

#endif