#include <unistd.h>

#include <toml.hpp>

#include <gateway/ctpMG.hpp>

using namespace ctptrader;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " config.toml" << std::endl;
    return 1;
  }

  auto res = toml::parse_file(argv[1]);
  if (!res.is_table()) {
    std::cout << "Error: " << argv[1] << " is not a valid toml file"
              << std::endl;
    return 1;
  }

  auto config = res.as_table();
  ctpMG::Start(*config);
  return 0;
}