#include <iostream>
#include <queue>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <random>
#include <string>
#include <type_traits>
#include <iostream>
#include <functional>




template<typename T>
class Server
{
    private:
        // параллельное выполнение num_workers задач
        std::vector<std::thread> workers;
        size_t num_workers;

        // флаг остановки сервера
        std::atomic<bool> stop_server{false};

        // мьютекс
        std::mutex mtx;
        // условная переменная
        std::condition_variable cond;

        // индекс задачи для отображания
        std::atomic<size_t> index_task{0};

        // очередь задач на выполнение. tasks[(index, std::packaged)]
        std::queue<std::pair<size_t, std::packaged_task<T()>>> tasks;

        // отображение для результатов. results[index] = result
        std::unordered_map<size_t, std::future<T>> results;

    public:
        Server(int threads = 4) : num_workers((threads > 0) ? threads : 1) { start(); }

        // 1) void start() - запустить сервер;
        void start()
        {
            stop_server.store(false);
            for (size_t worker = 0; worker < num_workers; worker++)
            {
                workers.emplace_back([this]() {
                    // цикл пока сервер работает И есть задачи в очереди
                    while (true)
                        {
                            std::pair<size_t, std::packaged_task<T()>> task_pair;
                            {
                                // берем мьютекс на данные
                                std::unique_lock<std::mutex> lock(mtx);

                                // временно отдаем его, чтобы у клиента была возможность добавить в очреедь И засыпаем
                                cond.wait(lock, [this] { return stop_server.load() || !tasks.empty(); });
                
                                if (stop_server.load() && tasks.empty()) break;

                                task_pair = std::move(tasks.front());
                                tasks.pop();
                            }
                            task_pair.second();
                        }
                    }
                );
            }
        }

        // 2) void stop() - остановить сервер;
        void stop()
        {   
            stop_server.store(true);
            cond.notify_all();
            for (auto& worker : workers) worker.join();
            workers.clear();
        }

        // 3) size_t add_task(task) - добавить задачу, тип task в параметрах функции на ваше усмотрения. Возвращает id задачи;
        template<typename Func, typename... Args>
        size_t add_task(Func&& func, Args&& ... args)
        {
            auto task = std::packaged_task<T()>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );
            std::future<T> future = task.get_future();

            size_t index = index_task.fetch_add(1);
            {
                std::unique_lock<std::mutex> lock(mtx);
        
                tasks.push({index, std::move(task)});
                results[index] = std::move(future);
            }
            cond.notify_one();
            return index;
        }

        // 4) T request_result(id_res) - возвращает результат типа T. Может быть блокирующим или не блокирующим.
        T request_result(size_t id_res)
        {
            std::future<T> future;
            {
                std::unique_lock<std::mutex> lock(mtx);
        
                auto result = results.find(id_res);
                if (result == results.end()) throw std::runtime_error("Result not found: " + std::to_string(id_res));
        
                future = std::move(result->second);
                results.erase(id_res);
            }
            return future.get();
        }

        ~Server() { stop(); }
};

template<typename T>
struct arity_helper;

template<typename R, typename... Args>
struct arity_helper<R(*)(Args...)>
{
    static constexpr size_t value = sizeof...(Args);
};

// Удобная обертка
template<typename F>
constexpr size_t get_arity(F&&) {
    return arity_helper<std::decay_t<F>>::value;
}

thread_local std::mt19937 gen(std::random_device{}());
thread_local std::uniform_real_distribution<double> dist(1.0, 5.0);


template<typename Func, typename T, std::size_t... Is>
auto invoke_with_vector(Func func, const std::vector<T>& args, std::index_sequence<Is...>) {
    return func(args[Is]...);
}

template<typename T, typename Func>
void client(Server<T>& server, Func&& func, const std::string& filename)
{
    size_t N = 6 + (rand() % 9994);

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    
    constexpr size_t num_args = arity_helper<std::decay_t<Func>>::value;
    constexpr auto indices = std::make_index_sequence<num_args>{};

    
    for (size_t i = 0; i < N; i++)
    {
        std::vector<T> args;
        for (size_t j = 0; j < num_args; j++)
        {
            args.push_back(static_cast<T>(dist(gen)));
        }

        auto task = [func, args, indices]() -> T {
            return invoke_with_vector(func, args, indices);
        };

        size_t id_res = server.add_task(task);
        T res = server.request_result(id_res);

        file << res << "\n";
    }
}


using my_type =
    #ifdef DOUBLE
        double
    #else
        float
    #endif
    ;

    
template<typename T>
T my_sin(T arg) { return std::sin(arg); }

template<typename T>
T my_sqrt(T arg) { return std::sqrt(arg); }

template<typename T>
T my_pow(T x, T y) { return std::pow(x, y); }

template<typename T>
T my_func(T x, T y, T z, T e, T d) {
    return std::sin(x) - std::cos(y) - std::tan(z) - std::pow(e, d) + 2 * d;
}

template<typename Func>
std::thread run_client(Server<my_type>& server, Func func, const std::string& filename)
{
    return std::thread([&, func, filename]() { client<my_type>(server, func, filename); });
}


int main()
{
    Server<my_type> server;

    auto t1 = run_client(server, &my_sin<my_type>,  "res_sin.txt");
    auto t2 = run_client(server, &my_sqrt<my_type>, "res_sqrt.txt");
    auto t3 = run_client(server, &my_pow<my_type>,  "res_pow.txt");
    auto t4 = run_client(server, &my_func<my_type>, "res_func.txt");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
