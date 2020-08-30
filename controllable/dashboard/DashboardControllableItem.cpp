DashboardControllableItem::DashboardControllableItem(Controllable* item) :
	DashboardInspectableItem(item),
	controllable(item)
{
	viewUISize->setPoint(200, 50);

	showLabel = addBoolParameter("Show Label", "If checked, label is shown on controller", true);
	textColor = addColorParameter("Text Color", "Color of the text", TEXT_COLOR, false);
	contourColor = addColorParameter("Contour Color", "Color of the contour", BG_COLOR.brighter(), false);
	contourThickness = addFloatParameter("Contour Thickness", "Thickness of the contour", 2, 1);
	opaqueBackground = addBoolParameter("Opaque Background", "If checked, background is opaque", true);
	customLabel = addStringParameter("Custom text", "If not empty, will override the label of this control", "", false);
	customDescription = addStringParameter("Custom description", "If not empty, will override the description of this control", "", false);

	textColor->canBeDisabledByUser = true;
	contourColor->canBeDisabledByUser = true;
	customLabel->canBeDisabledByUser = true;
	customDescription->canBeDisabledByUser = true;
}

DashboardControllableItem::~DashboardControllableItem()
{

}

var DashboardControllableItem::getJSONData()
{
	var data = DashboardInspectableItem::getJSONData();
	if (controllable != nullptr) data.getDynamicObject()->setProperty("controllable", controllable->getControlAddress());
	return data;
}

void DashboardControllableItem::loadJSONDataItemInternal(var data)
{
	String address = data.getProperty("controllable", inspectableGhostAddress);
	setInspectable(Engine::mainEngine->getControllableForAddress(address));

	DashboardInspectableItem::loadJSONDataItemInternal(data);

}


void DashboardControllableItem::setInspectableInternal(Inspectable* i)
{
	controllable = dynamic_cast<Controllable*>(i);
}

void DashboardControllableItem::ghostInspectable()
{
	if (!inspectable.wasObjectDeleted() && inspectable != nullptr) inspectableGhostAddress = dynamic_cast<Controllable*>(inspectable.get())->getControlAddress();
}

void DashboardControllableItem::checkGhost()
{
	setInspectable(Engine::mainEngine->getControllableForAddress(inspectableGhostAddress));
}

var DashboardControllableItem::getServerData()
{
	var data = DashboardInspectableItem::getServerData();
	DynamicObject* o = data.getDynamicObject();

	o->setProperty("id", shortName);

	if (controllable == nullptr || controllable.wasObjectDeleted())
	{
		o->setProperty("ghostAddress",  inspectableGhostAddress);
		return var(o);
	}

	o->setProperty("type", controllable->getTypeString());
	o->setProperty("controlAddress", controllable->getControlAddress());

	o->setProperty("showLabel", showLabel->value);
	if(textColor->enabled) o->setProperty("textColor", textColor->value);
	if (contourColor->enabled)
	{
		o->setProperty("contourColor", contourColor->value);
		o->setProperty("contourThickness", contourThickness->value);
	}

	o->setProperty("opaqueBackground", opaqueBackground->value);
	o->setProperty("label", customLabel->enabled ? customLabel->value.toString() : controllable->niceName);
	o->setProperty("customDescription", customDescription->value);

	return data;
}
