// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#include <fstream> 
#include <iostream>
#include <regex>
#include <stack>
#include <stdexcept>

#include "vspec-parser.h"

using namespace microsoft::automotive::data;


void VspecParser::ExtractPath(std::string fullPath, std::string& path)
{
	size_t pathEndIndex = fullPath.rfind("/");
	if (pathEndIndex != std::string::npos)
	{
		path = fullPath.substr(0, pathEndIndex);
	}
	else
	{
		path = "";
	}	
}

void VspecParser::Preprocess(const std::string& vspecFilePath, std::ostream& os, std::string namePrefix)
{	
	std::ifstream filein(vspecFilePath);

	std::string path;
	ExtractPath(vspecFilePath, path);

	std::regex includePattern("^#include ([^ ]+) ([^ ]+)$");
	std::regex namePattern("^- ([^ ]+):$");

    for (std::string line; std::getline(filein, line); )
    {
		std::smatch matches;
		if (std::regex_match(line, matches, includePattern))
		{
			if (matches.size() == 3)
			{
				std::ssub_match filePathSubmatch = matches[1];
				std::ssub_match branchSubmatch = matches[2];			
				std::string newfullpath(path.c_str());
				newfullpath.append("/");
				newfullpath.append(filePathSubmatch.str());
				std::string newNamePrefix;
				if (namePrefix.size() != 0)
				{
					newNamePrefix.append(namePrefix);
					newNamePrefix.append(".");
				}
				newNamePrefix.append(branchSubmatch.str());
				Preprocess(newfullpath, os, newNamePrefix);
			}
		}
		else if (std::regex_match(line, matches, namePattern))
		{
			if (matches.size() == 2)
			{
				std::ssub_match nameSubmatch = matches[1];
				std::string name = nameSubmatch.str();

				if (namePrefix.size() == 0)
				{
					os << line <<  std::endl;
				}
				else
				{
					os << "- " << namePrefix << "." << name << ":" << std::endl;
				}
			}
		}
		else
		{
			os << line << std::endl;
		}
		
    }
}

bool VspecParser::ParseRange(const std::string& str, vspec::Range& range)
{
	std::regex rangePattern("^([A-Za-z0-9]+)\\[([0-9]+),([0-9]+)\\]$");

	std::smatch matches;
	if (std::regex_match(str, matches, rangePattern))
	{
		if (matches.size() == 4)
		{
			std::ssub_match nameSubmatch = matches[1];
			range.Name = nameSubmatch.str();
			std::ssub_match beginSubmatch = matches[2];
			range.Begin = std::atoi(beginSubmatch.str().c_str());
			std::ssub_match endSubmatch = matches[3];
			range.End = std::atoi(endSubmatch.str().c_str());
		}
		else
		{
			return false;
		}		
	}
	else
	{
		return false;
	}
	
	return true;
}

