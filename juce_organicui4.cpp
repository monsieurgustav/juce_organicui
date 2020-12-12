/*
  ==============================================================================

   

  ==============================================================================
*/

#include "juce_organicui.h"

//==============================================================================

#include "ui/AssetManager.cpp"
#include "ui/LookAndFeelOO.cpp"
#include "ui/Grabber.cpp"

#include "ui/shapeshifter/GapGrabber.cpp"
#include "ui/shapeshifter/ShapeShifter.cpp"
#include "ui/shapeshifter/ShapeShifterContainer.cpp"
#include "ui/shapeshifter/ShapeShifterContent.cpp"
#include "ui/shapeshifter/ShapeShifterFactory.cpp"
#include "ui/shapeshifter/ShapeShifterManager.cpp"
#include "ui/shapeshifter/ShapeShifterPanel.cpp"
#include "ui/shapeshifter/ShapeShifterPanelHeader.cpp"
#include "ui/shapeshifter/ShapeShifterPanelTab.cpp"
#include "ui/shapeshifter/ShapeShifterWindow.cpp"

#include "help/HelpPanel.cpp"
#include "undo/UndoMaster.cpp"
#include "updater/AppUpdater.cpp"


#include "app/CrashHandler.cpp"
#include "app/OrganicMainComponent.cpp"
#include "app/OrganicMainComponentCommands.cpp"
#include "app/OrganicApplication.cpp"

#include "remotecontrol/OSCRemoteControl.cpp"

#include "warning/WarningTarget.cpp"
#include "warning/WarningReporter.cpp"
#include "warning/ui/WarningTargetUI.cpp"
#include "warning/ui/WarningReporterPanel.cpp"