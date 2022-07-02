// DW_393014.cpp : main project file.
#include "stdafx.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Linq;

public ref class CCommands
{
private:
	//
   static void _doShowDir(void)
   {
      Console::WriteLine(
         String::Join("\r\n",
         Enumerable::ToArray<String^>(
         Directory::GetFiles(
         Directory::GetCurrentDirectory()))));
   }
	//
   static void _doHelp()
   {
      Console::WriteLine("Commands are: " +
         String::Join(", ",
         Enumerable::ToArray<String^>(
         Enumerable::Select(arr_kvp_s2aCommands, getKeyFromKvpS2a))) + ", quit, exit");
   }
	//
	static Action^ _getActFromKvpS2aJoin(KeyValuePair<String^, Action^> kvpS2a, String^) { return kvpS2a.Value; }
	//
	static Action^ _clear = gcnew Action(Console::Clear);
	static Action^ _help = gcnew Action(_doHelp);
	static Action^ _showDir = gcnew Action(_doShowDir);
	//
	static String^ _getKeyFromKvpS2a(KeyValuePair<String^, Action^> kvp_s2a) { return kvp_s2a.Key; }
	static String^ _getStringAsString(String^ str) { return str; }
	//
	static bool _notNothing(Action^ act) { return nullptr != act; }
	static void _doNothing(){}

public:
	static Func<KeyValuePair<String^, Action^>, String^>^ getKeyFromKvpS2a =
		gcnew Func<KeyValuePair<String^, Action^>, String^>(_getKeyFromKvpS2a);

	static Func<String^, String^>^ getStringAsString = gcnew Func<String^, String^>(_getStringAsString);

	static Func<KeyValuePair<String^, Action^>, String^, Action^>^ getActFromKvpS2aJoin =
		gcnew Func<KeyValuePair<String^, Action^>, String^, Action^>(_getActFromKvpS2aJoin);

	static String^ NowTime(DateTime^ dt)
	{
		return String::Format("{0}:{1}:{2}",
			dt->Hour.ToString("D2"),
			dt->Minute.ToString("D2"),
			dt->Second.ToString("D2"));
	}

	//
	static IEnumerable<KeyValuePair<String^, Action^>>^ arr_kvp_s2aCommands =
		gcnew array<KeyValuePair<String^, Action^>>
		{
			KeyValuePair<String^, Action^>("spy", _showDir),
			KeyValuePair<String^, Action^>("wipe", _clear),
			KeyValuePair<String^, Action^>("help", _help),
		};

	static Action^ DoNothing = gcnew Action(_doNothing);
	static Func<Action^, bool>^ IsSomething = gcnew Func<Action^, bool>(_notNothing);
};

int main(void)
{	//array<System::String ^> ^args)
   Console::WriteLine(L"Welcome to CLI Shell (v1.1)\n");

   array<String^>^ arr_strQuitExit = { "exit", "quit" };
   String^ strCommand = "";

   while (!Enumerable::Contains<String^>(arr_strQuitExit, strCommand))
   {
      Console::Write("{0}>: ", CCommands::NowTime(DateTime::Now));
      strCommand = Console::ReadLine()->ToLower();

      //////////////////////////////////////////////////////////////////////////////
      // Invoke the first non-null Action that matches the command
      // If no match, call the DoNothing action.
      Enumerable::First<Action^>
      (	// construct pieces ensures no nulls
         gcnew array < Action^ >
			{	// [0]=desired command, [1]=fall-back
				Enumerable::FirstOrDefault<Action^>
					( // returns nullptr if no matching command
					Enumerable::Join<KeyValuePair<String^, Action^>, String^, String^, Action^>
					(
					CCommands::arr_kvp_s2aCommands,
					gcnew array < String^ > { strCommand }, // array of 1
					CCommands::getKeyFromKvpS2a,		// get the Key (string)
					CCommands::getStringAsString,	   // return the string
					CCommands::getActFromKvpS2aJoin  // (KVP, STR) => KVP->Key
					)
					),
					CCommands::DoNothing // when no match or nullptr
			},
			CCommands::IsSomething // not nullptr
      )->Invoke();
   }
   //
   return 0;
}
