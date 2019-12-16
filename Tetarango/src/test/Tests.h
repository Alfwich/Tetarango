#pragma once
#pragma warning(disable : 4129) //Unrecognized character escape sequence

#include "GameConfig.h"
#include "engine/module/SystemModuleBundle.h"
#include "TestActor.h"
#include "util/StringHelper.h"
#include "util/RandomGenerator.h"

namespace MTTest
{


	class Tests
	{
	public:
		static void run(std::shared_ptr<AWCore::SystemModuleBundle> modules)
		{
			Tests::serializationTests(modules->serialization);
			Tests::filesystemTests(modules->filesystem);
			Tests::textCompressionTests(modules->storage);
			Tests::encodingTests(modules->storage);
			Tests::rngTests();
			//Tests::memoryAndDeepTreeTests(modules); // Infinite - Test as needed
			SDL_ClearError();
		}

	private:
		static void throwException(std::string category, std::string msg)
		{
			throw "Tests::Exception found:" + category + "::" + msg;
		}

		template <typename T>
		static void assertEqual(const std::string category, const T a, const T b)
		{
			if (a != b)
			{
				Tests::throwException(category, "'" + a + "' does not equal '" + b + "'.");
			}
		}

		static void filesystemTests(std::shared_ptr<AWCore::Filesystem> fs)
		{
			const std::string filename("test-file.txt");
			std::string fileContent("");

			fs->writeContentToFile(filename, fileContent);
			Tests::assertEqual("FILESYSTEM", fs->readContentsFromFile(filename), fileContent);

			fileContent = "write content.";
			fs->writeContentToFile(filename, fileContent);
			Tests::assertEqual("FILESYSTEM", fs->readContentsFromFile(filename), fileContent);

			const std::string fileAppendContent("\nappend content.");
			fileContent += fileAppendContent;
			fs->appendContentToFile(filename, fileAppendContent);
			Tests::assertEqual("FILESYSTEM", fs->readContentsFromFile(filename), fileContent);
		}

		static void memoryAndDeepTreeTests(std::shared_ptr<AWCore::SystemModuleBundle> modules)
		{

			while (true)
			{
				std::shared_ptr<AWCore::Scene> infScene = std::make_unique<AWCore::Scene>("second");
				{
					{
						std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
						to->setPosition(460.0, 140.0);
						infScene->add(to);
					}
					{
						std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
						to->setPosition(460.0, 140.0);
						infScene->add(to);
					}
					{
						std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
						to->setPosition(460.0, 140.0);
						infScene->add(to);
					}
					{
						std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
						to->setPosition(460.0, 140.0);
						infScene->add(to);
					}
					{
						std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
						to->setPosition(460.0, 140.0);
						infScene->add(to);
					}

				}

				std::string t = modules->serialization->serialize(infScene);
				std::shared_ptr<AWCore::ISerializable> o = modules->serialization->hydrate(t);

				std::shared_ptr<AWCore::Scene> infS = std::make_unique<AWCore::Scene>("inf");
				for (int i = 0; i < 1000; ++i)
				{
					std::shared_ptr<TestActor> t = std::make_unique<TestActor>();
					infS->add(t);
				}
			}
		}

