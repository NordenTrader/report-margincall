#include "MarginCallPluginInterface.hpp"

using namespace ast;

extern "C" void AboutReport(Value& request,
                            Value& response,
                            Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Margin call report", allocator), allocator);
    response.AddMember("description", Value().SetString("Report with table", allocator), allocator);
    response.AddMember("type", REPORT_NONE_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(Value& request,
                             Value& response,
                             Document::AllocatorType& allocator,
                             CServerInterface* server) {}