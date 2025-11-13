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
    std::string group_mask;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }

    std::vector<AccountRecord> accounts_vector;

    server->GetAccountsByGroup(group_mask, &accounts_vector);

    // Таблица
    auto make_table = [&](const std::vector<AccountRecord>& accounts) -> Node {
        std::vector<Node> table_rows;

        // Заголовки
        table_rows.push_back(tr({
            th({ text("Login") }),
            th({ text("Name") }),
            th({ text("Leverage") }),
            th({ text("Balance") }),
            th({ text("Credit") }),
            th({ text("Floating P/L") }),
            th({ text("Equity") }),
            th({ text("Margin") }),
            th({ text("Free Margin") }),
            th({ text("Margin Limits") }),
            th({ text("Margin Level") }),
            th({ text("Add. Margin") }),
            th({ text("Currency") }),
        }));

        // Формирование строк
        for (const auto& account :accounts_vector) {
            double floating_pl = 0.0;
            double margin_used = 0.0;

            // Открытые сделки аккаунта
            std::vector<TradeRecord> trades_vector;

            const int result = server->GetOpenTradesByLogin(account.login, &trades_vector);

            std::cout << "GetOpenTradesByLogin: " << result << std::endl;

            table_rows.push_back(tr({
                td({ text(std::to_string(account.login)) }),
                td({ text(account.name) }),
                td({ text(std::to_string(account.leverage)) }),
                td({ text(std::to_string(account.balance)) }),
                td({ text(std::to_string(account.credit)) }),
            }));
        }



        return table(table_rows, props({{"className", "data-table"}}));
    };

    const Node report = div({
        h1({ text("Margin Call Report") }),
        make_table(accounts_vector)
    }, props({{"className", "report"}}));

    to_json(report, response, allocator);
}