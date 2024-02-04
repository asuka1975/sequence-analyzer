#ifndef ASUKA1975_SEQUENCE_ANALYZER_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_HPP

#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"
#include <concepts>
#include <memory>
#include <ranges>

#include <result/result.hpp>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    class SequenceAnalyzer {
    public:
        SequenceAnalyzer(std::unique_ptr<Rule<TItem, TOutput, TError>> rule);
        template <std::ranges::input_range TAnalyzeeSequence>
            requires std::same_as<std::ranges::range_value_t<TAnalyzeeSequence>, TItem>
        Result<TError, TOutput> analyze(const TAnalyzeeSequence& sequence);
    private:
        std::unique_ptr<Rule<TItem, TOutput, TError>> rule;
    };
}

#include "sequence-analyzer/sequence-analyzer.hpp"

#endif