		static void serializationTests(std::shared_ptr<AWCore::Serialization> serialization)
		{

			std::shared_ptr<AWCore::Scene> infScene = std::make_unique<AWCore::Scene>("second");
			{

				std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
				to->setPosition(460.0, 140.0);
				infScene->add(to);
				{
					std::shared_ptr<AWCore::Rectangle> rect4 = std::make_unique<AWCore::Rectangle>();
					rect4->setColor(0xff, 0, 0xff);
					rect4->setSizeAndPosition(450, 130, 300.0, 300.0);
					infScene->add(rect4);


					std::shared_ptr<AWCore::Rectangle> rect = std::make_unique<AWCore::Rectangle>();
					rect->setColor(0xff, 0, 0);
					rect->setSizeAndPosition(0.0, 0.0, 30.0, 30.0);
					to->add(rect);

					std::shared_ptr<AWCore::Rectangle> rect2 = std::make_unique<AWCore::Rectangle>();
					rect2->setColor(0, 0xff, 0);
					rect2->setSizeAndPosition(30.0, 0.0, 30.0, 30.0);
					rect->add(rect2);

					std::shared_ptr<AWCore::Rectangle> rect3 = std::make_unique<AWCore::Rectangle>();
					rect3->setColor(0, 0, 0xff);
					rect3->setSizeAndPosition(-15, 30.0, 30.0, 30.0);
					rect2->add(rect3);
				}

				{
					std::shared_ptr<AWCore::Rectangle> rect4 = std::make_unique<AWCore::Rectangle>();
					rect4->setColor(0xff, 0, 0xff);
					rect4->setSizeAndPosition(450, 130, 300.0, 300.0);
					infScene->add(rect4);

					std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
					to->setPosition(460.0, 140.0);
					infScene->add(to);

					std::shared_ptr<AWCore::Rectangle> rect = std::make_unique<AWCore::Rectangle>();
					rect->setColor(0xff, 0, 0);
					rect->setSizeAndPosition(0.0, 0.0, 30.0, 30.0);
					to->add(rect);

					std::shared_ptr<AWCore::Rectangle> rect2 = std::make_unique<AWCore::Rectangle>();
					rect2->setColor(0, 0xff, 0);
					rect2->setSizeAndPosition(30.0, 0.0, 30.0, 30.0);
					rect->add(rect2);

					std::shared_ptr<AWCore::Rectangle> rect3 = std::make_unique<AWCore::Rectangle>();
					rect3->setColor(0, 0, 0xff);
					rect3->setSizeAndPosition(-15, 30.0, 30.0, 30.0);
					rect2->add(rect3);
				}

				{
					std::shared_ptr<AWCore::Rectangle> rect4 = std::make_unique<AWCore::Rectangle>();
					rect4->setColor(0xff, 0, 0xff);
					rect4->setSizeAndPosition(450, 130, 300.0, 300.0);
					infScene->add(rect4);

					std::shared_ptr<TestActor> to = std::make_unique<TestActor>();
					to->setPosition(460.0, 140.0);
					infScene->add(to);

					std::shared_ptr<AWCore::Rectangle> rect = std::make_unique<AWCore::Rectangle>();
					rect->setColor(0xff, 0, 0);
					rect->setSizeAndPosition(0.0, 0.0, 30.0, 30.0);
					to->add(rect);

					std::shared_ptr<AWCore::Rectangle> rect2 = std::make_unique<AWCore::Rectangle>();
					rect2->setColor(0, 0xff, 0);
					rect2->setSizeAndPosition(30.0, 0.0, 30.0, 30.0);
					rect->add(rect2);

					std::shared_ptr<AWCore::Rectangle> rect3 = std::make_unique<AWCore::Rectangle>();
					rect3->setColor(0, 0, 0xff);
					rect3->setSizeAndPosition(-15, 30.0, 30.0, 30.0);
					rect2->add(rect3);
				}
			}



			std::string t = serialization->serialize(infScene);
			std::string orig = t;

			for (int i = 0; i < 5; ++i)
			{
				auto o = serialization->hydrate(t);
				auto ao = std::static_pointer_cast<AWCore::ApplicationObject>(o);

				if (ao == nullptr)
				{
					Tests::throwException("SERILIZATION", "Serialized did not produce an subclass of ApplicationObject.");
				}

				ao->attach();

				auto t2 = serialization->serialize(o);

				if (t.size() != t2.size() || orig.size() != t2.size())
				{
					Tests::throwException("SERILIZATION", "Serialized does not match expected output.");
				}

				t = t2;
			}
		}

