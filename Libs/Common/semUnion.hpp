#ifndef _SEM_UNION_HPP_
#define _SEM_UNION_HPP_

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};

#endif /* _SEM_UNION_HPP_ */
