// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#ifndef VSPEC_TO_DTDL_CONVERTER_H
#define VSPEC_TO_DTDL_CONVERTER_H

#include "vspec-definition.h"
#include "dtdl-definition.h"

/*
vspec has a type field that maps to DTDL’s @type field.  Here are my thoughts on the mappings of VSS types to DTDL types:
•       branch => Component
•       attribute => Property that is not writable
•       sensor => Telemetry
•       actuator => Property that is writable (note: not sure if a Property would be considered non-static; if not, then perhaps a Command plus an associated Telemetry value would be needed)
•       stream (note: no direct equivalent)
*/


namespace microsoft::automotive::data
{
    class ConversionException : public std::exception
    {
        public:
            ConversionException(const std::string& message)
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

    class VspecToDtdlConverter
    {
        public:
            static void Convert(const vspec::Definition& vspecDefinition, dtdl::Definition& dtdlDefinition);

        protected:
            static void ToLower(std::string& str);
            static bool ConvertDatatypeToSchema(const std::string& datatype, std::string& schema);
            static bool ConverVspecUnitToDtdlUnit(const std::string& vspecUnit, std::string& dtdlUnit);
            static void TraverseAttribute(const vspec::Node& attribute, dtdl::Property& property);
            static void TraverseActuator(const vspec::Node& actuator, dtdl::Property& property);
            static void TraverseSensor(const vspec::Node& sensor, dtdl::Telemetry& telemetry);
            static void TraverseBranch(const vspec::Node& branch, dtdl::Component& component);
    };

} // namespace microsoft::automotive::data

#endif // VSPEC_TO_DTDL_CONVERTER_H