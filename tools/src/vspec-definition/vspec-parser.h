// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#ifndef VSPEC_PARSER_H
#define VSPEC_PARSER_H

#include <exception>

#include <yaml-cpp/yaml.h>

#include "vspec-definition.h"

namespace microsoft::automotive::data
{
    class VspecParserException : public std::exception
    {
        public:
            VspecParserException(const std::string& message)
            {
                this->message = message;
            }

            virtual const char* what() const throw()
            {
                return message.c_str();
            }

        protected:
            std::string message;
    };

    class VspecParser
    {
        public:
            void Parse(const std::string& vspecFilePath, vspec::Definition& definition);

        protected:
            void ExtractPath(std::string fullPath, std::string& path);
            void Preprocess(const std::string& vspecFilePath, std::ostream& os, std::string namePrefix = "");
            bool ParseRange(const std::string& str, vspec::Range& range);
            void ParseNode(vspec::Node& vspecNode, const YAML::Node& node);
            void TraverseConfig(const YAML::Node& config, vspec::Node& rootVspecNode);
    };
} // microsoft::automotive::data

#endif // VSPEC_PARSER_H