void VspecParser::ParseNode(vspec::Node& vspecNode, const YAML::Node& node)
{
	for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
	{
		const std::string& key = it->first.as<std::string>();
		if (it->second.IsScalar())
		{
			// std::cout << it->second.as<std::string>() << "\n";
			const std::string& value = it->second.as<std::string>();
			if (key.compare("name") == 0)
			{
				vspecNode.Name = value;
			}
			else if (key.compare("type") == 0)
			{
				vspecNode.Type = value;
			} 
			else if (key.compare("description") == 0)
			{
				vspecNode.Description = value;
			} 
			else if (key.compare("datatype") == 0)
			{
				vspecNode.Datatype = value;
			} 
			else if (key.compare("unit") == 0)
			{
				vspecNode.Unit = value;
			} 
			else if (key.compare("min") == 0)
			{
				vspecNode.Min = value;
			}
			else if (key.compare("max") == 0)
			{
				vspecNode.Max = value;
			}
			else if (key.compare("default") == 0)
			{
				vspecNode.Default = value;
			}
			else if (key.compare("value") == 0)
			{
				vspecNode.Value = value;
			}
			else if (key.compare("aggregate") == 0)
			{
				vspecNode.Aggregate = value;
			}
			else if (key.compare("instances") == 0)
			{
				vspec::Range range;
				if (ParseRange(value, range))
				{
					vspecNode.Instances.push_back(range);
				}
				else
				{
					throw VspecParserException("Range expected '" + value + "'");
				}			
			}					
			else
			{
				throw VspecParserException("Unexpected key value '" + key + "'");
			}	
		}
		else if (it->second.IsNull())
		{		
			vspecNode.Name = key;
		}
		else if (it->second.IsSequence())
		{
			if (key.compare("enum") == 0)
			{
				for (YAML::const_iterator seqIt = it->second.begin(); seqIt != it->second.end(); seqIt++)
				{					
					if (!seqIt->IsScalar())
					{
						throw VspecParserException("enum: scalar expected");						
					}
					vspecNode.Enum.Items.push_back(seqIt->as<std::string>());
				}
			}
			else if (key.compare("instances") == 0)
			{
				vspec::Enumeration outerEnumeration;
				for (YAML::const_iterator seqIt = it->second.begin(); seqIt != it->second.end(); seqIt++)
				{
					if (seqIt->IsScalar())
					{
						vspec::Range range;
						if (ParseRange(seqIt->as<std::string>(), range))
						{
							vspecNode.Instances.push_back(range);
						}
						else
						{
							outerEnumeration.Items.push_back(seqIt->as<std::string>());
						}
						
					}					
					else if (seqIt->IsSequence())
					{
						vspec::Enumeration innerEnumeration;
						for (YAML::const_iterator enumSeqIt = seqIt->begin(); enumSeqIt != seqIt->end(); enumSeqIt++)
						{
							if (enumSeqIt->IsScalar())
							{
								innerEnumeration.Items.push_back(enumSeqIt->as<std::string>());
							}
							else
							{
								throw VspecParserException("instances: scalar expected");	
							}							
						}
						vspecNode.Instances.push_back(innerEnumeration);
					}
					else
					{
						throw VspecParserException("instances: scalar or sequence expected");	
					}						
				}

				if (outerEnumeration.Items.size() != 0)
				{
					vspecNode.Instances.push_back(outerEnumeration);	
				}						
			}					
			else
			{
				throw VspecParserException("Unexpected key value '" + key + "'");
			}
		}
		else
		{
			throw VspecParserException("not sure what the second value is");
		}
	}
}

void VspecParser::TraverseConfig(const YAML::Node& config, vspec::Node& rootVspecNode)
{
	if (!config.IsSequence())
	{
		throw VspecParserException("sequence expected");
	}

	std::stack<vspec::Node*> branchStack;

	YAML::const_iterator it = config.begin();
	if (it != config.end())	
	{
		if (!it->IsMap())
		{
			throw new VspecParserException("map expected");
		}

		ParseNode(rootVspecNode, *it);
		if (rootVspecNode.Type.compare("branch") != 0)
		{
			throw VspecParserException("root must be of type branch");
		}
		branchStack.push(&rootVspecNode);
		it++;
	}

	// No previous branch namespace
	std::string topBranchSuffix = "";

	std::string newBranchSuffix = "";

	for ( ; it != config.end(); it++)
	{
		if (!it->IsMap())
		{
			throw new VspecParserException("map expected");
		}

		std::shared_ptr<vspec::Node> vspecNode(new vspec::Node());

		ParseNode(*vspecNode, *it);	

		size_t newBranchLastDotPos = vspecNode->Name.find_last_of(".");
		if (newBranchLastDotPos == std::string::npos)
		{
			newBranchSuffix = "";
		}
		else
		{
			newBranchSuffix = vspecNode->Name.substr(0, newBranchLastDotPos);
		}

		while (!branchStack.empty())
		{	
			topBranchSuffix = branchStack.top()->Name;

			if (topBranchSuffix.compare(0, newBranchSuffix.length(), newBranchSuffix) == 0 && newBranchSuffix.compare(topBranchSuffix) != 0 && topBranchSuffix.compare("Vehicle") != 0)
			{
				branchStack.pop();
			}
			else
			{
				// std::cout << "BREAK top:'" << topBranchSuffix <<  "'  new:'" << newBranchSuffix << "'" << std::endl;
				break;
			}
		}

		branchStack.top()->Children.push_back(vspecNode);	

		if (vspecNode->Type.compare("branch")  == 0)
		{
			branchStack.push(vspecNode.get());			
		}
	}
} 

void VspecParser::Parse(const std::string& vspecFilePath, vspec::Definition& definition)
{
	std::stringstream strstream;

	Preprocess(vspecFilePath, strstream);

	// std::cout << strstream.str() << std::endl;

	TraverseConfig(YAML::Load(strstream), definition.Root);
}