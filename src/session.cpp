#include "telnetpp/session.hpp"
#include "telnetpp/detail/registration.hpp"
//#include <numeric>

namespace telnetpp {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
session::session()
{
    // By default, the session will respond to WILL/WONT(option) with
    // DONT(option), and to DO/DONT(option) with WONT(option).  This behaviour
    // is overridden with any installed options.
    negotiation_router_.set_unregistered_route(
        [](telnetpp::negotiation const &negotiation, auto &&cont)
        {
            auto const &request = negotiation.request();

            negotiation_type result =
                (request == telnetpp::will || request == telnetpp::wont)
              ? telnetpp::dont
              : telnetpp::wont;

            cont(telnetpp::negotiation{result, negotiation.option()});
        });
}

// ==========================================================================
// INSTALL
// ==========================================================================
void session::install(client_option &option)
{
    detail::register_client_option(
        option, negotiation_router_, subnegotiation_router_);
}

// ==========================================================================
// INSTALL
// ==========================================================================
void session::install(server_option &option)
{
    detail::register_server_option(
        option, negotiation_router_, subnegotiation_router_);
}

// ==========================================================================
// SEND
// ==========================================================================
// std::vector<boost::variant<byte_stream, boost::any>> session::send(
//     std::vector<token> const &tokens)
// {
//     return detail::generate(tokens);
// }

}
