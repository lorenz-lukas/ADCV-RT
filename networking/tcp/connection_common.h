#ifndef _connection_common_h_
#define _connection_common_h_

/**
 * must be at least 8 and must fit in struct data_t.length data type
*/
#define DEFAULT_CONNECTION_BUFFER_SIZE 128

enum Types {
    net_string_t,
    net_binary_t,
    net_bool_t,
    net_char_t,
    net_u_char_t,
    net_short_int_t,
    net_u_short_int_t,
    net_int_t,
    net_u_int_t,
    net_float_t
};

struct data_t { // Must be used as method-local struct to prevent race condition.
    char type;
    /**
     * For use with string_t and binary_t,
     * on other cases must be 0. Max (2^32)-1 = 4294967295 (4GB)
    */
    unsigned int length;
    char data[DEFAULT_CONNECTION_BUFFER_SIZE];
};

#endif /* _connection_common_h_ */
