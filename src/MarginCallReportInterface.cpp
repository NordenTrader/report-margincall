#include "MarginCallReportInterface.hpp"

#include <iomanip>

using namespace ast;

extern "C" void AboutReport(rapidjson::Value& request,
                            rapidjson::Value& response,
                            rapidjson::Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Margin call report", allocator), allocator);
    response.AddMember(
        "description",
        Value().SetString("Lists accounts currently under margin call or stop out. "
                          "Includes financial details such as balance, equity, margin, and full account details.",
                          allocator), allocator);
    response.AddMember("type", REPORT_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

// v.1
// extern "C" void CreateReport(rapidjson::Value& request,
//                              rapidjson::Value& response,
//                              rapidjson::Document::AllocatorType& allocator,
//                              CServerInterface* server) {
//     // Структура накопления итогов
//     struct Total {
//         std::string currency;
//         double balance = 0.0;
//         double credit = 0.0;
//         double floating_pl = 0.0;
//         double equity = 0.0;
//         double margin = 0.0;
//         double margin_free = 0.0;
//     };
//
//     std::unordered_map<std::string, Total> totals_map;
//
//     std::string group_mask;
//     if (request.HasMember("group") && request["group"].IsString()) {
//         group_mask = request["group"].GetString();
//     }
//
//     std::vector<AccountRecord> accounts_vector;
//     std::vector<GroupRecord> groups_vector;
//
//     try {
//         server->GetAccountsByGroup(group_mask, &accounts_vector);
//         server->GetAllGroups(&groups_vector);
//     } catch (const std::exception& e) {
//         std::cerr << "[MarginCallReportInterface]: " << e.what() << std::endl;
//     }
//
//     // Лямбда для поиска валюты аккаунта по группе
//     auto get_group_currency = [&](const std::string& group_name) -> std::string {
//         for (const auto& group : groups_vector) {
//             if (group.group == group_name) {
//                 return group.currency;
//             }
//         }
//         return "N/A"; // группа не найдена - валюта не определена
//     };
//
//     // Лямбда подготавливающая значения double для вставки в AST (округление до 2х знаков)
//     auto format_for_AST = [](double value) -> std::string {
//         std::ostringstream oss;
//         oss << std::fixed << std::setprecision(2) << value;
//         return oss.str();
//     };
//
//     // Таблица
//     auto make_table = [&](const std::vector<AccountRecord>& accounts) -> Node {
//         std::vector<Node> table_rows;
//
//         // Заголовки
//         table_rows.push_back(table({
//             tr({
//                 th({ text("Login") }),
//                 th({ text("Name") }),
//                 th({ text("Leverage") }),
//                 th({ text("Balance") }),
//                 th({ text("Credit") }),
//                 th({ text("Floating P/L") }),
//                 th({ text("Equity") }),
//                 th({ text("Margin") }),
//                 th({ text("Free Margin") }),
//                 th({ text("Margin Level") }),
//                 th({ text("Currency") }),
//             })
//         }));
//
//         // Формирование строк
//         for (const auto& account : accounts) {
//             std::vector<TradeRecord> trades_vector; // открытые сделки аккаунта
//             double floating_pl = 0.0;
//             MarginLevel margin_level;
//
//             server->GetAccountBalanceByLogin(account.login, &margin_level);
//
//             if (margin_level.level_type == MARGINLEVEL_MARGINCALL || margin_level.level_type == MARGINLEVEL_STOPOUT) {
//                 floating_pl = margin_level.equity - margin_level.balance;
//                 std::string currency = get_group_currency(account.group);
//
//                 auto& total = totals_map[currency];
//
//                 total.currency = currency;
//                 total.balance += margin_level.balance;
//                 total.credit += margin_level.credit;
//                 total.floating_pl += floating_pl;
//                 total.equity += margin_level.equity;
//                 total.margin += margin_level.margin;
//                 total.margin_free += margin_level.margin_free;
//
//                 table_rows.push_back(tr({
//                     td({text(std::to_string(account.login))}),
//                     td({text(account.name)}),
//                     td({text(format_for_AST(margin_level.leverage))}),
//                     td({text(format_for_AST(margin_level.balance))}),
//                     td({text(format_for_AST(margin_level.credit))}),
//                     td({text(format_for_AST(floating_pl))}),
//                     td({text(format_for_AST(margin_level.equity))}),
//                     td({text(format_for_AST(margin_level.margin))}),
//                     td({text(format_for_AST(margin_level.margin_free))}),
//                     td({text(format_for_AST(margin_level.margin_level))}),
//                     td({text(currency)}),
//                 }));
//             }
//         }
//
//         // Формирование строк Total
//         for (const auto& pair : totals_map) {
//             const Total& total = pair.second;
//
//             table_rows.push_back(tr({
//                 td({ text("TOTAL") }),
//                 td({ text("") }),
//                 td({ text("") }),
//                 td({ text(format_for_AST(total.balance)) }),
//                 td({ text(format_for_AST(total.credit)) }),
//                 td({ text(format_for_AST(total.floating_pl)) }),
//                 td({ text(format_for_AST(total.equity)) }),
//                 td({ text(format_for_AST(total.margin)) }),
//                 td({ text(format_for_AST(total.margin_free)) }),
//                 td({ text("") }),
//                 td({ text(total.currency) }),
//             }));
//         }
//
//         return table(table_rows, props({{"className", "data-table"}}));
//     };
//
//     const Node report = div({
//         h1({ text("Margin Call Report") }),
//         make_table(accounts_vector),
//     }, props({{"className", "report"}}));
//
//     to_json(report, response, allocator);
// }

// v.2
extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    // Структура накопления итогов
    struct Total {
        std::string currency;
        double balance = 0.0;
        double credit = 0.0;
        double floating_pl = 0.0;
        double equity = 0.0;
        double margin = 0.0;
        double margin_free = 0.0;
    };

    std::unordered_map<std::string, Total> totals_map;

    std::string group_mask;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }

    std::vector<AccountRecord> accounts_vector;
    std::vector<GroupRecord> groups_vector;

    try {
        server->GetAccountsByGroup(group_mask, &accounts_vector);
        server->GetAllGroups(&groups_vector);
    } catch (const std::exception& e) {
        std::cerr << "[MarginCallReportInterface]: " << e.what() << std::endl;
    }

    // Лямбда для поиска валюты аккаунта по группе
    auto get_group_currency = [&](const std::string& group_name) -> std::string {
        for (const auto& group : groups_vector) {
            if (group.group == group_name) {
                return group.currency;
            }
        }
        return "N/A"; // группа не найдена - валюта не определена
    };

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2х знаков)
    auto format_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    // Таблица
    auto make_table = [&](const std::vector<AccountRecord>& accounts) -> Node {
        std::vector<Node> table_rows;

        // Заголовки
        table_rows.push_back(table({
            tr({
                th({ text("Login") }),
                th({ text("Name") }),
                th({ text("Leverage") }),
                th({ text("Balance") }),
                th({ text("Credit") }),
                th({ text("Floating P/L") }),
                th({ text("Equity") }),
                th({ text("Margin") }),
                th({ text("Free Margin") }),
                th({ text("Margin Level") }),
                th({ text("Currency") }),
            })
        }));

        // Формирование строк
        for (const auto& account : accounts) {
            std::vector<TradeRecord> trades_vector; // открытые сделки аккаунта
            double floating_pl = 0.0;
            MarginLevel margin_level;

            server->GetAccountBalanceByLogin(account.login, &margin_level);

            if (margin_level.level_type == MARGINLEVEL_MARGINCALL || margin_level.level_type == MARGINLEVEL_STOPOUT) {
                floating_pl = margin_level.equity - margin_level.balance;
                std::string currency = get_group_currency(account.group);

                auto& total = totals_map[currency];

                total.currency = currency;
                total.balance += margin_level.balance;
                total.credit += margin_level.credit;
                total.floating_pl += floating_pl;
                total.equity += margin_level.equity;
                total.margin += margin_level.margin;
                total.margin_free += margin_level.margin_free;

                table_rows.push_back(tr({
                    td({text(std::to_string(account.login))}),
                    td({text(account.name)}),
                    td({text(format_for_AST(margin_level.leverage))}),
                    td({text(format_for_AST(margin_level.balance))}),
                    td({text(format_for_AST(margin_level.credit))}),
                    td({text(format_for_AST(floating_pl))}),
                    td({text(format_for_AST(margin_level.equity))}),
                    td({text(format_for_AST(margin_level.margin))}),
                    td({text(format_for_AST(margin_level.margin_free))}),
                    td({text(format_for_AST(margin_level.margin_level))}),
                    td({text(currency)}),
                }));
            }
        }

        // Формирование строк Total
        for (const auto& pair : totals_map) {
            const Total& total = pair.second;

            table_rows.push_back(tr({
                td({ text("TOTAL") }),
                td({ text("") }),
                td({ text("") }),
                td({ text(format_for_AST(total.balance)) }),
                td({ text(format_for_AST(total.credit)) }),
                td({ text(format_for_AST(total.floating_pl)) }),
                td({ text(format_for_AST(total.equity)) }),
                td({ text(format_for_AST(total.margin)) }),
                td({ text(format_for_AST(total.margin_free)) }),
                td({ text("") }),
                td({ text(total.currency) }),
            }));
        }

        return table(table_rows, props({{"className", "data-table"}}));
    };

    const Node report = div({
        h1({ text("Margin Call Report") }),
        make_table(accounts_vector),
    }, props({{"className", "report"}}));


    // Оборачивание в ui.modal структуру
    response.SetObject();

    // UI
    rapidjson::Value uiObj(kObjectType);

    // Modal
    rapidjson::Value modalObj(kObjectType);

    // size: "xxxl"
    modalObj.AddMember("size", rapidjson::Value("xxxl", allocator), allocator);

    // headerContent
    {
        rapidjson::Value headerContent(kArrayType);

        // Space → #text → {value: ...}
        rapidjson::Value space(kObjectType);
        space.AddMember("type", rapidjson::Value("Space", allocator), allocator);

        rapidjson::Value spaceChildren(kArrayType);

        rapidjson::Value textNode(kObjectType);
        textNode.AddMember("type", rapidjson::Value("#text", allocator), allocator);

        rapidjson::Value textProps(kObjectType);
        textProps.AddMember("value", rapidjson::Value("Report"), allocator);

        textNode.AddMember("props", textProps, allocator);
        spaceChildren.PushBack(textNode, allocator);

        space.AddMember("children", spaceChildren, allocator);
        headerContent.PushBack(space, allocator);

        modalObj.AddMember("headerContent", headerContent, allocator);
    }

    // footerContent
    {
        rapidjson::Value footerContent(kArrayType);

        rapidjson::Value space(kObjectType);
        space.AddMember("type", rapidjson::Value("Space", allocator), allocator);

        rapidjson::Value spaceProps(kObjectType);
        spaceProps.AddMember("justifyContent", rapidjson::Value("space-between", allocator), allocator);
        space.AddMember("props", spaceProps, allocator);

        rapidjson::Value spaceChildren(kArrayType);

        rapidjson::Value button(kObjectType);
        button.AddMember("type", rapidjson::Value("Button", allocator), allocator);

        rapidjson::Value btnProps(kObjectType);
        btnProps.AddMember("className", rapidjson::Value("form_action_button", allocator), allocator);
        btnProps.AddMember("borderType", rapidjson::Value("danger", allocator), allocator);
        btnProps.AddMember("buttonType", rapidjson::Value("outlined", allocator), allocator);
        btnProps.AddMember("onClick", rapidjson::Value("{\"action\":\"CloseModal\"}", allocator), allocator);
        button.AddMember("props", btnProps, allocator);

        rapidjson::Value btnChildren(kArrayType);

        rapidjson::Value btnText(kObjectType);
        btnText.AddMember("type", rapidjson::Value("#text", allocator), allocator);

        rapidjson::Value btnTextProps(kObjectType);
        btnTextProps.AddMember("value", rapidjson::Value("Close", allocator), allocator);
        btnText.AddMember("props", btnTextProps, allocator);

        btnChildren.PushBack(btnText, allocator);

        button.AddMember("children", btnChildren, allocator);

        spaceChildren.PushBack(button, allocator);

        space.AddMember("children", spaceChildren, allocator);
        footerContent.PushBack(space, allocator);

        modalObj.AddMember("footerContent", footerContent, allocator);
    }

    // content: [ report AST ]
    {
        rapidjson::Value content(kArrayType);

        rapidjson::Value reportJson(kObjectType);
        to_json(report, reportJson, allocator); // сериализация AST

        content.PushBack(reportJson, allocator);
        modalObj.AddMember("content", content, allocator);
    }

    // ui.modal
    uiObj.AddMember("modal", modalObj, allocator);

    // response.ui
    response.AddMember("ui", uiObj, allocator);

    to_json(report, response, allocator);
}