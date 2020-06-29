// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#ifndef VSPEC_DEFINITION_H
#define VSPEC_DEFINITION_H

#include <list>
#include <memory>

namespace microsoft::automotive::data::vspec
{
    class Series
    {
    };

    class Enumeration : public Series
    {
        public:
            std::list<std::string> Items;
    };

    class Range : public Series
    {
        public:
            std::string Name;
            int Begin;
            int End;
    };

    class Node
    {
        public:
            std::string Name;
            std::string Type;
            std::string Description;
            std::string Datatype;
            std::string Unit;
            std::string Min;
            std::string Max;
            std::string Default;
            std::string Value;
            Enumeration Enum;
            std::string Aggregate;
            std::list<Series> Instances;

            // if it is a branch
            std::list<std::shared_ptr<Node>> Children;

    };

    class Definition
    {
        public:
            Node Root;
    };
} // namespace microsoft::automotive::data::vspec

#endif // VSPEC_DEFINITION_H