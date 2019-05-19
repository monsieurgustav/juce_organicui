/*
  ==============================================================================

    GlobalSettings.cpp
    Created: 3 Jan 2018 3:52:13pm
    Author:  Ben

  ==============================================================================
*/

juce_ImplementSingleton(GlobalSettings)

ApplicationCommandManager& getCommandManager();

GlobalSettings::GlobalSettings() :
	ControllableContainer("Global Settings"),
	startupCC("Startup and Update"),
	saveLoadCC("Save and Load"),
	editingCC("Editing")
{
	saveAndLoadRecursiveData = true;

#if JUCE_WINDOWS
	launchOnStartup = startupCC.addBoolParameter("Launch on system startup", "If checked, this app will automatically launch on system startup", false);
#endif
	launchMinimised = startupCC.addBoolParameter("Launch minimised", "If checked, this app will automatically minimized it self when launched", false);

	checkUpdatesOnStartup = startupCC.addBoolParameter("Check updates on startup", "If enabled, app will check if any updates are available",true);
	checkBetaUpdates = startupCC.addBoolParameter("Check for beta updates", "If enabled the app will also check for beta versions of the software", true);
	onlyCheckBetaFromBeta = startupCC.addBoolParameter("Only Check beta from beta versions", "If enabled the app will only check beta version when running a beta version itself", true);
	updateHelpOnStartup = startupCC.addBoolParameter("Update help on startup", "If enabled, app will try and download the last help file locally", true);
	
	openLastDocumentOnStartup = startupCC.addBoolParameter("Load last "+(Engine::mainEngine != nullptr?Engine::mainEngine->fileExtension:"")+" on startup", "If enabled, app will load the last " + Engine::mainEngine->fileExtension + " on startup", false);
	openSpecificFileOnStartup = startupCC.addBoolParameter("Load specific "+(Engine::mainEngine != nullptr?Engine::mainEngine->fileExtension:"")+" on startup", "If enabled, app will load the " + Engine::mainEngine->fileExtension + " specified below on startup", false,false);
	 
	fileToOpenOnStartup = new FileParameter("File to load on startup", "File to load when start, if the option above is checked", "", false);
	startupCC.addParameter(fileToOpenOnStartup);
	
	addChildControllableContainer(&startupCC);


	enableAutoSave = saveLoadCC.addBoolParameter("Enable auto-save", "When enabled, a backup file will be saved every 5 min", true);
	autoSaveCount = saveLoadCC.addIntParameter("Auto-save count", "The number of different files to auto-save", 10, 1, 100);

	addChildControllableContainer(&saveLoadCC);

	askBeforeRemovingItems = editingCC.addBoolParameter("Ask before removing items", "If enabled, you will get a confirmation prompt before removing any item", false);
	constrainKeysToNeighbours = editingCC.addBoolParameter("Constrain curve keys editing", "If enabled, keys won't be able to be moved past their neighbours when editing a curve", false);

	addChildControllableContainer(&editingCC);

	addChildControllableContainer(OSCRemoteControl::getInstance());

	addChildControllableContainer(&keyMappingsCC);
}

GlobalSettings::~GlobalSettings()
{
}

void GlobalSettings::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	ControllableContainer::onControllableFeedbackUpdate(cc, c);

#if JUCE_WINDOWS	
	if (c == launchOnStartup)
	{

		String regKey = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\" + OrganicApplication::getInstance()->getApplicationName();
		String regValue = "\"" + File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getFullPathName() + "\"";
		if (launchOnStartup->boolValue()) WindowsRegistry::setValue(regKey, regValue);
		else WindowsRegistry::deleteValue(regKey);
	}
#endif

	if (c == openLastDocumentOnStartup)
	{
		if (openLastDocumentOnStartup->boolValue()) openSpecificFileOnStartup->setValue(false);
		openSpecificFileOnStartup->setEnabled(!openLastDocumentOnStartup->boolValue());
	}else if (c == openLastDocumentOnStartup || c == openSpecificFileOnStartup)
	{
		fileToOpenOnStartup->setEnabled(openSpecificFileOnStartup->boolValue());
	}else if (c == checkBetaUpdates)
	{
		onlyCheckBetaFromBeta->setEnabled(checkBetaUpdates->boolValue());
	} else if (c == enableAutoSave)
	{
		autoSaveCount->setEnabled(enableAutoSave->boolValue());
	}
}

void GlobalSettings::loadJSONDataInternal(var data)
{
	openSpecificFileOnStartup->setEnabled(!openLastDocumentOnStartup->boolValue());
	fileToOpenOnStartup->setEnabled(openSpecificFileOnStartup->boolValue());
}


KeyMappingsContainer::KeyMappingsContainer() :
	ControllableContainer("Key Mappings")
{

}

KeyMappingsContainer::~KeyMappingsContainer()
{

}

var KeyMappingsContainer::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	KeyPressMappingSet * kms = getCommandManager().getKeyMappings();
    std::unique_ptr<XmlElement> xmlElement(kms->createXml(true));
	String xmlData = xmlElement->toString();
	data.getDynamicObject()->setProperty("keyMappings", xmlData);
	return data;
}

void KeyMappingsContainer::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);
	
	KeyPressMappingSet * kms = getCommandManager().getKeyMappings();
	std::unique_ptr<XmlElement> element = XmlDocument::parse(data.getProperty("keyMappings", "").toString());
	if (element != nullptr) kms->restoreFromXml(*element);
}

InspectableEditor * KeyMappingsContainer::getEditor(bool isRoot)
{
	return new KeyMappingsContainerEditor(this, isRoot);
}
