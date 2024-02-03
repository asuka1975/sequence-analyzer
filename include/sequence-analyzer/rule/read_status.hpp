#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_READ_STATUS_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_READ_STATUS_HPP

namespace asuka1975 {
    enum ReadStatus {
        Continue,
        CompleteAccept,
        CompleteIgnore,
        Reject,
    };
}

#endif