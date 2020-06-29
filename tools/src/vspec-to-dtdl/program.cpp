// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

//
// To use:
// ./vspec-to-dtdl /home/ashbeitz/repos/Vehicle-DataModel-Tools/third-party/vehicle_signal_specification/src/vehicle_signal_specification/spec/VehicleSignalSpecification.vspec
//

#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "vspec-definition.h"
#include "vspec-parser.h"
#include "dtdl-definition.h"
#include "dtdl-composer.h"
#include "vspec-to-dtdl-converter.h"

using namespace boost;

using namespace microsoft::automotive::data;

int main(int argc, char *argv[])
{
    try
	{
		program_options::options_description options("Allowed options");
		options.add_options()
    		("help", "produce help message")
			("vspec-file", program_options::value<std::string>(), "location of the root vspec file")
		;
        program_options::positional_options_description positions;
		positions.add("vspec-file", -1);
        program_options::variables_map vm;        
        program_options::store(program_options::command_line_parser(argc, argv).options(options).positional(positions).run(), vm);
        program_options::notify(vm);    

        if (vm.count("help"))
		{
            std::cout << options << "\n";
            return 0;
        }

        std::string vspecFilePath;

        if (vm.count("vspec-file"))
		{
            vspecFilePath = vm["vspec-file"].as<std::string>();
            std::cout << "vspec-file  was set to " << vspecFilePath << "\n";
        }
		else
		{
            std::cout << "vspec-file  was not set.\n";
        }

        vspec::Definition vspecDefinition;
        VspecParser vspecParser;
        vspecParser.Parse(vspecFilePath, vspecDefinition);

        dtdl::Definition dtdlDefinition;
        VspecToDtdlConverter vspecToDtdlConverter;
        vspecToDtdlConverter.Convert(vspecDefinition, dtdlDefinition);

        DtdlComposer dtdlComposer;
        dtdlComposer.Compose(dtdlDefinition);
    }
    catch (const VspecParserException& pe)
    {
        std::cerr << pe.what() << '\n';
        return 1;
    }
    catch (std::exception& e)
	{
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
	{
        std::cerr << "Exception of unknown type!\n";
		return 1;
    }

    return 0;
}
