#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_CANDIDATES_DECLARE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_CANDIDATES_DECLARE_HPP

#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>
#include <ranges>
#include <utility>

#include "sequence-analyzer/rule/rule.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    class RuleCandidates : public virtual Rule<TItem, TOutput, TError> {
    public:
        template <std::ranges::input_range TCandidates>
            requires std::same_as<std::ranges::range_value_t<TCandidates>, std::unique_ptr<Rule<TItem, TOutput, TError>>>
        RuleCandidates(TCandidates candidates);
        ReadStatus read(const TItem& item) override;
        Result<TError, TOutput> create() const override;
        TError getError() const noexcept override;
        std::size_t getSeekBackCount() const noexcept override;
        void reset() override;
    private:
        typename std::list<std::unique_ptr<Rule<TItem, TOutput, TError>>>::const_iterator pickupRule() const;
    private:
        std::list<std::unique_ptr<Rule<TItem, TOutput, TError>>> candidates;
        std::vector<std::int32_t> finishOrder;
        std::int32_t finishCount = 0;
        std::size_t seekBackCount = 0;
    };
}


#endif