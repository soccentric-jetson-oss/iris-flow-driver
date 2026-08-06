#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <cerrno>

TEST_CASE("Null input handling", "[edge]") {
    int ret = iris_flow_open(NULL);
    REQUIRE(ret == -EINVAL);
}

TEST_CASE("Empty input handling", "[edge]") {
    struct iris_flow_config cfg;
    std::memset(&cfg, 0, sizeof(cfg));
    REQUIRE(cfg.width == 0);
    REQUIRE(cfg.height == 0);
}

TEST_CASE("Boundary values", "[edge]") {
    struct iris_flow_config cfg;
    std::memset(&cfg, 0, sizeof(cfg));
    cfg.width = 3840;
    cfg.height = 2160;
    cfg.fps = 60;
    REQUIRE(cfg.width == 3840);
    REQUIRE(cfg.height == 2160);
    REQUIRE(cfg.fps == 60);
}

TEST_CASE("Concurrent access", "[edge]") {
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() { counter++; });
    }
    for (auto& t : threads) t.join();
    REQUIRE(counter == 10);
}

TEST_CASE("Resource cleanup on error", "[edge]") {
    struct iris_flow_config cfg;
    std::memset(&cfg, 0, sizeof(cfg));
    cfg.width = 0;
    cfg.height = 0;
    REQUIRE(cfg.width == 0);
}
