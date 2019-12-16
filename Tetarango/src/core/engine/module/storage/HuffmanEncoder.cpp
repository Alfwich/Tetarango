#include "HuffmanEncoder.h"

#include <unordered_map>
#include <queue>
#include <iostream>
#include <bitset>
#include <sstream>
#include "util/StringHelper.h"

namespace AWCore
{

	std::string HuffmanEncoder::encode(std::string input)
	{
		if (input.empty())
		{
			return input;
		}

		const auto wrappedInput = wrapInput(input);

		const auto root = buildHuffmanTree(wrappedInput);
		const auto table = buildTable(root);

		std::string bitString = buildBitStringWithTable(wrappedInput, table);

		std::string data = encodeBitstring(bitString, table);

		std::string result;
		serializeTree(root, result);

		result += binaryDelimiter;

		return result + std::string(data.begin(), data.end());
	}

	std::string HuffmanEncoder::wrapInput(const std::string& input)
	{
		return encodingPadding + input + encodingPadding;
	}

	std::unordered_map<char, unsigned int> HuffmanEncoder::buildOccurrenceMap(const std::string& str)
	{
		std::unordered_map<char, unsigned int> occs;
		unsigned int p = 0;
		while (p < str.size())
		{
			occs[str[p++]]++;
		}

		return occs;
	}

	std::shared_ptr<HuffmanEncoder::HuffmanNode> HuffmanEncoder::buildHuffmanTree(const std::string& str)
	{
		std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, Compare> pQueue;

		for (const auto charToOccs : buildOccurrenceMap(str))
		{
			auto n = std::make_shared<HuffmanNode>();
			n->c = charToOccs.first;
			n->v = charToOccs.second;
			pQueue.push(n);
		}

		while (pQueue.size() > 1)
		{
			auto n1 = pQueue.top();
			pQueue.pop();
			auto n2 = pQueue.top();
			pQueue.pop();

			auto h = std::make_shared<HuffmanNode>();
			h->v = n1->v + n2->v;
			h->left = n1;
			h->right = n2;

			pQueue.push(h);

		}

		return pQueue.top();
	}

	std::unordered_map<char, std::string> HuffmanEncoder::buildTable(std::shared_ptr<HuffmanNode> node)
	{
		auto table = std::unordered_map<char, std::string>();

		traverseTree(node, table);

		return table;
	}

	void HuffmanEncoder::traverseTree(std::shared_ptr<HuffmanNode> node, std::unordered_map<char, std::string>& table, std::string current)
	{
		if (node == nullptr)
		{
			return;
		}

		if (node->c != -1)
		{
			table[node->c] = current;
		}

		traverseTree(node->left, table, current + '0');
		traverseTree(node->right, table, current + '1');
	}

	std::string HuffmanEncoder::buildBitStringWithTable(const std::string& input, const std::unordered_map<char, std::string>& table)
	{
		std::string bitString = "";

		unsigned int p = 0;
		while (p < input.size())
		{
			bitString += table.at(input[p++]);
		}

		return bitString;
	}

	std::string HuffmanEncoder::encodeBitstring(const std::string& input, const std::unordered_map<char, std::string>& table)
	{
		std::string data;

		unsigned int p = 0;
		while (p + 8 < input.size())
		{
			data += (char)std::bitset<8>(input.substr(p, 8)).to_ullong();
			p += 8;
		}

		if (p != input.size())
		{
			unsigned int finalInt = (unsigned int)input.size() - p;
			data += (char)std::bitset<8>(input.substr(input.size() - finalInt, finalInt)).to_ullong();
		}

		return data;
	}

	void HuffmanEncoder::serializeTree(std::shared_ptr<HuffmanNode> node, std::string& walk)
	{
		if (node == nullptr)
		{
			walk += (char)-2;
			return;
		}

		walk += node->c;

		serializeTree(node->left, walk);
		serializeTree(node->right, walk);
	}

	std::string HuffmanEncoder::decode(std::string input)
	{
		if (input.empty())
		{
			return input;
		}

		auto tableDataPair = getTableDataPair(input);

		std::shared_ptr<HuffmanNode> tree = hydrateTree(tableDataPair.first);

		std::string bitString = decodeInputToBitString(tableDataPair.second);

		std::string result = decodeBitStringToOutput(bitString, tree);

		return unwrapOutput(result);
	}

	std::pair<std::string, std::string> HuffmanEncoder::getTableDataPair(std::string& input)
	{
		unsigned int i = 0;
		bool inTree = true;
		std::string rawTree;
		std::string data;
		while (i < input.size())
		{
			if (inTree)
			{
				if (StringHelper::startsWith_Offset(&input, i, binaryDelimiter))
				{
					i += 2;
					inTree = false;
					continue;
				}
				rawTree += input[i++];
			}
			else
			{
				data += input[i++];
			}

		}

		return std::pair<std::string, std::string>(rawTree, data);
	}

	std::shared_ptr<HuffmanEncoder::HuffmanNode> HuffmanEncoder::hydrateTree(const std::string treeData)
	{
		std::shared_ptr<HuffmanNode> head;
		int p = 0;

		hydrateTreeRecursive(head, treeData, p);

		return head;
	}

	void HuffmanEncoder::hydrateTreeRecursive(std::shared_ptr<HuffmanNode>& root, const std::string& treeData, int& pos)
	{
		if (treeData[pos] == (char)-2)
		{
			pos++;
			return;
		}

		root = std::make_shared<HuffmanNode>();
		root->c = treeData[pos];
		pos++;

		hydrateTreeRecursive(root->left, treeData, pos);
		hydrateTreeRecursive(root->right, treeData, pos);
	}

	std::string HuffmanEncoder::decodeInputToBitString(const std::string& data)
	{
		std::stringstream ss;
		unsigned int i = 0, j = 0;

		while (i < data.size())
		{
			ss << std::bitset<8>(data[i++]).to_string();
		}

		return ss.str();
	}

	std::string HuffmanEncoder::decodeBitStringToOutput(const std::string& bitString, std::shared_ptr<HuffmanNode>& tree)
	{
		unsigned int i = 0;
		std::shared_ptr<HuffmanNode> current = tree;
		std::string result;

		while (i <= bitString.size())
		{
			if (current->c != -1)
			{
				result += current->c;
				current = tree;

				if (i == bitString.size())
				{
					break;
				}
			}

			if (bitString[i++] == '0')
			{
				current = current->left;
			}
			else
			{
				current = current->right;
			}
		}

		return result;
	}

	std::string HuffmanEncoder::unwrapOutput(std::string& result)
	{
		int tagLength = (int)encodingPadding.size();
		unsigned int i = (int)result.size() - (int)encodingPadding.size() / 2;
		while (i >= 0)
		{
			if (StringHelper::startsWith_Offset(&result, i, encodingPadding.substr(0, 5)))
			{
				break;
			}
			else
			{
				i--;
			}
		}

		return result.substr(tagLength, result.size() - tagLength - (result.size() - i));
	}



}
