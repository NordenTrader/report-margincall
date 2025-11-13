#include "MarginCallPluginInterface.hpp"

using namespace ast;

extern "C" void AboutReport(rapidjson::Value& request,
                            rapidjson::Value& response,
                            rapidjson::Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Margin call report", allocator), allocator);
    response.AddMember("description", Value().SetString("Report with table", allocator), allocator);
    response.AddMember("type", REPORT_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    LogJSON("request: ", request);

    std::vector<GroupRecord> groups;
    std::vector<AccountRecord> accounts;

    const int groups_result = server->GetAllGroups(&groups);
    // if (group_result != 0) {
    //
    // }

     for (const GroupRecord& group : groups) {
         server->GetAccountsByGroup(group.group, &accounts);
     }

    std::cout << "Groups size: " << groups.size()<< std::endl;
    std::cout << "Accounts size: " << accounts.size()<< std::endl;
}

void LogJSON(const std::string& name, const Value& value) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    value.Accept(writer); // сериализуем JSON в строку
    std::cout << "[LogJSON]: " << name << ": " << buffer.GetString() << std::endl;
}