#ifndef QUERY_GENERATOR_HPP
#define QUERY_GENERATOR_HPP

#include <string>

namespace cs453 {

enum QueryState {
    QueryStateNoMitigation,
    QueryStateWeakMitigation,
    QueryStateStrongMitigation,
};

std::string generateQuery(std::string &username, std::string &password, QueryState state = QueryStateNoMitigation);

}; // namespace cs453

#endif
