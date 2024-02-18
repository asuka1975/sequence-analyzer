#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_CANDIDATES_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE___INTERNAL_RULE_CANDIDATES_DEFINE_HPP

#include "../rule_candidates_declare.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

#include <algorithm>
#include <bits/ranges_util.h>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    template <std::ranges::input_range TCandidates>
        requires std::same_as<std::ranges::range_value_t<TCandidates>, std::unique_ptr<Rule<TItem, TOutput, TError>>>
    inline RuleCandidates<TItem, TOutput, TError>::RuleCandidates(TCandidates candidates) : finishOrder(std::ranges::size(candidates)) {
        std::ranges::move(candidates, std::back_inserter(this->candidates));
    }


    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleCandidates<TItem, TOutput, TError>::readInternal(const TItem& item) {
        history.push_back(item);
        seekBackCount++;
        std::size_t i = 0;
        for(auto& candidate : candidates) {
            if(finishOrder[i] != 0) {
                i++;
                continue;
            }
            auto status = candidate->read(item);
            if(status == ReadStatus::Complete) {
                finishOrder[i] = std::ranges::count_if(finishOrder, [](auto& v) { return v > 0; }) + 1;
                seekBackCount = 0;
                finishCount++;
            } else if(status == ReadStatus::Reject) {
                finishOrder[i] = -1;
                finishCount++;
            } else {
                auto _seekBackCount = candidate->getSeekBackCount();
                auto iter = std::prev(history.end(), _seekBackCount);
                while(iter != history.end()) {
                    auto status = candidate->read(*iter);
                    if(status == ReadStatus::Complete) {
                        finishOrder[i] = std::ranges::count_if(finishOrder, [](auto& v) { return v > 0; }) + 1;
                        seekBackCount = 0;
                        finishCount++;
                        break;
                    } else if(status == ReadStatus::Reject) {
                        finishOrder[i] = -1;
                        finishCount++;
                        break;
                    } else {
                        auto _seekBackCount = candidate->getSeekBackCount();
                        iter = std::prev(iter, _seekBackCount);
                    }
                    std::advance(iter, 1);
                }
            }

            i++;
        }


        if(finishCount == finishOrder.size()) {
            if(std::ranges::count(finishOrder, -1) == finishCount) {
                return ReadStatus::Reject;
            } else {
                return ReadStatus::Complete;
            }
        } else {
            return ReadStatus::Continue;
        }
    }

    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleCandidates<TItem, TOutput, TError>::readLastInternal(const TItem& item) {
        history.push_back(item);
        seekBackCount++;

        std::size_t i = 0;

        for(auto& candidate : candidates) {
            if(finishOrder[i] != 0) {
                i++;
                continue;
            }
            auto status = candidate->readLast(item);
            if(status == ReadStatus::Complete) {
                finishOrder[i] = std::ranges::count_if(finishOrder, [](auto& v) { return v > 0; }) + 1;
                seekBackCount = 0;
                finishCount++;
            } else {
                finishOrder[i] = -1;
                finishCount++;
            }

            i++;
        }

        if(finishCount == finishOrder.size()) {
            if(std::ranges::count(finishOrder, -1) == finishCount) {
                return ReadStatus::Reject;
            } else {
                return ReadStatus::Complete;
            }
        } else {
            return ReadStatus::Reject;
        }
    }

    template <class TItem, class TOutput, class TError>
    inline Result<TError, TOutput> RuleCandidates<TItem, TOutput, TError>::create() const {
        return (*pickupRule())->create();
    }

    template <class TItem, class TOutput, class TError>
    inline TError RuleCandidates<TItem, TOutput, TError>::getError() const noexcept {
        return (*pickupRule())->getError();
    }

    template <class TItem, class TOutput, class TError>
    inline std::size_t RuleCandidates<TItem, TOutput, TError>::getSeekBackCountInternal() const noexcept {
        if(std::ranges::find(finishOrder, 0) != std::ranges::end(finishOrder)) {
            return 0;
        }
        return (*pickupRule())->getSeekBackCount() + seekBackCount;
    }

    template <class TItem, class TOutput, class TError>
    inline void RuleCandidates<TItem, TOutput, TError>::resetInternal() {
        for(auto& candidate : candidates) {
            candidate->reset();
        }

        std::ranges::fill(finishOrder, 0);
        finishCount = 0;
        seekBackCount = 0;
    }

    template <class TItem, class TOutput, class TError>
    inline typename std::list<std::unique_ptr<Rule<TItem, TOutput, TError>>>::const_iterator RuleCandidates<TItem, TOutput, TError>::pickupRule() const {
        auto iter = std::ranges::max_element(finishOrder);
        auto index = std::distance(finishOrder.begin(), iter);
        return std::next(candidates.begin(), index);
    }
}

#endif