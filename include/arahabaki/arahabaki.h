/*
The MIT License (MIT)

Copyright (c) 2015 Hiroki Yagita

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef __arahabaki_arahabaki__
#define __arahabaki_arahabaki__
#include <functional>
#include <memory>
#include <vector>
#include "arguments.h"

namespace arahabaki {

typedef struct {
  std::vector<std::string> regexes;
  CommandRouter::CallbackFunction fn;
  std::vector<std::string> sub_matches;

  Route(const std::vector<std::string> &r,
        const CommandRouter::CallbackFunction &f)
      : regexes(r), fn(f) {}
} Route;

class Router {
 public:
  using CallbackFunction = std::function<void(const Arguments &)>;

  static std::shared_ptr<Router> create(int argc, const char *argv[]);

  bool run();

  void add_route(Route &route);

 private:
  void set(int argc, const char *argv[]);

  std::vector<std::string> _arguments;
  std::vector<Route> _routes;
};
}
#endif
