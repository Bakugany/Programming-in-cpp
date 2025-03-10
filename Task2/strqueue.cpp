#include "strqueue.h"

#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

namespace cxx {
    using Id_t = unsigned long;
    using Map_t = std::unordered_map<Id_t, std::deque<std::string> >;

    // Initialization of map of queues.
    static Map_t &map_of_queues() {
        static Map_t map = Map_t();
        return map;
    }

    // Function returns pointer to queue with given id.
    static std::deque<std::string> *find_queue(Id_t id) {
        Map_t &map = map_of_queues();
        auto found_queue_iterator = map.find(id);

        if (found_queue_iterator == map.end())
            return nullptr;

        return &found_queue_iterator->second;
    }

    // Function prints function name and it's parameters.
    static void debug_function_call_arguments(const std::string &function_name,
                                              const std::vector<Id_t> &arguments) {
        std::cerr << function_name << "(";
        if (!arguments.empty())
            for (size_t i = 0; i < arguments.size() - 1; i++)
                std::cerr << arguments[i] << ", ";

        if (!arguments.empty())
            std::cerr << arguments[arguments.size() - 1];

        std::cerr << ")" << std::endl;
    }

    static void debug_function_call_arguments(const std::string &function_name,
                                              const std::vector<Id_t> &arguments, const char *str) {
        std::cerr << function_name << "(";
        if (!arguments.empty())
            for (size_t i = 0; i < arguments.size() - 1; i++)
                std::cerr << arguments[i] << ", ";

        if (!arguments.empty())
            std::cerr << arguments[arguments.size() - 1];

        if (str == nullptr)
            std::cerr << ", NULL";
        else
            std::cerr << ", \"" << str << "\"";

        std::cerr << ")" << std::endl;
    }

    // Function prints the result of the function.
    static void debug_function_call_result(const std::string &function_name,
                                           const std::string &result) {
        std::cerr << function_name << " returns " << "\"" << result << "\"" << std::endl;
    }

    static void debug_function_call_result(const std::string &function_name,
                                           const Id_t result) {
        std::cerr << function_name << " returns " << result << std::endl;
    }

    static void debug_function_call_result(const std::string &function_name,
                                           const int32_t result) {
        std::cerr << function_name << " returns " << result << std::endl;
    }

    static void debug_function_call_result(const std::string &function_name) {
        std::cerr << function_name << " returns " << "NULL" << std::endl;
    }

    // Function prints if function is done or if it failed.
    static void debug_function_call_execution_status(
        const std::string &function_name, const std::string &status) {
        std::cerr << function_name << " " << status << std::endl;
    }

    // Function prints that queue with such id does not exist.
    static void debug_function_call_queue_not_found(
        const std::string &function_name, Id_t id) {
        std::cerr << function_name << ": queue " << id << " does not exist" << std::endl;
    }

    // Function prints at queue with given id does not have element at given position.
    static void debug_function_call_not_existing_element(
        const std::string &function_name, Id_t id, std::size_t position) {
        std::cerr << function_name << ": queue " << id
                << " does not contain string at position " << position << std::endl;
    }

    // Function creates new queue and returns it's id.
    Id_t strqueue_new() {
        static Id_t number_of_queues = 0;
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{});

        map_of_queues()[number_of_queues] = std::deque<std::string>();
        number_of_queues++;

        if constexpr (debug)
            debug_function_call_result(__func__, number_of_queues - 1);

