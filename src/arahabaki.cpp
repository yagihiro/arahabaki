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
#include <algorithm>
#include <format.h>
#include <regex>
#include <arahabaki/arahabaki.h>

namespace arahabaki {

std::shared_ptr<CommandRouter> Router::create(int argc,
                                              const char *argv[]) {
  auto router = std::make_shared<CommandRouter>();
  router->set(argc, argv);

  return router;
}

bool Router::run() {
  auto it = std::find_if(_routes.rbegin(), _routes.rend(), [&](Route &r) {
    if (_arguments.size() < r.regexes.size()) {
      return false;
    }

    auto limit = std::min(r.regexes.size(), _arguments.size());
    int i;
    for (i = 0; i < limit; i++) {
      std::regex re(r.regexes[i]);
      std::smatch sm;
      auto &text = _arguments[i];
      if (!std::regex_match(text, sm, re)) {
        break;
      }
      int i = 0;
      for (auto &one : sm) {
        i += 1;
        if (i == 1) continue;
        r.sub_matches.emplace_back(one.str());
      }
    }
    return i == limit;
  });

  if (it != _routes.rend()) {
    Arguments additional_args;
    auto &r = (*it);
    if (!r.sub_matches.empty()) {
      additional_args.emplace_back(r.sub_matches[0]);
    }
    if (r.regexes.size() < _arguments.size()) {
      for (auto j = r.regexes.size(); j < _arguments.size(); j++) {
        additional_args.emplace_back(_arguments[j]);
      }
    }

    auto f = r.fn;
    assert(f != nullptr);
    try {
      f(additional_args);
    } catch (InvalidArgument &e) {
      return false;
    }
    return true;
  }

  return false;
}

void Router::add_route(Route &route) {
  _routes.emplace_back(route);
}

void CommandRouter::set(int argc, const char *argv[]) {
  for (int i = 1; i < argc; i++) {
    _arguments.emplace_back(argv[i]);
  }
}
}
