//#include "framework/tcp_server.hpp"
#include "core/weak_call.hpp"

struct A : AServer::Core::WeakCallee<A> {
    explicit A(int a) : a(a) {}
    int a;
    void func(float b) {
        std::cout << a << " " << b << std::endl;
    }
};

template <typename T>
void func(T &&ts) {
    std::cout << ts;
}

template <typename F, typename... T>
void func(F &&f, T &&... ts) {
    std::cout << f;
    func(std::forward<T>(ts)...);
}

int main() {
    //    AServer::Framework::TCPServer server;
    //    server.run();

    auto p = std::make_shared<A>(2);
    auto weak = p->makeWeakCallable(&A::func, 3.09f);
    p.reset();
    weak();

    //    func(2.001, 3.002);
}