        return number_of_queues - 1;
    }

    // Function deletes queue with given id, otherwise does nothing.
    void strqueue_delete(Id_t id) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id});

        auto found_queue_iterator = find_queue(id);
        if (found_queue_iterator != nullptr) {
            map_of_queues().erase(id);
            if constexpr (debug)
                debug_function_call_execution_status(__func__, "done");
            return;
        }

        if constexpr (debug)
            debug_function_call_queue_not_found(__func__, id);
    }

    // Function returns size of queue with given id.
    std::size_t strqueue_size(Id_t id) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id});

        auto queue = find_queue(id);

        if (queue == nullptr) {
            if constexpr (debug) {
                debug_function_call_queue_not_found(__func__, id);
                debug_function_call_result(__func__, 0);
            }
            return 0;
        }

        if constexpr (debug)
            debug_function_call_result(__func__, queue->size());

        return queue->size();
    }

    // Function inserts given string to queue with given number at given position. 
    // If given string is null or queue doesn't exist does nothing.
    // If size of queue is smaller than position it places string at the end of queue.
    void strqueue_insert_at(Id_t id, std::size_t position, const char *str) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id, position}, str);

        auto queue = find_queue(id);
        if (queue == nullptr) {
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id);
            return;
        }

        if (str == nullptr) {
            if constexpr (debug)
                debug_function_call_execution_status(__func__, "failed");
            return;
        }

        if (position >= queue->size()) {
            queue->emplace_back(str);
            if constexpr (debug)
                debug_function_call_execution_status(__func__, "done");
            return;
        }

        queue->emplace(queue->begin() + position, str);
        if constexpr (debug)
            debug_function_call_execution_status(__func__, "done");
    }

    // Function removes element at given position from queue with given id.
    // If given queue does not exist or there is no such position in that queue does nothing.
    void strqueue_remove_at(Id_t id, std::size_t position) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id, position});

        auto queue = find_queue(id);
        if (queue == nullptr) {
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id);
            return;
        }

        if (position < queue->size()) {
            queue->erase(queue->begin() + position, queue->begin() + position + 1);
            if constexpr (debug)
                debug_function_call_execution_status(__func__, "done");
        } else if constexpr (debug)
            debug_function_call_not_existing_element(__func__, id, position);
    }

    // Function returns pointer to element at given position in given queue.
    // If there is no such element or given queue does not exist returns NULL.
    const char *strqueue_get_at(Id_t id, std::size_t position) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id, position});

        auto queue = find_queue(id);
        if (queue == nullptr) {
            if constexpr (debug){
                debug_function_call_queue_not_found(__func__, id);
                debug_function_call_result(__func__);
            }
            return NULL;
        }

        if (queue->size() <= position) {
            if constexpr (debug){
                debug_function_call_not_existing_element(__func__, id, position);
                debug_function_call_result(__func__);
            }
            return NULL;
        }

        if constexpr (debug)
            debug_function_call_result(__func__, queue->at(position));
        return queue->at(position).c_str();
    }

    // Function clears queue with given id.
    void strqueue_clear(Id_t id) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id});
        auto queue = find_queue(id);
        if (queue == nullptr) {
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id);
            return;
        }

        queue->clear();
        if constexpr (debug)
            debug_function_call_execution_status(__func__, "done");
    }

    // Function compares two queues lexicographically.
    // If queue with such id does not exist it's treated as empty queue.
    int strqueue_comp(Id_t id1, Id_t id2) {
        if constexpr (debug)
            debug_function_call_arguments(__func__, std::vector<Id_t>{id1, id2});

        auto queue_1 = find_queue(id1);
        auto queue_2 = find_queue(id2);

        if (queue_1 == nullptr && queue_2 == nullptr) {
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id1);
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id2);
            if constexpr (debug)
                debug_function_call_result(__func__, 0);
            return 0;
        }

        if (queue_1 == nullptr) {
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id1);

            if (queue_2->empty()) {
                if constexpr (debug)
                    debug_function_call_result(__func__, 0);
                return 0;
            }

            if constexpr (debug)
                debug_function_call_result(__func__, -1);
            return -1;
        }

        if (queue_2 == nullptr) {
            if constexpr (debug)
                debug_function_call_queue_not_found(__func__, id2);

            if (queue_1->empty()) {
                if constexpr (debug)
                    debug_function_call_result(__func__, 0);
                return 0;
            }

            if constexpr (debug)
                debug_function_call_result(__func__, 1);
            return 1;
        }

        if (*queue_1 > *queue_2) {
            if constexpr (debug)
                debug_function_call_result(__func__, 1);
            return 1;
        }

        if (*queue_2 > *queue_1) {
            if constexpr (debug)
                debug_function_call_result(__func__, -1);
            return -1;
        }
        if constexpr (debug)
            debug_function_call_result(__func__, 0);
        return 0;
    }
}
