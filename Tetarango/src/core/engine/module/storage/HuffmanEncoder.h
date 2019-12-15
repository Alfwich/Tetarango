#pragma once

#include <string>
#include <unordered_map>

namespace MT
{


	class HuffmanEncoder
	{
		class HuffmanNode
		{
		public:
			int v = 0;
			char c = -1;
			std::shared_ptr<HuffmanNode> left;
			std::shared_ptr<HuffmanNode> right;

		};

		struct Compare
		{
			bool operator() (const std::shared_ptr<HuffmanNode>& lhs, const std::shared_ptr<HuffmanNode>& rhs) const
			{
				return lhs->v > rhs->v;
			}
		};

		std::string encodingPadding = "[ENCODING_PADDING__]";
		std::string binaryDelimiter = "#B";

		std::string wrapInput(const std::string& input);
		std::unordered_map<char, unsigned int> buildOccurrenceMap(const std::string& str);
		std::shared_ptr<HuffmanNode> buildHuffmanTree(const std::string& str);
		std::unordered_map<char, std::string> buildTable(std::shared_ptr<HuffmanNode> node);
		void traverseTree(std::shared_ptr<HuffmanNode> node, std::unordered_map<char, std::string>& table, std::string current = "");
		std::string buildBitStringWithTable(const std::string& input, const std::unordered_map<char, std::string>& table);
		std::string encodeBitstring(const std::string& input, const std::unordered_map<char, std::string>& table);

		std::shared_ptr<HuffmanNode> hydrateTree(const std::string treeData);
		void hydrateTreeRecursive(std::shared_ptr<HuffmanNode>& root, const std::string& treeData, int& pos);
		std::pair<std::string, std::string> getTableDataPair(std::string& input);
		void serializeTree(std::shared_ptr<HuffmanNode> node, std::string& walk);
		std::string decodeInputToBitString(const std::string& data);
		std::string decodeBitStringToOutput(const std::string& bitString, std::shared_ptr<HuffmanNode>& tree);
		std::string unwrapOutput(std::string& result);



	public:
		std::string encode(std::string input);
		std::string decode(std::string input);
	};
}