		static void textCompressionTests(std::shared_ptr<AWCore::Storage> storage)
		{
			{
				std::string testStr = "abcd";
				std::string encodedStr = testStr;
				storage->compress(encodedStr);
				std::string decompressedStr = encodedStr;
				storage->decompress(decompressedStr);
				double compressionRatio = 1 - (double)encodedStr.size() / (double)testStr.size();

				Tests::assertEqual("COMPRESSION", testStr, decompressedStr);
			}

			{
				std::string testStr = "Hello my name is Arthur!";
				std::string encodedStr = testStr;
				storage->compress(encodedStr);
				std::string decompressedStr = encodedStr;
				storage->decompress(decompressedStr);
				double compressionRatio = 1 - (double)encodedStr.size() / (double)testStr.size();

				Tests::assertEqual("COMPRESSION", testStr, decompressedStr);
			}

			{
				std::string testStr = "qqwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890qwertyuiop1234567890wertyuiop1234567890";
				std::string encodedStr = testStr;
				storage->compress(encodedStr);
				std::string decompressedStr = encodedStr;
				storage->decompress(decompressedStr);
				double compressionRatio = 1 - (double)encodedStr.size() / (double)testStr.size();

				Tests::assertEqual("COMPRESSION", testStr, decompressedStr);
			}

			{
				std::string testStr = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccccccccccdd";
				std::string encodedStr = testStr;
				storage->compress(encodedStr);
				std::string decompressedStr = encodedStr;
				storage->decompress(decompressedStr);
				double compressionRatio = 1 - (double)encodedStr.size() / (double)testStr.size();

				Tests::assertEqual("COMPRESSION", testStr, decompressedStr);
			}

			{
				std::string testStr =
					std::string("##STORE_ARCHIVE_START####STORE_KEY_VALUE_START##whole_scene_graph##STORE_KEY_VALUE_DELIMITER####SCENE_ARCHIVE_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::SceneContainer##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene_container_cs##SCENE_PRAM_VALUE##splash##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneGame##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##game##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneMainMenu##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##main_menu##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Container##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##root##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##title_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCE")
					+ std::string("NE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##67.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##727.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##192.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Example Game - DEV - v0.00.01a##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##50##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##play_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE")
					+ std::string("_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##331.333333##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##58.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Play##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_STA")
					+ std::string("RT####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##option_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##474.666667##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##105.")
					+ std::string("000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Options##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##exit_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE")
					+ std::string("_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##618.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Exit##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneOptionsMenu##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELI")
					+ std::string("M##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##options_menu##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##title##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##38.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##99.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Options##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##28##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRA")
					+ std::string("M_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##back_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##668.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##67.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##S")
					+ std::string("CENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Back##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneSavedGamesMenu##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##saved_games_menu##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##saved_games_title##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##38.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##173.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SC")
					+ std::string("ENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Saved Games##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##28##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##save_slot_1##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##169.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_obje")
					+ std::string("ct_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##85.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Empty##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##save_slot_2##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PR")
					+ std::string("AM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##306.500000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##85.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_")
					+ std::string("DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Empty##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##save_slot_3##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##444.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DEL")
					+ std::string("IM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##85.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Empty##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##back_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCEN")
					+ std::string("E_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##668.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##67.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Back##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_P")
					+ std::string("RAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneSplash##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##splash##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##splash_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##0.858000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##72.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##205.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##384.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##SPLASH##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_")
					+ std::string("font.size##SCENE_PRAM_VALUE##54##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Scene##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE##game##SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##testScene##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::TileMap##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##tile-grass##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##768.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##1024.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##384.000000##SCENE_PRAM_DELIM##__tile_map_cTexName##SCENE_PRAM_VALUE##tile-grass##SCENE_PRAM_DELIM##__tile_map_tileH##SCENE_PRAM_VALUE##128##SCENE_PRAM_DELIM##__tile_map_tileW##SCENE_PRAM_VALUE##128##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::GameCamera##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__camera_cX##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__camera_cY##SCENE_PRAM_VALUE##384.000000##SCENE_PRAM_DELIM##__camera_dX##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__camera_dY##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__camera_dZ##SCENE_PRAM_V")
					+ std::string("ALUE##2.000000##SCENE_PRAM_DELIM##__camera_maxZI##SCENE_PRAM_VALUE##4.000000##SCENE_PRAM_DELIM##__camera_maxZO##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__camera_x##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__camera_y##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__camera_z##SCENE_PRAM_VALUE##2.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Rectangle##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##__solid__##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##200.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##45.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##200.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##384.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Rectangle##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##__solid__##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##20.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__rende")
					+ std::string("rable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##200.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##100.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##-200.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Rectangle##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##__solid__##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##10.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Rectangle##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##__solid__##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##3.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##3.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##25.000000##SCENE")
					+ std::string("_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##25.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Rectangle##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##__solid__##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##100.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##10.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Rectangle##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##__solid__##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##50.000000##SCE")
					+ std::string("NE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##200.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##10.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##53.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##383.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##100.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##10.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Rotating Hello World!##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##40##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::ParticleSystem##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##_")
					+ std::string("_application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__particle_system_globalEmt##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__particle_system_globalRcv##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__particle_system_ppS##SCENE_PRAM_VALUE##1000##SCENE_PRAM_DELIM##__particle_system_s.xV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_s.yV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_started##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__primitive_c.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::TestParticleFactory##SCENE_TYPE_NAME_END####SCENE_PRAM_START##test_particle_factory_b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##test_particle_factory_g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##test_particle_factory_r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::ParticleSystem##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__particle_system_globalEmt##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__particle_system_globalRcv##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__particle_system_ppS##SCENE_PRAM_VALUE##1000##SCENE_PRAM_DELIM##__particle_system_s.xV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__par")
					+ std::string("ticle_system_s.yV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_started##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__primitive_c.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##200.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::TestParticleFactory##SCENE_TYPE_NAME_END####SCENE_PRAM_START##test_particle_factory_b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##test_particle_factory_g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##test_particle_factory_r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::ParticleSystem##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__particle_system_globalEmt##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__particle_system_globalRcv##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__particle_system_ppS##SCENE_PRAM_VALUE##1000##SCENE_PRAM_DELIM##__particle_system_s.xV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_s.yV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_started##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__primitive_c.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__r")
					+ std::string("enderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##200.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##20.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::TestParticleFactory##SCENE_TYPE_NAME_END####SCENE_PRAM_START##test_particle_factory_b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##test_particle_factory_g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##test_particle_factory_r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::ParticleSystem##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__particle_system_globalEmt##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__particle_system_globalRcv##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__particle_system_ppS##SCENE_PRAM_VALUE##1000##SCENE_PRAM_DELIM##__particle_system_s.xV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_s.yV##SCENE_PRAM_VALUE##400.000000##SCENE_PRAM_DELIM##__particle_system_started##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__primitive_c.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_x##S")
					+ std::string("CENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##20.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::TestParticleFactory##SCENE_TYPE_NAME_END####SCENE_PRAM_START##test_particle_factory_b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##test_particle_factory_g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##test_particle_factory_r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::PawnHuman##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##stand##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##actor-pawn-human##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##18.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##2.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##2.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##7.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##584.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::ParticleSy")
					+ std::string("stem##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__particle_system_globalEmt##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__particle_system_globalRcv##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__particle_system_ppS##SCENE_PRAM_VALUE##1##SCENE_PRAM_DELIM##__particle_system_s.xV##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__particle_system_s.yV##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__particle_system_started##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__primitive_c.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__primitive_c.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_ARCHIVE_END####STORE_PROPERTY_DELIMITER####STORE_ARCHIVE_END##");
				std::string encodedStr = testStr;
				storage->compress(encodedStr);
				std::string decompressedStr = encodedStr;
				storage->decompress(decompressedStr);
				double compressionRatio = 1 - (double)encodedStr.size() / (double)testStr.size();

				Tests::assertEqual("COMPRESSION", testStr, decompressedStr);
			}
		}

