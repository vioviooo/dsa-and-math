#include <gtest/gtest.h>
#include <client_logger.h>
#include <client_logger_builder.h>


int main(int argc, char *argv[])
{
    auto* build1 = new client_logger_builder("ayo: %m");
    auto* build2 = new client_logger_builder();
    auto* build3 = new client_logger_builder();
    
    logger* log1 = build1->add_console_stream(logger::severity::trace)->add_console_stream(logger::severity::debug)->build();  
    // logger* log2 = build2->add_console_stream(logger::severity::trace)->add_console_stream(logger::severity::debug)->build();
    logger* log3 = build3->add_console_stream(logger::severity::information)->transform_with_configuration("/Users/vioviooo/Desktop/mp_os/logger/client_logger/tests/config.json", "logs/mainLogger/aboba")->build();

    log1->log("this is trace", logger::severity::trace);
    log1->log("this is debug", logger::severity::debug);

    // log2->log("boo", logger::severity::trace);

    log3->log("knock-knock", logger::severity::information);
    log3->log("boohoo", logger::severity::debug);

    delete log3;
    delete build3;

    delete log1;
    delete build1;

    // delete build2;
    // delete log2;
    
    return 0;
}