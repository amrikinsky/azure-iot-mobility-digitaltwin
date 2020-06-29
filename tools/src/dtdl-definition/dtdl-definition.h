// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#ifndef DTDL_DEFINITION_H
#define DTDL_DEFINITION_H

#include <list>
#include <memory>

#include <jsoncpp/include/json/json.h>

namespace microsoft::automotive::data::dtdl
{
    class Telemetry
    {
        public:
            std::string Name;
            std::string Schema;
            std::string Id;
            std::string Comment;
            std::string Description;
            std::string DisplayName;
            std::string Unit;
    };

    class Property
    {
        public:
            std::string Name;
            std::string Schema;
            std::string Id;
            std::string Comment;
            std::string Description;
            std::string DisplayName;
            std::string Unit;
            bool Writable;
    };

    class CommandPayload
    {
        public:
            std::string Name;
            std::string Schema;
            std::string Id;
            std::string Comment;
            std::string Description;
            std::string DisplayName;
    };

    class Command
    {
        public:
            std::string Name;
            std::string Id;
            std::string Comment;
            std::string Description;
            std::string DisplayName;
            std::string CommandType;
            CommandPayload Request;
            CommandPayload Response;
    };

    class Component
    {
        public:
            std::string Name;
            std::string Schema;
            std::string Id;
            std::string Comment;
            std::string Description;
            std::string DisplayName;

            // content
            std::list<std::shared_ptr<Telemetry>> Telemetries;
            std::list<std::shared_ptr<Property>> Properties;
            std::list<std::shared_ptr<Command>> Commands;
            std::list<std::shared_ptr<Component>> Components;

            void GenerateDTDL(std::list<std::shared_ptr<Json::Value>>& dtdl);
    };

    // https://en.wikibooks.org/wiki/JsonCpp
    // sudo apt install install libjsoncpp-dev

    // sudo apt install libyaml-cpp-dev
    // sudo apt install libyaml-cpp0.5v5

    class Definition
    {
        public:
            Component Root;
            void GenerateDTDL(std::list<std::shared_ptr<Json::Value>>& dtdl);
    };
} // namespace microsoft::automotive::data::dtdl

#endif // DTDL_DEFINITION_H
