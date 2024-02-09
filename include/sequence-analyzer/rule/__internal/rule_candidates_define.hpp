#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_CANDIDATES_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_CANDIDATES_DEFINE_HPP

#include "../rule_candidates_declare.hpp"
#include "sequence-analyzer/rule/read_status.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cstddef>
#include <cstdint>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError, class TRulePointer>
    template <std::ranges::input_range TCandidates>
        requires std::same_as<std::ranges::range_value_t<TCandidates>, TRulePointer> && std::same_as<decltype(*std::declval<TRulePointer>()), TItem>
    inline RuleCandidates<TItem, TOutput, TError, TRulePointer>::RuleCandidates(TCandidates candidates) : candidates(std::ranges::size(candidates)), finishOrder(std::ranges::size(candidates)) {
        std::ranges::move(candidates, this->candidates);
    }


    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline ReadStatus RuleCandidates<TItem, TOutput, TError, TRulePointer>::read(const TItem& item) {
        std::size_t i = 0;
        for(auto& candidate : candidates) {
            if(finishOrder[i] != 0) {
                continue;
            }
            auto status = candidate->read(item);
            if(status == ReadStatus::Complete) {
                finishOrder[i] = std::ranges::count_if(finishOrder, [](auto& v) { return v > 0; }) + 1;
                seekBackCount = 0;
            } else if(status == ReadStatus::Reject) {
                finishOrder[i] = -1;
            }

            i++;
        }

        seekBackCount++;
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline Result<TError, TOutput> RuleCandidates<TItem, TOutput, TError, TRulePointer>::create() const {
        return (*pickupRule())->create();
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline TError RuleCandidates<TItem, TOutput, TError, TRulePointer>::getError() const noexcept {
        return (*pickupRule())->getError();
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline std::size_t RuleCandidates<TItem, TOutput, TError, TRulePointer>::getSeekBackCount() const noexcept {
        return (*pickupRule())->getSeekBackCount() + seekBackCount;
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline void RuleCandidates<TItem, TOutput, TError, TRulePointer>::reset() {
        for(auto& candidate : candidates) {
            candidate->reset();
        }

        std::ranges::fill(finishOrder, 0);
        finishCount = 0;
        seekBackCount = 0;
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline typename std::list<TRulePointer>::const_iterator RuleCandidates<TItem, TOutput, TError, TRulePointer>::pickupRule() const {
        auto iter = std::ranges::max_element(finishOrder);
        auto index = std::distance(finishOrder.begin(), iter);
        return std::next(candidates.begin(), index);
    }
}

#endif