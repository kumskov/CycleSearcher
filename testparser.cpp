#include "testparser.hpp"

//##################################################//
//													//
//						InFile						//
//													//
//##################################################//


//##################################################//
//													//
//						TestParser					//
//													//
//##################################################//

//Constructors
TestParser::TestParser()
{
	_reponame = "BL4H";
	//_repo.open(flname);
	//_pkgs = std::vector<Package>();
}

TestParser::TestParser(std::string flname)
{
	_reponame = flname;
	//_pkgs = std::vector<Package>();
}

TestParser::TestParser(const char* flname)
{
	_reponame = flname;
	//_pkgs = std::vector<Package>();
}

TestParser::~TestParser()
{

}

void TestParser::load(std::string flname)
{

}

//Start working with the file
void TestParser::parse()
{
	Package verse1("Verse 1", "WAKE ME UP");
	verse1.addProvide("How can you see into my eyes like open doors?");
	verse1.addProvide("Leading you down into my core where I've become so numb");
	verse1.addProvide("Without a soul my spirit's sleeping somewhere cold");
	verse1.addProvide("Until you find it there and lead it back home");

	verse1.addRequire("(Wake me up)");
	verse1.addRequire("Wake me up inside");
	verse1.addRequire("(I can't wake up)");
	verse1.addRequire("Wake me up inside");
	verse1.addRequire("(Save me)");
	verse1.addRequire("Call my name and save me from the dark");
	verse1.addRequire("(Wake me up)");
	verse1.addRequire("Bid my blood to run");
	verse1.addRequire("(I can't wake up)");
	verse1.addRequire("Before I come undone");
	verse1.addRequire("(Save me)");
	verse1.addRequire("Save me from the nothing I've become");


	Package chorus("Chorus", "WAKE ME UP");
	chorus.addProvide("(Wake me up)");
	chorus.addProvide("Wake me up inside");
	chorus.addProvide("(I can't wake up)");
	chorus.addProvide("Wake me up inside");
	chorus.addProvide("(Save me)");
	chorus.addProvide("Call my name and save me from the dark");
	chorus.addProvide("(Wake me up)");
	chorus.addProvide("Bid my blood to run");
	chorus.addProvide("(I can't wake up)");
	chorus.addProvide("Before I come undone");
	chorus.addProvide("(Save me)");
	chorus.addProvide("Save me from the nothing I've become");


	Package verse2("Verse 2", "WAKE ME UP");
	verse2.addProvide("Now that I know what I'm without");
	verse2.addProvide("You can't just leave me");
	verse2.addProvide("Breathe into me and make me real");
	verse2.addProvide("Bring me to life");

	verse2.addRequire("(Wake me up)");
	verse2.addRequire("Wake me up inside");
	verse2.addRequire("(I can't wake up)");
	verse2.addRequire("Wake me up inside");
	verse2.addRequire("(Save me)");
	verse2.addRequire("Call my name and save me from the dark");
	verse2.addRequire("(Wake me up)");
	verse2.addRequire("Bid my blood to run");
	verse2.addRequire("(I can't wake up)");
	verse2.addRequire("Before I come undone");
	verse2.addRequire("(Save me)");
	verse2.addRequire("Save me from the nothing I've become");


	Package verse3("Verse 3", "WAKE ME UP");
	verse3.addProvide("Bring me to life");
	verse3.addProvide("(I've been living a lie, there's nothing inside)");
	verse3.addProvide("Bring me to life");

	verse3.addRequire("(Wake me up)");
	verse3.addRequire("Wake me up inside");
	verse3.addRequire("(I can't wake up)");
	verse3.addRequire("Wake me up inside");
	verse3.addRequire("(Save me)");
	verse3.addRequire("Call my name and save me from the dark");
	verse3.addRequire("(Wake me up)");
	verse3.addRequire("Bid my blood to run");
	verse3.addRequire("(I can't wake up)");
	verse3.addRequire("Before I come undone");
	verse3.addRequire("(Save me)");
	verse3.addRequire("Save me from the nothing I've become");


	Package verse4("Verse 4", "WAKE ME UP");
	verse4.addProvide("Frozen inside without your touch");
	verse4.addProvide("Without your love, darling");
	verse4.addProvide("Only you are the life among the dead");

	verse4.addRequire("(Wake me up)");
	verse4.addRequire("Wake me up inside");
	verse4.addRequire("(I can't wake up)");
	verse4.addRequire("Wake me up inside");
	verse4.addRequire("(Save me)");
	verse4.addRequire("Call my name and save me from the dark");
	verse4.addRequire("(Wake me up)");
	verse4.addRequire("Bid my blood to run");
	verse4.addRequire("(I can't wake up)");
	verse4.addRequire("Before I come undone");
	verse4.addRequire("(Save me)");
	verse4.addRequire("Save me from the nothing I've become");


	Package verse5("Verse 5", "WAKE ME UP");
	verse5.addProvide("All this time I can't believe I couldn't see");
	verse5.addProvide("Kept in the dark but you were there in front of me");
	verse5.addProvide("I've been sleeping a thousand years it seems");
	verse5.addProvide("Got to open my eyes to everything");
	verse5.addProvide("Without a thought, without a voice, without a soul");
	verse5.addProvide("Don't let me die here");
	verse5.addProvide("There must be something more");
	verse5.addProvide("Bring me to life");

	verse5.addRequire("(Wake me up)");
	verse5.addRequire("Wake me up inside");
	verse5.addRequire("(I can't wake up)");
	verse5.addRequire("Wake me up inside");
	verse5.addRequire("(Save me)");
	verse5.addRequire("Call my name and save me from the dark");
	verse5.addRequire("(Wake me up)");
	verse5.addRequire("Bid my blood to run");
	verse5.addRequire("(I can't wake up)");
	verse5.addRequire("Before I come undone");
	verse5.addRequire("(Save me)");
	verse5.addRequire("Save me from the nothing I've become");

	Package verse1dump("v1 text", "WAKE ME UP");
	verse1dump.addRequire("How can you see into my eyes like open doors?");
	verse1dump.addRequire("Leading you down into my core where I've become so numb");
	verse1dump.addRequire("Without a soul my spirit's sleeping somewhere cold");
	verse1dump.addRequire("Until you find it there and lead it back home");

	Package verse2dump("v2 text", "WAKE ME UP");
	verse2dump.addRequire("Now that I know what I'm without");
	verse2dump.addRequire("You can't just leave me");
	verse2dump.addRequire("Breathe into me and make me real");
	verse2dump.addRequire("Bring me to life");

	Package verse3dump("v3 text", "WAKE ME UP");
	verse3dump.addRequire("Bring me to life");
	verse3dump.addRequire("(I've been living a lie, there's nothing inside)");
	verse3dump.addRequire("Bring me to life");

	Package verse4dump("v4 text", "WAKE ME UP");
	verse4dump.addRequire("Frozen inside without your touch");
	verse4dump.addRequire("Without your love, darling");
	verse4dump.addRequire("Only you are the life among the dead");

	Package verse5dump("v5 text", "WAKE ME UP");
	verse5dump.addRequire("All this time I can't believe I couldn't see");
	verse5dump.addRequire("Kept in the dark but you were there in front of me");
	verse5dump.addRequire("I've been sleeping a thousand years it seems");
	verse5dump.addRequire("Got to open my eyes to everything");
	verse5dump.addRequire("Without a thought, without a voice, without a soul");
	verse5dump.addRequire("Don't let me die here");
	verse5dump.addRequire("There must be something more");
	verse5dump.addRequire("Bring me to life");

	_pkgs.addPackage(verse5dump);
	_pkgs.addPackage(verse5);
	_pkgs.addPackage(verse4dump);
	_pkgs.addPackage(verse4);
	_pkgs.addPackage(verse2dump);
	_pkgs.addPackage(verse2);
	_pkgs.addPackage(chorus);
	_pkgs.addPackage(verse1);
	_pkgs.addPackage(verse1dump);
	_pkgs.addPackage(verse3dump);
	_pkgs.addPackage(verse3);
}

Container TestParser::getContainer()
{
	return _pkgs;
}

Package TestParser::getPackage(int index)
{
	if ((index >= _pkgs.size()) ||
		(index < 0))
	{
		throw std::logic_error("TestParser: Requested package with unreachable index");
	}
	return _pkgs[index];
}

int TestParser::getAmountOfPackages()
{
	return _pkgs.size();
}


std::string TestParser::getClassName()
{
	return "\"Can't Wake Up\" Sample Parser";
}

std::string TestParser::getClassDescription()
{
	return "SAVE ME FROM THE NOTHING I'VE BECOME";
}

//dlopen hook
extern "C" Parser* getParser()
{
	return new TestParser;
}