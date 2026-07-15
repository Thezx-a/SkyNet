#pragma once
#include <functional>
#include <vector>
#include "skynet/http/request.h"
#include "skynet/http/response.h"

namespace skynet {
namespace proxy {

using Middleware = std::function<HttpResponse(const HttpRequest&)>;

class MiddlewareChain {
public:
    void add(Middleware mw);
    HttpResponse process(const HttpRequest& req);

private:
    std::vector<Middleware> chain_;
};

}  // namespace proxy
}  // namespace skynet
