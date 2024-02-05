#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_CANDIDATES_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_CANDIDATES_HPP

#include <algorithm>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError, class TRulePointer>
    template <std::ranges::input_range TCandidates>
        requires std::same_as<std::ranges::range_value_t<TCandidates>, TRulePointer> && std::same_as<decltype(*std::declval<TRulePointer>()), TItem>
    inline RuleCandidates<TItem, TOutput, TError, TRulePointer>::RuleCandidates(TCandidates candidates) : candidates(std::ranges::size(candidates)) {
        std::ranges::move(candidates, this->candidates);
    }


    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline ReadStatus RuleCandidates<TItem, TOutput, TError, TRulePointer>::read(const TItem& item) {
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline Result<TError, TOutput> RuleCandidates<TItem, TOutput, TError, TRulePointer>::create() const {
    }

    template <class TItem, class TOutput, class TError, class TRulePointer>
    inline TError RuleCandidates<TItem, TOutput, TError, TRulePointer>::getError() const noexcept {
    }
}

#endif