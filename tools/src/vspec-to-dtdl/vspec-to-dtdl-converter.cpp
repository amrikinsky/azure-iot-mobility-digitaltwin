// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#include <algorithm>
#include <cctype>
#include <iostream>

#include "vspec-to-dtdl-converter.h"

using namespace microsoft::automotive::data;

void VspecToDtdlConverter::Convert(const vspec::Definition& vspecDefinition, dtdl::Definition& dtdlDefinition)
{
	TraverseBranch(vspecDefinition.Root, dtdlDefinition.Root);
}

// TODO: Place in shared library
void VspecToDtdlConverter::ToLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(),
				[](unsigned char c) { return std::tolower(c); }
	);
}


bool VspecToDtdlConverter::ConvertDatatypeToSchema(const std::string& datatype, std::string& schema)
{
	std::string lowercaseDatatype = datatype;
	ToLower(lowercaseDatatype);
	if (lowercaseDatatype.compare("int32") == 0)
	{
		schema = "integer";
	}
	else if (lowercaseDatatype.compare("uint32") == 0)	// TODO: fix this
	{
		schema = "integer";
	}
	else if (lowercaseDatatype.compare("int8") == 0)	// TODO: fix this
	{
		schema = "integer";
	}
	else if (lowercaseDatatype.compare("uint8") == 0)	// TODO: fix this
	{
		schema = "integer";
	}
	else if (lowercaseDatatype.compare("int16") == 0)	// TODO: fix this
	{
		schema = "integer";
	}
	else if (lowercaseDatatype.compare("uint16") == 0)	// TODO: fix this
	{
		schema = "integer";
	}
	else if (lowercaseDatatype.compare("float") == 0)
	{
		schema = "float";
	}
	else if (lowercaseDatatype.compare("double") == 0)
	{
		schema = "double";
	}	
	else if (lowercaseDatatype.compare("boolean") == 0)
	{
		schema = "boolean";
	}
	else if (lowercaseDatatype.compare("string") == 0)
	{
		schema = "string";
	}	
	else
	{
		return false;
	}

	return true;
}

bool VspecToDtdlConverter::ConverVspecUnitToDtdlUnit(const std::string& vspecUnit, std::string& dtdlUnit)
{
	std::string lowercaseVspecUnit = vspecUnit;
	ToLower(lowercaseVspecUnit);

	if (vspecUnit.empty())
	{
		dtdlUnit.clear();	
	}
	else if (lowercaseVspecUnit.compare("s") == 0)
	{
		dtdlUnit = "second";
	}
	else if (lowercaseVspecUnit.compare("km/h") == 0)
	{
		dtdlUnit = "kilometrePerHour";
	}
	else if (lowercaseVspecUnit.compare("km") == 0)
	{
		dtdlUnit = "kilometre";
	}
	else if (lowercaseVspecUnit.compare("celsius") == 0)
	{
		dtdlUnit = "degreeCelsius";
	}
	else if (lowercaseVspecUnit.compare("m/s2") == 0)
	{
		dtdlUnit = "metrePerSecondSquared";
	}
	else if (lowercaseVspecUnit.compare("degrees/s") == 0)
	{
		dtdlUnit = "degreePerSecond";
	}
	else if (lowercaseVspecUnit.compare("kg") == 0)
	{
		dtdlUnit = "kilogram";
	}
	else if (lowercaseVspecUnit.compare("l") == 0)
	{
		dtdlUnit = "litre";
	}
	else if (lowercaseVspecUnit.compare("g/km") == 0)
	{
		dtdlUnit = "gramPerKiloemtre";	// TODO: Add this to DTDL (it's a unit of measurement for CO2 Emissions)
	}
	else if (lowercaseVspecUnit.compare("cm3") == 0)
	{
		dtdlUnit = "cubicCentimetre";
	}
	else if (lowercaseVspecUnit.compare("kw") == 0)
	{
		dtdlUnit = "kilowatt";
	}
	else if (lowercaseVspecUnit.compare("n.m") == 0)	// N.m (when in mixed case)
	{
		dtdlUnit = "newtonMetre";	// TODO: Add this to DTDL (it's the unit for Torque)
	}
	else if (lowercaseVspecUnit.compare("rpm") == 0)
	{
		dtdlUnit = "revolutionPerMinute";
	}
	else if (lowercaseVspecUnit.compare("kpa") == 0)
	{
		dtdlUnit = "kilopascal";
	}
	else if (lowercaseVspecUnit.compare("g/s") == 0)
	{
		dtdlUnit = "gramPerSecond";
	}
	else if (lowercaseVspecUnit.compare("percent") == 0)
	{
		dtdlUnit = "percent";
	}
	else if (lowercaseVspecUnit.compare("m") == 0)
	{
		dtdlUnit = "metre";
	}
	else if (lowercaseVspecUnit.compare("l/100km") == 0)
	{
		dtdlUnit = "litrePer100Kilometre";	// TODO: Add this to DTDL (it's the unit for Consumption)
	}
	else if (lowercaseVspecUnit.compare("degrees") == 0)
	{
		dtdlUnit = "degreeOfArc";
	}
	else if (lowercaseVspecUnit.compare("mm") == 0)
	{
		dtdlUnit = "millimetre";
	}
	else if (lowercaseVspecUnit.compare("inch") == 0)
	{
		dtdlUnit = "inch";
	}
	else if (lowercaseVspecUnit.compare("v") == 0)	// V
	{
		dtdlUnit = "volt";
	}
	else if (lowercaseVspecUnit.compare("a") == 0)	// A
	{
		dtdlUnit = "ampere";
	}
	else if (lowercaseVspecUnit.compare("pa") == 0)
	{
		dtdlUnit = "pascal";
	}
	else if (lowercaseVspecUnit.compare("ratio") == 0)
	{
		dtdlUnit = "ratio";	// TODO: Add this to DTDL
	}
	else if (lowercaseVspecUnit.compare("min") == 0)
	{
		dtdlUnit = "minute";
	}
	else if (lowercaseVspecUnit.compare("l/h") == 0)
	{
		dtdlUnit = "litrePerHour";
	}
	else if (lowercaseVspecUnit.compare("kwh") ==0)	// kWh
	{
		dtdlUnit = "kilowattHour";
	}
	else
	{
		return false;
	}

	return true;
}