		static void encodingTests(std::shared_ptr<AWCore::Storage> storage)
		{
			{
				std::string test1 = "hello world!";
				std::string testO = test1;

				storage->xorEncode(test1);
				storage->xorDecode(test1);

				if (test1 != testO)
				{
					Tests::throwException("ENCODING", "Encoded did not match decoded expected value.");
				}
			}

			{
				std::string test1 = "1";
				std::string testO = test1;

				storage->xorEncode(test1);
				storage->xorDecode(test1);

				if (test1 != testO)
				{
					Tests::throwException("ENCODING", "Encoded did not match decoded expected value.");
				}
			}

			{
				std::string test1 = "The quick []{}{}';/.,?><mn.,mxcnxclkj123102398710598123\"brown frog jumped over the slow dog !@#$!#%#$^&$*(^&*(&(*)&*)       \n\n\n\ \t \t \n\t\r\r !!!!1!____\0\0\333\22\4\56\"";
				std::string testO = test1;

				storage->xorEncode(test1);
				storage->xorDecode(test1);

				if (test1 != testO)
				{
					Tests::throwException("ENCODING", "Encoded did not match decoded expected value.");
				}
			}

			{
				std::string test1 =
					std::string("##STORE_ARCHIVE_START####STORE_KEY_VALUE_START##whole_scene_graph##STORE_KEY_VALUE_DELIMITER####SCENE_ARCHIVE_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::SceneContainer##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene_container_cs##SCENE_PRAM_VALUE##splash##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneGame##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##game##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneMainMenu##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##main_menu##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Container##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##root##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##title_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCE")
					+ std::string("NE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##67.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##727.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##192.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Example Game - DEV - v0.00.01a##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##50##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##play_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE")
					+ std::string("_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##331.333333##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##58.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Play##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_STA")
					+ std::string("RT####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##option_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##474.666667##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##105.")
					+ std::string("000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Options##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##exit_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE")
					+ std::string("_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##618.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Exit##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneOptionsMenu##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELI")
					+ std::string("M##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##options_menu##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##title##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##38.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##99.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Options##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##28##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRA")
					+ std::string("M_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##back_button##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##668.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##67.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##S")
					+ std::string("CENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Back##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::SceneSavedGamesMenu##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__scene__name##SCENE_PRAM_VALUE##saved_games_menu##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##saved_games_title##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##38.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##173.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##50.000000##SCENE_PRAM_DELIM##__text_config.bc.a##SC")
					+ std::string("ENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Saved Games##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##28##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##save_slot_1##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##169.000000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_obje")
					+ std::string("ct_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##85.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.text##SCENE_PRAM_VALUE##Empty##SCENE_PRAM_DELIM##__text_font.name##SCENE_PRAM_VALUE##medium##SCENE_PRAM_DELIM##__text_font.size##SCENE_PRAM_VALUE##30##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_CHILDREN_LIST_END####SCENE_OBJECT_END####SCENE_CHILDREN_LIST_START####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MTGame::ButtonBasic##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__animated_currentAN##SCENE_PRAM_VALUE##default##SCENE_PRAM_DELIM##__animated_fpsOffset##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_frame##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__animated_isPaused##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_FALSE####SCENE_PRAM_DELIM##__animated_isPlaying##SCENE_PRAM_VALUE####SCENE_TYPE_BOOL_TRUE####SCENE_PRAM_DELIM##__application_object_name##SCENE_PRAM_VALUE##save_slot_2##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PR")
					+ std::string("AM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE##button-basic##SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##150.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##0.500000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##350.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##512.000000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##306.500000##SCENE_PRAM_DELIM####SCENE_PRAM_END####SCENE_CHILDREN_LIST_DELIM####SCENE_OBJECT_START####SCENE_TYPE_NAME_START##class MT::Text##SCENE_TYPE_NAME_END####SCENE_PRAM_START##__application_object_name##SCENE_PRAM_VALUE##button_text##SCENE_PRAM_DELIM##__application_object_tags##SCENE_PRAM_VALUE##0000000000000000##SCENE_PRAM_DELIM##__application_object_timescope##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cTexName##SCENE_PRAM_VALUE####SCENE_PRAM_DELIM##__element_cm.a##SCENE_PRAM_VALUE##205##SCENE_PRAM_DELIM##__element_cm.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.g##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__element_cm.r##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__renderable_al##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_h##SCENE_PRAM_VALUE##41.000000##SCENE_PRAM_DELIM##__renderable_r##SCENE_PRAM_VALUE##0.000000##SCENE_PRAM_DELIM##__renderable_sX##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_sY##SCENE_PRAM_VALUE##1.000000##SCENE_PRAM_DELIM##__renderable_w##SCENE_PRAM_VALUE##85.000000##SCENE_PRAM_DELIM##__renderable_x##SCENE_PRAM_VALUE##87.500000##SCENE_PRAM_DELIM##__renderable_y##SCENE_PRAM_VALUE##37.500000##SCENE_PRAM_DELIM##__text_config.bc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.bc.b##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.g##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.bc.r##SCENE_PRAM_VALUE##0##SCENE_PRAM_DELIM##__text_config.mode##SCENE_PRAM_VALUE##2##SCENE_PRAM_DELIM##__text_config.tc.a##SCENE_PRAM_VALUE##255##SCENE_PRAM_DELIM##__text_config.tc.b##SCENE_PRAM_VALUE##255##SCENE_PRAM_");

				std::string testO = test1;

				storage->xorEncode(test1);
				storage->xorDecode(test1);

				if (test1 != testO)
				{
					Tests::throwException("ENCODING", "Encoded did not match decoded expected value.");
				}

			}
		}

