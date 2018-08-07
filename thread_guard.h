#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H


class thread_guard
{
    std::thread &t;
public:
    explicit thread_guard(std::thread &t_):
        t(t_)
    {}
    ~thread_guard();
};

#endif // THREAD_GUARD_H