void VspecToDtdlConverter::TraverseAttribute(const vspec::Node& attribute, dtdl::Property& property)
{
	property.Name = attribute.Name;
	property.Writable = false;
	if (!ConvertDatatypeToSchema(attribute.Datatype, property.Schema))
	{
		throw ConversionException("No schema equivalent for datatype '" + attribute.Datatype + "'");
	}
	if (!ConverVspecUnitToDtdlUnit(attribute.Unit, property.Unit))
	{
		throw ConversionException("No schema equivalent for unit '" + attribute.Unit + "'");
	}	
}

void VspecToDtdlConverter::TraverseActuator(const vspec::Node& actuator, dtdl::Property& property)
{
	property.Name = actuator.Name;
	property.Writable = true;
	if (!ConvertDatatypeToSchema(actuator.Datatype, property.Schema))
	{
		throw ConversionException("No schema equivalent for datatype '" + actuator.Datatype + "'");
	}
	if (!ConverVspecUnitToDtdlUnit(actuator.Unit, property.Unit))
	{
		throw ConversionException("No schema equivalent for unit '" + actuator.Unit + "'");
	}	
}

void VspecToDtdlConverter::TraverseSensor(const vspec::Node& sensor, dtdl::Telemetry& telemetry)
{
	telemetry.Name = sensor.Name;
	if (!ConvertDatatypeToSchema(sensor.Datatype, telemetry.Schema))
	{
		throw ConversionException("No schema equivalent for datatype '" + sensor.Datatype + "'");
	}
	if (!ConverVspecUnitToDtdlUnit(sensor.Unit, telemetry.Unit))
	{
		throw ConversionException("No schema equivalent for unit '" + sensor.Unit + "'");
	}	
}

void VspecToDtdlConverter::TraverseBranch(const vspec::Node& branch, dtdl::Component& component)
{
	// std::cout << branch.Name << std::endl;
	component.Name = branch.Name;

	for (auto it = branch.Children.begin(); it != branch.Children.end(); it++ )
	{
		if ((*it)->Type.compare("branch") == 0)
		{
			// PrintBranch(**it, level+1);
			std::shared_ptr<dtdl::Component> subComponent(new dtdl::Component());
			component.Components.push_back(subComponent);
			TraverseBranch(*(it->get()), *(subComponent.get()));		
		}
		else if ((*it)->Type.compare("sensor") == 0)
		{
			std::shared_ptr<dtdl::Telemetry> telemetry(new dtdl::Telemetry());
			component.Telemetries.push_back(telemetry);
			TraverseSensor(*(it->get()), *(telemetry.get()));	
		}
		else if ((*it)->Type.compare("attribute") == 0)
		{
			std::shared_ptr<dtdl::Property> property(new dtdl::Property());
			component.Properties.push_back(property);
			TraverseAttribute(*(it->get()), *(property.get()));	
		}
		else if ((*it)->Type.compare("actuator") == 0)
		{
			std::shared_ptr<dtdl::Property> property(new dtdl::Property());
			component.Properties.push_back(property);
			TraverseActuator(*(it->get()), *(property.get()));	
		}				
		else
		{
			// error - unexpected type
		}
	}
}