		static void rngTests()
		{
			{
				const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

				rng->registerCase(0, 0.5);
				rng->registerCase(1, 0.5);

				const int tests = 1000000;
				int result = 0;
				for (int i = 0; i < tests; ++i)
				{
					result += rng->get();
				}

				if (result < (tests / 2 - tests * 0.005) || result >(tests / 2 + tests * 0.005))
				{
					Tests::throwException("RNG", "Unexpected return value - too far from uniform average");
				}
			}


			const int totalTestPasses = 2;
			for (int k = 0; k < totalTestPasses; ++k)
			{
				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 1.0);
					rng->registerCase(1, 0.0);

					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < 100; ++i)
						{
							result += rng->get();
						}

						if (result != 0)
						{
							Tests::throwException("RNG", "Unexpected return value - expected 0");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 1.0);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 0.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 0.0);

					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < 100; ++i)
						{
							result += rng->get();
						}

						if (result != 0)
						{
							Tests::throwException("RNG", "Unexpected return value - expected 0");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 0.0);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 0.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 1.0);

					int iters = 100;
					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < iters; ++i)
						{
							result += rng->get();
						}

						if (result != iters * 4)
						{
							Tests::throwException("RNG", "Unexpected return value - expected iters * 4");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 0.0);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 0.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 0.1);

					int iters = 100;
					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < iters; ++i)
						{
							result += rng->get();
						}

						if (result != iters * 4)
						{
							Tests::throwException("RNG", "Unexpected return value - expected iters * 4");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 0.1);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 0.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 0.1);

					int iters = 100;
					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < iters; ++i)
						{
							result += rng->get();
						}

						if (result >= iters * 4)
						{
							Tests::throwException("RNG", "Unexpected return value - expected < iters * 4");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 0.01);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 0.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 0.1);

					int iters = 1000;
					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < iters; ++i)
						{
							result += rng->get();
						}

						if (result >= iters * 4 || result <= ((iters * 4) / 2))
						{
							Tests::throwException("RNG", "Unexpected return value - expected < iters * 4 and > (iters * 4) / 2");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();
					const auto rng2 = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 0.0);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 0.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 0.1);

					rng2->registerCase(0, 0.0);
					rng2->registerCase(1, 1.0);

					rng->registerSubGenerator(4, rng2);

					int iters = 100;
					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < iters; ++i)
						{
							result += rng->get();
						}

						if (result != iters)
						{
							Tests::throwException("RNG", "Unexpected return value - expected iters");
						}
					}
				}

				{
					const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();
					const auto rng2 = std::make_shared<AWCore::RandomGenerator<int>>();

					rng->registerCase(0, 0.0);
					rng->registerCase(1, 0.0);
					rng->registerCase(2, 1.0);
					rng->registerCase(3, 0.0);
					rng->registerCase(4, 0.0);

					rng2->registerCase(0, 0.0);
					rng2->registerCase(1, 1.0);
					rng2->registerCase(2, 1.0);
					rng2->registerCase(3, 1.0);
					rng2->registerCase(4, 1.0);

					rng->registerSubGenerator(4, rng2);

					int iters = 100;
					for (int checks = 0; checks < 10; ++checks)
					{
						int result = 0;
						for (int i = 0; i < iters; ++i)
						{
							result += rng->get();
						}

						if (result != iters * 2)
						{
							Tests::throwException("RNG", "Unexpected return value - expected 200");
						}
					}
				}
			}

			{
				const auto rng = std::make_shared<AWCore::RandomGenerator<int>>();
				const auto rng2 = std::make_shared<AWCore::RandomGenerator<int>>();
				const auto rng3 = std::make_shared<AWCore::RandomGenerator<int>>();

				rng->registerCase(0, 0.0);
				rng->registerCase(1, 0.0);
				rng->registerCase(2, 0.0);
				rng->registerCase(3, 1.0);
				rng->registerCase(4, 0.0);

				rng2->registerCase(0, 0.0);
				rng2->registerCase(1, 1.0);
				rng2->registerCase(2, 0.0);
				rng2->registerCase(3, 0.0);
				rng2->registerCase(4, 0.0);

				rng3->registerCase(0, 100.0);
				rng3->registerCase(1, 0.0);
				rng3->registerCase(2, 0.0);
				rng3->registerCase(3, 0.0);
				rng3->registerCase(4, 0.0);

				rng->registerSubGenerator(3, rng2);
				rng2->registerSubGenerator(1, rng3);

				int iters = 100;
				for (int checks = 0; checks < 10; ++checks)
				{
					int result = 0;
					for (int i = 0; i < iters; ++i)
					{
						result += rng->get();
					}

					if (result != 0)
					{
						Tests::throwException("RNG", "Unexpected return value - expected 0");
					}
				}
			}
		}
	};
}
